# 2de

## Introduction

2de is a general-purpose tilemap editor program. Map files are stored and
exported in easy-to-use formats so that you can integrate them into your game
with minimal effort. A variety of commonly desired map features are supported.
The native format, `.2de`, stores everything as serialized binary data, but you
can export maps as C headers (`.h`) as well, for direct integration into C/C++
games.

## Dependencies

* Make (for asset build, optional)
* xxd (for asset build, optional)
* SDL2
* SDL2 TTF
* MSYS2/MinGW64 (for windows build)
* wget (for windows build setup)
* unzip (for windows build setup)

## Management

* If on Windows, run `./windows-setup.sh` to fetch all needed dependencies
* Run `./resources-build.sh` to build all changed resources
* Run `./linux-build.sh` to build the game for Linux after resource build
* Run `./windows-build.sh` to build the game for Windows after resource build

## Usage

After build, run the program in the same way as any other. Either start it from
the command line, or launch it graphically.

## Contributing

Feel free to submit suggestions and bugfixes. Significant user-facing additions
will only be accepted if they are a substantial improvement. If you want to make
your own tilemap editor based on 2de, feel free to clone the repository and do
so.
