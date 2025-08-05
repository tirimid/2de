// SPDX-License-Identifier: GPL-3.0-or-later

e_editor_t e_editor;

void
e_main(void)
{
	for (;;)
	{
		begintick();
		
		// handle events.
		i_prepare();
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			i_handle(&e);
			if (e.type == SDL_QUIT)
			{
				exit(0);
			}
		}
		
		// update.
		u_begin(20, 20);
		u_label("[2de]");
		
		u_pad(0, 20);
		u_label("File");
		u_pad(0, 10);
		if (u_button("Open file"))
		{
		}
		if (u_button("Save file"))
		{
		}
		if (u_button("Export as header"))
		{
		}
		
		u_pad(0, 20);
		u_label("System");
		u_pad(0, 10);
		if (u_button("Tilemap"))
		{
			e_editor.sys = E_TILEMAP;
		}
		if (u_button("Region"))
		{
			e_editor.sys = E_REGION;
		}
		if (u_button("Attribute"))
		{
			e_editor.sys = E_ATTRIBUTE;
		}
		
		u_pad(0, 20);
		u_label("Edit");
		u_pad(0, 10);
		if (e_editor.sys == E_TILEMAP)
		{
			if (u_button("Pencil"))
			{
			}
			if (u_button("Bucket fill"))
			{
			}
			if (u_button("Eyedropper"))
			{
			}
			if (u_button("Select"))
			{
			}
			if (u_button("Box select"))
			{
			}
			if (u_button("Copy"))
			{
			}
			if (u_button("Paste"))
			{
			}
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
		u_render();
		SDL_RenderPresent(r_rend);
		
		endtick();
	}
}
