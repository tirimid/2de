// SPDX-License-Identifier: GPL-3.0-or-later

// window options.
#define O_SDLFLAGS SDL_INIT_VIDEO
#define O_IMGFLAGS IMG_INIT_PNG
#define O_ERRWNDTITLE "2de - Error"
#define O_WNDTITLE "2de"
#define O_WNDWIDTH 800
#define O_WNDHEIGHT 600
#define O_WNDFLAGS (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED)
#define O_RENDFLAGS 0

// system options.
#define O_TICKMS 20
#define O_TICKUS (1000 * O_TICKMS)

// stylistic options.
#define O_FONTSIZE 24
#define O_UIFONT R_VCROSDMONO
#define O_UIPAD 5
#define O_UITEXTFIELDBAR 2
#define O_BGCOLOR 80, 80, 80, 255
