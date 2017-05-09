#include "IMU.h"
#include "Arduino.h"
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <Wire.h>

// Member Vars
FreeSixIMU* m_sixDOF; // Object that does IMU calcs
float m_angles[3]; // yaw | pitch | roll
int m_fracUpdate;
int m_loopCounter;
float m_prev_angles[3];

IMU::IMU(float* angleArr, int fracUpdate){
   Serial.println("IMU started");
   m_sixDOF = new FreeSixIMU();
   Wire.begin();
   delay(5);
   m_sixDOF->init();
   m_sixDOF->getEuler(m_angles);
   memcpy(angleArr, m_angles, 3);
   m_fracUpdate = fracUpdate;
   m_loopCounter = 0;
}

//<<destructor>>
IMU::~IMU(){/*nothing to destruct*/}

void IMU::measureAngles() {
   memcpy(m_prev_angles, m_angles, sizeof(m_angles));
   m_sixDOF->getEuler(m_angles);
   for(int i = 0; i< 3; i++) {
    m_prev_angles[i] = m_angles[i] - m_prev_angles[i];
   }
}

void IMU::loop(float * angleArr) {

   measureAngles();

   // Allows Angle output to be tied to a fraction of the looping frequency
   if(m_loopCounter == m_fracUpdate) {
    angleArr[0] = m_angles[0];
    angleArr[1] = m_angles[1];
    angleArr[2] = m_angles[2];
//      memcpy(angleArr, m_angles, 3);
      // printAngles();
      m_loopCounter = 0;
   } else {
      m_loopCounter++;
   }
}

void IMU::printAngles() {
   Serial.print(m_angles[0]);
   Serial.print(" | ");
   Serial.print(m_angles[1]);
   Serial.print(" | ");
   Serial.println(m_angles[2]);
   Serial.print(" | ");
//   Serial.print(prev_angles[0]);
//   Serial.print(" | ");
//   Serial.print(prev_angles[1]);
//   Serial.print(" | ");
//   Serial.println(prev_angles[2]);
}
