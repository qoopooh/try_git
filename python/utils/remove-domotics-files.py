#!/usr/bin/env python

import os

def rem(name, path=None):
    if path is None:
        f = name
    else:
        f = os.path.join(path, name)

    os.remove(f)
    print "[DEL]", f

for root, dirs, files in os.walk("./"):
    for name in files:
        if name.endswith(( \
                    ".bak", ".o", ".hzo", ".hzx", ".eep", ".~PrjGrp", \
                    )):
            # Cannot remove *.db since subversion
            rem(name=name, path=root)
        if "conflicted" in name:
            rem(name=name, path=root)


