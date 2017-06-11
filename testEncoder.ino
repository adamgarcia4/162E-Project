#include <ADXL345.h>
#include <ITG_3200.h>
#include <Wire.h>
#include <Servo.h>
ITG_3200 gyro;
ADXL345 accel;
Servo xmotors, ymotors;
const float alpha_phi = 0.985;
const float alpha_theta = 0.985;
const float beta_phi = 0.95;
const float beta_theta = 0.95;
const float total_gain = 6.0f;
const float x_coeff = 1.0f;
const float y_coeff = 1.5f;
const float x_deadspot = 0.1;
const float y_deadspot = 0.1;
const float x_pid_d = 0.01;
const float y_pid_d = 0.01;
struct measurements_t
{
  float gx, gy, gz, ax, ay, az;
  float bax, bay, baz;
};
struct calc_t
{
  float angx, angy, angz;
  float fx, fy;
};
struct command_t
{
  float mx, my;
};
struct state_t
{
  measurements_t measurements;
  calc_t calc;
  command_t command;
};
state_t state;
void read_state()
{
  int x, y, z;
  accel.readAccel(&x, &y, &z);
  gyro.read();
  state.measurements.ax = x * 4 / 1024.0f;
  state.measurements.ax -= state.measurements.bax;
  state.measurements.ay = y * 4 / 1024.0f;
  state.measurements.ay -= state.measurements.bay;
  state.measurements.az = z * 4 / 1024.0f;
  state.measurements.az -= state.measurements.baz;
  state.measurements.gx = gyro.m.c.x / 32768.0f * 2000.0f / 180.0f * PI;
  state.measurements.gy = gyro.m.c.y / 32768.0f * 2000.0f / 180.0f * PI;
}
void update_state(float dt)
{
  float phi = atan2(-state.measurements.ax, state.measurements.az);
  float theta = atan2(state.measurements.ay, state.measurements.az);
  state.calc.angy = alpha_phi * (state.calc.angy + dt * state.measurements.gy) + (1 - alpha_phi) * phi;
  state.calc.angx = alpha_theta * (state.calc.angx + dt * state.measurements.gx) + (1 - alpha_theta) * theta;
}
float sign_transfer(float a, float b)
{
  if (b > 0)
    return a;
  return -a;
}
float deadspot(float v, float d)
{
  if (abs(v) < d)
    return 0.0f;
  return v - sign_transfer(d, v);
}
void update_control(float dt)
{
  float last_fx = state.calc.fx;
  float last_fy = state.calc.fy;
  state.calc.fx = beta_phi * state.calc.fx + (1 - beta_phi) * deadspot(total_gain * x_coeff * state.calc.angx, x_deadspot);
  state.calc.fy = beta_theta * state.calc.fy + (1 - beta_theta) * deadspot(total_gain * y_coeff * state.calc.angy, y_deadspot);
  state.command.mx = state.calc.fx - (state.calc.fx - last_fx) / dt * x_pid_d;
  state.command.my = state.calc.fy - (state.calc.fy - last_fy) / dt * y_pid_d;
  ymotors.writeMicroseconds(constrain(1500 - 500 * state.command.my, 1000, 2000));
  xmotors.writeMicroseconds(constrain(1500 - 1000 * state.command.mx, 1000, 2000));
}
void print_state()
{
  Serial.print(state.measurements.ax);
  Serial.print(" ");
  Serial.print(state.measurements.ay);
  Serial.print(" ");
  Serial.print(state.measurements.az);
  Serial.print(" ");
  Serial.print(state.measurements.gx);
  Serial.print(" ");
  Serial.print(state.measurements.gy);
  Serial.print(" ");
  Serial.print(state.measurements.gz);
  Serial.print(" ");
  Serial.print(state.calc.angx);
  Serial.print(" ");
  Serial.print(state.calc.angy);
  Serial.print(" ");
  Serial.print(state.command.mx);
  Serial.print(" ");
  Serial.print(state.command.my);
  Serial.println(" ");
}
void tare_accel()
{
  state.measurements.bax = 0;
  state.measurements.bay = 0;
  state.measurements.baz = 0;
  
  float tx = 0, ty = 0, tz = 0;
  for(uint32_t i = 0; i < 100; i++)
  {
    read_state();
    tx += state.measurements.ax;
    ty += state.measurements.ay;
  }
  state.measurements.bax = tx / 100;
  state.measurements.bay = ty / 100;
  Serial.println("Tare report:");
  Serial.print(state.measurements.bax);
  Serial.print(" ");
  Serial.print(state.measurements.bay);
  Serial.print(" ");
  Serial.print(state.measurements.baz);
  Serial.println(" ");
}
void setup() {
  Serial.begin(115200);
  Wire.begin();
  gyro.init();
  
  pinMode(2, INPUT);
  ymotors.attach(9);
  xmotors.attach(6);
  ymotors.writeMicroseconds(1500);
  xmotors.writeMicroseconds(1500);
  accel.powerOn();
  accel.setRangeSetting(4);
  accel.setFullResBit(true);
  memset(&state, 0, sizeof(state));
}
void loop() {
  // Last states
  static uint32_t last_time = 0;
  static float last_dt = 0.1f; // This is used to store our best guess of dt if micros() overflows
  uint32_t current_time = micros();
  uint32_t delta_time = current_time - last_time;
  float dt;
  
  if(current_time > last_time)
  {
    dt = delta_time / (1000000.0f);
  }
  else
  {
    // micros() overflowed. Use the last calculated dt
    dt = last_dt;
  }
  // Update last state
  last_dt = dt;
  last_time = current_time;
  read_state();
  update_state(dt);
  update_control(dt);
  
  print_state();
  if(digitalRead(2))
  {  
    ymotors.writeMicroseconds(1500);
    xmotors.writeMicroseconds(1500);
    Serial.print("Taring accelerometer...");
    tare_accel();
    Serial.println("DONE.");
  }
}
