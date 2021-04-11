#!/usr/bin/env python3
"""Toggle sprinkler everytime
"""

import sys
from os import environ

import requests

if 'HOST' not in environ:
    # 192.168.1.xxx:pppp
    sys.exit()
HOST = environ['HOST']

def toggle():
    """Toggle status"""

    url = 'http://{}'.format(HOST)
    resp = requests.get(url)
    if resp.status_code != 200:
        print(resp.error)
        sys.exit()

    if resp.text == 'ON':
        req = 'OFF'
    else:
        req = 'ON'

    requests.post(url, data={'led':req})

    print("{} -> {}".format(resp.text, req))

if __name__ == '__main__':

    toggle()
