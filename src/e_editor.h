// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum e_system
{
	E_TILEMAP = 0,
	E_REGION,
	E_ATTRIBUTE
} e_system_t;

typedef struct e_editor
{
	f32 camx, camy;
	f32 camzoom;
	e_system_t sys;
} e_editor_t;

extern e_editor_t e_editor;

void e_main(void);
