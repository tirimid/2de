// SPDX-License-Identifier: GPL-3.0-or-later

namespace Render
{

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* fonts[FONT_END];

static ZTGL::Res fontResources[FONT_END] =
{
	ZTGL_INC_RES(vcr_osd_mono_ttf)
};

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
		return 1;
	}
	
	renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
	if (!renderer)
	{
		ZTGL::Error("render: failed to create renderer - %s!", SDL_GetError());
		return 1;
	}
	
	// set up fonts.
	for (usize i = 0; i < FONT_END; ++i)
	{
		SDL_RWops *rwOps = SDL_RWFromConstMem(
			fontResources[i].m_Data,
			fontResources[i].m_Size
		);
		if (!rwOps)
		{
			ZTGL::Error("render: failed to create font RWops - %s!", SDL_GetError());
			return 1;
		}
		
		fonts[i] = TTF_OpenFontRW(rwOps, 1, FONT_SIZE);
		if (!fonts[i])
		{
			ZTGL::Error("render: failed to open font - %s!", TTF_GetError());
			return 1;
		}
	}
	
	// set initial state.
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	return 0;
}

SDL_Texture*
RenderText(Font font, char const* text, u8 r, u8 g, u8 b, u8 a)
{
	SDL_Surface* surface = TTF_RenderUTF8_Solid(
		fonts[font],
		text,
		SDL_Color{r, g, b, a}
	);
	if (!surface)
	{
		return nullptr;
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture)
	{
		return nullptr;
	}
	
	return texture;
}

void
RenderRectZTGL(i32 x, i32 y, i32 w, i32 h, ZTGL::Color color)
{
	SDL_SetRenderDrawColor(
		renderer,
		ZTGL::defaultColors[color][0],
		ZTGL::defaultColors[color][1],
		ZTGL::defaultColors[color][2],
		ZTGL::defaultColors[color][3]
	);
	
	SDL_Rect r{x, y, w, h};
	SDL_RenderFillRect(renderer, &r);
}

void
RenderTextZTGL(i32 x, i32 y, i32 w, i32 h, char const* text, ZTGL::Color color)
{
	SDL_Texture* texture = RenderText(
		UI_FONT,
		text,
		ZTGL::defaultColors[color][0],
		ZTGL::defaultColors[color][1],
		ZTGL::defaultColors[color][2],
		ZTGL::defaultColors[color][3]
	);
	
	SDL_Rect r{x, y, w, h};
	SDL_RenderCopy(renderer, texture, nullptr, &r);
	SDL_DestroyTexture(texture);
}

void
ScreenToGame(OUT f32& gameX, OUT f32& gameY, i32 screenX, i32 screenY)
{
	i32 windowWidth;
	i32 windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	
	gameX = (screenX - windowWidth / 2) / (Editor::camZoom * DRAW_SCALE) + Editor::camX;
	gameY = (screenY - windowHeight / 2) / (Editor::camZoom * DRAW_SCALE) + Editor::camY;
}

void
GameToScreen(OUT i32& screenX, OUT i32& screenY, f32 gameX, f32 gameY)
{
	i32 windowWidth;
	i32 windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	
	screenX = Editor::camZoom * DRAW_SCALE * (gameX - Editor::camX) + windowWidth / 2;
	screenY = Editor::camZoom * DRAW_SCALE * (gameY - Editor::camY) + windowHeight / 2;
}

void
RenderRectRel(f32 x, f32 y, f32 w, f32 h)
{
	// 1 is added to pixel width and height in order to remove any seams that
	// appear as a result of dynamic camera movement / zooming.
	SDL_Rect r{
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
	SDL_Rect r{
		0,
		0,
		(i32)(Editor::camZoom * DRAW_SCALE * w + 1),
		(i32)(Editor::camZoom * DRAW_SCALE * h + 1)
	};
	GameToScreen(r.x, r.y, x, y);
	SDL_RenderDrawRect(renderer, &r);
}

}
