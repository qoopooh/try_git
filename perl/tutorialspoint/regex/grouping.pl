#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

say "Grouping Examples";
say "";

my $text1 = "a123b45";
say "--- Example 1: \\D\\d+ ---";
say "No group: + repeats only the \\d";
say "Regex: /\\D\\d+/ on text '$text1'";
if ( $text1 =~ /(\D\d+)/ ) {
    say "Matched: $1";
}
say "";

my $text2 = "a1b2c3";
say "--- Example 2: (\\D\\d)+ ---";
say "Grouped: + repeats the (\\D\\d) pair";
say "Regex: /(\\D\\d)+/ on text '$text2'";
if ( $text2 =~ /((\D\d)+)/ ) {
    say "Matched: $1";    # Matches 'a1b2c3'
}
my $text3 = "a12b3";
say "Regex: /(\\D\\d)+/ on text '$text3'";
if ( $text3 =~ /((\D\d)+)/ ) {
    say "Matched: $1";    # Only matches 'a1'
}
say "";

say "--- Example 3: ([Pp]ython(, )?)+ ---";
say "Matches 'Python' or 'python', optionally followed by ', ' and repeated.";

my $regex3 = qr/^([Pp]ython(, )?)+$/;

my @tests = (
    "Python",                 "python",
    "Python, python, Python", "Python, ",
    "python, python",         "Java",
    "Pythons"
);

foreach my $test (@tests) {
    if ( $test =~ $regex3 ) {
        say "'$test' -> matches";
    }
    else {
        say "'$test' -> does not match";
    }
}
