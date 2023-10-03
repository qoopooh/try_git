#!/usr/bin/env perl

use strict;
use warnings;

my @counter = (1..10);
$counter[20]++;
my $len = @counter;

# print "$counter[19]\n";
print "$counter[20]\n";
print "$len\n";
