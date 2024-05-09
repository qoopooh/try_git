#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $lang = 'DE';    # use German
my $text = "das";

#$lang = 'EN'; # switch to English
#$text = "the";

say "matched"
  if $text =~ /(?(?{
                      $lang eq 'EN'; # is the language English?
                     })
                   the |             # if so, then match 'the'
                   (der|die|das)     # else, match 'der|die|das'
                 )
                /xi;

my $length = 3;
my $char   = 'a';
my $x      = 'aaaaabb';
$x =~ /(??{$char x $length})/x;    # matches, there are 5 of 'a'
say $&;

my $string = "super calif ragi listic expia lidocious";
my %count;
$count{ lc($1) }++ while $string =~ /([ieoau])/ig;
printf "%3d '%s'\n", $count{$_}, $_ for ( sort keys %count );

"that this" =~ m@(?{print "Start at position ", pos, "\n";})
                 t(?{print "t1\n";})
                 h(?{print "h1\n";})
                 i(?{print "i1\n";})
                 s(?{print "s1\n";})
                     |
                 t(?{print "t2\n";})
                 h(?{print "h2\n";})
                 a(?{print "a2\n";})
                 t(?{print "t2\n";})
                 (?{print "Done at position ", pos, "\n";})
                @x;
