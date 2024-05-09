#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

say "Anchor Examples - Specifying Match Positions";
say "";

# --- Example 1: ^Python ---

say "--- Example 1: ^Python ---";
say "Matches 'Python' at the start of a string or internal line";
say "";

my $regex1 = qr/^Python/;
my @tests1 = (
    "Python is great",    # should match
    "I love Python",      # should NOT match
    "Python\nPython",     # first line matches
);

say "Testing strings (single-line mode):";
foreach my $test (@tests1) {
    my $display = $test;
    $display =~ s/\n/\\n/g;    # show newlines clearly
    if ( $test =~ $regex1 ) {
        say "  '$display' -> matches";
    }
    else {
        say "  '$display' -> does not match";
    }
}
say "";

say "Testing with **/m** (multi-line mode):";
my $regex1m   = qr/^Python/m;
my $multiline = "Some text\nPython here\nMore text";
my @matches   = ( $multiline =~ /$regex1m/g );
say "  Text: 'Some text\\nPython here\\nMore text'";
if ( $multiline =~ $regex1m ) {
    say "  -> matches (Python at start of internal line)";
}
else {
    say "  -> does not match";
}
say "";

# --- Example 2: Python$ ---

say "--- Example 2: Python\$ ---";
say "Matches 'Python' at the end of a string or line";
say "";

my $regex2 = qr/Python$/;
my @tests2 = (
    "I love Python",          # should match
    "Python is great",        # should NOT match
    "Learn Python\ntoday",    # first line matches with /m
    "Learn Python\nPython",
);

say "Testing strings:";
foreach my $test (@tests2) {
    my $display = $test;
    $display =~ s/\n/\\n/g;
    if ( $test =~ $regex2 ) {
        say "  '$display' -> matches";
    }
    else {
        say "  '$display' -> does not match";
    }
}
say "";

# --- Example 3: \APython ---

say "--- Example 3: \\APython ---";
say "Matches 'Python' at the start of a string (not affected by /m)";
say "";

my $regex3 = qr/\APython/;
my @tests3 = (
    "Python is great",           # should match
    "I love Python",             # should NOT match
    "Some text\nPython here",    # should NOT match (even with /m)
);

say "Testing strings:";
foreach my $test (@tests3) {
    my $display = $test;
    $display =~ s/\n/\\n/g;
    if ( $test =~ $regex3 ) {
        say "  '$display' -> matches";
    }
    else {
        say "  '$display' -> does not match";
    }
}
say "";

# --- Example 4: Python\Z ---

say "--- Example 4: Python\\Z ---";
say "Matches 'Python' at the end of a string (not affected by /m)";
say "";

my $regex4 = qr/Python\Z/;
my @tests4 = (
    "I love Python",          # should match
    "Python is great",        # should NOT match
    "Learn Python\ntoday",    # should NOT match (even with /m)
);

say "Testing strings:";
foreach my $test (@tests4) {
    my $display = $test;
    $display =~ s/\n/\\n/g;
    if ( $test =~ $regex4 ) {
        say "  '$display' -> matches";
    }
    else {
        say "  '$display' -> does not match";
    }
}
say "";

# --- Example 5: \bPython\b ---

say "--- Example 5: \\bPython\\b ---";
say "Matches 'Python' at a word boundary";
say "";

my $regex5 = qr/\bPython\b/;
my @tests5 = (
    "Python",                       # should match
    "I love Python programming",    # should match
    "CPython",                      # should NOT match
    "Pythonic",                     # should NOT match
);

say "Testing strings:";
foreach my $test (@tests5) {
    if ( $test =~ $regex5 ) {
        say "  '$test' -> matches";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 6: \brub\B ---

say "--- Example 6: \\brub\\B ---";
say "\\B is nonword boundary: match 'rub' in 'rube' and 'ruby' but not alone";
say "";

my $regex6 = qr/\brub\B/;
my @tests6 = (
    "rub",        # should NOT match (word boundary at end)
    "rube",       # should match
    "ruby",       # should match
    "scrub",      # should NOT match (no word boundary at start)
    "rubbish",    # should match
);

say "Testing strings:";
foreach my $test (@tests6) {
    if ( $test =~ $regex6 ) {
        say "  '$test' -> matches";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 7: Python(?=!) ---

say "--- Example 7: Python(?=!) ---";
say "Matches 'Python', if followed by an exclamation point";
say "";

my $regex7 = qr/Python(?=!)/;
my @tests7 = (
    "Python!",             # should match
    "Python! is great",    # should match
    "Python",              # should NOT match
    "Python?",             # should NOT match
);

say "Testing strings:";
foreach my $test (@tests7) {
    if ( $test =~ $regex7 ) {
        say "  '$test' -> matches (matched: '$&')";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 8: Python(?!!) ---

say "--- Example 8: Python(?!!) ---";
say "Matches 'Python', if not followed by an exclamation point";
say "";

my $regex8 = qr/Python(?!!)/;
my @tests8 = (
    "Python",             # should match
    "Python?",            # should match
    "Python is great",    # should match
    "Python!",            # should NOT match
);

say "Testing strings:";
foreach my $test (@tests8) {
    if ( $test =~ $regex8 ) {
        say "  '$test' -> matches (matched: '$&')";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";
