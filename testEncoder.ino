#include "Controller.h"
#include "IMU.h"
#include <stdio.h>

// Initialize Controller and IMU object pointers.
Controller* m_controller1;
Controller* m_controller2;
IMU* m_imu;

// Encoder Stuff
volatile long m_microsTime;
volatile long m_lastMicrosTime;
volatile long m_timeElapsed;
volatile double m_motorRPM;

// IMU Stuff
float angleArr[3];

// Reset Button Stuff
bool m_lastButton = false;

// Looping Stuff
unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long interval = 20;

void setup(){

   // Preprocessing functions
   Serial.begin(9600); //Begin Serial Output
   delay(20); //Delay to ensure that Serial Output established

   // Motor Controller / Encoder Class
   m_controller1 = new Controller(2, 6, 20); //(EncoderPin, MotorPin, Cycletime)
    m_controller2 = new Controller(3, 9, 20); //(EncoderPin, MotorPin, Cycletime)
   // attachInterrupt(digitalPinToInterrupt(2), encoderPolarityChange, CHANGE); //Encoder Interrupt function
   // m_numEncoder = 0; //Storage mechanism for Encoder temporary Bucket routine


   // IMU Class
   m_imu = new IMU(angleArr, 5);

   // Final Setup Delay
   delay(20);
}

void loop(){
   currentMillis = millis();
//    Serial.println("bye");
   if(currentMillis - previousMillis > interval){
//      Serial.println("hi");
//      Serial.println(digitalRead(2));
      // To print values to console, pass in a 1.  Else, 0.
      m_imu->loop(angleArr, 0);

      // If reset button just pressed, reset IMU zero.
      if(m_lastButton == false && digitalRead(2) == 1) {
         Serial.println(digitalRead(2));
//         Serial.println(m_controller1->attached());
//         Serial.println(m_controller2->attached());
         m_imu->reset();
         m_lastButton = true;
      } else if(m_lastButton == true && digitalRead(2) == 0) {
         m_lastButton = false;
      }

      // Drive motors with given proportional power
      m_controller1->driveMotor(-(double)angleArr[1]/45);
      m_controller2->driveMotor(-(double)angleArr[2]/45);

      // Reset millis to check in next loop
      previousMillis = currentMillis;
   }

}

void encoderPolarityChange() {
   m_lastMicrosTime = m_microsTime;
   m_microsTime = millis();
   m_timeElapsed = m_microsTime - m_lastMicrosTime;

   m_motorRPM = 0.125 / m_timeElapsed * 1000 * 60;
//   m_controller1->updateRPM(m_motorRPM);
}
