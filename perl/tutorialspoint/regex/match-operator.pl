#!/usr/bin/perl

use strict;
use warnings;

my $time = "12:30:45";

my ( $hour, $minute, $second ) = $time =~ /(\d+):(\d+):(\d+)/;
print "Hour: $hour, Minute: $minute, Second: $second\n";

print "=== Matching Only Once ===\n";

my @list = qw/food foosball subeo footnote terfoot canic footbrdige/;
my ( $first, $last );
foreach (@list) {
    $first = $1 if /(foo.*?)/;
    $last  = $1 if /(foo.*)/;
}
print "First: $first, Last: $last\n";

print "=== \$` \$& \$' ===\n";
my $string = "I saw Barney with Fred.";
$string =~ /Barney/;
print "Before: $` \n";
print "Matched: $& \n";
print "After: $' \n";
