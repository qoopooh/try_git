#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x = "There once was a girl\nWho programmed in Perl\n";

say $x =~ /^Who/;      # doesn't match, "Who" not at start of string
say $x =~ /^Who/s;     # doesn't match, "Who" not at start of string
say $x =~ /^Who/m;     # matches, "Who" at start of second line
say $x =~ /^Who/sm;    # matches, "Who" at start of second line

say $x =~ /girl.Who/;      # doesn't match, "." doesn't match "\n"
say $x =~ /girl.Who/s;     # matches, "." matches "\n"
say $x =~ /girl.Who/m;     # doesn't match, "." doesn't match "\n"
say $x =~ /girl.Who/sm;    # matches, "." matches "\n"

say '---';

say $x =~ /^Who/m;         # matches, "Who" at start of second line
say $x =~ /\AWho/m;        # doesn't match, "Who" is not at start of string

say $x =~ /girl$/m;        # matches, "girl" at end of first line
say $x =~ /girl\Z/m;       # doesn't match, "girl" is not at end of string

say $x =~ /Perl\Z/m;       # matches, "Perl" is at newline before end
say $x =~ /Perl\z/m;       # doesn't match, "Perl" is not at end of string
