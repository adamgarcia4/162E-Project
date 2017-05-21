#ifndef IMU_H
#define IMU_H

class IMU {
public:
   IMU(float* angleArr, int fracUpdate);
   ~IMU();
   void loop(float * angleArr);
   void reset();
   // void readAngles();
   // void measureAngles();
   // void updateAngles();
   // void printAngles();

private:

};


#endif
