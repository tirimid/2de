// SPDX-License-Identifier: GPL-3.0-or-later

namespace Editor
{

f32			camX;
f32			camY;
f32			camZoom	= CAM_ZOOM_MAX;
System		system;
TilemapMode	tilemapMode;

bool				menu;
bool				unsaved;
char				targetFile[256];
char				tileNum[8];
ZTGL::TFData	targetFileTF;
ZTGL::TFData	tileNumTF;

Map::Map	map;

static void	UpdateActiveUI(ZTGL::UIPanel& u);
static void	UpdateInactiveUI(ZTGL::UIPanel& u);
static void	UpdateKeyboard();
static void	UpdateMouse();
static void	DrawIndicators();

i32
Init()
{
	targetFileTF = ZTGL::TFData{targetFile, sizeof(targetFile)};
	tileNumTF = ZTGL::TFData{tileNum, sizeof(tileNum)};
	
	if (Map::Map::New(map))
	{
		return (1);
	}
	
	return (0);
}

void
Main()
{
	for (;;)
	{
		ZTGL::BeginTick();
		
		// handle events.
		ZTGL::PrepareInput();
		SDL_Event	event	{};
		while (SDL_PollEvent(&event))
		{
			ZTGL::HandleInput(event);
			if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		
		ZTGL::UIElem	uBuffer[64]	{};
		ZTGL::UIPanel	u				{
			uBuffer,
			ARRAY_SIZE(uBuffer),
			Render::font,
			Render::window
		};
		
		if (ZTGL::CtrlDown() && ZTGL::KeyPressed(Options::menu))
		{
			menu = !menu;
		}
		
		// update.
		if (menu)
		{
			UpdateActiveUI(u);
		}
		else
		{
			UpdateInactiveUI(u);
			UpdateKeyboard();
			UpdateMouse();
		}
		
		// render.
		SDL_SetRenderDrawColor(Render::renderer, BG_COLOR);
		SDL_RenderClear(Render::renderer);
		
		map.RenderOutlines();
		map.Render();
		u.Render();
		DrawIndicators();
		
		SDL_RenderPresent(Render::renderer);
		
		ZTGL::EndTick();
	}
}

static void
UpdateActiveUI(ZTGL::UIPanel& u)
{
	u.m_X = UI_PAD;
	u.m_Y = UI_PAD;
	
	u.Label("[Tirimid's 2de]");
	
	u.m_Y += 20;
	u.Label("File");
	u.m_Y += 10;
	u.TextField("Map file", targetFileTF, 16);
	if (u.Button("Open file"))
	{
		FILE*	file	= fopen(targetFile, "rb");
		if (!file)
		{
			ZTGL::Error("editor: failed to open file for reading - %s!", file);
		}
		else
		{
			Map::Map::Read(map, file);
			fclose(file);
			unsaved = false;
		}
	}
	if (u.Button("Save file"))
	{
		FILE*	file	= fopen(targetFile, "wb");
		if (!file)
		{
			ZTGL::Error("editor: failed to open file for writing - %s!", file);
		}
		else
		{
			map.RefitBounds();
			map.Write(file);
			fclose(file);
			unsaved = false;
		}
	}
	if (u.Button("Export as header"))
	{
		// TODO: implement.
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
		if (u.Button("Pencil"))
		{
			tilemapMode = PENCIL;
		}
		if (u.Button("Bucket fill"))
		{
			tilemapMode = BUCKET_FILL;
		}
		if (u.Button("Eyedropper"))
		{
			tilemapMode = EYEDROPPER;
		}
		if (u.Button("Select"))
		{
			tilemapMode = SELECT;
		}
		if (u.Button("Box select"))
		{
			tilemapMode = BOX_SELECT;
		}
		if (u.Button("Copy"))
		{
			// TODO: implement.
		}
		if (u.Button("Paste"))
		{
			// TODO: implement.
		}
	}
	else if (system == REGION)
	{
		// TODO: implement.
	}
	else // attribute.
	{
		// TODO: implement.
	}
}

static void
UpdateInactiveUI(ZTGL::UIPanel& u)
{
	u.m_X = UI_PAD;
	u.m_Y = UI_PAD;
	
	u.Label("[Tirimid's 2de]");
	
	u.m_Y += 20;
	u.Label("<placeholder text on how to toggle menu>");
}

static void
UpdateKeyboard()
{
	// handle universal behavior.
	f32	camSpeed	= ZTGL::ShiftDown() ? CAM_SPEED_FAST : CAM_SPEED_BASE;
	f32	camMoveX	= ZTGL::KeyDown(Options::right) - ZTGL::KeyDown(Options::left);
	f32	camMoveY	= ZTGL::KeyDown(Options::down) - ZTGL::KeyDown(Options::up);
	camMoveX *= camSpeed;
	camMoveY *= camSpeed;
	
	f32	camMoveZoom	= ZTGL::KeyDown(Options::zoomIn) - ZTGL::KeyDown(Options::zoomOut);
	camMoveZoom *= CAM_ZOOM_SPEED;
	
	camX += camMoveX;
	camY += camMoveY;
	camZoom += camMoveZoom;
	camZoom = CLAMP(CAM_ZOOM_MIN, camZoom, CAM_ZOOM_MAX);
	
	// handle system mode-dependent behavior.
	if (system == TILEMAP)
	{
		// TODO: implement.
	}
	else if (system == REGION)
	{
		// TODO: implement.
	}
	else // attribute.
	{
		// TODO: implement.
	}
}

static void
UpdateMouse()
{
	// handle system mode-dependent behavior.
	if (system == TILEMAP)
	{
		SDL_Point	m	= ZTGL::MousePos(Render::window);
		
		f32	selX	{};
		f32	selY	{};
		Render::ScreenToGame(selX, selY, m.x, m.y);
		i32	mapSelX	= MAX(0.0f, selX);
		i32	mapSelY	= MAX(0.0f, selY);
		
		i64	tileType	= (i64)strtoll(tileNum, nullptr, 0);
		tileType = tileType < 0			? 0	: tileType;
		tileType = tileType > 65535	? 0	: tileType;
		
		if (tilemapMode == PENCIL && ZTGL::MouseDown(SDL_BUTTON_LEFT))
		{
			unsaved = true;
			
			i32	growDeltaX	= mapSelX - map.m_W + 1;
			i32	growDeltaY	= mapSelY - map.m_H + 1;
			growDeltaX = MAX(growDeltaX, 0);
			growDeltaY = MAX(growDeltaY, 0);
			
			if (growDeltaX > 0 || growDeltaY > 0)
			{
				map.Grow(growDeltaX, growDeltaY);
			}
			map.m_TileTypes[map.m_W * mapSelY + mapSelX] = tileType;
		}
		else if (tilemapMode == BUCKET_FILL)
		{
			// TODO: implement.
		}
		else if (tilemapMode == EYEDROPPER)
		{
			// TODO: implement.
		}
		else if (tilemapMode == SELECT)
		{
			// TODO: implement.
		}
		else // box select.
		{
			// TODO: implement.
		}
	}
	else if (system == REGION)
	{
		// TODO: implement.
	}
	else // attribute.
	{
		// TODO: implement.
	}
}

static void
DrawIndicators()
{
	// system mode-dependent indicators.
	if (system == TILEMAP)
	{
		// TODO: handle box select indication.
		
		SDL_Point	m	= ZTGL::MousePos(Render::window);
		
		f32	selX	{};
		f32	selY	{};
		Render::ScreenToGame(selX, selY, m.x, m.y);
		selX = (i32)MAX(0.0f, selX);
		selY = (i32)MAX(0.0f, selY);
		
		SDL_SetRenderDrawColor(Render::renderer, INDICATOR_COLOR);
		Render::RenderRectRel(selX, selY, 1.0f, 1.0f);
		
		char const*	toolNames[]	=
		{
			"P.", // pencil.
			"BF.", // bucket fill.
			"ED.", // eye dropper.
			"S.", // select.
			"BS." // box select.
		};
	}
	else if (system == REGION)
	{
		// TODO: implement.
	}
	else // attribute.
	{
		// TODO: implement.
	}
	
	// save status indicator.
	if (unsaved)
	{
		SDL_SetRenderDrawColor(Render::renderer, UNSAVED_COLOR);
	}
	else
	{
		SDL_SetRenderDrawColor(Render::renderer, SAVED_COLOR);
	}
	
	i32	windowWidth		{};
	i32	windowHeight	{};
	SDL_GetWindowSize(Render::window, &windowWidth, &windowHeight);
	
	SDL_Rect	r	{0, windowHeight - SAVE_BAR, windowWidth, SAVE_BAR};
	SDL_RenderFillRect(Render::renderer, &r);
}

}
