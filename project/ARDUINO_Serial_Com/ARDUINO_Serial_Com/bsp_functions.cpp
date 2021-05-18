
#include "bsp_functions.h"



bool init_IMU(ReducedMPU9250 *accelMag){
    Wire.begin();
    accelMag->initialize(); 
    accelMag->setFullScaleAccelRange(MPU9250_ACCEL_FS_2);

    return accelMag->testConnection();
}

void ValoresAccyMag(ReducedMPU9250 *accelMag,float* AccG, float* MagCal)
{ 
  bool measureOk;

  accelMag->getLinearAccelerationG(&AccG[0], &AccG[1], &AccG[2], true);
  accelMag->getMagnetometerCalibratedData(&MagCal[0], &MagCal[1], &MagCal[2], &measureOk);
  //accelMag.getMagnetometerReading(&Mag[0], &Mag[1], &Mag[2], &measureOk, true);

  // Imprimir por pantalla (comprobación). 
  
  Serial.print("G:");
  Serial.print(AccG[0]);
  Serial.print(" ");
  Serial.print(AccG[1]);
  Serial.print(" ");
  Serial.print(AccG[2]);
  Serial.print(" ");
  Serial.print(MagCal[0]);
  Serial.print(" ");
  Serial.print(MagCal[1]);
  Serial.print(" ");
  Serial.print(MagCal[2]);
  Serial.println();
  delay(1000);
}

void updateAccelerometerCalibrationOffsets(ReducedMPU9250 *accelMag,int axisIndex, int increment)
{
  accelMag->UpdateAccelerometerOffset(axisIndex, increment);
}

void updateAccelerometerCalibrationScale(ReducedMPU9250 *accelMag,int axisIndex, int increment)
{
  accelMag->UpdateAccelerometerScale(axisIndex, increment);
}

