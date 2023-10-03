#!/usr/bin/env perl

use strict;
use warnings;

my @array = (
	"apples",
	"bananas",
	(
		"inner",
		"list",
		"several",
		"entries",
	),
	"cherries",
);

my $i = 0;
while($i < scalar @array) {
	print $i, ": ", $array[$i], "\n";
	$i++;
}

$i = 0;
until($i >= scalar @array) {
	print $i, " = ", $array[$i], "\n";
	$i++;
}

$i = 0;
do {
	print $i, ": ", $array[$i], "\n";
	$i++;
} while ($i < scalar @array);

for(my $i = 0; $i < scalar @array; $i++) {
	print $i, " = ", $array[$i], "\n";
}

foreach my $string ( @array ) {
	print "$string\n";
}

foreach my $i ( 0 .. $#array ) {
	print $i, ": ", $array[$i], "\n";
}

my %scientists = (
    "Newton" => "Isaac",
    "Einstein" => "Albert",
    "Darwin" => "Charles",
);

foreach my $key (sort keys %scientists) {
	print "$key: $scientists{$key}\n";
}

foreach ( @array ) {
	print "$_ ";
}

print "$_\n" foreach @array;

# Loop control
CANDIDATE: for my $candidate ( 2 .. 100 ) {
	for my $divisor ( 2 .. sqrt $candidate ) {
		next CANDIDATE if $candidate % $divisor == 0;
	}
	print "$candidate is prime\n";
}