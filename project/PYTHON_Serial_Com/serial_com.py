import serial
import time
arduino = serial.Serial('COM8', 9600)
time.sleep(2)
arduino.write(b'Hello World')
rawString = arduino.readline()
print(rawString)
arduino.close()
