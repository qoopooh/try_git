#!/usr/bin/perl
use strict;
use warnings;
use feature 'say';

# Lookahead Assertions
# Positive Lookahead (?=...)
say "Matched 'Java' in 'JavaScript'" if "JavaScript" =~ /Java(?=Script)/;
say "No match for 'Java' in 'JavaCode'" unless "JavaCode" =~ /Java(?=Script)/;

# Negative Lookahead (?!...)
say "Matched 'Java' in 'JavaCode'" if "JavaCode" =~ /Java(?!Script)/;
say "No match for 'Java' in 'JavaScript'" unless "JavaScript" =~ /Java(?!Script)/;

# Lookbehind Assertions
# Positive Lookbehind (?<=...)
# Match "Script" only if preceded by "Java"
say "Matched 'Script' in 'JavaScript'" if "JavaScript" =~ /(?<=Java)Script/;
say "No match for 'Script' in 'TypeScript'" unless "TypeScript" =~ /(?<=Java)Script/;

# Negative Lookbehind (?<!...)
# Match "Script" only if NOT preceded by "Java"
say "Matched 'Script' in 'TypeScript'" if "TypeScript" =~ /(?<!Java)Script/;
say "No match for 'Script' in 'JavaScript'" unless "JavaScript" =~ /(?<!Java)Script/;
