#!/usr/bin/env python

import os, sys
import Image

size = 1024, 768

for infile in sys.argv[1:]:
    outfile = os.path.join(os.path.dirname(os.path.realpath(__file__)), os.path.basename(infile))
    print outfile
    if infile != outfile:
#try:
        im = Image.open(infile)
        im.thumbnail(size, Image.ANTIALIAS)
        im.save(outfile, "JPEG")
#except IOError:
#print "cannot create thumbnail for '%s'" % infile

