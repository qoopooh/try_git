#!/usr/bin/env perl

use strict;
use warnings;

print "what\n" unless -e "/usr/bin/perl";
print "good\n" if -e "/usr/bin/perl";
