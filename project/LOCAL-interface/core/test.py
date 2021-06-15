
print("Do you want to calibrate the antenna IMU?: ", end='')
print("(y) YES / (n) NO")
iResponse = input()

if iResponse == "y" or iResponse == "YES":
    print("Starting antenna IMU calibration. ")

    # Call arduino calibration function

