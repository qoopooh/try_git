#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

#my $a99a = '([a-z])(\d)\g2\g1';        # matches a11a, g22g, x33x, etc.
my $a99a =
  '([a-z])(\d)\g{-1}\g{-2}';  # same as above, but using relative backreferences
my $line = "code=e99e";
if ( $line =~ /^(\w+)=$a99a$/ ) {    # unexpected behavior!
    say "$1 is valid";
}
else {
    say "bad line: '$line'";
}
