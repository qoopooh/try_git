#!/usr/bin/env python

import os

for root, dirs, files in os.walk("./"):
    for name in files:
        if name.endswith((".bak", ".o", ".hzo", ".hzx", ".eep")):
            os.remove(os.path.join(root, name))
            print name
        if "conflicted" in name:
            os.remove(os.path.join(root, name))
            print name


