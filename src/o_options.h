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
#define O_MAXLOGLEN 512
#define O_MAXUIELEMS 128

// stylistic options.
#define O_FONTSIZE 24
#define O_UIFONT R_VCROSDMONO
#define O_UIPAD 5

// color options.
#define O_BGCOLOR 80, 80, 80, 255
#define O_UITEXTCOLOR 255, 255, 255, 255
#define O_UIPANELCOLOR 0, 0, 0, 128
#define O_UIBUTTONCOLOR 255, 255, 255, 128
#define O_UIBUTTONHOVERCOLOR 128, 128, 128, 255
#define O_UIBUTTONPRESSCOLOR 0, 0, 0, 255
