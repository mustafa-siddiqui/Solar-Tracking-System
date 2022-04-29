"""
 @file      uart_receive.py
 @brief     Code to run on Raspberry Pi 4 Model B to receive
            data via UART over the tx and rx lines.
 @author    Carter Bordeleau
 @date      04/16/22
"""

import serial
from time import sleep

baud_rate = 9600

# create serial object for serial0 pin
ser = serial.Serial("/dev/ttyS0/", baud_rate)

sleep(2)

# wait and receive data continuously
while True:
    print("waiting")
    received_data = ser.read()
    sleep(0.5)
    data_left = ser.inWaiting()
    received_data += ser.read(data_left)

    print(received_data.decode("ISO-8859-1"))

