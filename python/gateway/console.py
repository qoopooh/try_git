#!/usr/bin/env python

############### Default Values ####################
GATEWAY = '192.168.1.39'
TCP_PORT = 1470
UUID = "bb8342aed2ab395f1512604d55b35027d7ea99bf"
###################################################

import sys, getopt
import socket
from encrypt import Encrypt

BUFFER_SIZE = 1024
REQ_PHONE_ID = "R,U,{uuid}\r\n"

def remove_newline(msg):
    return msg.replace('\r\n','')

def main(argv, gateway=GATEWAY, uuid=UUID):
    help_msg = argv[0] + ' -g <gateway> -u <uuid>' 

    try:
        opts, args = getopt.getopt(argv[1:], "hg:u:", ["gateway=", "uuid="])
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

    print gateway, uuid
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((gateway, TCP_PORT))
    s.send(REQ_PHONE_ID.format(uuid=uuid))
    data = s.recv(BUFFER_SIZE)
    print '[HANDSHAKE]', data
    if len(data) != 9:
        print 'Incorrect message'
        sys.exit(1)
    e = Encrypt()
    e.phone_id = data[4]
    e.uuid = uuid * 2
    while True:
        data = s.recv(BUFFER_SIZE)
        if len(data) < 1:
            print 'Disconnected'
            sys.exit()
        print "[RECV]", len(data), remove_newline(e.decrypt(data))

if __name__ == "__main__":
    main(sys.argv)

