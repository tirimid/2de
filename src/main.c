// SPDX-License-Identifier: GPL-3.0-or-later

// standard library.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// system dependencies.
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sys/time.h>

// project headers.
#include "o_options.h"
#include "util.h"
#include "resources.h"
#include "r_render.h"
#include "i_input.h"
#include "m_map.h"
#include "u_ui.h"
#include "e_editor.h"

// project source.
#include "e_editor.c"
#include "i_input.c"
#include "m_map.c"
#include "r_render.c"
#include "u_ui.c"
#include "util.c"

int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	// initialize external systems.
	if (SDL_Init(O_SDLFLAGS))
	{
		showerr("main: failed to init SDL2!");
		return 1;
	}
	atexit(SDL_Quit);
	
	if (IMG_Init(O_IMGFLAGS) != O_IMGFLAGS)
	{
		showerr("main: failed to init SDL2 image!");
		return 1;
	}
	atexit(IMG_Quit);
	
	if (TTF_Init())
	{
		showerr("main: failed to init SDL TTF!");
		return 1;
	}
	atexit(TTF_Quit);
	
	// initialize program systems.
	if (r_init())
	{
		return 1;
	}
	
	e_main();
	
	return 0;
}
