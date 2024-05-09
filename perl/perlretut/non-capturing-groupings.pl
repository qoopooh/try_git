#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

my $x   = '12aba34ba5';
my @num = split /(a|b)+/, $x;    # @num = ('12','a','34','a','5')
say "@num";
@num = split /(?:a|b)+/, $x;     # @num = ('12','34','5')
say "@num";
