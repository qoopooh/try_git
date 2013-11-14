#!/usr/bin/env python

class Encrypt(object):

    def __init__(self, uuid="1111111111222222222233333333334444444444", phone_id='0'):
        self.uuid = uuid
        self.phone_id = phone_id

    @property
    def uuid(self):
        return self._uuid

    @uuid.setter
    def uuid(self, val):
        length = len(val)
        if length < 40:
            val += '0' * (40 - length)
            val *= 2
        elif length == 40:
            val *= 2
        elif length == 80:
            pass
        else:
            return
        self._uuid = val

    def encrypt(self, msg):
        '''Input: unencrypted message
        Output: phone_id + encrypted message
        '''
        if len(msg) > len(self._uuid):
            msg = msg[:len(self._uuid)]
        if self.phone_id == '0':
            return '0,' + msg
        return self._enc(msg)

    def _enc(self, msg):
        out = self.phone_id + ','
        i = 0
        while i < len(msg):
            ch = msg[i]
            if '0' <= ch <= '9' or \
                'A' <= ch <= 'Z' or \
                'a' <= ch <= 'z':
                shift = ord(self._uuid[0]) + ord(self._uuid[i+1])
                while shift > 0:
                    ch = chr(ord(ch) + 1)
                    if '9' < ch < 'A':
                        ch = 'A'
                    elif 'Z' < ch < 'a':
                        ch = 'a'
                    elif ch > 'z':
                        ch = '0'
                    shift -= 1
            out += ch
            i += 1
        return out

    def decrypt(self, msg):
        if len(msg) > len(self._uuid):
            msg = msg[:len(self._uuid)]
        if msg[0] == '0':
            return msg[2:]
        elif msg[:3] == 'C,U' or msg[:3] == 'I,V' or \
            msg[:3] == 'I,N' or msg[:3] == 'C,N':
            return msg
        return self._dec(msg[2:])

    def _dec(self, msg):
        out = ''
        i = 0
        while i < len(msg):
            ch = msg[i]
            if '0' <= ch <= '9' or \
                'A' <= ch <= 'Z' or \
                'a' <= ch <= 'z':
                shift = ord(self._uuid[0]) + ord(self._uuid[i+1])
                while shift > 0:
                    ch = chr(ord(ch) - 1)
                    if ch < '0':
                        ch = 'z'
                    elif '9' < ch < 'A':
                        ch = '9'
                    elif 'Z' < ch < 'a':
                        ch = 'Z'
                    shift -= 1
            out += ch
            i += 1
        return out


if __name__ == '__main__':
    e = Encrypt()
    msg = e.encrypt("E,L,1,O,A,B,< phuchit Kanjanakhan >,Berm,9876543210ABCDEF\n", '8')
    print 'enc', msg
    msg = e.decrypt(msg)
    print 'dec', msg

