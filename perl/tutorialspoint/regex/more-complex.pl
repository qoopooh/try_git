#!/usr/bin/perl

use strict;
use warnings;

my $string  = "Cats go Catatonic\nWhen given Catnip";
my ($start) = ( $string =~ /\A(.*?) / );
my @lines   = $string =~ /^(.*?) /gm;
print "First word: $start\n", "Line starts: @lines\n";  # First word: Cats
                                                        # Line starts: Cats When

#print "\n=== Matching Boundaries ===\n";
#/\bcat\b/ # Matches 'the cat sat' but not 'cat on the mat'
#/\Bcat\B/ # Matches 'verification' but not 'the cat on the mat'
#/\bcat\B/ # Matches 'catatonic' but not 'polecat'
#/\Bcat\b/ # Matches 'polecat' but not 'catatonic'

print "\n=== The \\G Assertion ===\n";
my $string = "The time 16:27:59 10/04/25 is: 12:31:02 on 2025/10/04";
$string =~ /:\s+/g;
print "String after first match: '$&'\n";    # String after first match: ': '
my ($time) = ( $string =~ /\G(\d+:\d+:\d+)/ );
$string =~ /.+\s+/g;
print "String after second match: '$&'\n"
  ;    # String after second match: '12:31:02 on '
my ($date) = ( $string =~ m{\G(\d+/\d+/\d+)} );

print "\n$string\nTime: $time, Date: $date\n";

