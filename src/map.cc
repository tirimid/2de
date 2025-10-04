// SPDX-License-Identifier: GPL-3.0-or-later

namespace Map
{

static u8	tileColors[255][3];

i32
Map::New(OUT Map& map)
{
	map.m_W = 1;
	map.m_H = 1;
	
	u32	nTiles	= (u32)map.m_W * map.m_H;
	ZTGL::AllocBatchDesc	allocs[]	=
	{
		{(void**)&map.m_TileTypes,			nTiles,				1},
		{(void**)&map.m_TileAttrLists,	nTiles,				2},
		{(void**)&map.m_RegionTypes,		map.m_NRegions,	1},
		{(void**)&map.m_RegionXs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionYs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionWs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionHs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionAttrLists,	map.m_NRegions,	2},
		{(void**)&map.m_AttrTypes,			map.m_NAttrs,		1},
		{(void**)&map.m_AttrNames,			map.m_NAttrs,		sizeof(AttrName)},
		{(void**)&map.m_AttrValues,		map.m_NAttrs,		sizeof(AttrValue)},
		{(void**)&map.m_AttrListLengths,	map.m_NAttrLists,	1},
		{(void**)&map.m_AttrLists,			map.m_NAttrLists,	sizeof(AttrList)}
	};
	
	map.m_Buffer = ZTGL::AllocBatch(allocs, sizeof(allocs) / sizeof(allocs[0]));
	if (!map.m_Buffer)
	{
		ZTGL::Error("map: failed to allocate batch buffer!");
		return (1);
	}
	
	map.m_TileTypes[0]		= 0;
	map.m_TileAttrLists[0]	= 0;
	
	return (0);
}

i32
Map::Read(OUT Map& map, FILE* file)
{
	// read and validate header.
	char	magic[9]	= {0};
	if (fread(magic, 1, 8, file) != 8)
	{
		ZTGL::Error("map: failed to read 8xu8 magic (header)!");
		return (1);
	}
	
	if (strcmp(magic, MAP_MAGIC))
	{
		ZTGL::Error("map: invalid magic (header) - %s!", magic);
		return (1);
	}
	
	u8	version	{};
	if (fread(&version, 1, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu8 version (header)!");
		return (1);
	}
	
	if (version != MAP_VERSION)
	{
		ZTGL::Error("map: unsupported version (header) - %u!", version);
		return (1);
	}
	
	// read and validate map size information.
	if (fread(&map.m_W, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 width!");
		return (1);
	}
	
	if (fread(&map.m_H, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 height!");
		return (1);
	}
	
	if (!map.m_W || !map.m_H)
	{
		ZTGL::Error("map: invalid wxh - %ux%u!", map.m_W, map.m_H);
		return (1);
	}
	
	if (fread(&map.m_NRegions, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 region count!");
		return (1);
	}
	
	if (fread(&map.m_NAttrs, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 attribute count!");
		return (1);
	}
	
	if (fread(&map.m_NAttrLists, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 attribute list count!");
		return (1);
	}
	
	// allocate space for map data.
	u32	nTiles	= (u32)map.m_W * map.m_H;
	ZTGL::AllocBatchDesc	allocs[]	=
	{
		{(void**)&map.m_TileTypes,			nTiles,				1},
		{(void**)&map.m_TileAttrLists,	nTiles,				2},
		{(void**)&map.m_RegionTypes,		map.m_NRegions,	1},
		{(void**)&map.m_RegionXs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionYs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionWs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionHs,			map.m_NRegions,	4},
		{(void**)&map.m_RegionAttrLists,	map.m_NRegions,	2},
		{(void**)&map.m_AttrTypes,			map.m_NAttrs,		1},
		{(void**)&map.m_AttrNames,			map.m_NAttrs,		sizeof(AttrName)},
		{(void**)&map.m_AttrValues,		map.m_NAttrs,		sizeof(AttrValue)},
		{(void**)&map.m_AttrListLengths,	map.m_NAttrLists,	1},
		{(void**)&map.m_AttrLists,			map.m_NAttrLists,	sizeof(AttrList)}
	};
	map.m_Buffer = ZTGL::AllocBatch(allocs, ARRAY_SIZE(allocs));
	if (!map.m_Buffer)
	{
		ZTGL::Error("map: failed to allocate batch buffer!");
		return (1);
	}
	
	// read tile data.
	if (fread(map.m_TileTypes, 1, nTiles, file) != nTiles)
	{
		ZTGL::Error("map: failed to read %uxu8 tile types!", nTiles);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_TileAttrLists, 2, nTiles, file) != nTiles)
	{
		ZTGL::Error("map: failed to read %uxu16 tile attribute lists!", nTiles);
		free(map.m_Buffer);
		return (1);
	}
	
	// read region data.
	if (fread(map.m_RegionTypes, 1, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxu8 region types!", map.m_NRegions);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_RegionXs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region x values!", map.m_NRegions);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_RegionYs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region y values!", map.m_NRegions);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_RegionWs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region w values!", map.m_NRegions);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_RegionHs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region h values!", map.m_NRegions);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_RegionAttrLists, 2, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxu16 region attribute lists!", map.m_NRegions);
		free(map.m_Buffer);
		return (1);
	}
	
	// read attribute data.
	if (fread(map.m_AttrTypes, 1, map.m_NAttrs, file) != map.m_NAttrs)
	{
		ZTGL::Error("map: failed to read %uxu8 attribute types!", map.m_NAttrs);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_AttrNames, sizeof(AttrName), map.m_NAttrs, file) != map.m_NAttrs)
	{
		ZTGL::Error("map: failed to read %uxm_attrname_t attribute names!", map.m_NAttrs);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_AttrValues, sizeof(AttrValue), map.m_NAttrs, file) != map.m_NAttrs)
	{
		ZTGL::Error("map: failed to read %uxm_attrval_t attribute names!", map.m_NAttrs);
		free(map.m_Buffer);
		return (1);
	}
	
	// read attribute list data.
	if (fread(map.m_AttrListLengths, 1, map.m_NAttrLists, file) != map.m_NAttrLists)
	{
		ZTGL::Error("map: failed to read %uxu8 attribute list lengths!", map.m_NAttrLists);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(map.m_AttrLists, sizeof(AttrList), map.m_NAttrLists, file) != map.m_NAttrLists)
	{
		ZTGL::Error("map: failed to read %uxm_attrlist_t attribute lists!", map.m_NAttrLists);
		free(map.m_Buffer);
		return (1);
	}
	
	// read and validate trailer.
	if (fread(magic, 1, 8, file) != 8)
	{
		ZTGL::Error("map: failed to read 8xu8 magic (trailer)!");
		free(map.m_Buffer);
		return (1);
	}
	
	if (strcmp(magic, MAP_MAGIC))
	{
		ZTGL::Error("map: invalid magic (trailer) - %s!", magic);
		free(map.m_Buffer);
		return (1);
	}
	
	if (fread(&version, 1, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu8 version (trailer)!");
		free(map.m_Buffer);
		return (1);
	}
	
	if (version != MAP_VERSION)
	{
		ZTGL::Error("map: unsupported version (trailer) - %u!", version);
		free(map.m_Buffer);
		return (1);
	}
	
	return (0);
}

i32
Map::Write(FILE* file)
{
	// TODO: implement map write to file.
	return (1);
}

void
Map::Render()
{
	i32	windowWidth		{};
	i32	windowHeight	{};
	SDL_GetWindowSize(Render::window, &windowWidth, &windowHeight);
	
	f32	minX	{};
	f32	minY	{};
	Render::ScreenToGame(minX, minY, 0, 0);
	minX = MAX(0.0f, minX);
	minY = MAX(0.0f, minY);
	
	f32	maxX	{};
	f32	maxY	{};
	Render::ScreenToGame(maxX, maxY, windowWidth, windowHeight);
	maxX = MIN((u32)maxX + 1, m_W);
	maxY = MIN((u32)maxY + 1, m_H);
	
	for (u32 x = minX; x < (u32)maxX; ++x)
	{
		for (u32 y = minY; y < (u32)maxY; ++y)
		{
			u8	type	= m_TileTypes[y * m_W + x];
			if (!type)
			{
				continue;
			}
			
			SDL_SetRenderDrawColor(
				Render::renderer,
				tileColors[type][0],
				tileColors[type][1],
				tileColors[type][2],
				255
			);
			Render::RenderRectRel(x, y, 1.0f, 1.0f);
		}
	}
}

void
Map::RenderOutlines()
{
	i32	windowWidth		{};
	i32	windowHeight	{};
	SDL_GetWindowSize(Render::window, &windowWidth, &windowHeight);
	
	f32	minX	{};
	f32	minY	{};
	Render::ScreenToGame(minX, minY, 0, 0);
	minX = MAX(0.0f, minX);
	minY = MAX(0.0f, minY);
	
	f32	maxX	{};
	f32	maxY	{};
	Render::ScreenToGame(maxX, maxY, windowWidth, windowHeight);
	maxX = MIN((u32)maxX + 1, m_W);
	maxY = MIN((u32)maxY + 1, m_H);
	
	SDL_SetRenderDrawColor(Render::renderer, OUTLINE_COLOR);
	for (u32 x = 0; x < (u32)maxX; ++x)
	{
		for (u32 y = 0; y < (u32)maxY; ++y)
		{
			Render::RenderHollowRectRel(x, y, 1.0f, 1.0f);
		}
	}
}

void
Map::Grow(u32 dx, u32 dy)
{
	u32	oldWidth		= m_W;
	u32	oldHeight	= m_H;
	
	m_W += dx;
	m_H += dy;
	
	u32	oldNTiles	= oldWidth * oldHeight;
	u32	newNTiles	= (u32)m_W * m_H;
	
	ZTGL::ReallocBatchDesc	reallocs[]	=
	{
		{(void**)&m_TileTypes,			oldNTiles,		newNTiles,		1},
		{(void**)&m_TileAttrLists,		oldNTiles,		newNTiles,		2},
		{(void**)&m_RegionTypes,		m_NRegions,		m_NRegions,		1},
		{(void**)&m_RegionXs,			m_NRegions,		m_NRegions,		4},
		{(void**)&m_RegionYs,			m_NRegions,		m_NRegions,		4},
		{(void**)&m_RegionWs,			m_NRegions,		m_NRegions,		4},
		{(void**)&m_RegionHs,			m_NRegions,		m_NRegions,		4},
		{(void**)&m_RegionAttrLists,	m_NRegions,		m_NRegions,		2},
		{(void**)&m_AttrTypes,			m_NAttrs,		m_NAttrs,		1},
		{(void**)&m_AttrNames,			m_NAttrs,		m_NAttrs,		sizeof(AttrName)},
		{(void**)&m_AttrValues,			m_NAttrs,		m_NAttrs,		sizeof(AttrValue)},
		{(void**)&m_AttrListLengths,	m_NAttrLists,	m_NAttrLists,	1},
		{(void**)&m_AttrLists,			m_NAttrLists,	m_NAttrLists,	sizeof(AttrList)}
	};
	m_Buffer = ZTGL::ReallocBatch(m_Buffer, reallocs, ARRAY_SIZE(reallocs));
	
	// create new empty cells (horizontal).
	usize	moveLength	= oldWidth * (oldHeight - 1);
	usize	moveIndex	= oldWidth;
	while (moveLength > 0)
	{
		memmove(
			&m_TileTypes[moveIndex + dx],
			&m_TileTypes[moveIndex],
			moveLength
		);
		memset(&m_TileTypes[moveIndex], 0, dx);
		
		memmove(
			&m_TileAttrLists[moveIndex + dx],
			&m_TileAttrLists[moveIndex],
			2 * moveLength
		);
		memset(&m_TileAttrLists[moveIndex], 0, 2 * dx);
		
		moveLength -= oldWidth;
		moveIndex += m_W;
	}
	
	memset(&m_TileTypes[moveIndex], 0, dx);
	memset(&m_TileAttrLists[moveIndex], 0, 2 * dx);
	
	// create new empty cells (vertical).
	memset(&m_TileTypes[m_W * oldHeight], 0, m_W * dy);
	memset(&m_TileAttrLists[m_W * oldHeight], 0, 2 * m_W * dy);
}

void
Init()
{
	srand(0);
	for (usize i = 0; i < ARRAY_SIZE(tileColors); ++i)
	{
		tileColors[i][0]	= rand() % 256;
		tileColors[i][1]	= rand() % 256;
		tileColors[i][2]	= rand() % 256;
	}
}

}
