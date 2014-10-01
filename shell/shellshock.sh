#!/usr/bin/env bash

# If you see the word “vulnerable” in the output, then your system is vulnerable.
# bash < 4.2

env 'x=() { :;}; echo vulnerable' 'BASH_FUNC_x()=() { :;}; echo vulnerable' bash -c "echo test"

