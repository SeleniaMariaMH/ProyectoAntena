  
# imports
from functions.Functions import *

# import of the communication library
from functions.AntennaCom import *

# import DRON position simulation
from functions.DronPositionSimulationClass import *

#   main program
# variables
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

# Create objects.
antenna = AntennaInterface(portName, baudRate) # Antenna.
gps = GPS(portName, baudRate, 10) # GPS.

# Homing.
# antenna.moveServo(0, 100)

# Multiplex to GPS.
while (ourLat, ourLon) == (None, None):

    # Switch to GPS.
    antenna.switchGPS(7000)
    print("************************ Serial communication with GPS started ************************")

    # know our position.
    (ourLat, ourLon) = GetOurPosition(gps) # (28.07147116814593, -15.453824236756027)

    # Wait for Arduino.
    antenna.waitForArduino(5)
    print("************************ Serial communication with Arduino started ************************")

# Calibration:
print("Do you want to calibrate antenna IMU?: ", end='')
print("(y) YES / (n) NO")
iResponse = input(">>")

if iResponse == "y":
    print("Starting antenna IMU calibration. ")

    # Call arduino calibration function
    # !!!!!!!

# Get magnetometer and accelerometer calibrated values (magValue, accValue)
print("Getting IMU Data... ")
try:
    imuData = antenna.getImuData(2)
    print("Response: ", end='')
    print(imuData)

except NoFeatures:
    print("ERROR! Timeout in 'getImuData'. ")

accValue = [float(imuData[0]), float(imuData[1]), float(imuData[2])] # imuData[0:3]
magValue = [float(imuData[3]), float(imuData[4]), float(imuData[5])] # imuData[3:6]

print("Accelerometer: [", accValue[0], ",", accValue[1], ",", accValue[2], "] \n")
print("Magnetometer: [", magValue[0], ",", magValue[1], ",", magValue[2], "] \n")

# calculate our rotation matrix.
rotMatrix = CalculateRotationMatrix(magValue, accValue)

# create DRON positions list
createDronPositionList(dronPosList, ourLat, ourLon)

# position object: DRON
dron = PositionSimulation(dronPosList, 10)

# start DRON
dron.start()

while(True):
    try:
        # DRON position
        (dronLat, dronLon, dronHei) = dron.getPosition()
        print("Dron position: (", dronLat, "º,", dronLon, "º,", dronHei, "m)")

        # calculate vector position.
        posVector = PositionVector(ourLat, ourLon, ourHei, dronLat, dronLon, dronHei)

        # change vector position reference.
        posVectorRef = VectorToVector(posVector, rotMatrix)
        print("Position vector: [", posVectorRef[0], ",", posVectorRef[1], ",", posVectorRef[2], "]")

        # calculate rotation and inclination angles.
        (rotDeg, incDeg) = RotationAndInclination(posVectorRef)
        print(  ">> Rotation angle: ", rotDeg, "º \n  "
                ">> Inclination angle: ", incDeg, "º \n")

        # send rotation and inclination angles to arduino (rotDeg, incDeg)
        try:
            data=antenna.moveServo(incDeg, 5)
        except NoFeatures:
            print("ERROR! Timeout in 'moveServo'.")
    
        try:
            data=antenna.moveStepper(rotDeg, 5)
        except NoFeatures:
            print("ERROR! Timeout in 'moveStepper'.")

        # loop delay
        sleep(0.1)

    except KeyboardInterrupt: # ctrl + c
        print("\n"
              "************ Menu ************ \n"
              "(c) Calibrate antenna IMU. \n"
              "(p) Print antenna position. \n"
              "(d) Print dron postition. \n"
              "************ Menu ************")
        iResponse = input(">>")

        if iResponse == "c":
            print("Starting antenna IMU calibration. ")

# stop position object
dron.stop()