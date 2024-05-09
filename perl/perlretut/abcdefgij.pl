#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

#my $input = 'abcdefgij';
my $input = 'abefgij';

# Use explicit captures for each segment so all group variables are populated.
my ($out) = $input =~ /(ab(cd|ef)((gi)|j))/;
say "Output: $out";
say "Matched: $&";    # Entire matched string
say "Group 1: $1";    # First capturing group
say "Group 2: $2";    # Second capturing group
say "Group 3: $3";    # Third capturing group
say "Group 4: $4";    # Fourth capturing group
