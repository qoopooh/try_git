#!/usr/bin/env perl

use strict;
use warnings;

sub hyphenate {

  # Extract the first argument from the array, ignore everything else
  my $word = shift @_;

  # An overly clever list comprehension
  $word = join "-", map { substr $word, $_, 1 } (0 .. (length $word) - 1);
  return $word;
} # hyphenate

print hyphenate("Example"), "\n";

#
# Call by reference
#
my $x = 7;

sub reassign {
  $_[0] = 42;
}

reassign($x);
print "$x\n"; # 42

#
# Unpacking arguments
#
sub left_pad {
  #my $oldString = shift @_;
  #my $width     = shift @_;
  #my $padChar   = shift @_;
  # or one row assign
  my ($oldString, $width, $padChar) = @_;

  return ($padChar x ($width - length $oldString)) . $oldString;
}

print left_pad("hello", 10, "+"), "\n"; # +++++hello

#
# For large number of arguments, let's use hash instead
#
sub right_pad {
  my %args = @_;

  return $args{"str"} . ($args{"pad"} x ($args{"width"} - length $args{"str"}))
}
print right_pad("str" => "hello", "width" => 10, "pad" => "+"), "\n"; # hello+++++

#
# Returning values
#
sub contextualSubroutine {
  # Caller wants a list. Return a list
  return ("Everest", "K2", "Etna") if wantarray;

  # Caller wants a scalar. Return a scalar
  return 3;
}

my @array = contextualSubroutine();
print "@array\n"; # Everest K2 Etna

my $scalar = contextualSubroutine();
print "$scalar\n"; # 3
print "\n";
