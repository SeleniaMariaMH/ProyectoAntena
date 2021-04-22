

#ifndef _ReducedMPU9250_H_
#define _ReducedMPU9250_H_

#include "Arduino.h"
#include "I2Cdev.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>


//Magnetometer Registers
//    I2C Adress
#define MPU9250_RA_MAG_ADDRESS      0x0C
//    Axis measurements
#define MPU9250_RA_MAG_XOUT_L       0x03
#define MPU9250_RA_MAG_XOUT_H       0x04
#define MPU9250_RA_MAG_YOUT_L       0x05
#define MPU9250_RA_MAG_YOUT_H       0x06
#define MPU9250_RA_MAG_ZOUT_L       0x07
#define MPU9250_RA_MAG_ZOUT_H       0x08
//    Status
#define MPU9250_RA_MAG_ST2          0x08
//    Control 1
#define MPU9250_RA_MAG_CNTL1        0x0A
//     Bypass enable pin to access magnetometer from MPU9250
#define MPU9250_RA_INT_PIN_CFG      0x37



// MPU9250 Registers
//    I2C address
#define MPU9250_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU9250_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU9250_DEFAULT_ADDRESS     MPU9250_ADDRESS_AD0_LOW

//    Device ID
#define MPU9250_RA_WHO_AM_I         0x75
#define MPU9250_WHO_AM_I_BIT        6
#define MPU9250_WHO_AM_I_LENGTH     8

//    Sleep mode control
#define MPU9250_RA_PWR_MGMT_1       0x6B
#define MPU9250_PWR1_SLEEP_BIT      6

//    Clock setup
#define MPU9250_CLOCK_PLL_XGYRO         0x01
#define MPU9250_RA_PWR_MGMT_1           0x6B
#define MPU9250_PWR1_CLKSEL_BIT         2
#define MPU9250_PWR1_CLKSEL_LENGTH      3

//    Gyroscope full range configuration
#define MPU9250_RA_GYRO_CONFIG          0x1B
#define MPU9250_GCONFIG_FS_SEL_BIT      4
#define MPU9250_GCONFIG_FS_SEL_LENGTH   2

//    Gyroscope available ranges
#define MPU9250_GYRO_FS_250             0x00
#define MPU9250_GYRO_FS_500             0x01
#define MPU9250_GYRO_FS_1000            0x02
#define MPU9250_GYRO_FS_2000            0x03

//    Accelerometer full range configuration
#define MPU9250_RA_ACCEL_CONFIG     0x1C
#define MPU9250_ACONFIG_AFS_SEL_BIT         4
#define MPU9250_ACONFIG_AFS_SEL_LENGTH      2

//    Accelerometer available ranges
#define MPU9250_ACCEL_FS_2          0x00
#define MPU9250_ACCEL_FS_4          0x01
#define MPU9250_ACCEL_FS_8          0x02
#define MPU9250_ACCEL_FS_16         0x03

//    Axis measurements
#define MPU9250_RA_ACCEL_XOUT_H     0x3B
#define MPU9250_RA_ACCEL_XOUT_L     0x3C
#define MPU9250_RA_ACCEL_YOUT_H     0x3D
#define MPU9250_RA_ACCEL_YOUT_L     0x3E
#define MPU9250_RA_ACCEL_ZOUT_H     0x3F
#define MPU9250_RA_ACCEL_ZOUT_L     0x40
#define MPU9250_RA_GYRO_XOUT_H      0x43
#define MPU9250_RA_GYRO_XOUT_L      0x44
#define MPU9250_RA_GYRO_YOUT_H      0x45
#define MPU9250_RA_GYRO_YOUT_L      0x46
#define MPU9250_RA_GYRO_ZOUT_H      0x47
#define MPU9250_RA_GYRO_ZOUT_L      0x48


// Custom constants

//    Eeprom addresses for persistent data
#define EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_LOW 0
#define EEPROM_ADDRESS_CALIBRATION_MAG_MAX_X_I2C_LOW 2
#define EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Y_I2C_LOW 4
#define EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Y_I2C_LOW 6
#define EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Z_I2C_LOW 8
#define EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Z_I2C_LOW 10

#define EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_HIGH 12
#define EEPROM_ADDRESS_CALIBRATION_MAG_MAX_X_I2C_HIGH 14
#define EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Y_I2C_HIGH 16
#define EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Y_I2C_HIGH 18
#define EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Z_I2C_HIGH 20
#define EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Z_I2C_HIGH 22

#define EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_X_I2C_LOW 23
#define EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Y_I2C_LOW 25
#define EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Z_I2C_LOW 27
#define EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_X_I2C_LOW 29
#define EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Y_I2C_LOW 31
#define EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Z_I2C_LOW 33

#define EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_X_I2C_HIGH 35
#define EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Y_I2C_HIGH 37
#define EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Z_I2C_HIGH 39
#define EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_X_I2C_HIGH 41
#define EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Y_I2C_HIGH 43
#define EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Z_I2C_HIGH 45





//    Calibration iterations
#define MAGNETOMETER_CALIBRATION_ITERATIONS 1000 //1000

//    Pi value for calculations
//#define PI 3.1415926535897932384626433832795
#define GRAVITY_ACCELERATION 9.81



// note: DMP code memory blocks defined at end of header file

class ReducedMPU9250 {

    public:

        ReducedMPU9250(bool staticPart);

        void initialize();
        bool testConnection();
        uint8_t getDeviceID();

        void setClockSource(uint8_t source);
        void setFullScaleGyroRange(uint8_t range);
        void setFullScaleAccelRange(uint8_t range);
        void setSleepEnabled(bool enabled);

        void getAccelerometerReading(int16_t* x, int16_t* y, int16_t* z);
        void getGyroscopeReading(int16_t* x, int16_t* y, int16_t* z);
        void getMagnetometerReading(int16_t* x, int16_t* y, int16_t* z, bool* resultOk);
        int16_t decodeMagnetometerBytes(unsigned char highPart, unsigned char lowPart);

        void getLinearAccelerationG(float *x, float *y, float *z, bool adjustAxis);
        void getLinearAccelerationMS(float *x, float *y, float *z, bool adjustAxis);

        void getMagnetometerCalibratedData(float *x, float *y, float *z, bool* resultOk);
        void disableMagnetometer();
        void CalibrateMagnetometer();

        void ReadEepromCalibrationValues_Magnetometer();
        void WriteEepromCalibrationValues_Magnetometer();
        int readSingleIntFromEeprom(int address);
        void writeSingleIntFromEeprom(int address, int value);

        void UpdateAccelerometerOffset(int axisIndex, int increment);
        void UpdateAccelerometerScale(int axisIndex, int increment);

        void ReadEepromCalibrationValues_Accelerometer();
        void WriteEepromCalibrationValues_Accelerometer();


    private:
        uint8_t devAddr;
        uint8_t buffer[14];
        int16_t Acc[3] = {0, 0, 0};
        int16_t Mag[3] = {0, 0, 0};
        float accelerometerScaleFactor;

        bool magnetometerIsEnabled = false;
        
        float MagCalMax[3] = {0.0, 0.0, 0.0};
        float MagCalMin[3] = {0.0, 0.0, 0.0};

        char i2c_level = 0;

        float AccCalOffsets[3] = {0.0, 0.0, 0.0};
        float AccCalScale[3] = {100.0, 100.0, 100.0};

};

#endif /* _MPU9250_H_ */

