
from SerialCom import SerialCommunications 
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
        self.serial.open()
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
                    self.serial.close()

                    return()

            if(time.monotonic() >= expireTime):
                raise NoFeatures("timeout")
                self.serial.close()

                return(None)
        
        

    def getImuData(self,timeout):
        self.serial.open()
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
                    self.serial.close()

                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                self.serial.close()

                raise NoFeatures("timeout")
                return(None)

    def moveServo(self,position,timeout):
        self.serial.open()
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
                    self.serial.close()

                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                self.serial.close()

                raise NoFeatures("timeout")
                return(None)


    def moveStepper(self,position,timeout):
        self.serial.open()
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
                    self.serial.close()

                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                self.serial.close()

                raise NoFeatures("timeout")
                return(None)

    def swithGPS(self,timeVal,timeout):
        self.serial.open()
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
                    self.serial.close()

                    return(decodeCom["params"])
                                    
            if(time.monotonic() >= expireTime):
                self.serial.close()

                raise NoFeatures("timeout")
                return(None)





antenna = AntennaInterface('COM4',9600)

i = 0
while True:
    time.sleep((0.1))
    antenna.readSentence()

    if (i==100):

        try:
            data=antenna.swithGPS(100,100)
            print("INCOMING DATA:")
            print(data)
        except NoFeatures:
            print("ERROR: TIMEOUT")

        '''
        if(antenna.getImuData(1)==None):
            print("Error Sending the string")
        else:
            print("SENT STRING")

            '''

    i=i + 1

serialObj.closePort()