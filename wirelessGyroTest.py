#!/usr/bin/python
#-*- coding:utf-8 -*-
import serial
import signal
import time
import sys
import csv

def run():
    # serial = serial.Serial('/dev/cu.SLAB_USBtoUART', 115200, timeout=10)
    serial = start_serial('/dev/cu.SLAB_USBtoUART', 230400)
    #serial port for BT
    # serial2 = start_serial('/dev/cu.HC-05-DevB', 9600)

    while True:

        receive_buf = []
        while True:

            read_chars = serial.read(1)
            if read_chars == '\n': #end of one data set
                break;
            receive_buf.append(read_chars)
            #remove \r in the end
            if receive_buf[len(receive_buf)-1] == '\r':
                receive_buf.pop()

        data_str = "".join(receive_buf)
        # print data_str #print like 0.016   -0.034    1.043

        splited_data = data_str.split(",")
        # print splited_data

        # change '100-id' to 'id' in [0]
        ID_data = splited_data[0].split("-")
        splited_data.pop(0)
        splited_data.insert(0, single_data[1])
        print splited_data

        sensor_data_set = []


    serial.close()

# def get_serial_data_set(serial_obj, sensor_num):




def start_serial(serial_port_name ,baudrate):
    ser = serial.Serial(serial_port_name, baudrate, timeout=10)

    return ser

if __name__ == '__main__':
    run()
