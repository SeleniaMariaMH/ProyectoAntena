// IMU.
#include "Wire.h"
#include <ReducedMPU9250.h>
#include <CalculateAngles.h>

ReducedMPU9250 accelMag(false);
CalculateAngles calculationObject;

int16_t Acc[3] = {0, 0, 0};
int16_t Mag[3] = {0, 0, 0};
float AccG[3] = {0.0, 0.0, 0.0};
float MagCal[3] = {0.0, 0.0, 0.0};
bool measureOk;
char appState = 'z';

void setup() 
{

  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  Serial.println("Initializing I2C devices...");
  accelMag.initialize(); 
  accelMag.setFullScaleAccelRange(MPU9250_ACCEL_FS_2);

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelMag.testConnection() ? "MPU9250-True connection successful" : "MPU9250 connection failed");

  delay(1000);
}

void loop() 
{
  //ValoresAccyMag();
  if(appState == 'r')
  {
    //Prueba();
    //ValoresAccyMag();
    accelMag.CalibrateMagnetometer();
    appState = 'z';
  }
  else if(appState == 'c')
  {
    ValoresAccyMag(AccG, MagCal);
  }
  else if(appState == 'o')
  {
    delay(1000);
    //appState = 'z';
  }

  delay(50);
}

void serialEvent() {
  while (Serial.available()) { 
    char inChar = (char)Serial.read(); 
    if(inChar == 'r')
    {
      appState = 'r';
    }
    else if(inChar == 'c')
    {
      appState = 'c';
    }
    else if(inChar == 'o')
    {
      appState = 'o';
    }
    else if(inChar == '1')
    {
      Serial.println(" Updating accelerometer X offset: +1 ");
      updateAccelerometerCalibrationOffsets(0, 1);  
    }
    else if(inChar == '2')
    {
      Serial.println(" Updating accelerometer X offset: -1 ");
      updateAccelerometerCalibrationOffsets(0, -1);  
    }
    else if(inChar == '3')
    {
      Serial.println(" Updating accelerometer y offset: +1 ");
      updateAccelerometerCalibrationOffsets(1, 1);  
    }
    else if(inChar == '4')
    {
      Serial.println(" Updating accelerometer y offset: -1 ");
      updateAccelerometerCalibrationOffsets(1, -1);  
    }
    else if(inChar == '5')
    {
      Serial.println(" Updating accelerometer z offset: +1 ");
      updateAccelerometerCalibrationOffsets(2, 1);  
    }
    else if(inChar == '6')
    {
      Serial.println(" Updating accelerometer z offset: -1 ");
      updateAccelerometerCalibrationOffsets(2, -1);  
    }
    
    // Accelerometer calibration scale
    else if(inChar == '7')
    {
      Serial.println(" Updating accelerometer X scale: +1 ");
      updateAccelerometerCalibrationScale(0, 1);  
    }
    else if(inChar == '8')
    {
      Serial.println(" Updating accelerometer X scale: -1 ");
      updateAccelerometerCalibrationScale(0, -1);  
    }
    else if(inChar == '9')
    {
      Serial.println(" Updating accelerometer y scale: +1 ");
      updateAccelerometerCalibrationScale(1, 1);  
    }
    else if(inChar == 'q')
    {
      Serial.println(" Updating accelerometer y scale: -1 ");
      updateAccelerometerCalibrationScale(1, -1);  
    }
    else if(inChar == 'w')
    {
      Serial.println(" Updating accelerometer z scale: +1 ");
      updateAccelerometerCalibrationScale(2, 1);  
    }
    else if(inChar == 'e')
    {
      Serial.println(" Updating accelerometer z scale: -1 ");
      updateAccelerometerCalibrationScale(2, -1);  
    }
  }   
}
void ValoresAccyMag(float* AccG, float* MagCal)
{ 
  accelMag.getLinearAccelerationG(&AccG[0], &AccG[1], &AccG[2], true);
  accelMag.getMagnetometerCalibratedData(&MagCal[0], &MagCal[1], &MagCal[2], &measureOk);
  //accelMag.getMagnetometerReading(&Mag[0], &Mag[1], &Mag[2], &measureOk, true);

  // Imprimir por pantalla (comprobación). 
  Serial.println("Valores del acelerómetro en G y del magnetómetro calibrado:");
  Serial.print("AccG");
  Serial.print("   ");
  Serial.print("AccG");
  Serial.print("   ");
  Serial.print("AccG");
  Serial.print("     |     ");
  Serial.print("MagCal");
  Serial.print("   ");
  Serial.print("MagCal");
  Serial.print("   ");
  Serial.print("MagCal");
  Serial.println("");
  Serial.print(AccG[0]);
  Serial.print("   ");
  Serial.print(AccG[1]);
  Serial.print("   ");
  Serial.print(AccG[2]);
  Serial.print("   |   ");
  Serial.print(MagCal[0]);
  Serial.print("   ");
  Serial.print(MagCal[1]);
  Serial.print("   ");
  Serial.print(MagCal[2]);
  Serial.println("  ");
  delay(1000);
}

void updateAccelerometerCalibrationOffsets(int axisIndex, int increment)
{
  accelMag.UpdateAccelerometerOffset(axisIndex, increment);
}

void updateAccelerometerCalibrationScale(int axisIndex, int increment)
{
  accelMag.UpdateAccelerometerScale(axisIndex, increment);
}
