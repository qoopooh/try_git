#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $text = "<p>one</p><p>two</p>";

say "Greedy vs. Non-Greedy Matching";
say "Text: $text";
say "";

# Greedy matching: .* matches as much as possible
if ( $text =~ /<p>.*<\/p>/ ) {
    say "Greedy .* match: $&";
}

# Non-greedy matching: .*? matches as little as possible
if ( $text =~ /<p>.*?<\/p>/ ) {
    say "Non-greedy .*? match: $&";
}
