
#include "ReducedMPU9250.h"


/** Default or Specific address constructor.
 * @param address I2C address
 * @see MPU9250_DEFAULT_ADDRESS
 * @see MPU9250_ADDRESS_AD0_LOW
 * @see MPU9250_ADDRESS_AD0_HIGH
 */
ReducedMPU9250::ReducedMPU9250(bool highAddress) {
    if (highAddress) {
        devAddr = MPU9250_ADDRESS_AD0_HIGH;
        i2c_level = 1;
    } else {
        devAddr = MPU9250_ADDRESS_AD0_LOW;
        i2c_level = 0;
    }
    
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool ReducedMPU9250::testConnection() {
    return getDeviceID() == 0x71;
}

/** Get Device ID.
 * This register is used to verify the identity of the device (0b110100, 0x34).
 * @return Device ID (6 bits only! should be 0x34)
 * @see MPU9250_RA_WHO_AM_I
 * @see MPU9250_WHO_AM_I_BIT
 * @see MPU9250_WHO_AM_I_LENGTH
 */
uint8_t ReducedMPU9250::getDeviceID() {
    I2Cdev::readBits(devAddr, MPU9250_RA_WHO_AM_I, MPU9250_WHO_AM_I_BIT, MPU9250_WHO_AM_I_LENGTH, buffer);
    return buffer[0];
}

/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to their most sensitive settings, namely +/- 2g and +/- 250 degrees/sec, and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source.
 */
void ReducedMPU9250::initialize() {
    // Set clock source
    //setClockSource(MPU9250_CLOCK_PLL_XGYRO);
    setClockSource(0x05);
    // Set giroscope full range value
    setFullScaleGyroRange(MPU9250_GYRO_FS_250);
    // Set accelerometer full range value
    setFullScaleAccelRange(MPU9250_ACCEL_FS_2);
    accelerometerScaleFactor = 2.0;
    // Disable sleep mode
    setSleepEnabled(false); 
    // Get calibration values from eeprom memory
    ReadEepromCalibrationValues_Magnetometer();
    ReadEepromCalibrationValues_Accelerometer();
}

/** Set clock source setting.
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see getClockSource()
 * @see MPU9250_RA_PWR_MGMT_1
 * @see MPU9250_PWR1_CLKSEL_BIT
 * @see MPU9250_PWR1_CLKSEL_LENGTH
 */
void ReducedMPU9250::setClockSource(uint8_t source) {
    I2Cdev::writeBits(devAddr, MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_CLKSEL_BIT, MPU9250_PWR1_CLKSEL_LENGTH, source);
}


/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU9250_GYRO_FS_250
 * @see MPU9250_RA_GYRO_CONFIG
 * @see MPU9250_GCONFIG_FS_SEL_BIT
 * @see MPU9250_GCONFIG_FS_SEL_LENGTH
 */
void ReducedMPU9250::setFullScaleGyroRange(uint8_t range) {
    I2Cdev::writeBits(devAddr, MPU9250_RA_GYRO_CONFIG, MPU9250_GCONFIG_FS_SEL_BIT, MPU9250_GCONFIG_FS_SEL_LENGTH, range);
}

/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 */
void ReducedMPU9250::setFullScaleAccelRange(uint8_t range) {
    I2Cdev::writeBits(devAddr, MPU9250_RA_ACCEL_CONFIG, MPU9250_ACONFIG_AFS_SEL_BIT, MPU9250_ACONFIG_AFS_SEL_LENGTH, range);
}

/** Set sleep mode status.
 * @param enabled New sleep mode enabled status
 * @see getSleepEnabled()
 * @see MPU9250_RA_PWR_MGMT_1
 * @see MPU9250_PWR1_SLEEP_BIT
 */
void ReducedMPU9250::setSleepEnabled(bool enabled) {
    I2Cdev::writeBit(devAddr, MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_SLEEP_BIT, enabled);
}






/** Get 3-axis accelerometer readings.
 * These registers store the most recent accelerometer measurements.
 * Accelerometer measurements are written to these registers at the Sample Rate
 * as defined in Register 25.
 *
 * The accelerometer measurement registers, along with the temperature
 * measurement registers, gyroscope measurement registers, and external sensor
 * data registers, are composed of two sets of registers: an internal register
 * set and a user-facing read register set.
 *
 * The data within the accelerometer sensors' internal register set is always
 * updated at the Sample Rate. Meanwhile, the user-facing read register set
 * duplicates the internal register set's data values whenever the serial
 * interface is idle. This guarantees that a burst read of sensor registers will
 * read measurements from the same sampling instant. Note that if burst reads
 * are not used, the user is responsible for ensuring a set of single byte reads
 * correspond to a single sampling instant by checking the Data Ready interrupt.
 *
 * Each 16-bit accelerometer measurement has a full scale defined in ACCEL_FS
 * (Register 28). For each full scale setting, the accelerometers' sensitivity
 * per LSB in ACCEL_xOUT is shown in the table below:
 *
 * <pre>
 * AFS_SEL | Full Scale Range | LSB Sensitivity
 * --------+------------------+----------------
 * 0       | +/- 2g           | 8192 LSB/mg
 * 1       | +/- 4g           | 4096 LSB/mg
 * 2       | +/- 8g           | 2048 LSB/mg
 * 3       | +/- 16g          | 1024 LSB/mg
 * </pre>
 *
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 * @see MPU9250_RA_GYRO_XOUT_H
 */
void ReducedMPU9250::getAccelerometerReading(int16_t* x, int16_t* y, int16_t* z) {
    I2Cdev::readBytes(devAddr, MPU9250_RA_ACCEL_XOUT_H, 6, buffer);
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}


/** Get 3-axis gyroscope readings.
 * These gyroscope measurement registers, along with the accelerometer
 * measurement registers, temperature measurement registers, and external sensor
 * data registers, are composed of two sets of registers: an internal register
 * set and a user-facing read register set.
 * The data within the gyroscope sensors' internal register set is always
 * updated at the Sample Rate. Meanwhile, the user-facing read register set
 * duplicates the internal register set's data values whenever the serial
 * interface is idle. This guarantees that a burst read of sensor registers will
 * read measurements from the same sampling instant. Note that if burst reads
 * are not used, the user is responsible for ensuring a set of single byte reads
 * correspond to a single sampling instant by checking the Data Ready interrupt.
 *
 * Each 16-bit gyroscope measurement has a full scale defined in FS_SEL
 * (Register 27). For each full scale setting, the gyroscopes' sensitivity per
 * LSB in GYRO_xOUT is shown in the table below:
 *
 * <pre>
 * FS_SEL | Full Scale Range   | LSB Sensitivity
 * -------+--------------------+----------------
 * 0      | +/- 250 degrees/s  | 131 LSB/deg/s
 * 1      | +/- 500 degrees/s  | 65.5 LSB/deg/s
 * 2      | +/- 1000 degrees/s | 32.8 LSB/deg/s
 * 3      | +/- 2000 degrees/s | 16.4 LSB/deg/s
 * </pre>
 *
 * @param x 16-bit signed integer container for X-axis rotation
 * @param y 16-bit signed integer container for Y-axis rotation
 * @param z 16-bit signed integer container for Z-axis rotation
 * @see getMotion6()
 * @see MPU9250_RA_GYRO_XOUT_H
 */
void ReducedMPU9250::getGyroscopeReading(int16_t* x, int16_t* y, int16_t* z) {
    I2Cdev::readBytes(devAddr, MPU9250_RA_GYRO_XOUT_H, 6, buffer);
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}



/** Get 3-axis magnetometer readings.
**/
void ReducedMPU9250::getMagnetometerReading(int16_t* x, int16_t* y, int16_t* z, bool* resultOk) {
    // Set i2c bypass enable pin to true to access magnetometer
    if(!magnetometerIsEnabled)
    {
    	I2Cdev::writeByte(devAddr, MPU9250_RA_INT_PIN_CFG, 0x02); 
    	delay(10);
    	// Enable the magnetometer for a continuous measurement mode reading
	    I2Cdev::writeByte(MPU9250_RA_MAG_ADDRESS, 0x0A, 0x02);
	    delay(10);
    	// Set the class control variable to true
    	magnetometerIsEnabled = true;
    }
    // Enable the magnetometer for a single reading
    //I2Cdev::writeByte(MPU9250_RA_MAG_ADDRESS, 0x0A, 0x01);
    //delay(10);
    // Read the 6 registers corresponding to the x, y, z axis and the Status one
    I2Cdev::readBytes(MPU9250_RA_MAG_ADDRESS, MPU9250_RA_MAG_XOUT_L, 7, buffer);
    // Verify that magnetic error overflow did not ocure
    uint8_t HOFL = buffer[6] & 0x08;
    if(HOFL == 0)
    {   resultOk[0] = true;
        x[0] = decodeMagnetometerBytes(buffer[1], buffer[0]);
        y[0] = decodeMagnetometerBytes(buffer[3], buffer[2]);
        z[0] = decodeMagnetometerBytes(buffer[5], buffer[4]);
    }else{
        resultOk[0] = false;
    }
}


void ReducedMPU9250::disableMagnetometer() {
	// Turn off the magnetometer
    I2Cdev::writeByte(MPU9250_RA_MAG_ADDRESS, 0x0A, 0x00);
    delay(10);
	// Set i2c bypass enable pin to false to prevent access magnetometer
	I2Cdev::writeByte(devAddr, MPU9250_RA_INT_PIN_CFG, 0x00); 
    delay(10);
    // Set the class control variable to false
    magnetometerIsEnabled = false;
}


/** Decode magnetometer bytes.
Convert 2's complement and little endian data to integer values
**/
int16_t ReducedMPU9250::decodeMagnetometerBytes(unsigned char highPart, unsigned char lowPart)
{
    int value;
    if( (highPart & 1<<7) == 0 )
    {
        value = ((int)(highPart) << 8) | (int)lowPart;
    }else{
        highPart = ~highPart;
        lowPart = ~lowPart;
        value = ((int)(highPart) << 8) | (int)lowPart;
        value = (-1)*(value+1);
    } 
    return (int16_t)value;
}










void ReducedMPU9250::getLinearAccelerationG(float *x, float *y, float *z, bool adjustAxis)
{   
    getAccelerometerReading(&Acc[0], &Acc[1], &Acc[2]);
    // Scale the read value according to the 
    // For accelerometer full range = MPU9250_ACCEL_FS_2 the read value has to be multiplied by 2
    // For accelerometer full range = MPU9250_ACCEL_FS_4 the read value has to be multiplied by 4
    // For accelerometer full range = MPU9250_ACCEL_FS_8 the read value has to be multiplied by 8
    // For accelerometer full range = MPU9250_ACCEL_FS_16 the read value has to be multiplied by 32
    x[0] = (((float)Acc[0])*accelerometerScaleFactor) / 32768;
    y[0] = (((float)Acc[1])*accelerometerScaleFactor) / 32768;
    z[0] = (((float)Acc[2])*accelerometerScaleFactor) / 32768;
    // Ajust the axis if desired
    if(adjustAxis)
    {
    	float supportVariable;
        supportVariable = x[0];
        x[0] = -y[0];
        y[0] = -supportVariable;
    }

    // Custom post-calibration
    /*
    x[0] = (x[0] + (float)AccCalOffsets[0]/127)*((float)(AccCalScale[0]+128)/128);
    y[0] = (y[0] + (float)AccCalOffsets[1]/127)*((float)(AccCalScale[1]+128)/128);
    z[0] = (z[0] + (float)AccCalOffsets[2]/127)*((float)(AccCalScale[2]+128)/128);
	*/

	x[0] = (x[0] + AccCalOffsets[0]/100)*(AccCalScale[0]/100);
    y[0] = (y[0] + AccCalOffsets[1]/100)*(AccCalScale[1]/100);
    z[0] = (z[0] + AccCalOffsets[2]/100)*(AccCalScale[2]/100);

}


void ReducedMPU9250::getLinearAccelerationMS(float *x, float *y, float *z, bool adjustAxis)
{
    getLinearAccelerationG(x, y, z, adjustAxis);
    x[0] = x[0] * GRAVITY_ACCELERATION;
    y[0] = y[0] * GRAVITY_ACCELERATION;
    z[0] = z[0] * GRAVITY_ACCELERATION;
}




void ReducedMPU9250::getMagnetometerCalibratedData(float *x, float *y, float *z, bool* resultOk)
{
    getMagnetometerReading(&Mag[0], &Mag[1], &Mag[2], resultOk);
    if(resultOk[0])
    {
        x[0] = 2*( ((float)Mag[0] - MagCalMin[0]) / (MagCalMax[0] - MagCalMin[0]) - 0.5);
        y[0] = 2*( ((float)Mag[1] - MagCalMin[1]) / (MagCalMax[1] - MagCalMin[1]) - 0.5);
        z[0] = 2*( ((float)Mag[2] - MagCalMin[2]) / (MagCalMax[2] - MagCalMin[2]) - 0.5);
    }

}




void ReducedMPU9250::CalibrateMagnetometer()
{
    Serial.println("\n\n Calibration started ");
    Serial.println(" Move the device in all directions for a while ");
    // Initialize the min and max values
    int16_t xMin;
    int16_t xMax;
    int16_t yMin;
    int16_t yMax;
    int16_t zMin;
    int16_t zMax;
    bool resultOk = false;
    while(!resultOk){
        getMagnetometerReading(&Mag[0], &Mag[1], &Mag[2], &resultOk);
    }
    xMin = Mag[0];
    xMax = xMin;
    yMin = Mag[1];
    yMax = yMin;
    zMin = Mag[2];
    zMax = zMin;
    // Calculate the min and max values
    for(long i=0; i<MAGNETOMETER_CALIBRATION_ITERATIONS; i++)
    {
        resultOk = false;
        while(!resultOk){
            getMagnetometerReading(&Mag[0], &Mag[1], &Mag[2], &resultOk);
        }
        if(Mag[0] > xMax){ xMax = Mag[0]; } 
        if(Mag[0] < xMin){ xMin = Mag[0]; } 
        if(Mag[1] > yMax){ yMax = Mag[1]; } 
        if(Mag[1] < yMin){ yMin = Mag[1]; } 
        if(Mag[2] > zMax){ zMax = Mag[2]; } 
        if(Mag[2] < zMin){ zMin = Mag[2]; } 
    }
    // Store the values as float in the class variable
    MagCalMax[0] = (float)xMax;
    MagCalMax[1] = (float)yMax;
    MagCalMax[2] = (float)zMax;
    MagCalMin[0] = (float)xMin;
    MagCalMin[1] = (float)yMin;
    MagCalMin[2] = (float)zMin;
    // Store the values in the persistent memory for future use
    WriteEepromCalibrationValues_Magnetometer();
    // Display new calibration values
    Serial.println("  ");
    Serial.println(" New magnetometer calibration values: ");
    Serial.print(MagCalMin[0]);
    Serial.print("  ");
    Serial.print(MagCalMax[0]);
    Serial.print(" | ");
    Serial.print(MagCalMin[1]);
    Serial.print("  ");
    Serial.print(MagCalMax[1]);
    Serial.print(" | ");
    Serial.print(MagCalMin[2]);
    Serial.print("  ");
    Serial.println(MagCalMax[2]);
    Serial.println("");
}



void ReducedMPU9250::ReadEepromCalibrationValues_Magnetometer()
{
    if(i2c_level==0)
    {
        // Magnetometer X valueS
        MagCalMin[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_LOW);
        MagCalMax[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_X_I2C_LOW);
        // Magnetometer Y valueS
        MagCalMin[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Y_I2C_LOW);
        MagCalMax[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Y_I2C_LOW);
        // Magnetometer z valueS
        MagCalMin[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Z_I2C_LOW);
        MagCalMax[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Z_I2C_LOW);
    }
    else
    {
        // Magnetometer X valueS
        MagCalMin[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_HIGH);
        MagCalMax[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_X_I2C_HIGH);
        // Magnetometer Y valueS
        MagCalMin[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Y_I2C_HIGH);
        MagCalMax[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Y_I2C_HIGH);
        // Magnetometer z valueS
        MagCalMin[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Z_I2C_HIGH);
        MagCalMax[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Z_I2C_HIGH);
    }
    /*
    Serial.println("  ");
    Serial.println(" Magnetometer calibration values: ");
    Serial.print(MagCalMin[0]);
    Serial.print("  ");
    Serial.print(MagCalMax[0]);
    Serial.print(" | ");
    Serial.print(MagCalMin[1]);
    Serial.print("  ");
    Serial.print(MagCalMax[1]);
    Serial.print(" | ");
    Serial.print(MagCalMin[2]);
    Serial.print("  ");
    Serial.println(MagCalMax[2]);
    Serial.println("");
    */
}


void ReducedMPU9250::WriteEepromCalibrationValues_Magnetometer()
{
    if(i2c_level==0)
    {
        // Magnetometer X valueS
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_LOW, (int)MagCalMin[0]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_X_I2C_LOW, (int)MagCalMax[0]);
        // Magnetometer Y valueS
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Y_I2C_LOW, (int)MagCalMin[1]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Y_I2C_LOW, (int)MagCalMax[1]);
        // Magnetometer < valueS
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Z_I2C_LOW, (int)MagCalMin[2]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Z_I2C_LOW, (int)MagCalMax[2]);
    }
    else
    {
        // Magnetometer X valueS
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_HIGH, (int)MagCalMin[0]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_X_I2C_HIGH, (int)MagCalMax[0]);
        // Magnetometer Y valueS
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Y_I2C_HIGH, (int)MagCalMin[1]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Y_I2C_HIGH, (int)MagCalMax[1]);
        // Magnetometer < valueS
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_Z_I2C_HIGH, (int)MagCalMin[2]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MAX_Z_I2C_HIGH, (int)MagCalMax[2]);
    }
}


// Little endian order
// 2 bytes value
int ReducedMPU9250::readSingleIntFromEeprom(int address)
{
    return (int)EEPROM.read(address) | ((int)EEPROM.read(address + 1) << 8);
}

// Little endian order
// 2 bytes value
void ReducedMPU9250::writeSingleIntFromEeprom(int address, int value)
{
    EEPROM.write(address, (char)value);
    EEPROM.write(address+1, (char)(value>>8));
}









// -- NEW -- Accelerometer calibration functions -- //

void ReducedMPU9250::UpdateAccelerometerOffset(int axisIndex, int increment)
{   
    // Offset values
    if(axisIndex >= 0 && axisIndex < 3)
    {
    	//AccCalOffsets[axisIndex] = constrain(AccCalOffsets[axisIndex] + increment, -127, 127);
    	AccCalOffsets[axisIndex] = constrain(AccCalOffsets[axisIndex] + increment, -127, 127);
	}
	WriteEepromCalibrationValues_Accelerometer();

	Serial.print("Calibration values | ");
	Serial.print(AccCalOffsets[0]);
	Serial.print(", ");
	Serial.print(AccCalOffsets[1]);
	Serial.print(", ");
	Serial.print(AccCalOffsets[2]);
	Serial.print(" | ");
	Serial.print(AccCalScale[0]);
	Serial.print(", ");
	Serial.print(AccCalScale[1]);
	Serial.print(", ");
	Serial.print(AccCalScale[2]);
	Serial.println("  ");

}



void ReducedMPU9250::UpdateAccelerometerScale(int axisIndex, int increment)
{   
    // Scale values
    if(axisIndex >= 0 && axisIndex < 3)
    {
    	AccCalScale[axisIndex] = constrain(AccCalScale[axisIndex] + increment, -127, 127); 
	}
	WriteEepromCalibrationValues_Accelerometer();

	Serial.print("Calibration values | ");
	Serial.print(AccCalOffsets[0]);
	Serial.print(", ");
	Serial.print(AccCalOffsets[1]);
	Serial.print(", ");
	Serial.print(AccCalOffsets[2]);
	Serial.print(" | ");
	Serial.print(AccCalScale[0]);
	Serial.print(", ");
	Serial.print(AccCalScale[1]);
	Serial.print(", ");
	Serial.print(AccCalScale[2]);
	Serial.println("  ");
}




void ReducedMPU9250::WriteEepromCalibrationValues_Accelerometer()
{
    if(i2c_level==0)
    {
    	// Accelerometer offset values
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_X_I2C_LOW, (int)AccCalOffsets[0]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Y_I2C_LOW, (int)AccCalOffsets[1]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Z_I2C_LOW, (int)AccCalOffsets[2]);
		// Accelerometer scale values
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_X_I2C_LOW, (int)AccCalScale[0]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Y_I2C_LOW, (int)AccCalScale[1]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Z_I2C_LOW, (int)AccCalScale[2]);
    }
    else
    {
    	// Accelerometer offset values
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_X_I2C_HIGH, (int)AccCalOffsets[0]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Y_I2C_HIGH, (int)AccCalOffsets[1]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Z_I2C_HIGH, (int)AccCalOffsets[2]);
		// Accelerometer scale values
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_X_I2C_HIGH, (int)AccCalScale[0]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Y_I2C_HIGH, (int)AccCalScale[1]);
        writeSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Z_I2C_HIGH, (int)AccCalScale[2]);
    }
}


void ReducedMPU9250::ReadEepromCalibrationValues_Accelerometer()
{
	//MagCalMin[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_MAG_MIN_X_I2C_LOW);


    if(i2c_level==0)
    {
    	// Accelerometer offset values
    	AccCalOffsets[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_X_I2C_LOW);
    	AccCalOffsets[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Y_I2C_LOW);
    	AccCalOffsets[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Z_I2C_LOW);
    	// Accelerometer scale values
    	AccCalScale[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_X_I2C_LOW);
    	AccCalScale[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Y_I2C_LOW);
    	AccCalScale[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Z_I2C_LOW);
    }
    else
    {
    	// Accelerometer offset values
    	AccCalOffsets[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_X_I2C_HIGH);
    	AccCalOffsets[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Y_I2C_HIGH);
    	AccCalOffsets[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_OFFSET_Z_I2C_HIGH);
    	// Accelerometer scale values
    	AccCalScale[0] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_X_I2C_HIGH);
    	AccCalScale[1] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Y_I2C_HIGH);
    	AccCalScale[2] = (float)readSingleIntFromEeprom(EEPROM_ADDRESS_CALIBRATION_ACC_SCALE_Z_I2C_HIGH);
    }

    Serial.print("Calibration values | ");
	Serial.print(AccCalOffsets[0]);
	Serial.print(", ");
	Serial.print(AccCalOffsets[1]);
	Serial.print(", ");
	Serial.print(AccCalOffsets[2]);
	Serial.print(" | ");
	Serial.print(AccCalScale[0]);
	Serial.print(", ");
	Serial.print(AccCalScale[1]);
	Serial.print(", ");
	Serial.print(AccCalScale[2]);
	Serial.println("  ");
}



