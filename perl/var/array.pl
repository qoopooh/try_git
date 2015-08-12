#!/usr/bin/env perl

use strict;
use warnings;

my @array = (
  "print",
  "these",
  "strings",
  "out",
  "for",
  "me", # trailing comma is okay
);

print "Message length: ".@array."\n";
print "Message: @array\n";
print 'Raw: @array';
print "\n";
print "Raw backslash: \@array\n";

