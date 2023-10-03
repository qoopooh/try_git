#!/usr/bin/env perl

use strict;
use warnings;

print "Hello World\n";

#my $undef = undef;
#print $undef;

#my $undef2;
#print $undef2;

my $num = 4040.5;
print $num."\n";

my $string = "world\n";
print "Hello ".$string."\n";


#my $str1 = "4G";
#my $str2 = "4H";

#print $str1 . $str2 . "\n";
#print $str1 + $str2 . "\n";
#print $str1 eq $str2 . "\n";
#print $str1 == $str2 . "\n";

# The classic error
print "yes" == "no"; # "1" with two warnings; both values evaluate to 0 when used as numbers
