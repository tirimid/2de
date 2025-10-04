#!/bin/bash

INCLUDE="-I. -Isrc -Idep"
DEFINES="-DZTGL_IMPLEMENTATION -DZTGL_SDL2_RENDERER"
WARNINGS="-Wall -Wextra -Wshadow"
LIBRARIES="$(pkg-config --cflags --libs sdl2 SDL2_ttf) -lm"
FLAGS="-std=c++20 -pedantic -D_GNU_SOURCE -fno-rtti -fno-exceptions"

CPP=g++
FULLFLAGS="$INCLUDE $DEFINES $WARNINGS $FLAGS $LIBRARIES"

echo "[$0] build: compilation" >&2
$CPP -o 2de resources.o src/main.cc $FULLFLAGS
if [ $? -ne 0 ]
then
	echo "[$0] build: failed to compile!" >&2
	exit 1
fi

echo "[$0] build: finished successfully" >&2
