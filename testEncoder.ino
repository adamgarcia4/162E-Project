#include "Controller.h"
#include "IMU.h"
// #include "SynchPID.h"
#include <PID_v1.h>
#include <stdio.h>
#include "Autotune.h"

Controller* m_controller1;
//Controller* m_controller2;
IMU* m_imu;
unsigned long m_currTime;
volatile int m_numEncoder;
float angleArr[3];
double input, output, setpoint;

PID PID1(&input, &output, &setpoint, 0.022, 0.000,0.00, DIRECT); //0.008, .8,0.00001
//.015

void setup(){
  // Preprocessing functions
  Serial.begin(9600); //Begin Serial Output
  delay(20); //Delay to ensure that Serial Output established

  // Motor Controller / Encoder Class
  m_controller1 = new Controller(2, 9, 20); //(EncoderPin, MotorPin, Cycletime)
//  m_controller2 = new Controller(3, 6, 20); //(EncoderPin, MotorPin, Cycletime)
  attachInterrupt(digitalPinToInterrupt(2), encoderChange, CHANGE); //Encoder Interrupt function
  m_numEncoder = 0; //Storage mechanism for Encoder temporary Bucket routine

  //initialize PID stuff
  setpoint = 40;
  PID1.SetMode(AUTOMATIC);
  PID1.SetOutputLimits(0,0.8);
  PID1.SetSampleTime(200);

  // Final Setup Delay
  delay(20);
}

void loop(){

    m_controller1->updateTickCount(m_numEncoder);
    m_numEncoder = 0;
   m_currTime = millis(); //Update current time
   m_controller1->updateTimer(m_currTime);

    // Routine to print rad/s

    for(int i=0;i<100;i++) {
      m_controller1->driveMotor(1);
      delay(2000);
      m_controller1->driveMotor(-1);
      delay(2000);
    }
      m_controller1->driveMotor(0);
      delay(100000);
//    m_controller1->driveMotor(0.5);
//    m_controller2->driveMotor(0.5);
//    Serial.println(m_controller1->getEncoderRate());
//
//
//  input = m_controller1->getEncoderRate();
//  PID1.Compute();
////   Serial.print(input); Serial.print(","); Serial.println(output);
//   Serial.println(m_controller1->getEncoderRate());
//   m_controller1->driveMotor(output);


   delay(200); //Looping refresh rate (ms)
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

  // IMU Class
  //  m_imu = new IMU(angleArr, 5);

   // m_imu->loop(angleArr);
   // Serial.print(angleArr[0]);
   // Serial.print(" | ");
   // Serial.print(angleArr[1]);
   // Serial.print(" | ");
   // Serial.println(angleArr[2]);
   // Serial.print(" | ");


     // for(int i=0;i<100;i++) {
  //   m_controller1->driveMotor(0.01 * i - 0.5);
  //   delay(1000);
  // }
