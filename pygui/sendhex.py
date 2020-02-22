import serial
import time
ser = serial.Serial('/dev/ttyUSB0', 115200)
print(ser.name)


def send():
    strser = b'\x24\xB2\x0a\x01\x21\x41\xd7\xc3\xbf\xf4\x1d\xad\xc3\x78\x3e\xb7\x46\x5c\x0f\xe2\xc3\x0a\xd7\xc3\xbf\xf4\x1d\xad\xc3\x78\x3e\xb7\x46\x3e'
    ser.write(strser)
    time.sleep(0.1)


while True:
    send()
# message_bytes = bytes.fromhex("24B20A012141d7c3bff41dadc3783eb7465c0fe2c30ad7c3bff41dadc3783eb7463e")
# ser.write(message_bytes)
# 

#24B20A012141d7c3bff41dadc3783eb7465c0fe2c30ad7c3bff41dadc3783eb7463e
