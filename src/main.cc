// SPDX-License-Identifier: GPL-3.0-or-later

// standard library.
#include <cstdio>
#include <cstdlib>
#include <ctime>

// system dependencies.
#include <ztgl.hh>

// project headers.
#include "options.hh"
#include "resources.hh"
#include "render.hh"
#include "map.hh"
#include "editor.hh"

// project source.
#include "editor.cc"
#include "map.cc"
#include "render.cc"

int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	// initialize external systems.
	ZTGL::conf.m_Log = stderr;
	ZTGL::conf.m_ErrorTitle = ERROR_WINDOW_TITLE;
	ZTGL::conf.m_TickMicro = TICK_MICRO;
	ZTGL::conf.m_UIPad = UI_PAD;
	ZTGL::conf.m_UITextFieldBar = UI_TEXT_FIELD_BAR;
	ZTGL::conf.m_RenderRect = Render::RenderRectZTGL;
	ZTGL::conf.m_RenderText = Render::RenderTextZTGL;
	
	if (SDL_Init(SDL_FLAGS))
	{
		ZTGL::Error("main: failed to init SDL2!");
		return 1;
	}
	atexit(SDL_Quit);
	
	if (TTF_Init())
	{
		ZTGL::Error("main: failed to init SDL TTF!");
		return 1;
	}
	atexit(TTF_Quit);
	
	// initialize program systems.
	if (Render::Init())
	{
		return 1;
	}
	
	if (Editor::Init())
	{
		return 1;
	}
	
	Editor::Main();
	
	return 0;
}
