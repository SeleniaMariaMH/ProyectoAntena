# includes
from Functions import SelectUSBPortAndBaudRate
from Functions import GetOurPosition
from Functions import CalculateRotationMatrix
from Functions import PositionVector
from Functions import RotationAndInclination
from Functions import VectorToVector

#Import of the communication library
from AntennaCom import AntennaInterface, NoFeatures


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

antenna = AntennaInterface('COM4',9600)

welcomeMessage = "######################## Antenna Project ######################## \n" \
                 "Team members: \n" \
                 "  ·   MEDINA HERNÁNDEZ, SELENIA MARÍA. \n" \
                 "  ·   BERCIANO RODRÍGUEZ, GEMMA. \n" \
                 "  ·   VEGA GARCÍA, CARLOS. \n" \
                 "######################## Antenna Project ######################## \n"

# Welcome message.
print(welcomeMessage)

# Select USB port and baud rate.
# (portName, baudRate) = SelectUSBPortAndBaudRate()

# multiplex to GPS.

# know our position.
# (ourLat, ourLon) = GetOurPosition(portName, baudRate)

# multiplex to arduino.

# get magnetometer and accelerometer calibrated values (magValue, accValue)
magValue = [201, 202, 203]
accValue = [101, 102, 103]

try:
    data=antenna.getImuData(2)
    print("INCOMING DATA:")
    print(data)
except NoFeatures:
    print("ERROR: TIMEOUT")

accValue = data[0:3]
magValue = data[3:6]

print(accValue)
print(magValue)


# calculate our rotation matrix.
rotMatrix = CalculateRotationMatrix(magValue, accValue)

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
    posVector = PositionVector(ourLat, ourLon, ourHei, dronLat, dronLon, dronHei)

    # change vector position reference.
    posVectorRef = VectorToVector(posVector, rotMatrix)
    print("Position vector: [", posVectorRef[0], ",", posVectorRef[1], ",", posVectorRef[2], "]")

    # calculate rotation and inclination angles.
    (rotDeg, incDeg) = RotationAndInclination(posVectorRef)
    print("Rotation angle: ", rotDeg, "º \n Inclination angle: ", incDeg, "º \n")

    # send rotation and inclination angles to arduino (rotDeg, incDeg)

    # get arduino response.

