#!/usr/bin/env perl
use strict;
use warnings;
use feature 'say';

say "Perl‐only regex feature demo\n";

# Example 1: \K  (resets the match start so $& is only what follows \K)
{
    my $s = "foobar";
    if ( $s =~ /foo\Kbar/ ) {
        say "Example 1 (\\K): matched '$&' in '$s'";
    }

    # python re: no equivalent to \K
}

# Example 2: Atomic grouping (?>...)
{
    my $s = "aaaaaaaaab";
    if ( $s =~ /(?>a+)(b)/ ) {
        say "Example 2 (atomic grouping): captured group 1 = '$1'";
    }
    if ( $s =~ /(a+)(b)/ ) {
        say "  [with normal (a+)] captured group 1 = '$1', group 2 = '$2'";
    }
}

# Example 3: Variable‐width lookbehind (bounded to max 3 digits)
{
    my $s = "12345abc";
    if ( $s =~ /(?<=\d{1,3})\w+/ ) {
        say "Example 3 (var‐width lookbehind {1,3}): matched '$&'";
    }

    # python: re.PatternError: look-behind requires fixed-width pattern
}

# Example 4a: Embedded code (?{ ... })
{
    my $s = "foobar";

    #my $s = "foo  bar";
    #my $s = "foo\nbar";
    #my $s = "foo ba"; # no match
    $s =~
/foo(?{ say "Example 4a (embedded code): got 'foo' inside the match" })\s{0,}bar/;
}

# Example 4b: Conditional branching based on a look‐ahead
#   If the next char is 'a' match 'aa', else match 'bb'.
for my $s ( "aa", "ab", "ba", "bb" ) {
    if ( $s =~ /^(?(?=a).a|.b)$/ ) {
        say "Example 4b (conditional) '$s' matched";
    }
    else {
        say "Example 4b (conditional) '$s' did NOT match";
    }
}    # python supports conditional branching too

# Example 5: Recursive subpattern for balanced parentheses
{
    my $s_ok   = "(foo(bar)(baz))";
    my $s_fail = "(foo(bar(baz))";    # unbalanced

    #my $re = qr{
    #(?<P>                           # named subpattern P
    #\(
    #(?: [^()]+ | (?&P) )*      # non‐parens or recurse
    #\)
    #)
    #}x;
    my $re = /
        (?<P>                       # named subpattern P
          \(
            (?: [^()]+ | (?&P) )*   # non‐parens or recurse
          \)
        )
    /x;

    for my $s ( $s_ok, $s_fail ) {
        if ( $s =~ /^$re$/ ) {
            say "Example 5 (recursive): '$s' is balanced";
        }
        else {
            say "Example 5 (recursive): '$s' is NOT balanced";
        }
    }
}

say "\nDone.";
