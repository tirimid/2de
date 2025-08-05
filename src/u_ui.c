// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum u_elemtype
{
	U_LABEL = 0,
	U_BUTTON
} u_elemtype_t;

typedef union u_elem
{
	// all UI elements have at least these properties.
	struct
	{
		u8 type;
		i32 x, y;
		i32 w, h;
	} any;
	
	struct
	{
		u8 type;
		i32 x, y;
		i32 w, h;
		char const *text;
	} label;
	
	struct
	{
		u8 type;
		i32 x, y;
		i32 w, h;
		char const *text;
	} button;
} u_elem_t;

static i32 u_x, u_y;
static u_elem_t u_elems[O_MAXUIELEMS];
static usize u_nelems;

void
u_begin(i32 x, i32 y)
{
	u_x = x;
	u_y = y;
	u_nelems = 0;
}

void
u_render(void)
{
	if (!u_nelems)
	{
		return;
	}
	
	// find render boundaries to draw panel.
	i32 minx = INT32_MAX, miny = INT32_MAX;
	i32 maxx = INT32_MIN, maxy = INT32_MIN;
	for (usize i = 0; i < u_nelems; ++i)
	{
		i32 x = u_elems[i].any.x, y = u_elems[i].any.y;
		i32 w = u_elems[i].any.w, h = u_elems[i].any.h;
		
		minx = x < minx ? x : minx;
		miny = y < miny ? y : miny;
		maxx = x + w > maxx ? x + w : maxx;
		maxy = y + h > maxy ? y + h : maxy;
	}
	
	// draw panel.
	SDL_SetRenderDrawColor(r_rend, O_UIPANELCOLOR);
	SDL_Rect r =
	{
		minx - O_UIPAD,
		miny - O_UIPAD,
		maxx - minx + 2 * O_UIPAD,
		maxy - miny + 2 * O_UIPAD
	};
	SDL_RenderFillRect(r_rend, &r);
	
	// draw UI elements.
	i32 mx, my;
	i_mpos(&mx, &my);
	
	for (usize i = 0; i < u_nelems; ++i)
	{
		i32 x = u_elems[i].any.x, y = u_elems[i].any.y;
		i32 w = u_elems[i].any.w, h = u_elems[i].any.h;
		
		switch (u_elems[i].any.type)
		{
		case U_LABEL:
		{
			SDL_Texture *tex = r_rendertext(O_UIFONT, u_elems[i].label.text, O_UITEXTCOLOR);
			
			r = (SDL_Rect){x, y, w, h};
			SDL_RenderCopy(r_rend, tex, NULL, &r);
			SDL_DestroyTexture(tex);
			
			break;
		}
		case U_BUTTON:
		{
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (i_mdown(SDL_BUTTON_LEFT))
				{
					SDL_SetRenderDrawColor(r_rend, O_UIBUTTONPRESSCOLOR);
				}
				else
				{
					SDL_SetRenderDrawColor(r_rend, O_UIBUTTONHOVERCOLOR);
				}
			}
			else
			{
				SDL_SetRenderDrawColor(r_rend, O_UIBUTTONCOLOR);
			}
			
			r = (SDL_Rect){x, y, w, h};
			SDL_RenderFillRect(r_rend, &r);
			
			SDL_Texture *tex = r_rendertext(O_UIFONT, u_elems[i].label.text, O_UITEXTCOLOR);
			
			r = (SDL_Rect){x + O_UIPAD, y + O_UIPAD, w - 2 * O_UIPAD, h - 2 * O_UIPAD};
			SDL_RenderCopy(r_rend, tex, NULL, &r);
			SDL_DestroyTexture(tex);
			
			break;
		}
		}
	}
}

void
u_pad(i32 dx, i32 dy)
{
	u_x += dx;
	u_y += dy;
}

void
u_label(char const *text)
{
	if (u_nelems >= O_MAXUIELEMS)
	{
		return;
	}
	
	i32 w, h;
	TTF_SizeText(r_fonts[O_UIFONT], text, &w, &h);
	
	u_elems[u_nelems++] = (u_elem_t)
	{
		.label =
		{
			.type = U_LABEL,
			.x = u_x,
			.y = u_y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	u_y += h;
}

bool
u_button(char const *text)
{
	if (u_nelems >= O_MAXUIELEMS)
	{
		return false;
	}
	
	bool state = false;
	
	i32 w, h;
	TTF_SizeText(r_fonts[O_UIFONT], text, &w, &h);
	w += 2 * O_UIPAD;
	h += 2 * O_UIPAD;
	
	i32 mx, my;
	i_mpos(&mx, &my);
	
	if (i_mreleased(SDL_BUTTON_LEFT)
		&& mx >= u_x
		&& my >= u_y
		&& mx < u_x + w
		&& my < u_y + h)
	{
		state = true;
	}
	
	u_elems[u_nelems++] = (u_elem_t)
	{
		.button =
		{
			.type = U_BUTTON,
			.x = u_x,
			.y = u_y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	u_y += h;
	
	return state;
}
