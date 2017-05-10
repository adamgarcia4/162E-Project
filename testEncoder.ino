#include "Controller.h"
#include "IMU.h"
// #include "SynchPID.h"
#include <PID_v1.h>
#include <stdio.h>

Controller* m_controller1;
IMU* m_imu;
unsigned long m_currTime;
volatile int m_numEncoder;
float angleArr[3];
double input, output, setpoint;

PID PID1(&input, &output, &setpoint, 1, 0,0, DIRECT); //0.008, .8,0.00001

void setup(){
  // Preprocessing functions
  Serial.begin(9600); //Begin Serial Output
  delay(20); //Delay to ensure that Serial Output established
  m_currTime = millis(); //Singleton current time counter class


  // Motor Controller / Encoder Class
  m_controller1 = new Controller(2, 9, 20); //(EncoderPin, MotorPin, Cycletime)
  attachInterrupt(digitalPinToInterrupt(2), encoderChange, CHANGE); //Encoder Interrupt function
  m_numEncoder = 0; //Storage mechanism for Encoder temporary Bucket routine

  // IMU Class
  //  m_imu = new IMU(angleArr, 5);

  //initialize the variables we're linked to
  setpoint = 10;

  //turn the PID on
  PID1.SetMode(AUTOMATIC);
  PID1.SetOutputLimits(0,1);
  PID1.SetSampleTime(200);


  // Final Setup Delay
  delay(20);
}

void loop(){

   m_currTime = millis(); //Update current time
   m_controller1->updateTimer(m_currTime);

   // m_imu->loop(angleArr);
   // Serial.print(angleArr[0]);
   // Serial.print(" | ");
   // Serial.print(angleArr[1]);
   // Serial.print(" | ");
   // Serial.println(angleArr[2]);
   // Serial.print(" | ");

   // Print out current value to screen

   // Read Encoder Speeds
   // Serial.println(m_controller1->readEncoder());
   // Serial.println(m_numEncoder);
    m_controller1->driveMotor(0.5);
//    Serial.println(m_controller1->updateTickCount(m_numEncoder));
     Serial.println(m_controller1->getEncoderRate());
   m_controller1->updateTickCount(m_numEncoder);
   m_numEncoder = 0;




   // Serial.println(m_controller1->getRefreshRate());

  // for(int i=0;i<100;i++) {
  //   m_controller1->driveMotor(0.01 * i - 0.5);
  //   delay(1000);
  // }
  //
  //  m_numEncoder = 0;
  // double newVal = m_PID1->calculate(currPIDval);
  // Serial.println(newVal);
  // currPIDval = newVal;


/*
  input = m_controller1->getEncoderRate();
  PID1.Compute();
   Serial.print(input); Serial.print(","); Serial.println(output);
   m_controller1->driveMotor(output);
*/

   delay(20); //Looping refresh rate (ms)
}

int convertVal(double input) {
  if(input > 1) {
    input = 1;
  } else if(input < -1) {
    input = -1;
  }

  return 90 * input + 90;
}

void encoderChange() {
   m_numEncoder++;
}
