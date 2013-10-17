#!/usr/bin/env python

import serial, time
from threading import Thread
from Queue import Queue

from simplefsm import SimpleFSM

#debug = True
debug = False

prefix = ('A', 'A', 'E')
splitter = (1, 2, 3, 4)

def response_none(payload):
    return payload is None

def response_success(payload):
    return payload[0] is 0

def response_data(payload):
    return payload

def response_epc_cyclic(payload):
    return len(payload)


AAE_COMMAND = { # code, response
    'GetSerial':            ((0x01, 0x01), response_data),
    'GetReaderType':        ((0x01, 0x02), response_data),
    'GetHardwareRev':       ((0x01, 0x03), response_data),
    'GetSoftwareRev':       ((0x01, 0x04), response_data),
    'GetBootloaderRev':     ((0x01, 0x05), response_data),
    'GetCurrentState':      ((0x01, 0x06), response_data),
    'GetStatusRegister':    ((0x01, 0x07), response_data),
    'GetAttenuation':       ((0x02, 0x01), response_data),
    'GetFrequency':         ((0x02, 0x02), response_data),
    'SetHeartbeat':         ((0x03, 0x02), response_success),
    'SetAntennaPower':      ((0x03, 0x03), response_success),
    'SetAttenuation':       ((0x03, 0x04), response_success),
    'SetFrequency':         ((0x03, 0x05), response_success),
    'RestoreFactory':       ((0x03, 0x20), None),
    'SaveSettings':         ((0x03, 0x21), None),
    'SetParam':             ((0x03, 0x30), None),
    'GetParam':             ((0x03, 0x31), response_data),
    'InventorySingle':      ((0x50, 0x01), response_data),
    'InventoryCyclic':      ((0x50, 0x02), response_success),
    'ReadFromTag':          ((0x50, 0x03), response_data),
    'WriteToTag':           ((0x50, 0x04), response_success),
    'HeartbeatInt':         ((0x90, 0x01), None),
    'InventoryCyclicInt':   ((0x90, 0x02), response_epc_cyclic),
}



class Protocol():
    """To build or extract AAE protocol"""

    def __init__(self):
        self._wait_rx_counter = 0

    def build(self, command, payload=None):
        """Please assign command from AAE_COMMAND, and payload as list (if need)
            The result will return as byte list
        """
        p = [ord(x) for x in prefix]
        p.append(splitter[0])
        if isinstance(command, tuple):
            p += AAE_COMMAND[command[0]][0]
            payload = command[1]
        else:
            p += AAE_COMMAND[command][0]
        p.append(splitter[1])
        if payload is None:
            p += [0, splitter[3]]
        elif not isinstance(payload, list):
            p += [1, splitter[2], payload, splitter[3]]
        else:
            p += [len(payload), splitter[2]]
            p += payload
            p += [splitter[3]]
        p.append(self.checksum(p))
        return p

    def extract(self, data):
        '''Get data as byte list
            - Normally data should have length more than 9
            - Please see Checksum (cs) for succesful extraction
            - If you have more than 1 protocol on list, please continue extract from Offset
        The return:
            Check sum result: should be True
            Command: one of AAE_COMMAND
            Payload: can be None
            Offset: Extracted position of data
        '''
        cs, command, payload = False, None, None

        if (debug):
            print type(data), data
        p = [ord(x) for x in prefix]
        if (p != data[:3]):
            return cs, None, 1
        for k in AAE_COMMAND:
            if (AAE_COMMAND[k][0] == (data[4], data[5])):
                command = k
        payload_len = data[7]
        if (debug):
            print data, payload_len
        if payload_len < 1:
            if splitter != (data[3], data[6], 3, data[8]):
                return cs, None, 9
            if data[9] is self.checksum(data[:9]):
                cs =True
            return cs, (command, payload), 10

        if (payload_len >= len(data) - 10):
            if (self._wait_rx_counter > 2):
                self._wait_rx_counter = 0
                return cs, None, len(data) # clear buffer
            self._wait_rx_counter += 1
            return cs, None, 0  # Waiting for the rest

        payload = data[9:9 + payload_len]
        if splitter != (data[3], data[6], 3, data[9 + payload_len]):
            return cs, None, 10 + payload_len
        if data[payload_len + 10] is self.checksum(data[:payload_len + 10]):
            cs =True

        self._wait_rx_counter = 0

        return cs, (command, payload), 11 + payload_len

    def checksum(self, data):
        """ XOR whole bytes on data list
            The result is integer (max = 255)
        """
        if (debug):
            print_hex(data, True)
        cs = 0
        for b in data:
            cs ^= b
        return cs


class Tx():

    def __init__(self, interface):
        self.i = interface
        self.busy = False
        self.sm = SimpleFSM({
            'initial': 'idle',
            'transitions': {
                'idle': {'sending'},
                'sending': {'wait_response', 'success'},
                'wait_response': {'resending', 'check_response'},
                'check_response': {'resending', 'wait_response', 'success'},
                'resending': {'wait_response', 'failure'},
                'success': {'idle', 'sending', 'check_response'},
                'failure': {'idle', 'sending', 'check_response'},
            }
        })
        self.resp = None

    def send(self, packet):
        if self.busy:
            return False
        self.sm.change_to('sending')
        command, payload = packet
        p = Protocol()
        self.ba = bytearray(p.build(command, payload))
        self.i.write(self.ba)
        if AAE_COMMAND[command][1] is None:
            self.sm.change_to('success')
            return True

        self.tx_cmd = command
        self.sm.change_to('wait_response')
        self.start = time.clock()
        self.resend_cnt = 0
        self.busy = True # not allow another command
        return True

    def get_response(self, data):
        if not self.busy:
            return
        command, payload = data

        self.rx_cmd = command
        if AAE_COMMAND[command][1] is None:
            self.resp = None
        else:
            self.resp = AAE_COMMAND[command][1](payload)
            c1 = self.sm.current
#self.sm.change_to('check_response')
            if self.sm.current is not 'check_response':
                try:
                    self.sm.change_to('check_response')
                except:
                    print('current', c1)
                    raise Error
                
                #self.sm.force_change_to('check_response')
            #finally:
            print('AAE_COMMAND resp', command, self.resp, self.sm.current),


    def exec_(self):
        if not self.busy:
            return

        #First Decision Table
        if self.sm.current is 'wait_response':
            t1 = time.clock() - self.start
            if (t1 > 2):
                self.sm.change_to('resending')
        elif self.sm.current is 'check_response':
            if self.rx_cmd is not self.tx_cmd:
                self.sm.change_to('wait_response')
                return
            if self.resp:
                self.sm.change_to('success')
            else:
                self.sm.change_to('wait_response')
        elif self.sm.current is 'success' or self.sm.current is 'failure':
            if self.resp is None:
                print(self.sm.current, self.tx_cmd)
            elif isinstance(self.resp, bool) or isinstance(self.resp, int):
                print(self.sm.current, self.resp)
            else:
                resp = ''.join('{0:02x}'.format(b) for b in self.resp)
                print(self.sm.current, self.tx_cmd, resp)
            self.sm.change_to('idle')
            self.busy = False


        #Second Decision Table (have to do immediately)
        if self.sm.current is 'resending':
            if self.resend_cnt > 3:
                self.sm.change_to('failure')
            else:
                self.i.write(self.ba)
                self.resend_cnt += 1
                self.sm.change_to('wait_response')


class Rx(Thread):
    def __init__(self, interface, parent):
        Thread.__init__(self)
        self.i = interface
        self.q = Queue()
        self.s_count = 0
        self._parent = parent

    def run(self):
        while self.i.isOpen() and self._parent:
            n = self.i.inWaiting()
            if n < 1:
                self.s_count += 1
                if (self.s_count > 5):
                    self.s_count = 0
                    time.sleep(0.02)
                continue

            selfks_count = 0
            data = self.i.read(n)
            #print('rx_get', len(data), 'last_q', self.q.qsize())
            for d in data:
                self.q.put(d)


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
    assert(print_hex(p.build(('InventoryCyclic', 0))) == '41 41 45 01 50 02 02 01 03 00 04 12')

    set_hb_off = [0x41, 0x41, 0x45, 0x01, 0x03, 0x02, 0x02, 0x03, 0x03, 0x00, 0x00, 0xFA, 0x04, 0xB9]
    assert(p.extract(set_hb_off) == (True, ('SetHeartbeat', [0, 0, 250]), 14))
    get_sw = [0x41, 0x41, 0x45, 0x01, 0x01, 0x04, 0x02, 0x00, 0x04, 0x47]
    assert(p.extract(get_sw) == (True, ('GetSoftwareRev', None), 10))

if __name__ == '__main__':
    main()

