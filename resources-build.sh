#!/bin/bash

INCLUDE="-I."
DEFINES=""
WARNING="-Wall -Wextra -Wshadow"
LIBRARIES=""
FLAGS="-std=c++20 -pedantic -fno-rtti -fno-exceptions"

CPP=g++
FULLFLAGS="$INCLUDE $DEFINES $WARNINGS $FLAGS $LIBRARIES"

echo "[$0] resources: make fonts" >&2
make -B -s -C font > /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] resources: failed to make fonts!" >&2
	exit 1
fi

echo "[$0] resources: compilation" >&2
$CPP -o resources.o -c src/resources.cc $FULLFLAGS
if [ $? -ne 0 ]
then
	echo "[$0] resources: failed to compile!" >&2
	exit 1
fi

echo "[$0] resources: finished successfully" >&2
