#include "Controller.h"
#include "IMU.h"

Controller* m_controller1;
IMU* m_imu;
unsigned long m_currTime;
volatile int m_numEncoder;

void setup(){
  /*nothing to setup*/
  Serial.begin(9600);
  m_controller1 = new Controller(2, 9, 20); //ms
  m_imu = new IMU();
  attachInterrupt(digitalPinToInterrupt(2), encoderChange, CHANGE);
  m_currTime = millis();
  m_numEncoder = 0;

//  analogWrite(5,10);

//  Serial.println(m_controller1->getFreq());
//  m_controller1->driveMotor(0.5);
}

int convertVal(double input) {
  if(input > 1) {
    input = 1;
  } else if(input < -1) {
    input = -1;
  }
  return 90 * input + 90;
}

void loop(){
//  m_controller1.blink(5000);//stay one second on, then a second off
   m_currTime = millis();
   m_controller1->updateTimer(m_currTime);
   
   // Serial.println(m_controller1->readEncoder());
   // Serial.println(m_controller1->getRefreshRate());
   // Serial.println(m_numEncoder);
   //m_controller1->updateTickCount(m_numEncoder);

  // for(int i=0;i<100;i++) {
  //   m_controller1->driveMotor(0.01 * i - 0.5);
  //   delay(1000);
  // }
  //
  //  m_numEncoder = 0;

  m_imu->updateAngles();
   delay(500);
}

void encoderChange() {
   m_numEncoder++;
}
