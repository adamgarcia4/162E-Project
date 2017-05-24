#include "Arduino.h"
#include "Controller.h" //include the declaration for this class
#include "Servo.h"

// Member Vars
int m_encoderSlot;
unsigned long m_lastTime;
unsigned long m_loopDuration;
int m_tickCount;
double m_encoderRad;
double m_encoderRate;
int m_motorPin;
Servo myServo;

const int numReadings = 1;
double readings[numReadings];
int readIndex = 0;
double total = 0;
double average = 0;

Controller::Controller(int encoderPin, int motorPin, int cycleTime){
   m_encoderSlot = encoderPin;
   m_motorPin = motorPin;
   pinMode(m_encoderSlot, INPUT);
   myServo.attach(m_motorPin,1000,2000);
   m_lastTime = 0;

   // Initialize boxcar array to 0
   for(int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings[thisReading] = 0;
   }
}

//<<destructor>>
Controller::~Controller(){/*nothing to destruct*/}


int Controller::readEncoder() {
  return digitalRead(m_encoderSlot);
}

void Controller::updateTimer(unsigned long newTime) {
   m_loopDuration = newTime - m_lastTime;
   m_lastTime = newTime;
}

unsigned long Controller::getRefreshRate() {
   return m_loopDuration;
}

double Controller::updateRPM(double newRPM){

   //subtract last reading
   total = total - readings[readIndex];
   //read from sensor
   readings[readIndex] = newRPM;
   // add reading to total
   total = total + readings[readIndex];
   //advance to next position in array
   readIndex +=1;
   if(readIndex >= numReadings) {
      readIndex = 0;
   }
   average = total / numReadings;
   m_tickCount = average;

   return m_tickCount;
}



void Controller::updateEncoderRate() {
   m_encoderRate = m_encoderRad / m_loopDuration * 1000;
}

double Controller::getEncoderRate() {
   return m_tickCount;
}

void Controller::driveMotor(double input){
  if(input > 1) {
    input = 1;
} else if(input < -1) {
    input = -1;
  }

  if (input > 0 && input < 0.04){
     input = 0.04;
 }
   double actualDutyCycle = 90 * input + 90;
   myServo.write(actualDutyCycle);
}
