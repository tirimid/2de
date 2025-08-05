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
		u_begin(40, 40);
		u_label("2de tilemap editor");
		// TODO: implement UI.
		
		// render.
		SDL_SetRenderDrawColor(r_rend, O_BGCOLOR);
		SDL_RenderClear(r_rend);
		m_render();
		u_render();
		SDL_RenderPresent(r_rend);
		
		endtick();
	}
}
