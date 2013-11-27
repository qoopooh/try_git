#!/usr/bin/env python

from socket import socket, AF_INET, SOCK_DGRAM, gethostbyname, gethostname, timeout
from threading import Timer

ALL_IP = '192.168.1.255'

HOST = gethostbyname(gethostname())
UDP_PORT = 1500
BROADCAST_MSG = '12345678901234567890123456789012345678'
SETTING_MSG = '110415'
WAIT = 1.8
TIMEOUT = 0.2
MODE = ("TCP Client", "UDP", "UDP Server", "TCP Server")

f_waiting = True

def clear():
    global f_waiting
    f_waiting = False

def is_timeout():
    return not f_waiting

def print_hex(data, p=False):
    s = "".join("{0:02x}".format(ord(c)) for c in data)
    if (p):
        print s
    return s

def ip_str(d):
    ip = reduce(lambda s,x: str(ord(x)) + '.' + s, d[1:], str(ord(d[0])))
    return ip

def port_number(d):
    p = (ord(d[1]) << 8) + ord(d[0])
    return p

def baud_number(d):
    b = (ord(d[2]) << 16) + (ord(d[1]) << 8) + ord(d[0])
    return b

def extract(d):
    mac = print_hex(d[:6])
    dest_ip = ip_str(d[7:11])
    dest_port = port_number(d[11:13])
    host_ip = ip_str(d[13:17])
    host_port = port_number(d[17:19])
    gateway_ip = ip_str(d[19:23])
    mode = MODE[ord(d[23])]
    baud = baud_number(d[24:27])
    print 'MAC:', mac.upper()
    print 'DEST:', dest_ip, dest_port
    print 'HOST:', host_ip, host_port
    print 'GATE:', gateway_ip
    print 'MODE:', mode
    print 'BAUD:', baud

s = socket(AF_INET, SOCK_DGRAM)
print 'bind host', HOST
s.bind((HOST, UDP_PORT))

t = Timer(WAIT, clear)
t.start()
s.sendto(BROADCAST_MSG, (ALL_IP, UDP_PORT))
s.settimeout(TIMEOUT)
c = 1

while f_waiting:
    try:
        data, addr = s.recvfrom(1024)
        if addr[0] == HOST:
            continue
        print '{count}: {info}'.format(count=c, info=addr)
        c += 1

        extract(data)
    except timeout:
        pass

