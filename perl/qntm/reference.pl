#!/usr/bin/env perl

use strict;
use warnings;

# hashs and arrays can contain only scalar
my @outer = ("Sun", "Mercury", "Venus", undef, "Mars");
my @inner = ("Earth", "Moon");
$outer[3] = @inner;
print $outer[3]; # "2"

# A reference is created using a backslash.
my $colour    = "Indigo";
my $scalarRef = \$colour;
print "\n$scalarRef\n";    	# e.g. "SCALAR(0x182c180)"
print "${ $scalarRef }\n";	# "Indigo"
print "$$scalarRef\n";	# "Indigo"

# Using arrow for array and hash
my @colours = ("Red", "Orange", "Yellow", "Green", "Blue");
my $arrayRef = \@colours;

print "$colours[0]\n";       # direct array access
print "${ $arrayRef }[0]\n"; # use the reference to get to the array
print "$arrayRef->[0]\n";    # exactly the same thing

my %atomicWeights = ("Hydrogen" => 1.008, "Helium" => 4.003, "Manganese" => 54.94);
my $hashRef = \%atomicWeights;

print $atomicWeights{"Helium"}; # direct hash access
print "\n${ $hashRef }{Helium}\n";  # use a reference to get to the hash
print $hashRef->{"Helium"};     # exactly the same thing - this is very common

# a data structure
my %owner1 = (
	"name" => "Santa Claus",
	"DOB"  => "1882-12-25",
);
my %owner2 = (
	"name" => "Mickey Mouse",
	"DOB"  => "1928-11-18",
);
my @owners = ( \%owner1, \%owner2 );
my %account = (
	"number" => "12345678",
	"opened" => "2000-01-01",
	"owners" => \@owners,
);
print "\n$account{owners}[1]{name}\n"; # Mickey Mouse


# Use square brackets for an anonymous array and braces for an anonymous hash
my %account1 = (
	"number" => "31415926",
	"opened" => "3000-01-01",
	"owners" => [
		{
			"name" => "Philip Fry",
			"DOB"  => "1974-08-06",
		},
		{
			"name" => "Hubert Farnsworth",
			"DOB"  => "2841-04-09",
		},
	],
);
print "$account1{owners}[1]{name}\n"; # Hubert Farnsworth

# extract owners
my @owners2 = @{ $account1{"owners"} };
my %owner21 = %{ $owners2[0] };
my %owner22 = %{ $owners2[1] };
print "$owner22{name}\n"; # Hubert Farnsworth


# How to shoot yourself in the foot with array references
my @array1 = (1, 2, 3, 4, 5);
print @array1; # "12345"
my @array2 = [1, 2, 3, 4, 5];
print @array2; # e.g. "ARRAY(0x182c180)" because of [] is a reference to an anonymous, five-element array

# This scalar is a reference to an anonymous, five-element array
my $array3Ref = [1, 2, 3, 4, 5];
print $array3Ref;      # e.g. "ARRAY(0x22710c0)"
print "\n@{ $array3Ref }\n"; # "1 2 3 4 5"
print @$array3Ref;     # "12345"

