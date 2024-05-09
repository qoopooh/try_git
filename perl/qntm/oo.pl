#!/usr/bin/perl

use lib '.';

require Animal;

my $animal = {
    "legs"   => 4,
    "colour" => "brown",
};    # $animal is an ordinary hash reference

print ref $animal, "\n";    # "HASH"
bless $animal, "Animal";    # now it is an object of class "Animal"
print ref $animal, "\n";    # "Animal"
$animal->eat( "insects", "curry", "eucalyptus" );

print "\n=== Second Animal ===\n";

my $animal2 = Animal->new();
print "Animal2 has ", $animal->{"legs"}, " leg(s)\n";

print "\n=== Dog ===\n";

my $puppy = Dog->new;
$puppy->eat( "chocolate", "curry", "eucalyptus" );
