#!/usr/bin/env python3

import re

print("Special Syntax with Parentheses")
print()

# --- Example 1: R(?#comment) ---

print("--- Example 1: R(?#comment) ---")
print("Matches 'R'. All the rest is a comment")
print()

regex1 = re.compile(r'R(?#this is a comment)uby')
tests1 = [
    "Ruby",             # should match
    "Ruby on Rails",    # should match
    "ruby",             # should NOT match (case-sensitive)
    "R uby",            # should NOT match (space not in pattern)
]

print("Regex: R(?#this is a comment)uby")
print("Testing strings:")
for test in tests1:
    if regex1.search(test):
        print(f"  '{test}' -> matches")
    else:
        print(f"  '{test}' -> does not match")
print()


# --- Example 2: R(?i)uby ---

print("--- Example 2: R(?i)uby ---")
print("NOTE: Python does NOT support this syntax!")
print("In Perl: (?i) makes only the part AFTER it case-insensitive")
print("In Python: Global flags like (?i) must be at the START of the pattern")
print()
print("Perl regex: R(?i)uby matches 'Ruby', 'RuBy', 'RubY', 'RuBY' (R must be uppercase)")
print("Python equivalent: Use (?i:...) for scoped case-insensitivity (see Example 3)")
print("Or match manually: R[uU][bB][yY]")
print()

# Python doesn't support R(?i)uby - showing the error would occur
print("This example is SKIPPED in Python - the syntax causes an error")
print("Error: 'global flags not at the start of the expression'")
print()


# --- Example 3: R(?i:uby) ---

print("--- Example 3: R(?i:uby) ---")
print("Same as above - case-insensitive only for the grouped part")
print()

regex3 = re.compile(r'R(?i:uby)')
tests3 = [
    "Ruby",                      # should match
    "Ruby",                      # should match
    "RuBy",                      # should match
    "RUBY",                      # should match
    "rUBY",                      # should NOT match (R is still case-sensitive)
    "ruby",                      # should NOT match (R is still case-sensitive)
]

print("Regex: R(?i:uby)")
print("The (?i:...) makes only the grouped part case-insensitive")
print("Testing strings:")
for test in tests3:
    if regex3.search(test):
        print(f"  '{test}' -> matches")
    else:
        print(f"  '{test}' -> does not match")
print()


# --- Example 4: rub(?:y|le) ---

print("--- Example 4: rub(?:y|le) ---")
print("Group only without creating \\1 backreference")
print()

regex4 = re.compile(r'rub(?:y|le)')
tests4 = [
    "ruby",                      # should match
    "ruble",                     # should match
    "rub",                       # should NOT match
    "rubies",                    # should match (contains ruby)
]

print("Regex: rub(?:y|le)")
print("The (?:...) is a non-capturing group - it groups but doesn't create a backreference")
print("Testing strings:")
for test in tests4:
    match = regex4.search(test)
    if match:
        print(f"  '{test}' -> matches (matched: '{match.group(0)}')")
    else:
        print(f"  '{test}' -> does not match")
print()

print("Demonstrating non-capturing vs capturing groups:")
text = "I found a ruby and a ruble"

print("With capturing group: rub(y|le)")
for match in re.finditer(r'rub(y|le)', text):
    print(f"  Matched: '{match.group(0)}', Capture group 1: '{match.group(1)}'")

print("With non-capturing group: rub(?:y|le)")
for match in re.finditer(r'rub(?:y|le)', text):
    try:
        print(f"  Matched: '{match.group(0)}', Capture group 1: '{match.group(1)}'")
    except IndexError:
        print(f"  Matched: '{match.group(0)}', Capture group 1: None")
print()
