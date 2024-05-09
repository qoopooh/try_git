#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x = "cat dog
house\tcar";
while ( $x =~ /(\w+)/g ) {
    my $word = $1;
    my $pos  = pos($x);
    say "Found '$word' at position $pos";
}

my @words = ( $x =~ /(\w+)/g );
say "Words: @words";
