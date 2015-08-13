#!/usr/bin/env perl

use strict;
use warnings;

my $colour    = "Indigo";
my $scalarRef = \$colour;

print $colour."\n";         # "Indigo"
print $scalarRef."\n";      # e.g. "SCALAR(0x182c180)"
print ${ $scalarRef }."\n"; # "Indigo"
print $$scalarRef."\n";     # "Indigo"
