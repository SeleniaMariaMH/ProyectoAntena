import serial
import time
import threading

class SerialCommunications:
    incData = ""

    def __init__(self,port,baudrate):
        self.serialPort = serial.Serial(port, baudrate)
        self.serialPort.close()

    def writeSerial(self,sentence):
        self.serialPort.write(sentence)
        self.serialPort.flush()

    def readSerial(self):
        if(self.serialPort.in_waiting > 0):
            return(self.serialPort.read_until().decode('utf8').strip())
        else:
            return(None)
    
    def openPort(self):
        self.serialPort.open()

    def closePort(self):
        self.serialPort.close()



