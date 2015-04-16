
import socket
import time

host = "lit-taiga-2854.herokuapp.com"
port = 80

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.settimeout(5)

get_header = """GET /weight HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Accept: application/json\r\n\r\n"""

del_header = """DELETE /weight/4 HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Accept: application/json\r\n\r\n"""

put_header = """PUT /weight/7 HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Content-Type: application/json\r
Content-Length: 16\r
Accept: application/json\r\n\r
{\"info\":\"9,900\"}\r\n\r\n"""

post_header = """POST /weight HTTP/1.1\r
Host: lit-taiga-2854.herokuapp.com\r
Content-Type: application/json\r
Accept: application/json\r
Content-Length: 16\r\n\r
{\"info\":\"2,200\"}\r\n\r\n"""


def CONNECT():
	try:
		sock.connect((host, port))
		return True
	except Exception,e:
		print e
		return False


def GET():
	sock.send(get_header)
	print sock.recv(100000)

def POST():
	print post_header
	sock.send(post_header)
	print sock.recv(100000)

def DELETE():
	sock.send(del_header)
	print sock.recv(100000)

def PUT():
	# print put_header
	sock.send(put_header)
	print sock.recv(100000)

if CONNECT():
	# POST()
	GET()
	sock.shutdown(1)
	sock.close()
else:
	print "Connect Failed"

