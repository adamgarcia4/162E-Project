#ifndef CONTROL_H
#define CONTROL_H

class Controller {
public:
   Controller(int encoderPin, int motorPin, int cycleTime);
   ~Controller();
   int readEncoder();
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
};


#endif
