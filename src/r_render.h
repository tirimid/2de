// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum r_font
{
	R_VCROSDMONO = 0,
	
	R_FONT_END
} r_font_t;

extern SDL_Window *r_wnd;
extern SDL_Renderer *r_rend;
extern TTF_Font *r_fonts[R_FONT_END];

i32 r_init(void);
