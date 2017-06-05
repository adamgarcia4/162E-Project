#include "Arduino.h"
#include "Controller.h" //include the declaration for this class
#include "Servo.h"

Controller::Controller(int encoderPin, int motorPin, int cycleTime){
   m_encoderSlot = encoderPin;
   m_motorPin = motorPin;
   pinMode(m_encoderSlot, INPUT);
   myServo = new Servo();
   Serial.println(m_motorPin);
   myServo->attach(m_motorPin,1000,2000);
   m_lastTime = 0;
}

//<<destructor>>
Controller::~Controller(){

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
   myServo->write(actualDutyCycle);
}

// double Controller::updateRPM(double newRPM){
// 
//    //subtract last reading
//    total = total - readings[readIndex];
//    //read from sensor
//    readings[readIndex] = newRPM;
//    // add reading to total
//    total = total + readings[readIndex];
//    //advance to next position in array
//    readIndex +=1;
//    if(readIndex >= numReadings) {
//       readIndex = 0;
//    }
//    average = total / numReadings;
//    m_tickCount = average;
//
//    return m_tickCount;
// }

bool Controller::attached() {
   return myServo->attached();
}
