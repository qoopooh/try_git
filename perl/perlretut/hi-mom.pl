#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x = "abcdef";
$x =~ /abc(?{say "Hi Mom (abcdef)!";})def/;    # matches,
                                               # prints 'Hi Mom!'
$x =~ /aaa(?{say "Hi Mom (aaa)!";})def/;       # doesn't match,
                                               # no 'Hi Mom!'
$x =~ /abcd(?{say "Hi Mom (abcd)!";})/;        # matches again

$x = "aaaa";
my $count = 0;                                 # initialize 'a' count
my $c     = 0;                                 # initialize $c variable
$x =~ /(?{$c = 0;})         # initialize count
       ( a                        # match 'a'
         (?{$c = $c + 1;})  # increment count
       )*                         # do this any number of times,
       aa                         # but match 'aa' at the end
       (?{$count = $c;})          # copy $c var into $count
      /x;
say "'a' count is $count";

say "---";

$x = "aaaa";
$x =~ /(a(?{say "Yow";}))*aa/;
