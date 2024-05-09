#!/usr/bin/env perl
use strict;
use warnings;
use utf8;
use open qw(:std :utf8);

use Getopt::Long qw(GetOptions);
use Text::CSV_XS;
use Mojo::UserAgent;
use Mojo::JSON qw(encode_json);

# ---------- CLI FLAGS ----------
my $provider = 'glm';    # claude|glm
my $model    = '';       # override per provider
GetOptions(
    'provider=s' => \$provider,
    'model=s'    => \$model,
  )
  or die
  "Usage: $0 [--provider claude|glm] [--model NAME] input.csv > output.csv\n";
$provider =~ s/^\s+|\s+$//g;    # trim whitespace
$provider = lc $provider;       # lowercase
die "provider must be 'claude' or 'glm'\n"
  unless $provider =~ /\A(claude|glm)\z/;

# ---------- ENV CONFIG ----------
my $ANTHROPIC_API_KEY = $ENV{ANTHROPIC_API_KEY};
my $ZAI_API_KEY       = $ENV{Z_AI_API_KEY};

my $MAX_TOKENS    = $ENV{MAX_TOKENS}  ? 0 + $ENV{MAX_TOKENS}  : 1024;
my $CONCURRENCY   = $ENV{CONCURRENCY} ? 0 + $ENV{CONCURRENCY} : 5;
my $RETRY_MAX     = $ENV{RETRY_MAX}   ? 0 + $ENV{RETRY_MAX}   : 3;
my $REQ_TIMEOUT_S = $ENV{TIMEOUT_SEC} ? 0 + $ENV{TIMEOUT_SEC} : 45;

# Defaults per provider if --model not set
$model ||=
  $provider eq 'claude' ? 'claude-3-5-haiku-20241022' : 'glm-4.5-flash';

# ---------- INPUT ----------
my $infile = shift @ARGV
  or die "Usage: $0 [--provider ..] [--model ..] input.csv > output.csv\n";

# ---------- CSV READ ----------
my $csv_in = Text::CSV_XS->new( { binary => 1, auto_diag => 1 } );
open my $fh, "<:encoding(utf8)", $infile or die "Can't open $infile: $!";
my @rows;
while ( my $row = $csv_in->getline($fh) ) {
    push @rows, ( defined $row->[0] ? $row->[0] : '' );
}
close $fh;

# ---------- CSV OUT ----------
my $csv_out = Text::CSV_XS->new( { binary => 1, eol => "\n" } );
$csv_out->print( *STDOUT, [ "text", "translation_th" ] );
exit 0 unless @rows;

# ---------- HTTP CLIENT ----------
my $ua = Mojo::UserAgent->new;
$ua->inactivity_timeout( $REQ_TIMEOUT_S + 5 );
$ua->request_timeout( $REQ_TIMEOUT_S + 5 );
$ua->connect_timeout(15);
$ua->max_redirects(0);

# ---------- HELPERS ----------
sub with_timeout_p {
    my ( $promise, $seconds, $label ) = @_;
    my $timer_p = Mojo::Promise->new;
    my $tid     = Mojo::IOLoop->timer( $seconds =>
          sub { $timer_p->reject("$label timed out after ${seconds}s") } );
    return Mojo::Promise->race( $promise, $timer_p )
      ->finally( sub { Mojo::IOLoop->remove($tid) } );
}

# -------- Provider: Anthropic (Claude) --------
sub req_claude_p {
    my ( $text, $attempt ) = @_;
    $attempt = 1 unless defined $attempt;
    die "Missing ANTHROPIC_API_KEY\n" unless $ANTHROPIC_API_KEY;

    my $payload = {
        model      => $model,
        max_tokens => $MAX_TOKENS,
        messages   => [
            {
                role    => "user",
                content => [
                    {
                        type => "text",
                        text =>
"Translate this text to Thai. Keep proper names when appropriate. Return only the translation:\n\n$text"
                    }
                ]
            }
        ],
    };

    my $p = $ua->post_p(
        'https://api.anthropic.com/v1/messages' => {
            'content-type'      => 'application/json',
            'accept'            => 'application/json',
            'x-api-key'         => $ANTHROPIC_API_KEY,
            'anthropic-version' => '2023-06-01',
        } => encode_json($payload)
    )->then(
        sub {
            my $tx  = shift;
            my $res = $tx->result;
            die "network error: " . ( $tx->error->{message} // 'unknown' )
              unless $res;
            if ( $res->is_success ) {
                my $data = $res->json // {};
                my $out  = '';
                if ( $data->{content} && ref $data->{content} eq 'ARRAY' ) {
                    $out = join "", map {
                            ( $_->{type} || '' ) eq 'text'
                          ? ( $_->{text} // '' )
                          : ''
                    } @{ $data->{content} };
                }
                $out =~ s/^\s+|\s+$//g;
                $out ||= '(empty)';
                return $out;
            }
            my $code   = $res->code                        // 0;
            my $msg    = $res->message                     // 'HTTP error';
            my $detail = eval { $res->json } || $res->body // '';
            $detail = $detail->{error}{message} // $detail->{message}
              if ref $detail eq 'HASH';
            if ( $attempt < $RETRY_MAX && ( $code == 429 || $code >= 500 ) ) {
                my $backoff = ( 2**( $attempt - 1 ) ) * 0.5;
                my $d       = Mojo::Promise->new;
                Mojo::IOLoop->timer( $backoff => sub { $d->resolve } );
                return $d->then( sub { req_claude_p( $text, $attempt + 1 ) } );
            }
            die "HTTP $code $msg" . ( $detail ? " - $detail" : "" );
        }
    );
    return with_timeout_p( $p, $REQ_TIMEOUT_S, "Anthropic request" );
}

# -------- Provider: ZhipuAI (GLM) --------
my $ZAI_BASE = $ENV{ZAI_BASE} || 'https://api.z.ai/api/paas/v4';

sub req_glm_p {
    my ( $text, $attempt ) = @_;
    $attempt = 1 unless defined $attempt;
    die "Missing ZAI_API_KEY\n" unless $ZAI_API_KEY;

    my $payload = {
        model    => $model,    # e.g., glm-4.5-flash
        messages => [
            {
                role    => "system",
                content =>
"You are a translation engine. Output only the Thai translation—no extra text."
            },
            { role => "user", content => $text }
        ],
        max_tokens  => $MAX_TOKENS,
        temperature => 0.0,
        stream      => Mojo::JSON->false,
    };

    my $p = $ua->post_p(
        "$ZAI_BASE/chat/completions" => {
            'content-type'  => 'application/json',
            'accept'        => 'application/json',
            'authorization' => "Bearer $ZAI_API_KEY",
        } => encode_json($payload)
    )->then(
        sub {
            my $tx  = shift;
            my $res = $tx->result;
            die "network error: " . ( $tx->error->{message} // 'unknown' )
              unless $res;
            if ( $res->is_success ) {
                my $data = $res->json // {};
                my $out  = '';
                if (   $data->{choices}
                    && ref $data->{choices} eq 'ARRAY'
                    && $data->{choices}[0]{message} )
                {
                    $out = $data->{choices}[0]{message}{content} // '';
                }
                elsif ( $data->{message} && $data->{message}{content} ) {
                    $out = $data->{message}{content};
                }
                $out =~ s/^\s+|\s+$//g;
                $out ||= '(empty)';
                return $out;
            }
            my $code   = $res->code                        // 0;
            my $msg    = $res->message                     // 'HTTP error';
            my $detail = eval { $res->json } || $res->body // '';
            $detail = $detail->{error}{message} // $detail->{message}
              if ref $detail eq 'HASH';
            if ( $attempt < $RETRY_MAX && ( $code == 429 || $code >= 500 ) ) {
                my $backoff = ( 2**( $attempt - 1 ) ) * 0.5;
                my $d       = Mojo::Promise->new;
                Mojo::IOLoop->timer( $backoff => sub { $d->resolve } );
                return $d->then( sub { req_glm_p( $text, $attempt + 1 ) } );
            }
            die "HTTP $code $msg" . ( $detail ? " - $detail" : "" );
        }
    );
    return with_timeout_p( $p, $REQ_TIMEOUT_S, "Z.ai request" );
}

# Selector
sub translate_p {
    my ($text) = @_;
    return $provider eq 'glm' ? req_glm_p($text) : req_claude_p($text);
}

# ---------- JOB ENGINE (ordered progressive output) ----------
my @jobs = map { { idx => $_, text => $rows[$_] } } 0 .. $#rows;
my %results;
my $next_to_flush = 0;
my $inflight      = 0;
my $stopped       = 0;

sub try_flush {
    while ( exists $results{$next_to_flush} ) {
        my $orig = $rows[$next_to_flush] // '';
        my $tr   = delete $results{$next_to_flush};
        $csv_out->print( *STDOUT, [ $orig, $tr ] );
        $next_to_flush++;
    }
}

sub start_next {
    return if $stopped;
    return if $inflight >= $CONCURRENCY;
    return unless @jobs;

    while ( $inflight < $CONCURRENCY && @jobs ) {
        my $job = shift @jobs;
        $inflight++;
        translate_p( $job->{text} )
          ->then( sub { my $tr = shift; $results{ $job->{idx} } = $tr; } )
          ->catch(
            sub { my $err = shift; $results{ $job->{idx} } = "(error: $err)"; }
          )
          ->finally(
            sub {
                $inflight--;
                try_flush();
                if ( !@jobs && $inflight == 0 ) {
                    $stopped = 1;
                    Mojo::IOLoop->stop;
                }
                else { start_next(); }
            }
          );
    }
}

start_next();
Mojo::IOLoop->start unless $stopped;
