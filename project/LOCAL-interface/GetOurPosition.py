from time import sleep
# GPSclass - lib
from GPSclass import GPS

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



