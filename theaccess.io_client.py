import requests 
import serial
import time
import threading
import configparser

if 0:
    import UserList
    import UserString
    import UserDict    
    import collections
    import commands
    import __buildin__
    import subprocess

config = configparser.ConfigParser()
config.read('theaccess.io_config.ini')


URL = "https://api.theaccess.io/access/"
SERIALPORT = config["SERIAL"]["SERIAL_PORT"]
TIMER = int(config["SERIAL"]["TIMER"], 10)

on_command_str = config["SERIAL"]["ON_COMMAND"]
on_command_array = on_command_str.split(',')
COMMAND_ON = bytearray(len(on_command_array))
for i in range(len(on_command_array)):
    COMMAND_ON[i] = int(on_command_array[i], 16)

off_command_str = config["SERIAL"]["OFF_COMMAND"]
off_command_array = off_command_str.split(',')
COMMAND_OFF = bytearray(len(off_command_array))
for i in range(len(off_command_array)):
    COMMAND_OFF[i] = int(off_command_array[i], 16)

CONTROLLERID=config["CONTROLLER"]["CONTROLLER_ID"]

def thread_ping():
    while True:
        requests.get(URL+"ping", headers={"controllerId": CONTROLLERID})
        time.sleep(10)

x = threading.Thread(target=thread_ping)
x.start()

while True:
    token=input("token : ")
    response=requests.get(URL, headers={"controllerId": CONTROLLERID, "Authorization": token})
    if response.status_code == 200:
        print("AUTHORIZED")
        s = serial.Serial(SERIALPORT, 9600)
        s.write(COMMAND_ON)
        time.sleep(TIMER)
        s.write(COMMAND_OFF)
        s.close()
    else: 
        print("UNAUTHORIZED")