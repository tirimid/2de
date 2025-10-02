// SPDX-License-Identifier: GPL-3.0-or-later

e_editor_t e_editor =
{
	.filetf =
	{
		.buf = e_editor.file,
		.cap = sizeof(e_editor.file)
	}
};

void
e_main(void)
{
	for (;;)
	{
		z_begintick();
		
		// handle events.
		z_prepareinput();
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			z_handleinput(&e);
			if (e.type == SDL_QUIT)
			{
				exit(0);
			}
		}
		
		// update.
		z_uielem_t ubuf[64];
		z_ui_t u = z_beginui(
			ubuf,
			sizeof(ubuf) / sizeof(ubuf[0]),
			O_UIPAD,
			O_UIPAD,
			r_fonts[O_UIFONT],
			r_wnd
		);
		
		z_uilabel(&u, "[2de]");
		
		u.y += 20;
		z_uilabel(&u, "File");
		u.y += 10;
		z_uitextfield(&u, "Map file", &e_editor.filetf, 16);
		if (z_uibutton(&u, "Open file"))
		{
		}
		if (z_uibutton(&u, "Save file"))
		{
		}
		if (z_uibutton(&u, "Export as header"))
		{
		}
		
		u.y += 20;
		z_uilabel(&u, "System");
		u.y += 10;
		if (z_uibutton(&u, "Tilemap"))
		{
			e_editor.sys = E_TILEMAP;
		}
		if (z_uibutton(&u, "Region"))
		{
			e_editor.sys = E_REGION;
		}
		if (z_uibutton(&u, "Attribute"))
		{
			e_editor.sys = E_ATTRIBUTE;
		}
		
		u.y += 20;
		z_uilabel(&u, "Edit");
		u.y += 10;
		if (e_editor.sys == E_TILEMAP)
		{
			i32 sx = u.x;
			
			u.horizontal = true;
			z_uilabel(&u, "F ");
			u.horizontal = false;
			if (z_uibutton(&u, "Pencil"))
			{
			}
			u.x = sx;
			
			u.horizontal = true;
			z_uilabel(&u, "B ");
			u.horizontal = false;
			if (z_uibutton(&u, "Bucket fill"))
			{
			}
			u.x = sx;
			
			u.horizontal = true;
			z_uilabel(&u, "Q ");
			u.horizontal = false;
			if (z_uibutton(&u, "Eyedropper"))
			{
			}
			u.x = sx;
			
			u.horizontal = true;
			z_uilabel(&u, "C ");
			u.horizontal = false;
			if (z_uibutton(&u, "Select"))
			{
			}
			u.x = sx;
			
			u.horizontal = true;
			z_uilabel(&u, "Z ");
			u.horizontal = false;
			if (z_uibutton(&u, "Box select"))
			{
			}
			u.x = sx;
			
			if (z_uibutton(&u, "Copy"))
			{
			}
			u.x = sx;
			
			if (z_uibutton(&u, "Paste"))
			{
			}
			u.x = sx;
		}
		else if (e_editor.sys == E_REGION)
		{
		}
		else if (e_editor.sys == E_ATTRIBUTE)
		{
		}
		
		// render.
		SDL_SetRenderDrawColor(r_rend, O_BGCOLOR);
		SDL_RenderClear(r_rend);
		m_render();
		z_renderui(&u);
		SDL_RenderPresent(r_rend);
		
		z_endtick();
	}
}
