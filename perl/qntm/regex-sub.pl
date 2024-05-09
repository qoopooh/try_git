#!/usr/bin/env perl

use strict;
use warnings;

my $string = "Good morning world";
$string =~ s/world/Vietnam/;
print $string, "\n";    # "Good morning Vietnam"
