#!/usr/bin/env python

from socket import socket, AF_INET, SOCK_DGRAM

ALL_IP = '192.168.1.39'
UDP_IP = 'localhost'
BIND_PORT = 1500
SEND_PORT = 1500
BROADCAST_MSG = '12345678901234567890123456789012345678'

#r = socket(AF_INET, SOCK_DGRAM)
#r.bind(('', BIND_PORT))

s = socket(AF_INET, SOCK_DGRAM)
s.sendto(BROADCAST_MSG, (ALL_IP, SEND_PORT))

#data, addr = r.recvfrom(1024)
#print 'rcv', addr, data

