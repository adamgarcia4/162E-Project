#include "Controller.h"
#include "IMU.h"
// #include "SynchPID.h"
#include <PID_v1.h>
#include <stdio.h>
//#include "Autotune.h"

Controller* m_controller1;
//Controller* m_controller2;
IMU* m_imu;
unsigned long m_currTime;
volatile int m_numEncoder;
volatile long m_microsTime;
volatile long m_lastMicrosTime;
volatile long m_timeElapsed;
volatile double m_motorRPM;

volatile bool updateFlag;
int stallCounter = 0;

float angleArr[3];
double input, output, setpoint;

PID PID1(&input, &output, &setpoint, 0.0015, 0.000,0.00, DIRECT); //0.008, .8,0.00001 | .022
//.015

void setup(){
   // Preprocessing functions
   Serial.begin(9600); //Begin Serial Output
   delay(20); //Delay to ensure that Serial Output established

   // Motor Controller / Encoder Class
   m_controller1 = new Controller(2, 9, 20); //(EncoderPin, MotorPin, Cycletime)
   //  m_controller2 = new Controller(3, 6, 20); //(EncoderPin, MotorPin, Cycletime)
   attachInterrupt(digitalPinToInterrupt(2), encoderPolarityChange, CHANGE); //Encoder Interrupt function
   m_numEncoder = 0; //Storage mechanism for Encoder temporary Bucket routine

   //initialize PID stuff
   setpoint = 200;
   PID1.SetMode(AUTOMATIC);
   PID1.SetOutputLimits(0,1);
   PID1.SetSampleTime(200);
   //  input = 0;
   updateFlag = false;

   // Final Setup Delay
   delay(20);
}

void loop(){

   //TEST
    m_controller1->driveMotor(0.5);
    Serial.println(m_controller1->getEncoderRate());

  if(updateFlag) { //new value
    updateFlag = false;
    stallCounter = 0;
  } else {
    stallCounter++;
    if(stallCounter >=20) {
      m_controller1->updateRPM(0);
      stallCounter = 0;
    }
  }
//       
//        input = m_controller1->getEncoderRate();
//       PID1.Compute();
//        Serial.print(input); Serial.print(","); Serial.println(output);
//        m_controller1->driveMotor(output);

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

void encoderPolarityChange() {
   m_lastMicrosTime = m_microsTime;
   m_microsTime = millis();
   m_timeElapsed = m_microsTime - m_lastMicrosTime;

   m_motorRPM = 0.125 / m_timeElapsed * 1000 * 60;
   m_controller1->updateRPM(m_motorRPM);
   updateFlag = true;

   // m_numEncoder++;
}
