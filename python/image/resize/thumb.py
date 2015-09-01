#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
from PIL import Image
from glob import glob

THUMB = "_thumb"
SIZE = 1024, 768
TYPES = ('*.jpg', '*.JPG', '*.png', '*.jpeg')

def resize(fn):
    if THUMB in fn:
        return
    outfile = os.path.splitext(fn)
    im = Image.open(fn)
    im.thumbnail(SIZE, Image.ANTIALIAS)
    try:
        im.save(outfile[0] + THUMB + outfile[1], "JPEG")
    except IOError as e:
        print 'IOError:', e, '\n'
        print '''# install libjpeg-dev with apt
sudo apt-get install libjpeg-dev
# reinstall pillow
pip install -I pillow'''
        quit()

    print 'Thumbnailing', fn

def remove(fn):
    if THUMB in fn:
        return
    os.remove(fn)
    print 'Removed', fn

for files in TYPES:
    for fn in glob(files):
        resize(fn)
        #remove(fn)

