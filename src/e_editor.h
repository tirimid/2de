// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct e_editor
{
	f32 camx, camy;
	f32 camzoom;
} e_editor_t;

extern e_editor_t e_editor;

void e_main(void);
