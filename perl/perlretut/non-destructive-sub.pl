#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x = "Cats are great. \x{ 263a }";
my $y = $x =~ s/Cats/Dogs/r =~ s/Dogs/Frogs/r =~ s/Frogs/Hedgehogs/r, "\n";
say $x;    # Keeps "Cats are great."
say $y;    # prints "Hedgehogs are great."

$x = "BOB";
say $x;
say $x =~ /^\p{IsUpper}/;    # matches, uppercase char class
say $x =~ /^\P{IsUpper}/;    # doesn't match, char class sans uppercase
say $x =~ /^\p{IsLower}/;    # doesn't match, lowercase char class
say $x =~ /^\P{IsLower}/;    # matches, char class sans lowercase
