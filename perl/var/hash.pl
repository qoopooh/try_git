#!/usr/bin/env perl

use strict;
use warnings;

my %scientists = (
    "Newton"    => "Isaac",
    "Einstein"  => "Albert",
    "Darwin"    => "Charles",
);
my @scientists = %scientists;

print $scientists{"Newton"};
print "\n";
print $scientists{"Berm"};
print "\n";
print %scientists;
print "\n";
print "@scientists";

