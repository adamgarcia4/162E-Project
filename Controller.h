#ifndef CONTROL_H
#define CONTROL_H

class Controller {
public:
   Controller(int pwmVal);
   ~Controller();
   int readEncoder();
   void updateTimer(unsigned long newTime);
   unsigned long getRefreshRate();
   void on();
   void off();
   void blink(int time);
};


#endif
