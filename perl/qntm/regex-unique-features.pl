#!/usr/bin/env perl

use strict;
use warnings;

sub name_capture {
    my $string = "Name: John Doe, Age: 30";
    if ( $string =~ /Name:\s(?<name>[^,]+), Age:\s(?<age>\d+)/ ) {
        print "Captured Name: $+{name}\n";
        print "Captured Age: $+{age}\n";
    }
}

sub lookahead_lookbehind {
    my $string = "foo123bar";
    if ( $string =~ /(?<=foo)\d+(?=bar)/ ) {
        print "Lookahead/Lookbehind matched: $&\n";
    }
}

# Call the function to demonstrate named capture groups
name_capture();
lookahead_lookbehind();
