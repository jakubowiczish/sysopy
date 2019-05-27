#!/usr/bin/env bash

echo "cleaning binaries..."
find "$(git rev-parse --show-toplevel)" -name "zad*" -type d -exec make -C {} clean \; > /dev/null

find "$(git rev-parse --show-toplevel)/lab" -mindepth 1 -maxdepth 1 -type d -exec make -C {} clean \; > /dev/null

echo "formatting code..."
find "$(git rev-parse --show-toplevel)" \( -name \*.h -o -name \*.cpp -o -name \*.c \) -type f -exec clang-format -i {} \;