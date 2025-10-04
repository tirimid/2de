// SPDX-License-Identifier: GPL-3.0-or-later

namespace Render
{

SDL_Window*		window;
SDL_Renderer*	renderer;
TTF_Font*		font;

static ZTGL::Res	fontResource	= ZTGL_INC_RES(vcr_osd_mono_ttf);

i32
Init(void)
{
	window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FLAGS
	);
	if (!window)
	{
		ZTGL::Error("render: failed to create window - %s!", SDL_GetError());
		return (1);
	}
	
	renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
	if (!renderer)
	{
		ZTGL::Error("render: failed to create renderer - %s!", SDL_GetError());
		return (1);
	}
	
	// set up font.
	SDL_RWops*	rwOps	= SDL_RWFromConstMem(
		fontResource.m_Data,
		fontResource.m_Size
	);
	if (!rwOps)
	{
		ZTGL::Error("render: failed to create font RWops - %s!", SDL_GetError());
		return (1);
	}
	
	font = TTF_OpenFontRW(rwOps, 1, FONT_SIZE);
	if (!font)
	{
		ZTGL::Error("render: failed to open font - %s!", TTF_GetError());
		return (1);
	}
	
	// set initial state.
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	return (0);
}

void
ScreenToGame(OUT f32& gameX, OUT f32& gameY, i32 screenX, i32 screenY)
{
	i32	windowWidth		{};
	i32	windowHeight	{};
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	
	gameX = (screenX - windowWidth / 2) / (Editor::camZoom * DRAW_SCALE) + Editor::camX;
	gameY = (screenY - windowHeight / 2) / (Editor::camZoom * DRAW_SCALE) + Editor::camY;
}

void
GameToScreen(OUT i32& screenX, OUT i32& screenY, f32 gameX, f32 gameY)
{
	i32	windowWidth		{};
	i32	windowHeight	{};
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	
	screenX = Editor::camZoom * DRAW_SCALE * (gameX - Editor::camX) + windowWidth / 2;
	screenY = Editor::camZoom * DRAW_SCALE * (gameY - Editor::camY) + windowHeight / 2;
}

void
RenderRectRel(f32 x, f32 y, f32 w, f32 h)
{
	// 1 is added to pixel width and height in order to remove any seams that
	// appear as a result of dynamic camera movement / zooming.
	SDL_Rect	r	{
		0,
		0,
		(i32)(Editor::camZoom * DRAW_SCALE * w + 1),
		(i32)(Editor::camZoom * DRAW_SCALE * h + 1)
	};
	GameToScreen(r.x, r.y, x, y);
	SDL_RenderFillRect(renderer, &r);
}

void
RenderHollowRectRel(f32 x, f32 y, f32 w, f32 h)
{
	SDL_Rect	r	{
		0,
		0,
		(i32)(Editor::camZoom * DRAW_SCALE * w + 1),
		(i32)(Editor::camZoom * DRAW_SCALE * h + 1)
	};
	GameToScreen(r.x, r.y, x, y);
	SDL_RenderDrawRect(renderer, &r);
}

}
