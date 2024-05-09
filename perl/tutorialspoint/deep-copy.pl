#!/usr/bin/perl
# Creates a FULL COPY (deep copy for simple data types), not a reference copy.

use strict;
use warnings;
use feature 'say';

my @array = ( 1, 2, 3, 4, 5 );
my @copy  = @array;              # FULL COPY

$copy[0] = 999;                  # Modify copy

print "Original: @array\n";      # Original: 1 2 3 4 5
print "Copy: @copy\n";           # Copy: 999 2 3 4 5

say "--- Reference Copy Example ---";
my $ref = \@array;                 # Reference - points to same data
$$ref[0] = 888;                    # Modifies original!
say "After ref change: @array";    # Original after ref change: 888 2 3 4 5

# or
my $ref = \@array;
$ref->[1] = 777;                   # Same thing, arrow notation
say "After ref change: @array";    # Original after ref change: 888 777 3 4 5
