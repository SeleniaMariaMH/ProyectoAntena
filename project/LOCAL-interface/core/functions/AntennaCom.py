
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
                #string[lastSep:i].append(decodeCom["par"])
                decodeCom["params"].append(string[lastSep:i])
                lastSep = i
            if(i == (len(string)-1)):
                #string[lastSep:i].append(decodeCom["par"])
                decodeCom["params"].append(string[lastSep:i])

        return(decodeCom)




    def getImuData(self,timeout):
        self.serial.writeSerial(b'G:0\n')
        
        expireTime = time.monotonic() + timeout

        while True:
            inString = self.serial.readSerial()
            if(inString != None):
                decodeCom= self.decodeCommand(inString)
                print(inString)
                print(decodeCom)
                return(decodeCom["params"])
            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                return(None)


