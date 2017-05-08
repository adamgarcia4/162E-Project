#include "IMU.h"
#include "Arduino.h"
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Wire.h>


// Member Vars
float angles[3]; // yaw pitch roll
// float prev_angles[3];
FreeSixIMU* sixDOF;

IMU::IMU(){
   Serial.println("IMU started");
   sixDOF = new FreeSixIMU();
   Wire.begin();
   delay(5);
   sixDOF->init(); //begin the IMU
   sixDOF->getEuler(angles);
}

//<<destructor>>
IMU::~IMU(){/*nothing to destruct*/}

void IMU::updateAngles() {
  // memcpy(prev_angles, angles, sizeof(angles));
   sixDOF->getEuler(angles);
   for(int i = 0; i< (sizeof(prev_angles)/sizeof(*prev_angles)); i++) {
    prev_angles[i] = prev_angles[i] - angles[i];
   }

   Serial.print(angles[0]);
   Serial.print(" | ");
   Serial.print(angles[1]);
   Serial.print(" | ");
   Serial.println(angles[2]);
   Serial.print(" | ");
//   Serial.print(prev_angles[0]);
//   Serial.print(" | ");
//   Serial.print(prev_angles[1]);
//   Serial.print(" | ");
//   Serial.println(prev_angles[2]);

}
