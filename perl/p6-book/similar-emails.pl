#!/usr/bin/perl
#
# Installation: sudo cpan String::Similarity
#
# Usage: ./similar-emails.pl [needle] [haystack_file]
#
use strict;
use warnings;
use String::Similarity;

if ( @ARGV == 0 ) {
    print "Usage: $0 [needle] [haystack_file]\n";
    print "\n";
    print
      "  needle         - Email address to compare (e.g., john\@example.com)\n";
    print
"  haystack_file  - File containing emails to compare against (optional)\n";
    print "\n";
    print "If only needle provided, compares it against default emails.\n";
    print
      "If both provided, compares needle against emails in haystack_file.\n";
    exit 0;
}

my $needle        = $ARGV[0];
my $haystack_file = $ARGV[1];

my @emails;
my $threshold = 0.8;

# Streaming mode: compare needle against haystack
if ( $haystack_file && -f $haystack_file ) {
    open( my $fh, '<', $haystack_file ) or die "Cannot open $haystack_file: $!";
    while ( my $email = <$fh> ) {
        chomp $email;
        my $similarity = similarity( $needle, $email );
        if ( $similarity >= $threshold ) {
            printf "Similar (%.2f): %s <-> %s\n", $similarity, $needle, $email;
        }
    }
    close($fh);
}
else {
    # Use default emails
    @emails = qw/
      john.doe@example.com
      john.dae@example.com
      jane.smith@example.com
      /;
    foreach my $email (@emails) {
        my $similarity = similarity( $needle, $email );
        if ( $similarity >= $threshold ) {
            printf "Similar (%.2f): %s <-> %s\n", $similarity, $needle, $email;
        }
    }
}
