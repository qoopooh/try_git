#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my @samples = (
    '9:30 PST',     # civil time with zone
    '09:45 CET',    # civil time with leading zero hour and zone
    '1230 UTC',     # military time with zone
    '0730 EST',     # military time with leading zero and zone
    '9:30',         # civil time without zone (should not match)
);

for my $time (@samples) {
    print "$time -> ";
    if ( $time =~ /(?|\b(\d\d|\d):(\d\d)|(\d\d)(\d\d))\s+([A-Z][A-Z][A-Z])/ ) {
        say "hour=$1 minute=$2 zone=$3";
    }
    else {
        say 'no match';
    }
}
