// SPDX-License-Identifier: GPL-3.0-or-later

namespace Render
{

enum Font
{
	VCR_OSD_MONO = 0,
	FONT_END
};

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* fonts[FONT_END];

i32 Init(void);
SDL_Texture* RenderText(Font font, char const* text, u8 r, u8 g, u8 b, u8 a);
void RenderRectZTGL(i32 x, i32 y, i32 w, i32 h, ZTGL::Color color);
void RenderTextZTGL(i32 x, i32 y, i32 w, i32 h, char const* text, ZTGL::Color color);
void ScreenToGame(OUT f32& gameX, OUT f32& gameY, i32 screenX, i32 screenY);
void GameToScreen(OUT i32& screenX, OUT i32& screenY, f32 gameX, f32 gameY);
void RenderRectRel(f32 x, f32 y, f32 w, f32 h);
void RenderHollowRectRel(f32 x, f32 y, f32 w, f32 h);

};
