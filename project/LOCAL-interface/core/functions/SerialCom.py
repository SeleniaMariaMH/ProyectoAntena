import serial
import time
import threading

class SerialCommunications:

    incData = ""

    def __init__(self,port,baudrate):
        try:
            self.serialPort = serial.Serial(port, baudrate)
        except:
            print("ERROR PORT")

    def writeSerial(self,sentence):
        self.serialPort.write(sentence)
        self.serialPort.flush()

    def readSerial(self):
        if(self.serialPort.in_waiting > 0):
            return(self.serialPort.read_until().decode('utf8').strip())
        else:
            return(None)
    
    def closePort(self):
        print("Thread stopped.")
        self.serialPort.close()



