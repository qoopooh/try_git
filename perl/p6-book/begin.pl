#!/usr/bin/env perl

use strict;
use warnings;
use feature 'say';

BEGIN {
    say 'BEGIN 1';
}

END {
    say 'END';
}
say 'Hello, World!';

BEGIN {
    say 'BEGIN 2';
}
CHECK {
    say 'CHECK';
}
INIT {
    say 'INIT';
}

