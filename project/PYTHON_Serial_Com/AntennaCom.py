
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




antenna = AntennaInterface('COM4',9600)

i = 0
while True:
    time.sleep((0.1))
    antenna.readSentence()

    if (i==100):

        try:
            data=antenna.getImuData(1)
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