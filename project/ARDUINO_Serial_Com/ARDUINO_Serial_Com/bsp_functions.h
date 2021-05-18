
#ifndef BSP_F
#define BSP_F

#include <Wire.h>
#include <ReducedMPU9250.h>
#include <CalculateAngles.h>

//ReducedMPU9250 accelMag;

bool init_IMU(ReducedMPU9250 *accelMag);
void ValoresAccyMag(ReducedMPU9250 *accelMag,float* AccG, float* MagCal);
void updateAccelerometerCalibrationOffsets(ReducedMPU9250 *accelMag,int axisIndex, int increment);
void updateAccelerometerCalibrationScale(ReducedMPU9250 *accelMag,int axisIndex, int increment);

#endif