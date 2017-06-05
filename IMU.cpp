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

// int m_rawData[6]; // | Y-acc | X-acc | Z-acc | Y-gyro | X-gyro | Z-gyro |

IMU::IMU(float* angleArr, int fracUpdate){
   Serial.println("IMU started");
   m_sixDOF = new FreeSixIMU();
   Wire.begin();
   delay(5);
   m_sixDOF->init();
   for(int i=0;i<3;i++) {
    m_resetAngles[i] = 0;
   }
}

//<<destructor>>
IMU::~IMU(){/*nothing to destruct*/}

void IMU::loop(float * angleArr, bool shouldPrint) {

   // Read Angles from IMU
   m_sixDOF->getEuler(m_angles);

   // Store Angle values for use
    angleArr[0] = m_angles[0]-m_resetAngles[0];
    angleArr[1] = m_angles[1]-m_resetAngles[1];
    angleArr[2] = m_angles[2]-m_resetAngles[2];

    if(shouldPrint) {
      Serial.print(angleArr[0]); // Rotation about Z (psi)
      Serial.print(" | ");
      Serial.print(angleArr[1]); // Rotation about X (theta)
      Serial.print(" | ");
      Serial.println(angleArr[2]); // Rotation about Y (phi)
      Serial.print(" | ");
   }

}

// Pseudo-Zero IMU readings
void IMU::reset() {
   for(int i=0;i<3;i++) {
      m_resetAngles[i] = m_angles[i];
   }
}
