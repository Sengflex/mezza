#!/usr/bin/bash

gcc -E -x c -o "$1" "$1.in"
clang-format -i "$1"
