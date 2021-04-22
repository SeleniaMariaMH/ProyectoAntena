#include <stdio.h>
#include <math.h> 
//#include <cstring>
//#include "Arduino.h"

#define PI 3.141596
#define RADTODEG 180/PI
#define SIZE 3

// Preventing the user from declaring the library twice
#ifndef CalculateAngles_h  
#define CalculateAngles_h


class CalculateAngles{ 
  
  public:
    void CrossProduct(float* firstVector, float* secondVector, float* resultVector);
    float CalculateNorm(float vector[][SIZE], int index);
    void CalculatePhaseAngleAccelerometerMagnetometer(float* accelerometerData,
                                                    float* magnetometerData, 
                                                    float* orientationData);
    void custom_memcpy(void *dest, void *src, size_t n);

};



#endif