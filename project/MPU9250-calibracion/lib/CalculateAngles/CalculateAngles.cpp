#include "CalculateAngles.h"
using namespace std;

//Cross product for two vectors
void CalculateAngles::CrossProduct(float* firstVector, float* secondVector, float* resultVector) {
    resultVector[0] = firstVector[1]*secondVector[2] - firstVector[2]*secondVector[1];
    resultVector[1] = firstVector[2]*secondVector[0] - firstVector[0]*secondVector[2];
    resultVector[2] = firstVector[0]*secondVector[1] - firstVector[1]*secondVector[0];
}

//Norm calculation for a vector
float CalculateAngles::CalculateNorm(float vector[][SIZE], int index) {
    float normValue = sqrt( (pow(vector[0][index], 2) + pow(vector[1][index], 2) + pow(vector[2][index], 2)));
    return normValue;
}

void CalculateAngles::custom_memcpy(void *dest, void *src, size_t n) {
   int i;
   //cast src and dest to char*
   char *src_char = (char *)src;
   char *dest_char = (char *)dest;
   for (i=0; i<n; i++)
      dest_char[i] = src_char[i]; //copy contents byte by byte
}

//Calculate phase angles for magnetometer and accelerometer
void CalculateAngles::CalculatePhaseAngleAccelerometerMagnetometer(float* accelerometerData, float* magnetometerData, float* orientationData) {

    //Initial data for operating
    float RotationMatrix[SIZE][SIZE] = {{0.0, 0.0, 0.0},
                                        {0.0, 0.0, 0.0},
                                        {0.0, 0.0, 0.0}};
                                        
    float resultVector[SIZE]         = {0.0, 0.0, 0.0};
    float resultVectorAux[SIZE]      = {0.0, 0.0, 0.0};

    //R1 = crossProduct(crossProduct(A,M),A);
    CalculateAngles::CrossProduct(accelerometerData, magnetometerData, resultVector);
    CalculateAngles::custom_memcpy((void*)resultVectorAux, (void*)resultVector, SIZE*sizeof(float));  
    CalculateAngles::CrossProduct(resultVectorAux, accelerometerData, resultVector);

    RotationMatrix[0][0] = resultVector[0];
    RotationMatrix[1][0] = resultVector[1];
    RotationMatrix[2][0] = resultVector[2];

    //R2 = crossProduct(A,M);
    CalculateAngles::CrossProduct(accelerometerData, magnetometerData, resultVector);
    RotationMatrix[0][1] = resultVector[0];
    RotationMatrix[1][1] = resultVector[1];
    RotationMatrix[2][1] = resultVector[2];

    //R3 = A;
    RotationMatrix[0][2] = accelerometerData[0];
    RotationMatrix[1][2] = accelerometerData[1];
    RotationMatrix[2][2] = accelerometerData[2];

    //Calculate Norm
    float r1Norm = CalculateAngles::CalculateNorm(RotationMatrix, 0);
    float r2Norm = CalculateAngles::CalculateNorm(RotationMatrix, 1);
    float r3Norm = CalculateAngles::CalculateNorm(RotationMatrix, 2);

    //R1 = R1 / calculateNorm(R1);
    RotationMatrix[0][0] /= r1Norm;
    RotationMatrix[1][0] /= r1Norm;
    RotationMatrix[2][0] /= r1Norm;
    
    //R2 = R2 / calculateNorm(R2);
    RotationMatrix[0][1] /= r2Norm;
    RotationMatrix[1][1] /= r2Norm;
    RotationMatrix[2][1] /= r2Norm;

    //R3 = R3 / calculateNorm(R3);
    RotationMatrix[0][2] /= r3Norm;
    RotationMatrix[1][2] /= r3Norm;
    RotationMatrix[2][2] /= r3Norm;

    //Orientation from Rotation matrix (R)
    float theta = -asin(RotationMatrix[0][2]);
    float psi   = atan2(RotationMatrix[1][2]/cos(theta),RotationMatrix[2][2]/cos(theta));
    float rho   = atan2(RotationMatrix[0][1]/cos(theta),RotationMatrix[0][0]/cos(theta));

    float deg1 = rho * RADTODEG;
    float deg2 = theta * RADTODEG;
    float deg3 = psi * RADTODEG;

    //Orientation vector, results
    orientationData[0] = deg1;
    orientationData[1] = deg2;
    orientationData[2] = deg3;
    
}

