#ifndef CONTROL_H
#define CONTROL_H

#include "Servo.h"

class Controller {
public:
   Controller(int encoderPin, int motorPin, int cycleTime);
   ~Controller();
   int readEncoder();
   bool attached();
   void updateTimer(unsigned long newTime);
   double updateRPM(double newRPM);
   unsigned long getRefreshRate();
   void setEncoderRate();
   double getEncoderRate();
   void updateEncoderRate();
   int updateTickCount(int tickNum);
   void driveMotor(double dutyCycle);
   double getFreq();

private:
   double radToDeg(double rad);
   int m_encoderSlot;
   unsigned long m_lastTime;
   unsigned long m_loopDuration;
   int m_tickCount;
   double m_encoderRad;
   double m_encoderRate;
   int m_motorPin;
   const int numReadings = 1;
   double readings[numReadings];
   int readIndex = 0;
   double total = 0;
   double average = 0;
   Servo* myServo;
};


#endif
