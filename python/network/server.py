#!/usr/bin/env python
# Echo server program
import socket

HOST = ''                 # Symbolic name meaning all available interfaces
PORT = 1470               # Arbitrary non-privileged port
count = 0

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print 'Connected by', addr
while 1:
    data = conn.recv(1024)
    if not data: break
    count += 1
    conn.sendall(data + str(count))
conn.close()

