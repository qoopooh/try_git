#!/usr/bin/env python

import os

def rem(f):
    os.remove(f)
    print f

for root, dirs, files in os.walk("./"):
    for name in files:
        if name.endswith((".bak", ".o", ".hzo", ".hzx", ".eep", ".~PrjGrp", \
                    ".db")):
            rem(os.path.join(root, name))
        if "conflicted" in name:
            rem(os.path.join(root, name))


