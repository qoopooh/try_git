#!/usr/bin/env python3

import re

print("Backreference Examples")
print()

# --- Example 1: ([Pp])ython&\1ails ---

print("--- Example 1: ([Pp])ython&\\1ails ---")
print("This regex matches 'python&pails' or 'Python&Pails'.")
print("The \\1 backreference matches the same character captured by the first group ([Pp]).")
print()

regex1 = re.compile(r'([Pp])ython&\1ails')

tests1 = [
    "python&pails",      # should match
    "Python&Pails",      # should match
    "python&Pails",      # should NOT match
    "Python&pails"       # should NOT match
]

print("Testing strings against the regex:")
for test in tests1:
    if regex1.search(test):
        print(f"  '{test}' -> matches")
    else:
        print(f"  '{test}' -> does not match")
print()


# --- Example 2: (['"])[^\1]*\1 ---

print("--- Example 2: Matching Quoted Strings ---")
print("This regex is intended to match a single or double-quoted string.")
print("The \\1 backreference ensures the closing quote is the same as the opening quote.")
print()

print("NOTE: The pattern (['\"])[^\\1]*\\1, while common in some documentation, does not work as expected in Python.")
print("Inside a character class [], \\1 is not a backreference.")
print("A better and more common way in Python is to use a non-greedy match: (['\"])(.*?)\\1")
print()

text2 = 'He said "hello" and she said \'world\'. Then he replied, "it\'s a \'nice\' day".'
print(f"Original text: '{text2}'")
print()

# Using non-greedy match for quoted strings
regex2 = re.compile(r'(["\'])(.*?)\1')

print("Finding all quoted strings in the text:")
for match in regex2.finditer(text2):
    print(f"  Matched full string: {match.group(0)}")
    print(f"    -> Quote type: {match.group(1)}")
    print(f"    -> Content:    {match.group(2)}")
