# imports
from functions.Functions import *

# import of the communication library
from functions.AntennaCom import *

# import DRON position simulation
from functions.DronPositionSimulationClass import *

#   main program
# variables.
ourLat = 0
ourLon = 0
ourHei = 0
dronLat = 0
dronLon = 0
dronHei = 0
posVector = [0, 0, 0]
posVectorRef = [0, 0, 0]
rotDeg = 0
incDeg = 0
portName = "None"
baudRate = 0
magValue = [0, 0, 0]
accValue = [0, 0, 0]
rotMatrix = [0, 0, 0]
welcomeMessage = "######################## Antenna Project ######################## \n" \
                 "Team members: \n" \
                 "  ·   MEDINA HERNÁNDEZ, SELENIA MARÍA. \n" \
                 "  ·   BERCIANO RODRÍGUEZ, GEMMA. \n" \
                 "  ·   VEGA GARCÍA, CARLOS. \n" \
                 "######################## Antenna Project ######################## \n"

# dron positions list
positionList = []
positionList.append({'Latitude': 1.1, 'Longitude': 1.2, 'Altitude': 1.3})
positionList.append({'Latitude': 2.1, 'Longitude': 2.2, 'Altitude': 2.3})
positionList.append({'Latitude': 3.1, 'Longitude': 3.2, 'Altitude': 3.3})
positionList.append({'Latitude': 4.1, 'Longitude': 4.2, 'Altitude': 4.3})

# Welcome message.
print(welcomeMessage)

# Select USB port and baud rate.
(portName, baudRate) = SelectUSBPortAndBaudRate()

# Antenna interface object
antenna = AntennaInterface(portName, baudRate)

# multiplex to GPS.

# know our position.
(ourLat, ourLon) = GetOurPosition(portName, baudRate)

# multiplex to arduino.

# get magnetometer and accelerometer calibrated values (magValue, accValue)
try:
    data = antenna.getImuData(2)
    print("INCOMING DATA:")
    print(data)
except NoFeatures:
    print("ERROR: TIMEOUT")

# format:
# magValue = [0, 0, 0]
# accValue = [0, 0, 0]
accValue = data[0:3]
print("Accelerometer value: [", accValue[0], ",", accValue[1], ",", accValue[2], "] \n")
magValue = data[3:6]
print("Magnetometer value: [", magValue[0], ",", magValue[1], ",", magValue[2], "] \n")

# calculate our rotation matrix.
rotMatrix = CalculateRotationMatrix(magValue, accValue)

# start position object
positionObject = PositionSimulation(positionList)
positionObject.start()

while(True):

    # DRON position
    (dronLat, dronLon, dronHei) = positionObject.getPosition()
    print("Current DRON position: (", dronLat, "º,", dronLon, "º,", dronHei, "m)")

    # calculate vector position.
    posVector = PositionVector(ourLat, ourLon, ourHei, dronLat, dronLon, dronHei)

    # change vector position reference.
    posVectorRef = VectorToVector(posVector, rotMatrix)
    print("Position vector: [", posVectorRef[0], ",", posVectorRef[1], ",", posVectorRef[2], "]")

    # calculate rotation and inclination angles.
    (rotDeg, incDeg) = RotationAndInclination(posVectorRef)
    print("Rotation angle: ", rotDeg, "º \n Inclination angle: ", incDeg, "º \n")

    # send rotation and inclination angles to arduino (rotDeg, incDeg)

    # get arduino response

# stop position object
# positionObject.stop()