import time
import serial

def read(n):
	rcv = ""
	for i in range(n):
		rcv = rcv + port.readline()
	return rcv

port = serial.Serial("/dev/ttyUSB0", baudrate = 9600, timeout = 3.0)
port.write("AT\r")
time.sleep(0.2)
print read(2)
port.write("AT+CSQ\r")
time.sleep(0.2)
print read(2)
port.write("AT+GSV\r")
time.sleep(0.2)
print read(8)
port.write("ATI\r")
time.sleep(0.2)
print read(2)
port.write("AT&V\r")
time.sleep(0.2)
print read(13)
port.write("AT+COPS?\r")
time.sleep(0.2)
print read(4)