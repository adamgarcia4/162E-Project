#ifndef IMU_H
#define IMU_H

class IMU {
public:
   IMU(float* angleArr, int fracUpdate);
   ~IMU();
   void measureAngles();
   void loop(float * angleArr);
   void updateAngles();
   void printAngles();

private:

};


#endif
