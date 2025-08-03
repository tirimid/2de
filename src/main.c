// SPDX-License-Identifier: GPL-3.0-or-later

// standard library.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// system dependencies.
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sys/time.h>

// project headers.
#include "o_options.h"
#include "util.h"
#include "resources.h"
#include "r_render.h"

// project source.
#include "r_render.c"
#include "util.c"

int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	// initialize external systems.
	if (!SDL_Init(O_SDLFLAGS))
	{
		showerr("main: failed to init SDL3!");
		return 1;
	}
	atexit(SDL_Quit);
	
	if (!TTF_Init())
	{
		showerr("main: failed to init SDL3 TTF!");
		return 1;
	}
	atexit(TTF_Quit);
	
	// initialize program systems.
	if (r_init())
	{
		return 1;
	}
	
	return 0;
}
