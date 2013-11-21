#!/usr/bin/env python

############### Default Values ####################
GATEWAY = '192.168.1.39'
TCP_PORT = 1470
UUID = "bb8342aed2ab395f1512604d55b35027d7ea99bf"
HEX = False
###################################################

import sys, getopt, os
from socket import *
from time import localtime, strftime
from Queue import Queue, Empty
from threading import Thread
from fcntl import fcntl, F_SETFL

from encrypt import Encrypt

BUFFER_SIZE = 1024
REQ_PHONE_ID = "R,U,{uuid}\r\n"

def remove_newline(msg):
    return msg.replace('\r\n','')


def print_hex(data, p=False):
    s = " ".join("{0:02x}".format(ord(c)) for c in data)
    if (p):
        print s
    return s

def listen_gateway(s, q):
    while True:
        data = s.recv(BUFFER_SIZE)
        if not data or len(data) < 1:
            s.close()
            print 'Disconnected'
            sys.exit()
        q.put(data)



def main(argv, gateway=GATEWAY, uuid=UUID, f_hex=HEX):
    q_gw_recv = Queue()
    help_msg = argv[0] + ' -g <gateway> -u <uuid>'

    try:
        opts, args = getopt.getopt(argv[1:], "hxg:u:", ["gateway=", "uuid="])
    except getopt.GetoptError:
        print help_msg
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print help_msg
            sys.exit()
        elif opt in ("-g", "--gateway"):
            gateway = arg
        elif opt in ("-u", "--uuid"):
            uuid = arg
        elif opt in ("-x", "--hex"):
            f_hex = True


    print gateway, uuid
    s = socket(AF_INET, SOCK_STREAM)
    s.connect((gateway, TCP_PORT))
    s.send(REQ_PHONE_ID.format(uuid=uuid))
    data = s.recv(BUFFER_SIZE)
    print '[HANDSHAKE]', data
    if len(data) != 9:
        print 'Incorrect message'
        sys.exit(1)
    e = Encrypt(uuid=uuid, phone_id=data[4])
    t = Thread(target=listen_gateway, args=(s, q_gw_recv))
    t.daemon = True
    t.start()
    
    host = socket(AF_INET, SOCK_STREAM)
    host.bind(('localhost', TCP_PORT))
    host.listen(1)
    conn, addr = host.accept()
    fcntl(conn, F_SETFL, os.O_NONEBLOCK)
    print 'Connected by', addr
    #while True:
        #data = conn.recv(1024)
        #if not data: break
        #count += 1
        #conn.sendall(data + str(count))
    while True:
        try:
            data = q_gw_recv.get(timeout=1)
            dec = e.decrypt(data)
            print "[RECV %s]" % (strftime("%H:%M:%S", localtime())), len(data), \
                    remove_newline(data[:2] + dec)
            if f_hex:
                print_hex(data, True)
        except Empty:
            pass

        print '.'
    conn.close()

if __name__ == "__main__":
    main(sys.argv)

