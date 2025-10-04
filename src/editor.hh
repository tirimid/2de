// SPDX-License-Identifier: GPL-3.0-or-later

namespace Editor
{

enum System
{
	TILEMAP = 0,
	REGION,
	ATTRIBUTE
};

extern f32		camX;
extern f32		camY;
extern f32		camZoom;
extern System	system;

extern bool				unsaved;
extern char				file[256];
extern char				tileNum[8];
extern ZTGL::TFData	fileTF;
extern ZTGL::TFData	tileNumTF;

extern Map::Map	map;

i32	Init();
void	Main();

}
