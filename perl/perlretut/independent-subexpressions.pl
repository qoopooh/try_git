#!/usr/bin/perl
# Still hard to understand?
use strict;
use warnings;
use feature 'say';

# Demonstrates how independent (atomic) subexpressions affect backtracking.
# Atomic groups keep what they match and do not reconsider other options,
# which can change whether the overall pattern succeeds.

sub describe_match {
    my ( $string, $regex ) = @_;

    if ( $string =~ $regex ) {
        my $start    = $-[0];
        my $end      = $+[0];
        my $consumed = substr( $string, $start, $end - $start );
        my $rest     = substr( $string, $end );
        return ( "matched", $consumed, $rest );
    }

    return ( "did not match", q{}, $string );
}

sub compare_patterns {
    my ( $title, $strings_ref, $cooperative, $atomic, $note ) = @_;

    say $title;
    say "  ordinary pattern : $cooperative";
    say "  atomic pattern   : $atomic";
    say "  note             : $note" if defined $note && length $note;

    for my $string ( @{$strings_ref} ) {
        my ( $plain_state, $plain_consumed, $plain_rest ) =
          describe_match( $string, $cooperative );
        my ( $atomic_state, $atomic_consumed, $atomic_rest ) =
          describe_match( $string, $atomic );

        say "    string '$string'";
        say "      ordinary -> $plain_state"
          . (
            $plain_state eq 'matched'
            ? " (consumed '$plain_consumed', rest '$plain_rest')"
            : q{}
          );
        say "      atomic   -> $atomic_state"
          . (
            $atomic_state eq 'matched'
            ? " (consumed '$atomic_consumed', rest '$atomic_rest')"
            : q{}
          );
    }

    say '';
}

sub tag_team_demo {
    my ($string) = @_;

    say "Handing off with /g and \\G on '$string'";

    my $work        = $string;
    my $first_match = q{};
    if ( $work =~ /a*/g ) {
        my $start = $-[0];
        my $end   = $+[0];
        $first_match = substr( $work, $start, $end - $start );
        say "  /a*/g grabbed '$first_match'";
    }
    else {
        say "  /a*/g found nothing";
    }

    my $second_match;
    if ( $work =~ /\Gab/g ) {
        my $start = $-[0];
        my $end   = $+[0];
        $second_match = substr( $work, $start, $end - $start );
        say "  /\\Gab/g saw '$second_match'";
    }
    else {
        say "  /\\Gab/g could not match";
    }

    say '';
}

say
  "Using independent subexpressions (atomic groups) to prevent backtracking\n";

compare_patterns(
    'Example 1: Greedy star cooperates with the rest of the pattern',
    [qw(ab aaab b aaaab)],
    qr/a*ab/,
    qr/(?>a*)ab/,
'The plain a* can give characters back; the atomic version never backtracks.'
);

compare_patterns(
    'Example 2: Atomic alternation locks in the first successful branch',
    [ 'aba', 'abba', 'aaba' ],
    qr/(ab|a)ba/,
    qr/(?>ab|a)ba/,
    'Once (?>ab|a) chooses ab it will not try the shorter a alternative.'
);

compare_patterns(
    'Example 3: Nested quantifiers with and without atomic groups',
    [ 'aaaaac', 'aaaaad', 'aaaab' ],
    qr/(a+)+c/,
    qr/(?>a+)+c/,
'Both patterns either match or fail, but the atomic version avoids work by keeping each chunk of a+ intact.'
);

tag_team_demo('ab');
tag_team_demo('aaab');

if ( $] >= 5.028 ) {
    say 'Bonus: (*atomic:...) works the same as (?>...) when available';
    my $atomic_regex = eval 'qr/(?:(*atomic:a*)ab)/';
    if ($@) {
        chomp( my $error = $@ );
        say "  this perl could not compile (*atomic:...) : $error";
    }
    else {
        my ( $state, $consumed, $rest ) =
          describe_match( 'aaab', $atomic_regex );
        say "  pattern (*atomic:a*)ab on 'aaab' -> $state"
          . (
            $state eq 'matched'
            ? " (consumed '$consumed', rest '$rest')"
            : q{}
          );
    }
    say '';
}

say "Done.";
