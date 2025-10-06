// SPDX-License-Identifier: GPL-3.0-or-later

namespace Options
{

// window options (static).
constexpr u64				SDL_FLAGS				= SDL_INIT_VIDEO;
constexpr char const*	ERROR_WINDOW_TITLE	= "2de - Error";
constexpr char const*	WINDOW_TITLE			= "2de";
constexpr i32				WINDOW_WIDTH			= 900;
constexpr i32				WINDOW_HEIGHT			= 700;
constexpr u64				WINDOW_FLAGS			= SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
constexpr u64				RENDERER_FLAGS			= 0;

// system options (static).
constexpr i32				TICK_MILLI				= 20;
constexpr i32				TICK_MICRO				= 1000 * TICK_MILLI;
constexpr f32				CAM_ZOOM_SPEED			= 0.03f;
constexpr f32				CAM_SPEED_BASE			= 0.15f;
constexpr f32				CAM_SPEED_FAST			= 0.4f;

// stylistic options (static).
constexpr i32				FONT_SIZE				= 24;
constexpr i32				UI_PAD					= 5;
constexpr i32				UI_TEXT_FIELD_BAR		= 2;
constexpr i32				DRAW_SCALE				= 50;
constexpr i32				SAVE_BAR					= 10;
constexpr SDL_Color		BG_COLOR					{80,	80,	80,	255};
constexpr SDL_Color		SAVED_COLOR				{0,	255,	0,		255};
constexpr SDL_Color		UNSAVED_COLOR			{255,	0,		0,		255};
constexpr SDL_Color		OUTLINE_COLOR			{0,	0,		0,		255};
constexpr SDL_Color		INDICATOR_COLOR		{0,	255,	0,		90};
constexpr f32				CAM_ZOOM_MAX			= 1.2f;
constexpr f32				CAM_ZOOM_MIN			= 0.2f;
constexpr i32				COLOR_SEED				= 0;

// key options (dynamic).
extern SDL_Keycode	menu;
extern SDL_Keycode	up;
extern SDL_Keycode	right;
extern SDL_Keycode	down;
extern SDL_Keycode	left;
extern SDL_Keycode	zoomIn;
extern SDL_Keycode	zoomOut;
extern SDL_Keycode	pencil;
extern SDL_Keycode	bucket;
extern SDL_Keycode	eyeDropper;
extern SDL_Keycode	select;
extern SDL_Keycode	boxSelect;

void							Default();
i32							Read();
i32							Write();
std::filesystem::path	Path();

}
