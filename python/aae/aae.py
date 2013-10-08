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
        '''This method will return:
           Check sum result: should be True
           Command: one of CommandBytes
           Payload: can be None
           Offset: Extracted position of data
        '''
        cs, command, payload = False, None, None
        
        p = [ord(x) for x in prefix]
        if (p != data[:3]):
            return cs, command, payload, 1
        for k in CommandBytes:
            if (CommandBytes[k] == (data[4], data[5])):
                command = k
        payload_len = data[7]
        if (debug):
            print data, payload_len
        if payload_len < 1:
            if splitter != (data[3], data[6], 3, data[8]):
                return cs, command, payload, 9
            if data[9] is self.checksum(data[:9]):
                cs =True
            return cs, command, payload, 10

        if (payload_len > len(data) - 10):
            return cs, command, payload, 8

        payload = data[9:9 + payload_len]
        if splitter != (data[3], data[6], 3, data[9 + payload_len]):
            return cs, command, payload, 10 + payload_len
        if data[payload_len + 10] is self.checksum(data[:payload_len + 10]):
            cs =True

        return cs, command, payload, 11 + payload_len

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
    assert(print_hex(p.build('GetSerial')) == '41 41 45 01 01 01 02 00 04 42')
    assert(print_hex(p.build('InventoryCyclic', [0])) == '41 41 45 01 50 02 02 01 03 00 04 12')
    assert(print_hex(p.build('InventoryCyclic', 1)) == '41 41 45 01 50 02 02 01 03 01 04 13')

    set_hb_off = [0x41, 0x41, 0x45, 0x01, 0x03, 0x02, 0x02, 0x03, 0x03, 0x00, 0x00, 0xFA, 0x04, 0xB9]
    assert(p.extract(set_hb_off) == (True, 'SetHeartbeat', [0, 0, 250], 14))
    get_sw = [0x41, 0x41, 0x45, 0x01, 0x01, 0x04, 0x02, 0x00, 0x04, 0x47]
    assert(p.extract(get_sw) == (True, 'GetSoftwareRev', None, 10))

if __name__ == '__main__':
    main()

