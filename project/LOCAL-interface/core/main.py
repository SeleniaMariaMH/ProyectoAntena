from GetOurPosition import GetOurPosition
from GetOurPosition import SelectUSBPortAndBaudRate
from RotationMatrixFunction import CalculateRotationMatrix
from RotationAndInclinationFunctions import PositionVector
from RotationAndInclinationFunctions import VectorToVector
from RotationAndInclinationFunctions import RotationAndInclination

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
                 "######################## Antenna Project ########################"

# Welcome message.
print(welcomeMessage)

# Select USB port and baud rate.
# (portName, baudRate) = SelectUSBPortAndBaudRate()

# multiplex to GPS.

# know our position.
# (ourLat, ourLon) = GetOurPosition(portName, baudRate)

# multiplex to arduino.

# get magnetometer and accelerometer calibrated values (magValue, accValue)
# magValue = [1, 3, 5]
# accValue = [5, 6, 8]

# calculate our rotation matrix.
# rotMatrix = CalculateRotationMatrix(magValue, accValue)

while(True):

    # know dron position.
    print("Introduce DRON position: ")
    print("Latitude (degrees): ")
    dronLat = input()

    print("Longitude (degrees): ")
    dronLon = input()

    print("Height (meters): ")
    dronHei = input()

    print("DRON position introduced: (", dronLat, "º,", dronLon, "º,", dronHei, "m)")

    # calculate vector position.
    # posVector = PositionVector(ourLat, ourLon, ourHei, dronLat, dronLon, dronHei)

    # change vector position reference.
    # posVectorRef = VectorToVector(posVector, rotMatrix)

    # calculate rotation and inclination angles.
    # (rotDeg, incDeg) = RotationAndInclination(posVectorRef)

    # send rotation and inclination angles to arduino (rotDeg, incDeg)

    # get arduino response.

