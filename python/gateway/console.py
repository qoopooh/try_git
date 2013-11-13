#!/usr/bin/env python

import sys, getopt

def main(argv):
    gateway = 'localhost'
    uuid = "AAAAAAAAAABBBBBBBBBBCCCCCCCCCCDDDDDDDDDD"
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

if __name__ == "__main__":
    main(sys.argv)

