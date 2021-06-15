import time

while(True):
    try:
        print("Controlling antenna...")
        time.sleep(1)


    except KeyboardInterrupt:
        print("\n"
              "************ Menu ************ \n"
              "(c) Calibrate antenna IMU. \n"
              "(p) Print antenna position. \n"
              "(d) Print dron postition. \n"
              "************ Menu ************")

        iResponse = input(">>")

        if iResponse == 'c':
            print("Starting antenna IMU calibration. ")
            for i in range(0,3):
                print("Calibrating...")
                time.sleep(1)