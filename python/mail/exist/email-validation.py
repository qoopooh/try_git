#!/usr/bin/env python

MAIL = 'berm@gmail.com'

import sys
if len(sys.argv) > 1:
    MAIL = sys.argv[1]

from validate_email import validate_email
is_valid = validate_email(MAIL, verify=True)

print is_valid

