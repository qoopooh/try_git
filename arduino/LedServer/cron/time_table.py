#!/usr/bin/env python3
"""ON/OFF sprinkler from a time table
"""
import argparse
import sys
from datetime import datetime, timedelta
from os import environ, path

import requests

TIME_TABLE = (
    ("7:00", "07:15"),
    ("17:30", "18:00"),
) # ON, OFF time

PARSER = argparse.ArgumentParser()
PARSER.add_argument('-v', '--verbose', help='print debug info', action='store_true')

if 'HOST' not in environ:
    # 192.168.1.xxx:pppp
    print("No host")
    sys.exit()
HOST = environ['HOST']


def check_table(verbose=False):
    """Set ON/OFF from time table

    Args:
        verbose (bool): print out data
    """

    url = 'http://{}'.format(HOST)
    try:
        resp = requests.get(url, timeout=5)
    except requests.exceptions.ConnectTimeout:
        print('timeout')
        return

    if resp.status_code != 200:
        print(resp.error)
        return

    led_status = resp.text
    table_status = _get_table(verbose)

    if led_status == table_status:
        if verbose:
            print("Keep", led_status)
        return

    requests.post(url, data={'led':table_status}, timeout=5)
    print("{} -> {}".format(led_status, table_status))


def _get_table(verbose):
    """Get LED status from table

    Args:
        verbose (bool): print out data

    Returns:
        string: ON / OFF
    """

    now = datetime.now()
    if verbose:
        print("now", now)

    for period in TIME_TABLE:
        start_arr = period[0].split(':')
        start = now.replace(hour=int(start_arr[0]), minute=int(start_arr[1]))
        if now < start:
            if verbose:
                print("Not start yet ({})".format(start))
            continue

        end_arr = period[1].split(':')
        end = now.replace(hour=int(end_arr[0]), minute=int(end_arr[1]))
        if now > end:
            if verbose:
                print("It's over ({})".format(end))
            continue

        return "ON"

    return "OFF"


def should_check_after_x_minutes(minutes):
    """Check the right time to query sprinkler

    Args:
        minutes (number): delay in minutes

    Returns:
        bool: should query now
    """

    basename = path.basename(__file__).replace('.py', '.txt')
    tmp_file = path.join('/tmp', basename)

    now = datetime.now()

    if not path.exists(tmp_file):
        with open(tmp_file, 'w') as outfile:
            outfile.write(now.isoformat())
        return True

    with open(tmp_file) as infile:
        last_check = datetime.fromisoformat(infile.read())

    if now - last_check > timedelta(minutes=minutes):
        with open(tmp_file, 'w') as outfile:
            outfile.write(now.isoformat())
        return True

    return False


if __name__ == '__main__':

    ARGS = PARSER.parse_args()
    if should_check_after_x_minutes(5):
        check_table(ARGS.verbose)
