
from functions.SerialCom import SerialCommunications
import time

class NoFeatures(Exception):
    pass

class AntennaInterface:
    
    def __init__(self,port,baudrate):
        self.serial = SerialCommunications(port,baudrate)


    def readSentence(self):
        incomingSentence = self.serial.readSerial() 
        if( incomingSentence != None):
            print(incomingSentence)
            return(0)
        else:
            return(-1)

    def decodeCommand(self,string):
        decodeCom = {
            "id": 0,
            "params" : []
        }
        decodeCom["id"] = string[0]
        decodeCom["params"]= []
        sep = ' '
        lastSep = 2


        for i in range(2,len(string)):
            if(string[i]== sep):
                decodeCom["params"].append(string[lastSep:i])
                lastSep = i
            if(i == (len(string)-1)):
                decodeCom["params"].append(string[lastSep:i+1])

        return(decodeCom)



    def calibrateImu(self,timeout):
        self.serial.writeSerial(b'C:0\n')
        
        expireTime = time.monotonic() + timeout

        while True:
            inString = self.serial.readSerial()
            if(inString != None):
                decodeCom= self.decodeCommand(inString)

                #Check if the incoming sentence is the one that we are waiting.
                if(decodeCom["id"]=='C'):
                    print(inString)
                    print(decodeCom)
                    return()

            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                return(None)


    def getImuData(self,timeout):
        self.serial.writeSerial(b'G:0\n')
        
        expireTime = time.monotonic() + timeout

        while True:
            inString = self.serial.readSerial()
            if(inString != None):
                decodeCom= self.decodeCommand(inString)
                #Check if the incoming sentence is the one that we are waiting.
                if(decodeCom["id"]=='G'):
                    print(inString)
                    print(decodeCom)
                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                return(None)

    def moveServo(self,position,timeout):
        string = 'S:'+str(position)+'\n'
        self.serial.writeSerial(str.encode(string))
        
        expireTime = time.monotonic() + timeout

        while True:
            inString = self.serial.readSerial()
            if(inString != None):
                decodeCom= self.decodeCommand(inString)
                #Check if the incoming sentence is the one that we are waiting.
                if(decodeCom["id"]=='S'):
                    print(inString)
                    print(decodeCom)
                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                return(None)


    def moveStepper(self,position,timeout):
        string = 'M:'+str(position)+'\n'
        self.serial.writeSerial(str.encode(string))
        
        expireTime = time.monotonic() + timeout

        while True:
            inString = self.serial.readSerial()
            if(inString != None):
                decodeCom= self.decodeCommand(inString)
                #Check if the incoming sentence is the one that we are waiting.
                if(decodeCom["id"]=='M'):
                    print(inString)
                    print(decodeCom)
                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                return(None)

    def swithGPS(self,timeVal,timeout):
        string = 'A:'+str(timeVal)+'\n'
        self.serial.writeSerial(str.encode(string))

        expireTime = time.monotonic() + timeout

        while True:
            inString = self.serial.readSerial()
            if(inString != None):
                decodeCom= self.decodeCommand(inString)
                #Check if the incoming sentence is the one that we are waiting.
                if(decodeCom["id"]=='A'):
                    print(inString)
                    print(decodeCom)
                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                return(None)



