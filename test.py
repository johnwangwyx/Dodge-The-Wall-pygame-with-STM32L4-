import serial
import time

# Setup serial port
ser = serial.Serial('/dev/cu.usbmodem1103', 115200, timeout=0)

while True:
    # Read data from UART
    line = ser.readline().decode('utf-8')
    if "roll and pitch," in line:
        _, roll_str, pitch_str = line.split(',')
        try:
            roll = int(roll_str)
            pitch = int(pitch_str)
            print("--------Roll--------")
            print(roll)
            print("--------Pitch--------")
            print(pitch)
        except ValueError:
            pass
    time.sleep(0.1)