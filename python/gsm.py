import time
import serial

get_header = """GET /weight HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Accept: application/json\r\n\r\n"""

del_header = """DELETE /weight/8 HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Accept: application/json\r\n\r\n"""

put_header = """PUT /weight/8 HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Content-Type: application/json\r
Content-Length: 16\r
Accept: application/json\r\n\r
{\"info\":\"4,420\"}\r\n\r\n"""

post_header = """POST /weight HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Content-Type: application/json\r
Accept: application/json\r
Content-Length: 16\r\n\r
{\"info\":\"2,200\"}\r\n\r\n"""

def read(n):
	rcv = ""
	for i in range(n):
		rcv = rcv + port.readline()
	return rcv

port = serial.Serial("/dev/ttyUSB0", baudrate = 9600, timeout = 3.0)
port.write("AT+CIPSTATUS\r")
resp = read(4)
print resp
if resp.find("IP INITIAL") != -1 or resp.find("PDP DEACT") != -1:
	print "PDP Context Not Registered"
	port.write("AT+CIPSHUT\r")
	print read(2)
	port.write("AT+CSTT=\"TATA.DOCOMO.INTERNET\"\r")
	print read(2)
	port.write("AT+CIICR\r")
	print read(2)
else:
	print "PDP Context Registered"

print "getting IP Address"
port.write("AT+CIFSR\r")
print read(2)

print "Starting socket comm"
port.write("AT+CIPSTART=\"TCP\",\"lit-taiga-2854.herokuapp.com\",\"80\"\r")
# time.sleep(10)
print read(4)
sub = "1A"
print "Sending header"
port.write("AT+CIPSEND\r")
time.sleep(0.5)
print port.read(15)

port.write(get_header)
port.write(sub.decode("hex"))
# time.sleep(5)
print read(21)#port.read(1024)

# print "Closing socket comm"
port.write("AT+CIPCLOSE\r")
print read(2)