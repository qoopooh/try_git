#!/usr/bin/perl
use strict;
use warnings;

die "Usage: $0 <markdown_file>\n" unless @ARGV == 1;

my $file = $ARGV[0];
die "File '$file' not found\n" unless -e $file;

# Read file
local $/;
open( my $fh, '<', $file ) or die "Cannot read '$file': $!\n";
my $content = <$fh>;
close($fh);

# Remove bold formatting
$content =~ s/(\*\*|__)(.+?)\1/$2/gs;

# Write back to same file
open( $fh, '>', $file ) or die "Cannot write '$file': $!\n";
print $fh $content;
close($fh);

print "Removed bold formatting from $file\n";
