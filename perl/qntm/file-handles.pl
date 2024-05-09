#!/usr/bin/env perl

use strict;
use warnings;

my $filename = 'README.md';

my $result = open my $fh, '<', $filename;
if ( not $result ) {
    die "Could not open file '$filename': $!";
}

my $line_count = 0;
while ( my $line = <$fh> ) {
    $line_count++;
    chomp $line;
    print $line_count, ' ', $line, "\n";
}
