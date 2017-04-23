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
    sensor_num = 6

    #serial port for BT
    # serial2 = start_serial('/dev/cu.HC-05-DevB', 9600)

    # Change output format in euler
    # serial.write("<sof1>\r\n")

    date_stamp = time.strftime("%Y-%m-%d-%H-%M-%S", time.gmtime())
    with open('./'+ date_stamp + '.csv', 'w') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        # add index to the first row
        writer.writerow(['1_x','1_y','1_z','2_x','2_y','2_z','3_x','3_y','3_z',\
            '4_x','4_y','4_z','5_x','5_y','5_z','6_x','6_y','6_z'])

        # init data to compare diff
        init_data = get_single_data(serial, sensor_num)
        print init_data

        while True:
            unsorted_data = get_serial_data_set(serial, sensor_num)
            arranged_data = parse_in_writer_form(unsorted_data)

            # write only when arms are not in init position
            if is_diff_init(init_data, arranged_data) == True:
                print arranged_data
                writer.writerow(arranged_data)

    serial.close()

def is_diff_init(init_data, current_data, tolerance = 7):

    for i in range(0, len(init_data)):
        if abs(float(init_data[i]) - float(current_data[i])) > tolerance:
            return True

    return False

def get_single_data(serial_obj, sensor_num):
    unsorted_data = get_serial_data_set(serial_obj, sensor_num)
    return parse_in_writer_form(unsorted_data)

def parse_in_writer_form(unsorted_data):
    # make list to write
    sorted_data = []

    for i in range(1, len(unsorted_data)+1):
        is_filled = False
        for j in range(0, len(unsorted_data)):
            if unsorted_data[j][0] == str(i):
                sorted_data.append(unsorted_data[j])
                is_filled = True
                break
        if is_filled == False:
            dummy_data = [0,0,0,0,0]
            sorted_data.append(dummy_data)


    # print sorted_data

    # select data to write in cvs file
    data_to_write = []

    for i in range(0, len(sorted_data)):
        data_to_write.append(sorted_data[i][1]) # x value
        data_to_write.append(sorted_data[i][2]) # y value
        data_to_write.append(sorted_data[i][3]) # z value

    # print data_to_write

    return data_to_write


def get_serial_data_set(serial_obj, sensor_num):
    sensor_data_set = []

    for i in range(0, sensor_num):

        receive_buf = []
        while True:

            read_chars = serial_obj.read(1)
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
        splited_data.insert(0, ID_data[1])
        # print splited_data

        sensor_data_set.append(splited_data)

    # print sensor_data_set

    return sensor_data_set



def start_serial(serial_port_name ,baudrate):
    ser = serial.Serial(serial_port_name, baudrate, timeout=10)

    return ser

if __name__ == '__main__':
    run()
