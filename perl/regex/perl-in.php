<?php
//atomic       → MATCH
//lookbehind+  → ERROR: Internal error
//lookbehind{1,3} → MATCH
//lookbehind alt → MATCH
//conditional  → MATCH
//conditional  → MATCH
//embedded     → ERROR: Internal error
//recursive    → NO MATCH
//recursive    → NO MATCH

function test($name, $pattern, $subject) {
    $ok = @preg_match($pattern, $subject);
    if ($ok === false) {
        echo str_pad($name,12) . " → ERROR: " . preg_last_error_msg() . "\n";
    }
    else {
        echo str_pad($name,12) . " → " . ($ok ? "MATCH" : "NO MATCH") . "\n";
    }
}

// 1) Atomic grouping
test('atomic', '/^(?>a+)(b)$/', "aaaaab");

// 2) Lookbehind
//    a) unbounded (will error)
test('lookbehind+', '/(?<=\d+)\w+/', "123abc");
//    b) bounded but variable (will still error)
test('lookbehind{1,3}', '/(?<=\d{1,3})\w+/', "123abc");
//    c) fixed-width via alternation (ok)
test('lookbehind alt', '/(?<=(?:\d{1}|\d{2}|\d{3}))\w+/', "123abc");

// 3) Conditional based on lookahead
test('conditional', '/^(?(?=a).a|.b)$/', "aa");
test('conditional', '/^(?(?=a).a|.b)$/', "bb");

// 4) Embedded-code (Perl-only)
test('embedded', '/foo(?{ echo "Hi"; })bar/', "foobar");

// 5) Recursion for balanced parens
$rec = '/^
  (\(                     # open paren
     (?: [^()]+            #  non-paren chars
       | (?R)              #  recurse entire pattern
     )*
   \))$                    # close paren
/x';
test('recursive', $rec, "(foo(bar)(baz))");
test('recursive', $rec, "(foo(bar(baz))");
