#!/usr/bin/perl

use strict;
use warnings;
use utf8
  ; # Declares that the source code itself is UTF-8 encoded (good practice for Unicode)
use open ':std',
  ':encoding(UTF-8)';    # Sets STDIN, STDOUT, STDERR to use UTF-8 encoding
use feature 'say';

# Accept string parameter from command line
my $escaped_string = $ARGV[0]
  || '\u0e44\u0e21\u0e48\u0e2a\u0e32\u0e21\u0e32\u0e23\u0e16\u0e42\u0e2d\u0e19\u0e44\u0e14\u0e49';

# Use a regular expression to find and replace \uXXXX sequences
# Breakdown of the substitution:
#   s/ ... / ... /ge
#   - \\u([0-9a-fA-F]{4}) : Matches '\u' followed by exactly four hexadecimal characters.
#                           The parentheses capture the four hex chars into $1.
#   - chr(hex($1))       : This is the replacement part, evaluated as Perl code because of /e.
#                           - hex($1)    : Converts the hexadecimal string (e.g., "0e44") to its decimal value.
#                           - chr(...)   : Converts the decimal Unicode codepoint to the corresponding character.
#   - /g                 : Global replacement, replaces all occurrences in the string.
#   - /e                 : Evaluate the replacement part as Perl code.
$escaped_string =~ s/\\u([0-9a-fA-F]{4})/chr(hex($1))/ge;

say $escaped_string;
