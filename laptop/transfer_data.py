import serial
import json

ser = serial.Serial(port='COM7', baudrate=9600, timeout=.1)

def send_data(string):
    ser.write(bytes(string, 'utf-8'))

def get_data():
    while True:
        data = ser.readline()
        data = data.decode("utf-8")
        if data != "":
            print(data)
            return json.loads(data)