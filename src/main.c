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
#include <ztgl.h>

// project headers.
#include "o_options.h"
#include "resources.h"
#include "r_render.h"
#include "m_map.h"
#include "e_editor.h"

// project source.
#include "e_editor.c"
#include "m_map.c"
#include "r_render.c"

int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	// initialize external systems.
	z_conf = (z_conf_t)
	{
		.log = stderr,
		.errtitle = O_ERRWNDTITLE,
		.tickus = O_TICKUS,
		.uipad = O_UIPAD,
		.uitextfieldbar = O_UITEXTFIELDBAR,
		.renderrect = r_zrenderrect,
		.rendertext = r_zrendertext
	};
	
	if (SDL_Init(O_SDLFLAGS))
	{
		z_err("main: failed to init SDL2!");
		return 1;
	}
	atexit(SDL_Quit);
	
	if (IMG_Init(O_IMGFLAGS) != O_IMGFLAGS)
	{
		z_err("main: failed to init SDL2 image!");
		return 1;
	}
	atexit(IMG_Quit);
	
	if (TTF_Init())
	{
		z_err("main: failed to init SDL TTF!");
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
