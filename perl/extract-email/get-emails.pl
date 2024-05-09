#!/usr/bin/perl

use strict;
use warnings;

while (<>) {
    print "$1\n" if /([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})/;
}
