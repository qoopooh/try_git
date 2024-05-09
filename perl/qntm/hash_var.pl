#!/usr/bin/env perl

use strict;

# use warnings;

my %scientists = (
    "Newton"   => "Isaac",
    "Einstein" => "Albert",
    "Darwin"   => "Charles",
);

print $scientists{"Newton"} . "\n";      # "Isaac"
print $scientists{"Einstein"} . "\n";    # "Albert"
print $scientists{"Darwin"} . "\n";      # "Charles"
print $scientists{"Dyson"};              # returns

my @scientists = %scientists;
print "\n%scientists\n";
print "$scientists{Einstein}\n";
print "@scientists\n";
print "===\n";

# Same name for different variable types
my $data = "orange";
my @data = ("purple");
my %data = ( "0" => "blue" );

print "$data\n";         # "orange"
print "$data[0]\n";      # "purple"
print $data["0"];        # "purple"
print "\n$data{0}\n";    # "blue"
print $data{"0"};        # "blue"

print "\n===\n";

# Nested lists (not recommended)
my @array = (
    "apples", "bananas", ( "inner", "list", "several", "entries", ), "cherries",
);

print $array[0] . "\n";    # "apples"
print $array[1] . "\n";    # "bananas"
print $array[2] . "\n";    # "inner"
print $array[3] . "\n";    # "list"
print $array[4] . "\n";    # "several"
print $array[5] . "\n";    # "entries"
print $array[6] . "\n";    # "cherries"
print $array[7];           # undef, so prints "" and raises a warning

print "\n===\n";

# Nested data structures (not recommended)
my %hash = (
    "beer"    => "good",
    "bananas" => (
        "green"  => "wait",
        "yellow" => "eat",
    ),
);

# The above raises a warning because the hash was declared using a 7-element list
print $hash{"beer"} . "\n";       # "good"
print $hash{"bananas"} . "\n";    # "green"
print $hash{"wait"} . "\n";       # "yellow";
print $hash{"eat"} . "\n";        # undef, so prints "" and raises a warning
