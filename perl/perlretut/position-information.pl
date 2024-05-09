#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x = "Mmm...donut, thought Homer";
$x =~ /^(Mmm|Yech)\.\.\.(donut|peas)/;    # matches
foreach my $exp ( 1 .. $#- ) {
    no strict 'refs';
    print "Match $exp: '$$exp' at position ($-[$exp],$+[$exp])\n";
}
