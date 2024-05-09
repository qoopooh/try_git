#!/usr/bin/env python3
import sys
import os
import re

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <markdown_file>", file=sys.stderr)
    sys.exit(1)

file = sys.argv[1]
if not os.path.exists(file):
    print(f"File '{file}' not found", file=sys.stderr)
    sys.exit(1)

# Read file
with open(file, 'r') as fh:
    content = fh.read()

# Remove bold formatting
content = re.sub(r'(\*\*|__)(.+?)\1', r'\2', content, flags=re.DOTALL)

# Write back to same file
with open(file, 'w') as fh:
    fh.write(content)

print(f"Removed bold formatting from {file}")
