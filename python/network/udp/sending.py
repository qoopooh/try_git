#!/usr/bin/env python
import socket, sys

UDP_IP = '127.0.0.1'
UDP_PORT = 1470
MESSAGE = 'Hello, World!'

print 'UDP target IP:', UDP_IP
print 'UDP target port:', UDP_PORT

sock = socket.socket(socket.AF_INET,		# Internet
		socket.SOCK_DGRAM)		# UDP
if len(sys.argv) > 1:
    msg = ' '.join(sys.argv[1:])
    sock.sendto(msg, (UDP_IP, UDP_PORT))
    print 'message:', msg
else:
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
    print 'message:', MESSAGE

