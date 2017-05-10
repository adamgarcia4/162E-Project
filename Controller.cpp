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
double m_drivingFrequency;
Servo myServo;

Controller::Controller(int encoderPin, int motorPin, int cycleTime){
   m_encoderSlot = encoderPin;
   m_motorPin = motorPin;
   //m_drivingFrequency = 1000 / cycleTime;
   pinMode(m_encoderSlot, INPUT);
   myServo.attach(m_motorPin,1000,2000);
   m_lastTime = 0;
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

int Controller::updateTickCount(int tickNum){
   m_tickCount = tickNum;
   m_encoderRad = m_tickCount * 2 * 3.141592654 / 8; //rads

   updateEncoderRate();
   return m_tickCount;
}

void Controller::updateEncoderRate() {
   m_encoderRate = m_encoderRad / m_loopDuration * 1000;
}

double Controller::getEncoderRate() {
   return m_encoderRate;
}

double Controller::radToDeg(double rad) {
   return rad * 180 / 3.141592654;
}

double Controller::getFreq() {
  return m_drivingFrequency;
}

void Controller::driveMotor(double input){
  if(input > 1) {
    input = 1;
} else if(input < -1) {
    input = -1;
  }
   double actualDutyCycle = 90 * input + 90;
   myServo.write(actualDutyCycle);
}
