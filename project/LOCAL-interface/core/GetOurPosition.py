from time import sleep
import serial.tools.list_ports
# GPSclass - lib
from GPSclass import GPS

def DefineGPSPortAndBaudRate():
    portsName = serial.tools.list_ports.comports()
    baudRates = [9600, 14400, 19200, 38400, 57600, 115200]

    print("Available ports: ")
    for iPortName in range(len(portsName)):
        print("{", iPortName, "}: ", portsName[iPortName].device)
    print("Select port name [0 to", (len(portsName) - 1), "]: ")

    status = True
    while(status):
        iPortName = input()
        if (int(iPortName) > (len(portsName) - 1)):
            print("ERROR! Select a correct value. ")
            print("Select port name [0 to", (len(portsName) - 1), "]: ")
            status = True
        else:
            status = False

    portName = portsName[int(iPortName)].device

    print("Available baud rates: ")
    for iBaudRate in range(len(baudRates)):
        print("{", iBaudRate, "}: ", baudRates[iBaudRate])
    print("Select baud rate [0 to", (len(baudRates) - 1), "]: ")

    status = True
    while (status):
        iBaudRate = input()
        if (int(iBaudRate) > (len(baudRates) - 1)):
            print("ERROR! Select a correct value. ")
            print("Select baud rate [0 to", (len(baudRates) - 1), "]: ")
            status = True
        else:
            status = False

    baudRate = baudRates[int(iBaudRate)]

    return portName, baudRate

# function to get our position via GPS
# @param:   portName (GPS USB port), baudRate, timeOut (default: 3s), numMeasure (default: 10 measures)
# @return:  latitude (degrees) and longitude (degrees)
def GetOurPosition(portName, baudRate, timeOut = 3, numMeasure = 10):

    gps = GPS(portName, baudRate, timeOut)
    gps.connect()
    gps.startMeasuring()

    latSum = 0
    lonSum = 0

    while(gps.getPosition() == (None, None)):
        print("No coverage...")
        sleep(3)

    for i in range(0, numMeasure):
        (ourLat, ourLon) = gps.getPosition()
        print("----- Measure ", i, " -----")
        print("Our latitude: ", ourLat, "º")
        print("Our longitude: ", ourLon, "º")

        latSum += ourLat
        lonSum += ourLon

        sleep(2)

    gps.stopMeasuring()

    ourLat = latSum/numMeasure
    ourLon = lonSum/numMeasure

    print("----- OUR POSITION: -----")
    print("Latitude: ", ourLat, "º")
    print("Longitude: ", ourLon, "º")

    return ourLat, ourLon