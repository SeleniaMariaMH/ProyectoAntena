import serial
import time
import threading

arduino = serial.Serial('COM4', 9600)
read = True
datos = ""


def serialEvent():
    global arduino, read, datos
    while read is True:
        datos = arduino.read_until().decode('ascii').strip()
    return


t = threading.Thread(target=serialEvent)
t.start()


arduino.write(b'Hello World')

print("Entering while")

i = 0
while i < 5:
    if datos != "":
        print(datos)
        datos = ""
        i += 1

print("Finished while")

read = False
t.join()


print("Thread stopped.")


arduino.close()
