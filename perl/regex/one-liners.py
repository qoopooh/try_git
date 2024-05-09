#!/usr/bin/env python3
import subprocess
from typing import Optional


PATTERN = r"(?<=\d{1,3})\w+"


def lookbehind_example(input_string) -> Optional[str]:
    """
    Demonstrates a Perl one-liner that uses a lookbehind assertion.
    The Perl code matches word characters following 1 to 3 digits.
    """
    perl_code = r'print $& if "' + input_string + r'" =~ /' + PATTERN + r"/;"
    cmd = ["perl", "-E", perl_code]

    result = subprocess.run(
        cmd, capture_output=True, text=True  # capture stdout & stderr  # decode to str
    )

    return result.returncode == 0 and result.stdout.strip()


if __name__ == "__main__":
    print("Lookbehind Example:", PATTERN)
    data = [
        "12345abc",
        "12abc",
        "1abc",
        "abc",
        "1234",
    ]
    for line in data:
        out = lookbehind_example(line)
        if out:
            print(f"{line}: {out}")
        else:
            print(f"{line}: No match")
