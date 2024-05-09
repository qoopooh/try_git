#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

say "Backreference Examples";
say "";

# --- Example 1: ([Pp])ython&\1ails ---

say "--- Example 1: ([Pp])ython&\\1ails ---";
say "This regex matches 'python&pails' or 'Python&Pails'.";
say
"The \1 backreference matches the same character captured by the first group ([Pp]).";
say "";

my $regex1 = qr/([Pp])ython&\1ails/;

my @tests1 = (
    "python&pails",    # should match
    "Python&Pails",    # should match
    "python&Pails",    # should NOT match
    "Python&pails"     # should NOT match
);

say "Testing strings against the regex:";
foreach my $test (@tests1) {
    if ( $test =~ $regex1 ) {
        say "  '$test' -> matches";
    }
    else {
        say "  '$test' -> does not match";
    }
}
say "";

# --- Example 2: (['"])[^\1]*\1 ---

say "--- Example 2: Matching Quoted Strings ---";
say "This regex is intended to match a single or double-quoted string.";
say
"The \1 backreference ensures the closing quote is the same as the opening quote.";
say "";

say
q{NOTE: The pattern (['"])[^\1]*\1, while common in some documentation, does not work as expected in Perl.};
say q{Inside a character class [], \1 is not a backreference.};
say
q{A better and more common way in Perl is to use a non-greedy match: (['"])(.*?)\1};
say "";

my $text2 =
q{He said "hello" and she said 'world'. Then he replied, "it's a 'nice' day".};
say "Original text: '$text2'";
say "";

# The /g modifier is used to find all matches in the string.
my $regex2 = qr/(['"])(.*?)\1/;

say "Finding all quoted strings in the text:";
while ( $text2 =~ /$regex2/g ) {
    say "  Matched full string: $&";
    say "    -> Quote type: $1";
    say "    -> Content:    $2";
}

