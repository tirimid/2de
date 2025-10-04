// SPDX-License-Identifier: GPL-3.0-or-later

namespace Editor
{


f32    camX;
f32    camY;
f32    camZoom = CAM_ZOOM_MAX;
System system;

bool   unsaved;
char   file[256];
char   tileNum[8];
ZTGL::TFData fileTF;
ZTGL::TFData tileNumTF;

Map::Map map;

static void UpdateUI(ZTGL::UIPanel& u);
static void UpdateKeyboard();
static void UpdateMouse();
static void DrawIndicators();

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
			Render::font,
			Render::window
		};
		
		UpdateUI(u);
		UpdateKeyboard();
		UpdateMouse();
		
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

static void
UpdateUI(ZTGL::UIPanel& u)
{
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
}

static void
UpdateKeyboard()
{
	f32 camSpeed = ZTGL::ShiftDown() ? CAM_SPEED_FAST : CAM_SPEED_BASE;
	f32 camMoveX = ZTGL::KeyDown(Options::right) - ZTGL::KeyDown(Options::left);
	f32 camMoveY = ZTGL::KeyDown(Options::down) - ZTGL::KeyDown(Options::up);
	camMoveX *= camSpeed;
	camMoveY *= camSpeed;
	
	f32 camMoveZoom = ZTGL::KeyDown(Options::zoomIn) - ZTGL::KeyDown(Options::zoomOut);
	camMoveZoom *= CAM_ZOOM_SPEED;
	
	camX += camMoveX;
	camY += camMoveY;
	camZoom += camMoveZoom;
	camZoom = camZoom < CAM_ZOOM_MIN ? CAM_ZOOM_MIN : camZoom;
	camZoom = camZoom > CAM_ZOOM_MAX ? CAM_ZOOM_MAX : camZoom;
}

static void
UpdateMouse()
{
	// TODO: implement.
}

static void
DrawIndicators()
{
	// TODO: implement.
}

}
