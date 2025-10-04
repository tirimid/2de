// SPDX-License-Identifier: GPL-3.0-or-later

// window options.
#define SDL_FLAGS SDL_INIT_VIDEO
#define ERROR_WINDOW_TITLE "2de - Error"
#define WINDOW_TITLE "2de"
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700
#define WINDOW_FLAGS (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED)
#define RENDERER_FLAGS 0

// system options.
#define TICK_MILLI 20
#define TICK_MICRO (1000 * TICK_MILLI)
#define CAM_ZOOM_SPEED 0.03f
#define CAM_SPEED_BASE 0.15f
#define CAM_SPEED_FAST 0.4f

// stylistic options.
#define FONT_SIZE 24
#define UI_PAD 5
#define UI_TEXT_FIELD_BAR 2
#define DRAW_SCALE 50
#define SAVE_BAR 10
#define BG_COLOR 80, 80, 80, 255
#define SAVED_COLOR 0, 255, 0, 255
#define UNSAVED_COLOR 255, 0, 0, 255
#define OUTLINE_COLOR 0, 0, 0, 255
#define CAM_ZOOM_MAX 1.2f
#define CAM_ZOOM_MIN 0.2f

namespace Options
{

extern SDL_Keycode up;
extern SDL_Keycode right;
extern SDL_Keycode down;
extern SDL_Keycode left;
extern SDL_Keycode zoomIn;
extern SDL_Keycode zoomOut;
extern SDL_Keycode pencil;
extern SDL_Keycode bucket;
extern SDL_Keycode eyeDropper;
extern SDL_Keycode select;
extern SDL_Keycode boxSelect;

void Default();
i32  Read();
i32  Write();
std::filesystem::path Path();

};
