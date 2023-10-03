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

# map
my @capitals = ("Baton Rouge", "Indianapolis", "Columbus", "Montgomery", "Helena", "Denver", "Boise");

print "\n";
print join ", ", map { uc $_ } @capitals;

# grep
print "\n";
print join ", ", grep { length $_ == 6 } @capitals; # "Helena, Denver"

# grep count
print "\n";
print scalar grep { $_ eq "Columbus" } @capitals; # "1"

# sort
my @elevations = (19, 1, 2, 100, 3, 98, 100, 1056);

print "\n";
print join ", ", sort @elevations; # "1, 100, 100, 1056, 19, 2, 3, 98"

# sort explain
print "\n";
print join ", ", sort { $a cmp $b } @elevations; # "1, 100, 100, 1056, 19, 2, 3, 98"

# sort number
print "\n";
print join ", ", sort { $a <=> $b } @elevations; # 1, 2, 3, 19, 98, 100, 100, 1056

print "\n";
