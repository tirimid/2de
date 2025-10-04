// SPDX-License-Identifier: GPL-3.0-or-later

namespace Render
{

extern SDL_Window*	window;
extern SDL_Renderer*	renderer;
extern TTF_Font*		font;

i32	Init(void);
void	ScreenToGame(OUT f32& gameX, OUT f32& gameY, i32 screenX, i32 screenY);
void	GameToScreen(OUT i32& screenX, OUT i32& screenY, f32 gameX, f32 gameY);
void	RenderRectRel(f32 x, f32 y, f32 w, f32 h);
void	RenderHollowRectRel(f32 x, f32 y, f32 w, f32 h);

};
