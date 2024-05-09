#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x = "I catch the housecat 'Tom-cat' with catnip";
$x =~ /(cat(*pla:\s))/;    # matches 'cat' in 'housecat'
say $&;                    # prints 'cat'
say $1;
say "----";

my @catwords = ( $x =~ /(?<=\s)cat\w+/g );    # matches,
                                              # $catwords[0] = 'catch'
                                              # $catwords[1] = 'catnip'
say for @catwords;
say "----";
say $x =~ /\bcat\b/;             # matches 'cat' in 'Tom-cat'
say $&;                          # prints 'cat'
say $x =~ /(?<=\s)cat(?=\s)/;    # doesn't match; no isolated 'cat' in
                                 # middle of $x

say "--- foobar ---";
$x = "foobar";
say $x =~ /foo(?!bar)/;          # doesn't match, 'bar' follows 'foo'
say $x =~ /foo(?!baz)/;          # matches, 'baz' doesn't follow 'foo'
say $x =~ /(?<!\s)foo/;          # matches, there is no \s before 'foo'

say "--- blank-separated words ---";
my $str  = "one two - --6-8";
my @toks = split / \s+              # a run of spaces
              | (?<=\S) (?=-)    # any non-space followed by '-'
              | (?<=-)  (?=\S)   # a '-' followed by any non-space
              /x, $str;          # @toks = qw(one two - - - 6 - 8)
say "@toks";
