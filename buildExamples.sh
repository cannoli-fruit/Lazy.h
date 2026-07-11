#!/bin/sh
for f in examples/*.c; do
  cc -O2 -std=c99 "$f" -o "${f%.c}"
done
