  
# imports
from functions.Functions import *

# import of the communication library
from functions.AntennaCom import *

# import DRON position simulation
from functions.DronPositionSimulationClass import *

#   main program
# variables.
ourLat = None
ourLon = None
ourHei = 0.0
dronLat = 0.0
dronLon = 0.0
dronHei = 0.0
posVector = [0, 0, 0]
posVectorRef = [0, 0, 0]
rotDeg = 0.0
incDeg = 0.0
portName = "None"
baudRate = 0
magValue = [0, 0, 0]
accValue = [0, 0, 0]
rotMatrix = [0, 0, 0]
dronPosList = []
welcomeMessage = "\n************************ Antenna Project ************************ \n" \
                 "Team members: \n" \
                 "  ·   MEDINA HERNÁNDEZ, SELENIA MARÍA. \n" \
                 "  ·   BERCIANO RODRÍGUEZ, GEMMA. \n" \
                 "  ·   VEGA GARCÍA, CARLOS. \n" \
                 "************************ Antenna Project ************************ \n"

# Welcome message.
print(welcomeMessage)

# Select USB port and baud rate.
(portName, baudRate) = SelectUSBPortAndBaudRate()

# Antenna interface object.
antenna = AntennaInterface(portName, baudRate)

# multiplex to GPS.
while((ourLat, ourLon) == (None, None)):

    antenna.switchGPS(10000)
    print("Serial communication with GPS is started.")

    # know our position.
    (ourLat, ourLon) = GetOurPosition(portName, baudRate)
    # (ourLat, ourLon) = (28.07147116814593, -15.453824236756027)

    # wait for the mux to come back to to Arduino
    print("Serial communication with Arduino is started.")
    try:
        antenna.waitForArduino(1)
    except NoFeatures:
        print("ERROR: TIMEOUT SERVO COM")
    

# create DRON positions list
createDronPositionList(dronPosList, ourLat, ourLon)

# get magnetometer and accelerometer calibrated values (magValue, accValue)
try:
    data = antenna.getImuData(2)
    print("INCOMING DATA:")
    print(data)
except NoFeatures:
    print("ERROR: TIMEOUT")

accValue = data[0:3]
print("Accelerometer value: [", accValue[0], ",", accValue[1], ",", accValue[2], "] \n")
magValue = data[3:6]
print("Magnetometer value: [", magValue[0], ",", magValue[1], ",", magValue[2], "] \n")

# calculate our rotation matrix.
rotMatrix = CalculateRotationMatrix(magValue, accValue)

# position object: DRON
dron = PositionSimulation(dronPosList)

# start DRON
dron.start()

while(True):

    # DRON position
    (dronLat, dronLon, dronHei) = dron.getPosition()
    print("Current DRON position: (", dronLat, "º,", dronLon, "º,", dronHei, "m)")

    # calculate vector position.
    posVector = PositionVector(ourLat, ourLon, ourHei, dronLat, dronLon, dronHei)

    # change vector position reference.
    posVectorRef = VectorToVector(posVector, rotMatrix)
    print("  -   Position vector: [", posVectorRef[0], ",", posVectorRef[1], ",", posVectorRef[2], "]")

    # calculate rotation and inclination angles.
    (rotDeg, incDeg) = RotationAndInclination(posVectorRef)
    print("  -   Rotation angle: ", rotDeg, "º \n  -   Inclination angle: ", incDeg, "º \n")

    # send rotation and inclination angles to arduino (rotDeg, incDeg)
    try:
        data=antenna.moveServo(incDeg,5)
    except NoFeatures:
        print("ERROR: TIMEOUT SERVO COM")
    
    try:
        data=antenna.moveStepper(rotDeg,5)
    except NoFeatures:
        print("ERROR: TIMEOUT STEPPER COM")

    # get arduino response (NOT  NEEDED)

    # loop delay
    time.sleep(2)

# stop position object
# dron.stop()