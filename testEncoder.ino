//// digital pin 2 has a pushbutton attached to it. Give it a name:
//int pinNum = 2;
//
//// the setup routine runs once when you press reset:
//void setup() {
//  // initialize serial communication at 9600 bits per second:
//  Serial.begin(9600);
//  // make the pushbutton's pin an input:
//  pinMode(pinNum, INPUT);
//}
//
//// the loop routine runs over and over again forever:
//void loop() {
//  // read the input pin:
//  int encoderState = digitalRead(pinNum);
//  // print out the state of the button:
//  Serial.println(encoderState);
//  delay(1);        // delay in between reads for stability
//}

#include "Controller.h"
#include "Servo.h"

Controller* m_controller1;
unsigned long m_currTime;

volatile int m_numEncoder;




void setup(){
  /*nothing to setup*/
  m_controller1 = new Controller(2);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), encoderChange, CHANGE);
  m_currTime = millis();
  m_numEncoder = 0;
}

void loop(){
//  m_controller1.blink(5000);//stay one second on, then a second off
   m_currTime = millis();
   m_controller1->updateTimer(m_currTime);
   // Serial.println(m_controller1->readEncoder());
   Serial.println(m_controller1->getRefreshRate());
   Serial.println(m_numEncoder);
   m_numEncoder = 0;
   delay(10000);
}

void encoderChange() {
   m_numEncoder++;
   Serial.println(m_numEncoder);
}
