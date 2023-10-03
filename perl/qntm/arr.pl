#!/usr/bin/env perl

use strict;
use warnings;

my @array = (
    "print",
    "these",
    "strings",
    "out",
    "for",
    "me",
);

print $array[-1]; # "me"
print $array[-2]; # "for"

print "\nThis array has ".(scalar @array)." elements\n"; # Size / Length
print "The last populated index is ".$#array;
print "\n@array\n";

# Manipulate array
#
# push / pop (stack)
print pop @array;   # me
push @array, "Bob", "Alice";
print "\n@array\n"; # print these strings out for Bob Alice

# shift / unshift
print shift @array; # print
unshift @array, "Write", "All";
print "\n@array\n"; # Write All these strings out for Bob Alice

# replace in range
print splice(@array, 1, 4, "one", "four"); # Allthesestringsout
print "\n@array\n"; # Write one fourfor Bob Alice

# join
print join(", ", @array), "\n";

print scalar reverse @array;

print "\n";
