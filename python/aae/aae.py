#!/usr/bin/env python

debug = False

prefix = ('A', 'A', 'E')
splitter = (1, 2, 3, 4)
CommandBytes = {
    'GetSerial':            (0x01, 0x01),
    'GetReaderType':        (0x01, 0x02),
    'GetHardwareRev':       (0x01, 0x03),
    'GetSoftwareRev':       (0x01, 0x04),
    'GetBootloaderRev':     (0x01, 0x05),
    'GetAttenuation':       (0x02, 0x01),
    'GetFrequency':         (0x02, 0x02),
    'SetHeartbeat':         (0x03, 0x02),
    'SetAntennaPower':      (0x03, 0x03),
    'SetAttenuation':       (0x03, 0x04),
    'SetFrequency':         (0x03, 0x05),
    'RestoreFactory':       (0x03, 0x20),
    'SaveSettings':         (0x03, 0x21),
    'SetParam':             (0x03, 0x30),
    'GetParam':             (0x03, 0x31),
    'InventorySingle':      (0x50, 0x01),
    'InventoryCyclic':      (0x50, 0x02),
    'ReadFromTag':          (0x50, 0x03),
    'WriteToTag':           (0x50, 0x04),
    'HeartbeatInt':         (0x90, 0x01),
    'InventoryCyclicInt':   (0x90, 0x02),
}


class Protocol():

    def build(self, command, data=None):
        p = [ord(x) for x in prefix]
        p.append(splitter[0])
        p += CommandBytes[command]
        p.append(splitter[1])
        if (debug):
            print type(data)
        if not data:
            p += [0, splitter[3]]
        elif not isinstance(data, list):
            p += [1, splitter[2], data, splitter[3]]
        else:
            p += [len(data), splitter[2]]
            p += data
            p += [splitter[3]]
        p.append(self.checksum(p))
        return p

    def extract(self, data):
        cs = False


        return cs, command, data

    def checksum(self, data):
        if (debug):
            print_hex(data, True)
        cs = 0
        for b in data:
            cs ^= b
        return cs


def print_hex(data, p=False):
    s = " ".join("{0:02x}".format(c) for c in data)
    if (p):
        print s
    return s

def main():
    p = Protocol();
    d = p.build('GetSerial')
    print_hex(d, True)
    d = p.build('InventoryCyclic', [0])
    print_hex(d, True)
    d = p.build('InventoryCyclic', 1)
    print_hex(d, True)

if __name__ == '__main__':
    main()

