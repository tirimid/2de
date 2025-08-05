// SPDX-License-Identifier: GPL-3.0-or-later

SDL_Window *r_wnd;
SDL_Renderer *r_rend;
TTF_Font *r_fonts[R_FONT_END];

static resdata_t r_fontres[R_FONT_END] =
{
	INCRES(vcr_osd_mono_ttf)
};

i32
r_init(void)
{
	r_wnd = SDL_CreateWindow(
		O_WNDTITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		O_WNDWIDTH,
		O_WNDHEIGHT,
		O_WNDFLAGS
	);
	if (!r_wnd)
	{
		showerr("render: failed to create window - %s!", SDL_GetError());
		return 1;
	}
	
	r_rend = SDL_CreateRenderer(r_wnd, -1, O_RENDFLAGS);
	if (!r_rend)
	{
		showerr("render: failed to create renderer - %s!", SDL_GetError());
		return 1;
	}
	
	// set up fonts.
	for (usize i = 0; i < R_FONT_END; ++i)
	{
		SDL_RWops *rwops = SDL_RWFromConstMem(r_fontres[i].data, *r_fontres[i].size);
		if (!rwops)
		{
			showerr("render: failed to create font RWops - %s!", SDL_GetError());
			return 1;
		}
		
		r_fonts[i] = TTF_OpenFontRW(rwops, 1, O_FONTSIZE);
		if (!r_fonts[i])
		{
			showerr("render: failed to open font - %s!", TTF_GetError());
			return 1;
		}
	}
	
	// set initial state.
	SDL_SetRenderDrawBlendMode(r_rend, SDL_BLENDMODE_BLEND);
	
	return 0;
}

SDL_Texture *
r_rendertext(r_font_t font, char const *text, u8 r, u8 g, u8 b, u8 a)
{
	SDL_Surface *surf = TTF_RenderUTF8_Solid(
		r_fonts[font],
		text,
		(SDL_Color){r, g, b, a}
	);
	if (!surf)
	{
		return NULL;
	}
	
	SDL_Texture *tex = SDL_CreateTextureFromSurface(r_rend, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		return NULL;
	}
	
	return tex;
}
