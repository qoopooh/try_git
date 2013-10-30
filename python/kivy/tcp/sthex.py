__all__ = ['str_to_hex', 'hexstr_to_hex', 'hex_to_hexstr', 'hex_to_str',
        'hex_trim']

class HexToStringError(Exception):
    pass

def str_to_hex(s):
    return map(ord, s)

def hexstr_to_hex(s):
    return map(ord, s.decode("hex"))

def hex_to_hexstr(l):
    h = str_to_hex(l)
    return ''.join('{0:02x}'.format(b) for b in h)

def hex_to_str(l):
    s = ''
    for i in l:
        if 31 < i < 127:
            s += chr(i)
        else:
            raise HexToStringError()
    return s

def hex_trim(s):
    for c in (' ','\t','\n','\r','-'):
        s = s.replace(c, '')
           
    return s

