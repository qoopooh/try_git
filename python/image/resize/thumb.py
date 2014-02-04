#!/usr/bin/env python

import os
import Image
from glob import glob

THUMB = "-thumb"

size = 1024, 768

def resize(infile):
    if THUMB in infile:
        return
    outfile = os.path.splitext(infile)
    im = Image.open(infile)
    im.thumbnail(size, Image.ANTIALIAS)
    im.save(outfile[0] + THUMB + outfile[1], "JPEG")
    print 'Thumbnailing', infile
    
for fn in glob('*.jpg'):
    resize(fn)

