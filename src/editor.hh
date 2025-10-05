// SPDX-License-Identifier: GPL-3.0-or-later

namespace Editor
{

enum System : u8
{
	TILEMAP	= 0,
	REGION,
	ATTRIBUTE
};

enum TilemapMode : u8
{
	PENCIL	= 0,
	BUCKET_FILL,
	EYEDROPPER,
	SELECT,
	BOX_SELECT
};

extern f32				camX;
extern f32				camY;
extern f32				camZoom;
extern System			system;
extern TilemapMode	tilemapMode;

extern bool				menu;
extern bool				unsaved;
extern char				targetFile[256];
extern char				tileNum[8];
extern ZTGL::TFData	targetFileTF;
extern ZTGL::TFData	tileNumTF;

extern Map::Map	map;

i32	Init();
void	Main();

}
