#!/usr/bin/python

import os
import subprocess

def rem(name, path=None):
    if path is None:
        f = name
    else:
        f = os.path.join(path, name)

    os.remove(f)
    print "[DEL]", f

def svnrmdir(name, path=None):
    f = os.path.join(path, name)
    f = f.replace(" ", "\ ")
    f = f.replace("(", "\(")
    f = f.replace(")", "\)")
    f = f.replace("$", "\$")

    print "[DEL]", f
    os.popen('svn rm ' + f)  # it works with some error print out

def svnrm(name, path=None):
    devnull = open(os.devnull, 'w')
    f = os.path.join(path, name)
    f = f.replace(" ", "\ ")
    f = f.replace("(", "\(")
    f = f.replace(")", "\)")
    f = f.replace("$", "\$")
    os.popen('svn rm ' + f)  # it works with some error print out
#subprocess.Popen(['svn', 'rm', f], stdout=devnull) # still not work
    print "[DEL]", f


# this routine does not good for subversion since we could not just remove folder
for root, dirs, files in os.walk("./"):
    for name in dirs:
        if name == "History" or "Logs" in name:
            svnrmdir(name, root)
devnull = open(os.devnull, 'w')
subprocess.Popen(['svn', 'ci', '-m', '"Remove altium generated folders"'], stdout=devnull) # still not work


for root, dirs, files in os.walk("./"):
    for name in files:
        if name.lower().endswith(( \
                    ".bak", ".o", ".hzo", ".hzx", ".eep", ".htm", \
                    ".~prjgrp", ".pcbdoc.zip", "schdoc.zip", "cam.zip", \
                    ".log", "preview", \
                    )):
            # Cannot remove *.db since subversion
            #rem(name=name, path=root)
            svnrm(name=name, path=root)
        if "conflicted" in name:
            rem(name=name, path=root)
devnull = open(os.devnull, 'w')
subprocess.Popen(['svn', 'ci', '-m', '"Remove altium generated files"'], stdout=devnull) # still not work

