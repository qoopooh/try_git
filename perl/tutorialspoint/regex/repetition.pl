#!/usr/bin/perl

use strict;
use warnings;
use feature 'say';

# 1. ruby? - Matches "rub" or "ruby": the y is optional
my $text1 = "rub";
my $text2 = "ruby";
my $text3 = "rubyy";

say "1. ruby?";
say "  '$text1' matches: ", $text1 =~ /ruby?/ ? "yes" : "no";
say "  '$text2' matches: ", $text2 =~ // ? "yes" : "no";
say "  '$text3' matches: ", $text3 =~ // ? "yes" : "no";
say "";

# 2. ruby* - Matches "rub" plus 0 or more ys
my $text4 = "rub";
my $text5 = "ruby";
my $text6 = "rubyyy";
my $text7 = "rubz";

say "2. ruby*";
say "  '$text4' matches: ", $text4 =~ /ruby*/ ? "yes" : "no";
say "  '$text5' matches: ", $text5 =~ // ? "yes" : "no";
say "  '$text6' matches: ", $text6 =~ // ? "yes" : "no";
say "  '$text7' matches: ", $text7 =~ // ? "yes" : "no";
say "";

# 3. ruby+ - Matches "rub" plus 1 or more ys
my $text8  = "rub";
my $text9  = "ruby";
my $text10 = "rubyyy";

say "3. ruby+";
say "  '$text8' matches: ",  $text8  =~ /ruby+/ ? "yes" : "no";
say "  '$text9' matches: ",  $text9  =~ // ? "yes" : "no";
say "  '$text10' matches: ", $text10 =~ // ? "yes" : "no";
say "";

# 4. \d{3} - Matches exactly 3 digits
my $text11 = "123";
my $text12 = "12";
my $text13 = "1234";

say "4. \\d{3}";
say "  '$text11' matches: ", $text11 =~ /\d{3}/ ? "yes" : "no";
say "  '$text12' matches: ", $text12 =~ // ? "yes" : "no";
say "  '$text13' matches: ", $text13 =~ // ? "yes" : "no";
say "";

# 5. \d{3,} - Matches 3 or more digits
my $text14 = "123";
my $text15 = "12345";
my $text16 = "12";

say "5. \\d{3,}";
say "  '$text14' matches: ", $text14 =~ /\d{3,}/ ? "yes" : "no";
say "  '$text15' matches: ", $text15 =~ // ? "yes" : "no";
say "  '$text16' matches: ", $text16 =~ // ? "yes" : "no";
say "";

# 6. \d{3,5} - Matches 3, 4, or 5 digits
my $text17 = "123";
my $text18 = "1234";
my $text19 = "12345";
my $text20 = "123456";
my $text21 = "12";

say '6. \\d{3,5}';
say "  '$text17' matches: ", $text17 =~ /\d{3,5}/ ? "yes" : "no";
say "  '$text18' matches: ", $text18 =~ // ? "yes" : "no";
say "  '$text19' matches: ", $text19 =~ // ? "yes" : "no";
say "  '$text20' matches: ", $text20 =~ // ? "yes" : "no";
say "  '$text21' matches: ", $text21 =~ // ? "yes" : "no";
say "";
