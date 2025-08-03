// SPDX-License-Identifier: GPL-3.0-or-later

SDL_Window *r_wnd;
SDL_Renderer *r_rend;

i32
r_init(void)
{
	r_wnd = SDL_CreateWindow(O_WNDTITLE, O_WNDWIDTH, O_WNDHEIGHT, O_WNDFLAGS);
	if (!r_wnd)
	{
		showerr("render: failed to create window - %s!", SDL_GetError());
		return 1;
	}
	
	r_rend = SDL_CreateRenderer(r_wnd, NULL);
	if (!r_rend)
	{
		showerr("render: failed to create renderer - %s!", SDL_GetError());
		return 1;
	}
	
	return 0;
}
