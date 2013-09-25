#!/usr/bin/env python

import serial

ser=serial.Serial('/dev/ttyUSB0') # default is 9600
print(ser.portstr)
ser.write("hello")
ser.close()

