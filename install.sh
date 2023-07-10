#!/bin/sh

#check if wget is present
if command -v "git" >/dev/null 2>&1; then
    printf "git not found\nexit\n"
    exit 1
fi

#check if clang is present
if command -v "clang" >/dev/null 2>&1; then
    printf "clang not found\nexit\n"
    exit 1
fi

#check if mktemp is present
if command -v "mktemp" >/dev/null 2>&1; then
    printf "mktemp not found\nexit\n"
    exit 1
fi

#get temp dir
TMP_DIR=$(mktemp -d -t cotty)

printf "downloading..."
git clone "https://github.com/DaBigBlob/cotty.git" "$TMP_DIR" -q
printf "done\n"


printf "Compiling..."
clang "$TMP_DIR/cotty.c" -O3 -Wall -o "$TMP_DIR/dist/cotty"
printf "done\n"

OS=$(uname -s)
if [ "$OS" = "Linux" ]; then
    printf "Linux detected\nInstalling..."
    mv "$TMP_DIR/dist/cotty" "/usr/local/bin"
    printf "done\n"
else
    printf "placing binery in %s (here)..." "$(pwd)"
    mv "$TMP_DIR/dist/cotty" "$(pwd)"
    print "done\n"
fi

exit 0