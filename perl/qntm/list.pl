#!/usr/bin/env perl

use strict;

my @arr = ("one", 1, "three", 3, "five", 5);
my @arr2 = ("one" => 1, "three" => 3, "five" => 5);

print "@arr\n";
print "@arr2\n";

my %hash1 = ("one", 1, "three", 3, "five", 5);
my %hash2 = ("one" => 1, "three" => 3, "five" => 5);


print "$hash1{one}\n";
print "$hash2{five}\n";

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

print "@array\n";

my %hash = (
	"beer" => "good",
	"bananas" => (
		"green"  => "wait",
		"yellow" => "eat",
	),
);

# The above raises a warning because the hash was declared using a 7-element list

print $hash{"beer"};    # "good"
print $hash{"bananas"}; # "green"
print $hash{"wait"};    # "yellow";
print $hash{"eat"};     # undef, so prints """

my @bones   = ("humerus", ("jaw", "skull"), "tibia");
my @fingers = ("thumb", "index", "middle", "ring", "little");
my @parts   = (@bones, @fingers, ("foot", "toes"), "eyeball", "knuckle");
print "\n@parts\n";

my @array = ("Alpha", "Beta", "Gamma", "Pie");
my $arr_len = @array;
print "$arr_len\n"; # "4"   used scalar to get the length of an array

my $scalar = ("Alpha", "Beta", "Gamma", "Pie");
print "$scalar\n"; # "Pie"

my @array = ("Alpha", "Beta", "Goo");
my $scalar = "-X-";
print "@array\n";              # "AlphaBetaGoo";
print $scalar, @array, 98, "\n"; # "-X-AlphaBetaGoo98";

my $string = reverse "hello world";
print "$string\n"; # "dlrow olleh"