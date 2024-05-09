#!/usr/bin/perl

use strict;
use warnings;

my $text = "The quick brown fox jumps over the lazy dog.";
$text =~ s/fox/cat/;    # Substitute 'fox' with 'cat'
print "$text\n";        # Output: The quick brown cat jumps over the lazy dog.
