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
SDL_Texture *r_rendertext(r_font_t font, char const *text, u8 r, u8 g, u8 b, u8 a);
void r_zrenderrect(i32 x, i32 y, i32 w, i32 h, z_color_t col);
void r_zrendertext(i32 x, i32 y, i32 w, i32 h, char const *text, z_color_t col);
void r_screen2game(OUT f32 *gx, OUT f32 *gy, i32 sx, i32 sy);
void r_game2screen(OUT i32 *sx, OUT i32 *sy, f32 gx, f32 gy);
void r_renderrelrect(f32 x, f32 y, f32 w, f32 h);
void r_renderrelhollowrect(f32 x, f32 y, f32 w, f32 h);
