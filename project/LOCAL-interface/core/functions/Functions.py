# includes
from time import sleep
import serial.tools.list_ports
# GPSclass - lib
from functions.GPSclass import GPS
import math

# to convert from degrees to radians
def DegToRad(degData):
    return degData * math.pi/180

# to convert from radians to degrees
def RadToDeg(radData):
    return radData * 180/math.pi

# to calculate the position vector between two points from theirs latitude, longitude, and height values based on earth's global system
# @param:   latitude_ini (deg), longitude_ini (deg), height_ini (m) = ZERO,
#           latitude_end (deg), longitude_end (deg), height_end (m)
# @return:  position vector [axisX axisY axisZ]
def PositionVector(latIni, lonIni, hInit, latEnd, lonEnd, hEnd):

    rEarth = 6371000  # Earth radius (meters)

    axisX = ((DegToRad(lonEnd) - DegToRad(lonIni)) * math.cos((DegToRad(latEnd) + DegToRad(lonIni))/2)) * rEarth
    axisY = (DegToRad(latEnd) - DegToRad(latIni)) * rEarth
    axisZ = hEnd - hInit

    posVector = [axisX, axisY, axisZ]

    return posVector

# to change the reference system of a vector using a rotation matrix
def VectorToVector(vector, rotMat):

    axisX = rotMat[0][0] * vector[0] + rotMat[0][1] * vector[1] + rotMat[0][2] * vector[2]
    axisY = rotMat[1][0] * vector[0] + rotMat[1][1] * vector[1] + rotMat[1][2] * vector[2]
    axisZ = rotMat[2][0] * vector[0] + rotMat[2][1] * vector[1] + rotMat[2][2] * vector[2]

    vectorRet = [axisX, axisY, axisZ]

    return vectorRet

# to calculate the angle of rotation and inclination of a vector
# to orient the antenna the vector must be referenced to the antenna reference system
# @param:   vector referenced to the antenna system (meters)
# @return:  angle of rotation (degrees) and inclination (degrees)
def RotationAndInclination(vector):

    dist = math.sqrt((vector[0] ** 2) + (vector[1] ** 2))

    rotRad = math.atan(vector[1]/vector[0])
    incRad = math.atan(vector[2]/dist)

    rotDeg = RadToDeg(rotRad)
    incDeg = RadToDeg(incRad)

    return rotDeg, incDeg


def SelectUSBPortAndBaudRate():

    # Variables.
    status = True
    
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
def GetOurPosition(portName, baudRate, timeOut = 10, numMeasure = 10, nTry = 3):

    gps = GPS(portName, baudRate, timeOut)
    gps.connect()
    gps.startMeasuring()

    sleep(0.1)

    numTry = 0
    ourLat = None
    ourLon = None
    latSum = 0
    lonSum = 0

    while(((ourLat, ourLon) == (None, None)) and (numTry < nTry)):
        print("{", numTry, "} No coverage...")
        (ourLat, ourLon) = gps.getPosition()
        numTry += 1

        sleep(1)

    if ((ourLat, ourLon) != (None, None)):
        for i in range(0, numMeasure):
            (ourLat, ourLon) = gps.getPosition()
            print("----- Measure ", i, " -----")
            print("Our latitude: ", ourLat, "º")
            print("Our longitude: ", ourLon, "º")

            latSum += ourLat
            lonSum += ourLon

<<<<<<< HEAD
            sleep(1)
=======
            sleep(2)
        gps.stopMeasuring()
>>>>>>> refs/remotes/origin/main

        ourLat = latSum/numMeasure
        ourLon = lonSum/numMeasure

        print("----- OUR POSITION: -----")
        print("Latitude: ", ourLat, "º")
        print("Longitude: ", ourLon, "º")

    else:
        print("ERROR! No coverage. Try again.")

<<<<<<< HEAD
    gps.stopMeasuring()

=======
    print("GPS Disconnecting")
    
    #gps.disconnect()
    print("GPS Disconnected")

    
>>>>>>> refs/remotes/origin/main
    return ourLat, ourLon


# rotation matrix -> R = [(A×M)×A, A×M, A];
def CalculateRotationMatrix(magnetometerData, accelerometerData):

    rX = CrossProduct(CrossProduct(accelerometerData, magnetometerData), accelerometerData)
    rY = CrossProduct(accelerometerData, magnetometerData)
    rZ = accelerometerData

    rX[:] = [x / CalculateNorm(rX) for x in rX]
    rY[:] = [x / CalculateNorm(rY) for x in rY]
    rZ[:] = [x / CalculateNorm(rZ) for x in rZ]

    r = [rX, rY, rZ]

    print("Rotation matrix: ", r)

    return r

# calculate cross product for 2 vectors of 3 components
def CrossProduct(vectorA, vectorB):
    
    vectorCX = vectorA[1] * vectorB[2] - vectorA[2] * vectorB[1]
    vectorCY = vectorA[2] * vectorB[0] - vectorA[0] * vectorB[2]
    vectorCZ = vectorA[0] * vectorB[1] - vectorA[1] * vectorB[0]

    vectorC = [vectorCX, vectorCY, vectorCZ]

    return vectorC

# calculate normal of a 3 components vector
def CalculateNorm(vector):
    
    normValue = (vector[0]**2 + vector[1]**2 + vector[2]**2) ** 0.5

    return normValue

def createDronPositionList(list, ourLat, ourLon):
    # DRON moves from 5m to 1m of altitude (inclination)
    list.append({'Latitude': ourLat + 0.00020, 'Longitude': ourLon + 0.00020, 'Altitude': 5})
    list.append({'Latitude': ourLat + 0.00020, 'Longitude': ourLon + 0.00020, 'Altitude': 4})
    list.append({'Latitude': ourLat + 0.00020, 'Longitude': ourLon + 0.00020, 'Altitude': 3})
    list.append({'Latitude': ourLat + 0.00020, 'Longitude': ourLon + 0.00020, 'Altitude': 2})
    list.append({'Latitude': ourLat + 0.00020, 'Longitude': ourLon + 0.00020, 'Altitude': 1})

    # DRON moves to the left (rotation)
    list.append({'Latitude': ourLat + 0.00018, 'Longitude': ourLon + 0.00022, 'Altitude': 1})
    list.append({'Latitude': ourLat + 0.00016, 'Longitude': ourLon + 0.00024, 'Altitude': 1})
    list.append({'Latitude': ourLat + 0.00014, 'Longitude': ourLon + 0.00026, 'Altitude': 1})
    list.append({'Latitude': ourLat + 0.00012, 'Longitude': ourLon + 0.00028, 'Altitude': 1})
    list.append({'Latitude': ourLat + 0.00010, 'Longitude': ourLon + 0.00030, 'Altitude': 1})
