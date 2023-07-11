#!/bin/sh

#check if wget is present
if command -v git >/dev/null 2>&1; then
    printf "git present...ok\n"
else
    printf "git not found\nexit\n"
    exit 1
fi

#check if clang is present
if command -v clang >/dev/null 2>&1; then
    printf "clang present...ok\n"
else
    printf "clang not found\nexit\n"
    exit 1
fi

#check if mktemp is present
if command -v mktemp >/dev/null 2>&1; then
    printf "mktmp present...ok\n"
else
    printf "mktemp not found\nexit\n"
    exit 1
fi

#check if cp is present
if command -v cp >/dev/null 2>&1; then
    printf "cp present...ok\n"
else
    printf "cp not found\nexit\n"
    exit 1
fi

#check if rm is present
if command -v rm >/dev/null 2>&1; then
    printf "rm present...ok\n"
else
    printf "rm not found\nexit\n"
    exit 1
fi

#check if uname is present
if command -v uname >/dev/null 2>&1; then
    printf "uname present...ok\n"
else
    printf "uname not found\nexit\n"
    exit 1
fi

#check if mkdir is present
if command -v mkdir >/dev/null 2>&1; then
    printf "mkdir present...ok\n"
else
    printf "mkdir not found\nexit\n"
    exit 1
fi

#get temp dir
TMP_DIR=$(mktemp -d)

printf "downloading..."
git clone "https://github.com/DaBigBlob/cotty.git" "$TMP_DIR" -q
printf "done\n"


printf "Compiling..."
mkdir -p "$TMP_DIR/dist"
clang "$TMP_DIR/cotty.c" -O3 -Wall -o "$TMP_DIR/dist/cotty"
printf "done\n"

OS=$(uname -s)
if [ "$OS" = "Linux" ]; then
    printf "Linux detected\nInstalling..."
    cp "$TMP_DIR/dist/cotty" "/usr/local/bin"
    printf "done\n"
else
    printf "placing binery in %s (here)..." "$(pwd)"
    cp "$TMP_DIR/dist/cotty" "$(pwd)"
    printf "done\n"
fi

printf "cleaning up..."
rm -rf "$TMP_DIR"
printf "done\n"

printf "\nrun ./cotty to access it\n"
printf "or place it in a directory in your path\n"

exit 0