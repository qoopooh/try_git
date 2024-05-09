#!/usr/bin/env perl

use strict;
use warnings;

my $string  = "colourless green ideas sleep furiously";
my @matches = $string =~ m/(\w+)\s+((\w+)\s+(\w+))\s+(\w+)\s+(\w+)/;

print join ", ", map { "'" . $_ . "'" } @matches;

# prints "'colourless', 'green ideas', 'green', 'ideas', 'sleep', 'furiously'"

print "\n=== Global Match ===\n";
my $string = "a tonne of feathers or a tonne of bricks";
while ( $string =~ m/(\w+)/g ) {
    print "'" . $1 . "'\n";
}

my @matches = $string =~ m/(\w+)/g;
print join ", ", map { "'" . $_ . "'" } @matches;

print "\n=== /x ===\n";
"Hello world" =~ m/
  (\w+) # one or more word characters
  [ ]   # single literal space, stored inside a character class
  world # literal "world"
/x
  ; # /x flag allows your regular expression to contain whitespace (e.g., line breaks) and comments.

print "$1\n";
print "$0\n";
