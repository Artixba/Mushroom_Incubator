import serial
import time

for i in range(10):
    with serial.Serial('COM3', 9600, timeout=1) as ser:
        time.sleep(0.5)
        ser.write(b'H')
        time.sleep(0.5)
        ser.write(b'L')