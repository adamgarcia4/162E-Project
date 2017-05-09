#include "Controller.h"
#include "IMU.h"

Controller* m_controller1;
IMU* m_imu;
unsigned long m_currTime;
volatile int m_numEncoder;
float angleArr[3];

void setup(){
  /*nothing to setup*/
  Serial.begin(9600);
  m_controller1 = new Controller(2, 9, 20); //ms
  m_imu = new IMU(angleArr, 5);
  attachInterrupt(digitalPinToInterrupt(2), encoderChange, CHANGE);
  m_currTime = millis();
  m_numEncoder = 0;
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

   m_currTime = millis();
   m_controller1->updateTimer(m_currTime);

   m_imu->loop(angleArr);
   Serial.print(angleArr[0]);
   Serial.print(" | ");
   Serial.print(angleArr[1]);
   Serial.print(" | ");
   Serial.println(angleArr[2]);
   Serial.print(" | ");

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


   delay(20); //Looping refresh rate (ms)
}

void encoderChange() {
   m_numEncoder++;
}
