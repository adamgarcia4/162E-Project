#ifndef CONTROL_H
#define CONTROL_H

#include "Servo.h"

class Controller {
public:
   Controller(int encoderPin, int motorPin, int cycleTime);
   ~Controller();
   void driveMotor(double dutyCycle);
   // double updateRPM(double newRPM);
   bool attached();

private:
   int m_encoderSlot;
   unsigned long m_lastTime;
   unsigned long m_loopDuration;
   int m_tickCount;
   double m_encoderRad;
   double m_encoderRate;
   int m_motorPin;
   int readIndex = 0;
   double total = 0;
   double average = 0;
   Servo* myServo;
};


#endif
