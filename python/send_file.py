import sys
import serial
import time

s = serial.Serial('COM7')
time.sleep(1)
with open(sys.argv[1]) as f:
    for line in f:
        for character in line:
            print(character, end='', flush=True)
            s.write(character.encode())
            _ = s.read()
