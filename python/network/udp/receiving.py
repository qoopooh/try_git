#!/usr/bin/env python
import socket

UDP_IP = '127.0.0.1'
UDP_PORT = 1470
MESSAGE = 'Hello, World!'

sock = socket.socket(socket.AF_INET,	# Internet
		socket.SOCK_DGRAM) 	# UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
	data, addr = sock.recvfrom(1024)	# buffer size is 1024 bytes
	print 'received message:', data
