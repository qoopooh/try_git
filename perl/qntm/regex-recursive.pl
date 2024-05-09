#!/usr/bin/env perl

use strict;
use warnings;

#my $string = '((abc) (def))'; # Balanced
#my $string = '((abc)def))'; # Unbalanced
my $string = '( (abc)( def (ghi) ))';    # Unbalanced

if (
    $string =~ /^
    (?&balanced)        # Entry point: named subpattern
    $
(?(DEFINE)
    (?<balanced>        # Named pattern 'balanced'
        \(
            (?:
                [^()]*      # Non-parenthesis characters
                |
                (?&balanced)  # Recursive call to match nested ()
            )*
        \)
    )
)/x
  )
{
    print "Balanced parentheses!\n";
}
else {
    print "Unbalanced parentheses!\n";
}
