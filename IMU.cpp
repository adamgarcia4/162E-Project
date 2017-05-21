#include "IMU.h"
#include "Arduino.h"
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <Wire.h>

// Member Vars
FreeSixIMU* m_sixDOF; // Object that does IMU calcs
float m_angles[3]; // Z (psi) | X (Theta) | Y (phi)
float m_resetAngles[3];

// int m_fracUpdate;
// int m_loopCounter;
// float m_prev_angles[3];
// int m_rawData[6]; // | Y-acc | X-acc | Z-acc | Y-gyro | X-gyro | Z-gyro |
// int m_accData[3];
// int m_gyroData[3];
// float m_lastGyroReading[3];
// float m_newAngle[2];


IMU::IMU(float* angleArr, int fracUpdate){
   Serial.println("IMU started");
   m_sixDOF = new FreeSixIMU();
   Wire.begin();
   delay(5);
   m_sixDOF->init();
   for(int i=0;i<3;i++) {
    m_resetAngles[i] = 0;
   }
   // m_fracUpdate = fracUpdate;
   // m_loopCounter = 0;
}

//<<destructor>>
IMU::~IMU(){/*nothing to destruct*/}

//
// void IMU::measureAngles() {
//    memcpy(m_prev_angles, m_angles, sizeof(m_angles));
//    m_sixDOF->getEuler(m_angles);
//    for(int i = 0; i< 3; i++) {
//     m_prev_angles[i] = m_angles[i] - m_prev_angles[i];
//    }
// }

void IMU::loop(float * angleArr) {

   // measureAngles();
   m_sixDOF->getEuler(m_angles);

   // Allows Angle output to be tied to a fraction of the looping frequency
   // if(m_loopCounter == m_fracUpdate) {
    angleArr[0] = m_angles[0]-m_resetAngles[0];
    angleArr[1] = m_angles[1]-m_resetAngles[1];
    angleArr[2] = m_angles[2]-m_resetAngles[2];
//      memcpy(angleArr, m_angles, 3);
      // printAngles();
      // m_loopCounter = 0;
   // } else {
      // m_loopCounter++;
   // }
}

void IMU::reset() {
   for(int i=0;i<3;i++) {
      m_resetAngles[i] = m_angles[i];
   }
}

// void IMU::printAngles() {
//    Serial.print(m_angles[0]);
//    Serial.print(" | ");
//    Serial.print(m_angles[1]);
//    Serial.print(" | ");
//    Serial.println(m_angles[2]);
//    Serial.print(" | ");
// //   Serial.print(prev_angles[0]);
// //   Serial.print(" | ");
// //   Serial.print(prev_angles[1]);
// //   Serial.print(" | ");
// //   Serial.println(prev_angles[2]);
// }
