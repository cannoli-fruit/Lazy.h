#!/bin/sh
for f in examples/*.c; do
  cc -O2 -Wall -Wextra -std=c99 "$f" -o "${f%.c}"
done
