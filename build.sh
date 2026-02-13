#!/usr/bin/env sh

SRC="src/grid.cpp src/strats.cpp src/main.cpp"
OUT="random_paths.out"
GFLAGS="-Wall -Wextra -Wpedantic -Werror -std=c++23 -I./src/"
DFLAGS="-ggdb -fsanitize=address -fsanitize=undefined"
RFLAGS="-O2"

LDFLAGS="-lraylib"

CFLAGS="$GFLAGS $DFLAGS"
if [ "$1" = "release" ]
then
    CFLAGS="$GFLAGS $RFLAGS";
    shift 1;
elif [ "$1" = "debug" ]
then
    shift 1;
fi

set -xe

c++ $CFLAGS -o $OUT $SRC $LDFLAGS;

if [ "$1" = "run" ]
then
    ./$OUT $@
fi
