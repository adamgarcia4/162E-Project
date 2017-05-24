#include "Controller.h"
#include "IMU.h"
// #include "SynchPID.h"
#include <PID_v1.h>
#include <stdio.h>
//#include "Autotune.h"

Controller* m_controller1;
Controller* m_controller2;
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

bool m_lastButton = false;

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long interval = 20;

PID PID1(&input, &output, &setpoint, 0.0015, 0.000,0.00, DIRECT); //0.008, .8,0.00001 | .022
//.015

void setup(){
   // Preprocessing functions
   Serial.begin(9600); //Begin Serial Output
   delay(20); //Delay to ensure that Serial Output established

   // Motor Controller / Encoder Class
   m_controller1 = new Controller(2, 6, 20); //(EncoderPin, MotorPin, Cycletime)
    m_controller2 = new Controller(3, 9, 20); //(EncoderPin, MotorPin, Cycletime)
   attachInterrupt(digitalPinToInterrupt(2), encoderPolarityChange, CHANGE); //Encoder Interrupt function
   m_numEncoder = 0; //Storage mechanism for Encoder temporary Bucket routine

   //initialize PID stuff
   setpoint = 200;
   PID1.SetMode(AUTOMATIC);
   PID1.SetOutputLimits(0,1);
   PID1.SetSampleTime(200);
   //  input = 0;
   updateFlag = false;


   // IMU Class
   m_imu = new IMU(angleArr, 5);



   // Final Setup Delay
   delay(20);
}

void loop(){
   currentMillis = millis();

   if(currentMillis - previousMillis > interval){
      // m_imu->readAngles();
      m_imu->loop(angleArr);
      // Serial.print(angleArr[0]); // Rotation about Z (psi)
      // Serial.print(" | ");
      // Serial.print(angleArr[1]); // Rotation about X (theta)
      // Serial.print(" | ");
      // Serial.println(angleArr[2]); // Rotation about Y (phi)
      // Serial.print(" | ");

      if(m_lastButton == false && digitalRead(2) == 1) {
         Serial.println(digitalRead(2));
         Serial.println(m_controller1->attached());
         Serial.println(m_controller2->attached());
         m_imu->reset();
         m_lastButton = true;
      } else if(m_lastButton == true && digitalRead(2) == 0) {
         m_lastButton = false;
      }

      m_controller1->driveMotor((double)angleArr[1]/90);
      m_controller2->driveMotor((double)angleArr[2]/90);



      //  //TEST
      //   m_controller1->driveMotor(0.5);
      //   Serial.println(m_controller1->getEncoderRate());
      //
      // if(updateFlag) { //new value
      //   updateFlag = false;
      //   stallCounter = 0;
      // } else {
      //   stallCounter++;
      //   if(stallCounter >=20) {
      //     m_controller1->updateRPM(0);
      //     stallCounter = 0;
      //   }
      // }
      //
      //        input = m_controller1->getEncoderRate();
      //       PID1.Compute();
      //        Serial.print(input); Serial.print(","); Serial.println(output);
      //        m_controller1->driveMotor(output);
      previousMillis = currentMillis;
   }

   // delay(20); //Looping refresh rate (ms)
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
