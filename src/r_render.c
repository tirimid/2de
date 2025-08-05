// SPDX-License-Identifier: GPL-3.0-or-later

#define R_INCFONT(name) \
	{ \
		.data = name##_ttf, \
		.size = &name##_ttf_len \
	}

typedef struct r_fontdata
{
	u8 const *data;
	u32 const *size;
	TTF_Font *font;
} r_fontdata_t;

SDL_Window *r_wnd;
SDL_Renderer *r_rend;

static r_fontdata_t r_fonts[R_FONT_END] =
{
	R_INCFONT(vcr_osd_mono)
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
		SDL_RWops *rwops = SDL_RWFromConstMem(r_fonts[i].data, *r_fonts[i].size);
		if (!rwops)
		{
			showerr("render: failed to create font RWops - %s!", SDL_GetError());
			return 1;
		}
		
		r_fonts[i].font = TTF_OpenFontRW(rwops, 1, O_FONTSIZE);
		if (!r_fonts[i].font)
		{
			showerr("render: failed to open font - %s!", TTF_GetError());
			return 1;
		}
	}
	
	return 0;
}
