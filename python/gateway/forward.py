#!/usr/bin/env python

__version__ = '0.0.3'

############### Default Values ####################
#GATEWAY = '188.82.100.29'
GATEWAY = '192.168.1.31'
HOST = '192.168.1.44'
TCP_PORT = 1470
#UUID = "bb8342aed2ab395f1512604d55b35027d7ea99bf" # chrome
UUID = "0123456789ABCDEF212a7c75af448aa012345678" # android
#UUID = "unknown80f94bfcae14353012345678901234567" # android tablet
#UUID = "82b6e7e9da3f4d4f8d70cc5e1a1426c100000000" # Sergio's ipad
#UUID = "6b6c90b1f70643baa0cd0b84ec65fc5800000000" # breeze's iphone
#UUID = "8b9dc8142baa417bb49337343133633f00000000" # breeze's iphone 2
HEX = False
ONLY = False
###################################################

import sys, getopt, os
from socket import *
from time import localtime, strftime
from Queue import Queue, Empty
from threading import Thread

from encrypt import Encrypt

BUFFER_SIZE = 1024
TIMEOUT = 0.05
REQ_UUID = "R,U"
REQ_PHONE_ID = REQ_UUID + ",{uuid}\r\n"

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
            print "[SERVER] Disconnected"
            sys.exit()
        q.put(data)



def main(argv, gateway=GATEWAY, uuid=UUID, f_hex=HEX):
    q_gw_recv = Queue()
    help_msg = argv[0] + '-h <host> -g <gateway> -u <uuid>'
    local = HOST

    try:
        opts, args = getopt.getopt(argv[1:], "xh:g:u:", \
            ["host", "gateway=", "uuid="])
    except getopt.GetoptError:
        print help_msg
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--host"):
            local = arg
        elif opt in ("-g", "--gateway"):
            gateway = arg
        elif opt in ("-u", "--uuid"):
            uuid = arg
        elif opt == "-x":
            f_hex = True


    print '[VERSION]', __version__
    print '[GATEWAY]', gateway, uuid
    s = socket(AF_INET, SOCK_STREAM)
    s.connect((gateway, TCP_PORT))
    s.send(REQ_PHONE_ID.format(uuid=uuid))
    data = s.recv(BUFFER_SIZE)
    print '[HANDSHAKE]', data
    if len(data) != 9:
        print 'Incorrect message'
        sys.exit(1)
    e = Encrypt(uuid=uuid, phone_id=data[4], only_compatible=ONLY)
    t = Thread(target=listen_gateway, args=(s, q_gw_recv))
    t.daemon = True
    t.start()
    
    host = socket(AF_INET, SOCK_STREAM)
    host.bind((local, TCP_PORT))
    host.listen(1)
    print '[HOST]', local, TCP_PORT
    conn, addr = host.accept()
    conn.settimeout(TIMEOUT)
    print 'Connected by', addr
    cnt = 0
    while True:
        try:
            data = q_gw_recv.get(timeout=TIMEOUT)
            dec = e.decrypt(data)
            print "[RECV %s]" % (strftime("%H:%M:%S", localtime())), len(data), \
                    remove_newline(data[:2] + dec)
            conn.sendall(data)
            if f_hex:
                print_hex(data, True)
        except Empty:
            pass

        try:
            data = conn.recv(1024)
            if REQ_UUID in data:
                e.uuid = data[(data.index(REQ_UUID) + 4):]
                print "[ENC UUID]", e.uuid
            if len(data) < 1:
                print "[CLIENT] Disconnected"
                conn, addr = host.accept()
                conn.settimeout(TIMEOUT)
                print '[CLIENT] Connected by', addr
                continue
            dec = e.decrypt(data)
            print "[SENT %s]" % (strftime("%H:%M:%S", localtime())), len(data), \
                    remove_newline(data[:2] + dec)
            s.sendall(data)
        except timeout:
            if cnt > 1000:
                cnt = 0
                print 'tout'

    conn.close()

if __name__ == "__main__":
    main(sys.argv)

