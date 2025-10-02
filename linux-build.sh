#!/bin/bash

INCLUDE="-I. -Isrc -Idep"
DEFINES="-DZ_IMPLEMENTATION"
WARNINGS="-Wall -Wextra -Wshadow"
LIBRARIES="$(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf) -lm"
CFLAGS="-std=c99 -pedantic -Og -g3 -fsanitize=address -D_GNU_SOURCE"

CC=gcc
CFLAGS_FULL="$INCLUDE $DEFINES $WARNINGS $CFLAGS $LIBRARIES"

echo "[$0] build: compilation" >&2
$CC -o 2de resources.o src/main.c $CFLAGS_FULL
if [ $? -ne 0 ]
then
	echo "[$0] build: failed to compile!" >&2
	exit 1
fi

echo "[$0] build: finished successfully" >&2
