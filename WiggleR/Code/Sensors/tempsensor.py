#!/usr/bin/env python

import os
import glob
import time
import datetime

def read_temp(decimals = 1, sleeptime = 3):

    """Reads the temperature from a 1-wire device"""

    device = glob.glob("/sys/bus/w1/devices/" + "28*")[0] + "/w1_slave"
    while True:
        try:
            timepoint = datetime.datetime.now()
            with open(device, "r") as f:
                lines = f.readlines()
            while lines[0].strip()[-3:] != "YES":
                time.sleep(0.2)
                lines = read_temp_raw()
            timepassed = (datetime.datetime.now() - timepoint).total_seconds()
            equals_pos = lines[1].find("t=")
            if equals_pos != -1:
                temp_string = lines[1][equals_pos+2:]
                temp = round(float(temp_string) / 1000.0, decimals)
                print(time.strftime("%d/%m/%y@%H:%M:%S - ")+str(temp)+" C")
                time.sleep(sleeptime-timepassed)
                timepoint = datetime.datetime.now()
        except KeyboardInterrupt:
            break

if __name__ == "__main__":
    read_temp()