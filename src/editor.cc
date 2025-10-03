// SPDX-License-Identifier: GPL-3.0-or-later

namespace Editor
{

f32    camX;
f32    camY;
f32    camZoom;
System system;

bool   unsaved;
char   file[256];
char   tileNum[8];
ZTGL::TFData fileTF;
ZTGL::TFData tileNumTF;

Map::Map map;

i32
Init()
{
	fileTF = ZTGL::TFData{file, sizeof(file)};
	tileNumTF = ZTGL::TFData{tileNum, sizeof(tileNum)};
	
	if (Map::Map::New(map))
	{
		return 1;
	}
	
	return 0;
}

void
Main()
{
	for (;;)
	{
		ZTGL::BeginTick();
		
		// handle events.
		ZTGL::PrepareInput();
		SDL_Event event{};
		while (SDL_PollEvent(&event))
		{
			ZTGL::HandleInput(event);
			if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		
		// update.
		ZTGL::UIElem uBuffer[64]{};
		ZTGL::UIPanel u{
			uBuffer,
			sizeof(uBuffer) / sizeof(uBuffer[0]),
			Render::fonts[UI_FONT],
			Render::window
		};
		
		u.m_X = UI_PAD;
		u.m_Y = UI_PAD;
		
		u.Label("[Tirimid's 2de]");
		
		u.m_Y += 20;
		u.Label("File");
		u.m_Y += 10;
		u.TextField("Map file", fileTF, 16);
		if (u.Button("Open file"))
		{
		}
		if (u.Button("Save file"))
		{
		}
		if (u.Button("Export as header"))
		{
		}
		
		u.m_Y += 20;
		u.Label("System");
		u.m_Y += 10;
		if (u.Button("Tilemap"))
		{
			system = TILEMAP;
		}
		if (u.Button("Region"))
		{
			system = REGION;
		}
		if (u.Button("Attribute"))
		{
			system = ATTRIBUTE;
		}
		
		u.m_Y += 20;
		u.Label("Edit");
		u.m_Y += 10;
		if (system == TILEMAP)
		{
			u.TextField("Tile number", tileNumTF, 16);
			
			i32 initialX = u.m_X;
			
			if (u.Button("Pencil"))
			{
			}
			
			if (u.Button("Bucket fill"))
			{
			}
			
			if (u.Button("Eyedropper"))
			{
			}
			
			if (u.Button("Select"))
			{
			}
			
			if (u.Button("Box select"))
			{
			}
			
			if (u.Button("Copy"))
			{
			}
			
			if (u.Button("Paste"))
			{
			}
		}
		else if (system == REGION)
		{
		}
		else // attribute.
		{
		}
		
		// render.
		SDL_SetRenderDrawColor(Render::renderer, BG_COLOR);
		SDL_RenderClear(Render::renderer);
		
		map.RenderOutlines();
		map.Render();
		
		u.Render();
		
		if (unsaved)
		{
			SDL_SetRenderDrawColor(Render::renderer, UNSAVED_COLOR);
		}
		else
		{
			SDL_SetRenderDrawColor(Render::renderer, SAVED_COLOR);
		}
		
		i32 windowWidth;
		i32 windowHeight;
		SDL_GetWindowSize(Render::window, &windowWidth, &windowHeight);
		
		SDL_Rect r{0, windowHeight - SAVE_BAR, windowWidth, SAVE_BAR};
		SDL_RenderFillRect(Render::renderer, &r);
		
		SDL_RenderPresent(Render::renderer);
		
		ZTGL::EndTick();
	}
}

}
