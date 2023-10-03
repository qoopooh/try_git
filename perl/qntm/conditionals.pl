#!/usr/bin/env perl

use strict;
use warnings;

# if else
my $word = "antidisestablishmentarianism";
my $strlen = length $word;
if($strlen >= 15) {
	print "'", $word, "' is a very long word\n";
} elsif(10 <= $strlen && $strlen < 15) {
	print "'", $word, "' is a medium-length word\n";
} else {
	print "'", $word, "' is a short word\n";
}

# unless
my $temperature = 20;
unless($temperature > 30) {
	# try to avoid it as this case
	print $temperature, " degrees Celsius is not very hot\n";
} else {
	print $temperature, " degrees Celsius is actually pretty hot\n";
}

print "Oh no it's too cold" unless $temperature > 15; # best practice: easy to read

# ternary operator
my $gain = 48;
print "You gained ", $gain, " ", ($gain == 1 ? "experience point" : "experience points"), "!\n";