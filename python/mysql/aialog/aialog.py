#!/usr/bin/env python
# -*- coding: utf-8; -*- 

import urllib, json, logging
from aiadb import preparedb, update_price

URL = 'https://go-uranus.appspot.com/aia'

def get_json():
    response = urllib.urlopen(URL);
    data = json.loads(response.read())
    return data

def get_json_from_file():
    with open('data.json~') as data_file:    
        return json.load(data_file)

def print_prices(j):
    funds = j.get('Funds')

    for f in funds:
        print f.get('Name'), '::', f.get('Value')

def get_prices(j):
    """Return prices as dict
    """
    funds = j.get('Funds')
    out = {}
    for f in funds:
        out[f.get('Code')] = [f.get('Name'), f.get('Value')]
    return out

def get_time(j):
    return j.get('Information').get('Time')

def is_ok(j):
    return j.get('Status') == 'OK'

def init_log():
    logger = logging.getLogger('aialog')
    hdlr = logging.FileHandler('/var/tmp/aia.log')

    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr) 
    logger.setLevel(logging.DEBUG)
    return logger

if __name__ == "__main__":
    logger = init_log()

    try:
        preparedb()
    except Exception as err:
        logger.error(err)
        exit(1)

    #j = get_json_from_file()
    j = get_json()
    
    if not is_ok(j):
        logger.error('Load aia source:', j.get('Status'))
        exit(1)

    time = get_time(j)
    prices = get_prices(j)

    success = True
    for k, v in prices.iteritems():
        if not update_price(k, v[0], v[1], time):
            success = False

    if success:
        logger.info("Update price successfully for " + time)
    else:
        logger.warn("Cannot update price on " + time)

