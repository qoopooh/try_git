#!/usr/bin/env perl

use strict;
# use warnings;

my %scientists = (
    "Newton" => "Isaac",
    "Einstein" => "Albert",
    "Darwin" => "Charles",
);

print $scientists{"Newton"};   # "Isaac"
print $scientists{"Einstein"}; # "Albert"
print $scientists{"Darwin"};   # "Charles"
print $scientists{"Dyson"};    # returns

my @scientists = %scientists;
print "\n%scientists\n";
print "$scientists{Einstein}\n";
print "@scientists\n";
print "\n";

my $data = "orange";
my @data = ("purple");
my %data = ( "0" => "blue");

print "$data\n";      # "orange"
print "$data[0]\n";   # "purple"
print $data["0"]; # "purple"
print "\n$data{0}\n";   # "blue"
print $data{"0"}; # "blue"

print "\n";
