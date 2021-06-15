import serial
import time
import threading

class SerialCommunications:
    incData = ""

    def __init__(self,port,baudrate):
        self.serialPort = serial.Serial(port, baudrate)
        time.sleep(0.05)
        self.serialPort.close()

    def writeSerial(self,sentence):
        self.serialPort.write(sentence)
        self.serialPort.flush()

    def readSerial(self):
        if(self.serialPort.in_waiting > 0):
            return(self.serialPort.read_until().decode('utf8').strip())
        else:
            return(None)


    def flushBufferSerial(self):
        try:
            self.serialPort.reset_input_buffer()
        except:
            print("ERROR CLEANING INPUT PORT")
    
    def openPort(self):
        try:
            self.serialPort.open()
        except:
            print("ERROR OPENING PORT")

        time.sleep(0.05)

    def closePort(self):
        self.serialPort.close()





