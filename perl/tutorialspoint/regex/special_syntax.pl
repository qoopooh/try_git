#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

say "Special Syntax with Parentheses";
say "";

# --- Example 1: R(?#comment) ---

say "--- Example 1: R(?#comment) ---";
say "Matches 'R'. All the rest is a comment";
say "";

my $regex1 = qr/R(?#this is a comment)uby/;
my @tests1 = (
    "Ruby",             # should match
    "Ruby on Rails",    # should match
    "ruby",             # should NOT match (case-sensitive)
    "R uby",            # should NOT match (space not in pattern)
);

say "Regex: R(?#this is a comment)uby";
say "Testing strings:";
foreach my $test (@tests1) {
    if ( $test =~ $regex1 ) {
        say "  '$test' -> matches";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 2: R(?i)uby ---

say "--- Example 2: R(?i)uby ---";
say "Case-insensitive while matching 'uby'";
say "";

my $regex2 = qr/R(?i)uby/;
my @tests2 = (
    "Ruby",    # should match
    "Ruby",    # should match
    "RuBy",    # should match
    "rUBY",    # should NOT match (R is still case-sensitive)
    "ruby",    # should NOT match (R is still case-sensitive)
);

say "Regex: R(?i)uby";
say "The (?i) makes everything after it case-insensitive";
say "Testing strings:";
foreach my $test (@tests2) {
    if ( $test =~ $regex2 ) {
        say "  '$test' -> matches";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 3: R(?i:uby) ---

say "--- Example 3: R(?i:uby) ---";
say "Same as above - case-insensitive only for the grouped part";
say "";

my $regex3 = qr/R(?i:uby)/;
my @tests3 = (
    "Ruby",    # should match
    "Ruby",    # should match
    "RuBy",    # should match
    "rUBY",    # should NOT match (R is still case-sensitive)
    "ruby",    # should NOT match (R is still case-sensitive)
);

say "Regex: R(?i:uby)";
say "The (?i:...) makes only the grouped part case-insensitive";
say "Testing strings:";
foreach my $test (@tests3) {
    if ( $test =~ $regex3 ) {
        say "  '$test' -> matches";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 4: rub(?:y|le) ---

say "--- Example 4: rub(?:y|le) ---";
say "Group only without creating \\1 backreference";
say "";

my $regex4 = qr/rub(?:y|le)/;
my @tests4 = (
    "ruby",      # should match
    "ruble",     # should match
    "rub",       # should NOT match
    "rubies",    # should NOT match
);

say "Regex: rub(?:y|le)";
say
"The (?:...) is a non-capturing group - it groups but doesn't create a backreference";
say "Testing strings:";
foreach my $test (@tests4) {
    if ( $test =~ $regex4 ) {
        say "  '$test' -> matches (matched: '$&')";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

say "Demonstrating non-capturing vs capturing groups:";
my $text = "I found a ruby and a ruble";

say "With capturing group: rub(y|le)";
while ( $text =~ /rub(y|le)/g ) {
    say "  Matched: '$&', Capture group \$1: '$1'";
}

say "With non-capturing group: rub(?:y|le)";
while ( $text =~ /rub(?:y|le)/g ) {
    say "  Matched: '$&', Capture group \$1: "
      . ( defined($1) ? "'$1'" : "undefined" );
}
say "";
