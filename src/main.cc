// SPDX-License-Identifier: GPL-3.0-or-later

// standard library.
#include	<cstdio>
#include	<cstdlib>
#include	<ctime>
#include	<filesystem>
#include	<string>

// external dependencies.
#include	<ztgl.hh>

// project headers.
#include	"util.hh"
#include	"options.hh"
#include	"resources.hh"
#include	"render.hh"
#include	"map.hh"
#include	"editor.hh"

// project source.
#include	"editor.cc"
#include	"map.cc"
#include	"options.cc"
#include	"render.cc"

int
main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	
	ZTGL::conf.m_Log					= stderr;
	ZTGL::conf.m_ErrorTitle			= ERROR_WINDOW_TITLE;
	ZTGL::conf.m_TickMicro			= TICK_MICRO;
	ZTGL::conf.m_UIPad				= UI_PAD;
	ZTGL::conf.m_UITextFieldBar	= UI_TEXT_FIELD_BAR;
	ZTGL::conf.m_RenderRect			= ZTGL::Platform::RenderRect;
	ZTGL::conf.m_RenderText			= ZTGL::Platform::RenderText;
	
	if (SDL_Init(SDL_FLAGS))
	{
		ZTGL::Error("main: failed to init SDL2!");
		return (1);
	}
	atexit(SDL_Quit);
	
	if (TTF_Init())
	{
		ZTGL::Error("main: failed to init SDL TTF!");
		return (1);
	}
	atexit(TTF_Quit);
	
	if (Options::Read())
	{
		Options::Default();
		Options::Write();
	}
	
	if (Render::Init())
	{
		return (1);
	}
	
	ZTGL::platformConf.m_Renderer	= Render::renderer;
	ZTGL::platformConf.m_Font		= Render::font;
	
	Map::Init();
	
	if (Editor::Init())
	{
		return (1);
	}
	
	Editor::Main();
}
