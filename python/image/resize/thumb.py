#!/usr/bin/env python

import os
import Image
from glob import glob

THUMB = "-thumb"
SIZE = 1024, 768
TYPES = ('*.jpg', '*.JPG', '*.png', '*.jpeg')

def resize(infile):
    if THUMB in infile:
        return
    outfile = os.path.splitext(infile)
    im = Image.open(infile)
    im.thumbnail(SIZE, Image.ANTIALIAS)
    im.save(outfile[0] + THUMB + outfile[1], "JPEG")
    print 'Thumbnailing', infile
    
for files in TYPES:
    for fn in glob(files):
        resize(fn)
