#!/usr/bin/env python

import threading
import time

class Monitor(threading.Thread):
    def __init__(self, mon):
        threading.Thread.__init__(self)
        self.mon = mon

    def run(self):
        while True:
            if self.mon[0] == 2:
                print "Mon = 2"
                self.mon[0] = 3
            elif self.mon[0] == -1:
                break

