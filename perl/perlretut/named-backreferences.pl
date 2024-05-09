#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $fmt1 = '(?<y>\d\d\d\d)-(?<m>\d\d)-(?<d>\d\d)';
my $fmt2 = '(?<m>\d\d)/(?<d>\d\d)/(?<y>\d\d\d\d)';
my $fmt3 = '(?<d>\d\d)\.(?<m>\d\d)\.(?<y>\d\d\d\d)';
for my $d (qw(2006-10-21 15.01.2007 10/31/2005)) {
    if ( $d =~ m{$fmt1|$fmt2|$fmt3} ) {
        say "day=$+{d} month=$+{m} year=$+{y}";
    }
}
