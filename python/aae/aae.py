#!/usr/bin/env python

#prefix = ('A', 'A', 'E')
prefix = (0x41, 0x41, 0x45)
splitter = (1, 2, 3, 4)
Command = {
    'GetSerial':        (0x01, 0x01),
    'GetReaderType':    (0x01, 0x02),
}

class Protocol():

    def build(self, command, data=None):
        p = []
        p += prefix
        p.append(splitter[0])
        p += Command[command]
        p.append(splitter[1])
        if not data:
            p += [0, splitter[3]]
        else:
            p += [len(data), splitter[2], data, splitter[3]]
        p.append(self.checksum(p))
        return p

    def checksum(self, data):
        sm = 0
        for b in data:
            sm ^= b
        return sm


def print_hex(data, p):
    s = " ".join("{0:02x}".format(c) for c in data)
    if (p):
        print s
    return s

def main():
    p = Protocol();
    d = p.build('GetSerial')
    print_hex(d, True)

if __name__ == '__main__':
    main()

