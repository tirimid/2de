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

// stylistic options.
#define FONT_SIZE 24
#define UI_FONT Render::VCR_OSD_MONO
#define UI_PAD 5
#define UI_TEXT_FIELD_BAR 2
#define DRAW_SCALE 50
#define SAVE_BAR 10
#define BG_COLOR 80, 80, 80, 255
#define SAVED_COLOR 0, 255, 0, 255
#define UNSAVED_COLOR 255, 0, 0, 255

namespace Options
{

SDL_Keycode up;
SDL_Keycode right;
SDL_Keycode down;
SDL_Keycode left;
SDL_Keycode pencil;
SDL_Keycode bucket;
SDL_Keycode eyeDropper;
SDL_Keycode select;
SDL_Keycode boxSelect;
SDL_Keycode copy;
SDL_Keycode paste;

void Default();
i32 Read();
i32 Write();

};
