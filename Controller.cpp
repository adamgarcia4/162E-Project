#include "Arduino.h"
#include "Controller.h" //include the declaration for this class


int m_encoderSlot;
unsigned long m_lastTime;
unsigned long m_loopDuration;
int m_tickCount;

const int LED_PIN = 13; //use the LED @ Arduino pin 13

//<<constructor>> setup the LED, make pin 13 an OUTPUT
Controller::Controller(int pwmVal){
   m_encoderSlot = pwmVal;
   pinMode(m_encoderSlot, INPUT);
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

//turn the LED on
void Controller::on(){
	digitalWrite(LED_PIN,HIGH); //set the pin HIGH and thus turn LED on
}

//turn the LED off
void Controller::off(){
	digitalWrite(LED_PIN,LOW); //set the pin LOW and thus turn LED off
}

//blink the LED in a period equal to paramterer -time.
void Controller::blink(int time){
	on(); 			//turn LED on
	delay(time/2);  //wait half of the wanted period
	off();			//turn LED off
	delay(time/2);  //wait the last half of the wanted period
}
