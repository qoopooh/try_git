#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
from PIL import Image
from glob import glob

THUMB = "-thumb"
SIZE = 1024, 768
TYPES = ('*.jpg', '*.JPG', '*.png', '*.jpeg')

def resize(fn):
    if THUMB in fn:
        return
    outfile = os.path.splitext(fn)
    im = Image.open(fn)
    im.thumbnail(SIZE, Image.ANTIALIAS)
    im.save(outfile[0] + THUMB + outfile[1], "JPEG")
    print 'Thumbnailing', fn

def remove(fn):
    if THUMB in fn:
        return
    os.remove(fn)
    print 'Removed', fn

for files in TYPES:
    for fn in glob(files):
        resize(fn)
        remove(fn)

