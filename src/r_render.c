// SPDX-License-Identifier: GPL-3.0-or-later

SDL_Window *r_wnd;
SDL_Renderer *r_rend;
TTF_Font *r_fonts[R_FONT_END];

static resdata_t r_fontres[R_FONT_END] =
{
	Z_INCRES(vcr_osd_mono_ttf)
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
		z_err("render: failed to create window - %s!", SDL_GetError());
		return 1;
	}
	
	r_rend = SDL_CreateRenderer(r_wnd, -1, O_RENDFLAGS);
	if (!r_rend)
	{
		z_err("render: failed to create renderer - %s!", SDL_GetError());
		return 1;
	}
	
	// set up fonts.
	for (usize i = 0; i < R_FONT_END; ++i)
	{
		SDL_RWops *rwops = SDL_RWFromConstMem(r_fontres[i].data, *r_fontres[i].size);
		if (!rwops)
		{
			z_err("render: failed to create font RWops - %s!", SDL_GetError());
			return 1;
		}
		
		r_fonts[i] = TTF_OpenFontRW(rwops, 1, O_FONTSIZE);
		if (!r_fonts[i])
		{
			z_err("render: failed to open font - %s!", TTF_GetError());
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

void
r_zrenderrect(i32 x, i32 y, i32 w, i32 h, z_color_t col)
{
	SDL_SetRenderDrawColor(
		r_rend,
		z_defaultcolors[col][0],
		z_defaultcolors[col][1],
		z_defaultcolors[col][2],
		z_defaultcolors[col][3]
	);
	
	SDL_Rect r = {x, y, w, h};
	SDL_RenderFillRect(r_rend, &r);
}

void
r_zrendertext(i32 x, i32 y, i32 w, i32 h, char const *text, z_color_t col)
{
	SDL_Texture *tex = r_rendertext(
		O_UIFONT,
		text,
		z_defaultcolors[col][0],
		z_defaultcolors[col][1],
		z_defaultcolors[col][2],
		z_defaultcolors[col][3]
	);
	
	SDL_Rect r = {x, y, w, h};
	SDL_RenderCopy(r_rend, tex, NULL, &r);
	SDL_DestroyTexture(tex);
}

void
r_screen2game(OUT f32 *gx, OUT f32 *gy, i32 sx, i32 sy)
{
	i32 wndw, wndh;
	SDL_GetWindowSize(r_wnd, &wndw, &wndh);
	
	*gx = (sx - wndw / 2) / (e_editor.camzoom * O_DRAWSCALE) + e_editor.camx;
	*gy = (sy - wndh / 2) / (e_editor.camzoom * O_DRAWSCALE) + e_editor.camy;
}

void
r_game2screen(OUT i32 *sx, OUT i32 *sy, f32 gx, f32 gy)
{
	i32 wndw, wndh;
	SDL_GetWindowSize(r_wnd, &wndw, &wndh);
	
	*sx = e_editor.camzoom * O_DRAWSCALE * (gx - e_editor.camx) + wndw / 2;
	*sy = e_editor.camzoom * O_DRAWSCALE * (gy - e_editor.camy) + wndh / 2;
}

void
r_renderrelrect(f32 x, f32 y, f32 w, f32 h)
{
	// 1 is added to pixel width and height in order to remove any seams that
	// appear as a result of dynamic camera movement / zooming.
	SDL_Rect r =
	{
		.w = e_editor.camzoom * O_DRAWSCALE * w + 1,
		.h = e_editor.camzoom * O_DRAWSCALE * h + 1
	};
	r_game2screen(&r.x, &r.y, x, y);
	SDL_RenderFillRect(r_rend, &r);
}

void
r_renderrelhollowrect(f32 x, f32 y, f32 w, f32 h)
{
	SDL_Rect r =
	{
		.w = e_editor.camzoom * O_DRAWSCALE * w + 1,
		.h = e_editor.camzoom * O_DRAWSCALE * h + 1
	};
	r_game2screen(&r.x, &r.y, x, y);
	SDL_RenderDrawRect(r_rend, &r);
}
