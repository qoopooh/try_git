#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

# Capture the specific animal substring so list context assigns it.
my ($out) = "cats and dogs" =~ /(cat|dog|bird)/;    # matches "cat"
say $out;                                           # prints "cat"
($out) = "cats and dogs" =~ /(dog|cat|bird)/;       # matches "cat"
say $out;                                           # prints "cat"

# No parantheses, so no capture.
($out) = "cats" =~ /c|ca|cat|cats/;                 # matches "c"
say $out;                                           # prints 1

($out) = "cats" =~ /(c|ca|cat|cats)/;               # matches "c"
say $out;                                           # prints "c"
($out) = "cats" =~ /(cats|cat|ca|c)/;               # matches "cats"
say $out;                                           # prints "cats"

($out) = "cats and dogs" =~ /(g|t)s/;
say $out;                                           # prints "t"
say $1;                                             # prints "t"
say $&;                                             # prints "ts"

($out) = "cats and dogs" =~ /(g|t)(s)/;
say $out;                                           # prints "t"
say $1;                                             # prints "t"
say $2;                                             # prints "s"
say $&;                                             # prints "ts"

say '---';
my $x = "the cat in the hat";

#$x =~ /^(.*)(.at)(.*)$/; # grab "cat" in $2, but greedy so $1 is "the cat in the "
$x =~ /^(.*?)(.at)(.*)$/;                           # grab "cat" in $2
say "\$&: $&";
say "1: $1";
say "2: $2";
say "3: $3";
