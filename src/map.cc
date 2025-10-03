// SPDX-License-Identifier: GPL-3.0-or-later

namespace Map
{

i32
Map::New(OUT Map& map)
{
	map.m_W = 1;
	map.m_H = 1;
	
	u32 nTiles = (u32)map.m_W * map.m_H;
	ZTGL::AllocBatchDesc allocs[] =
	{
		{(void**)&map.m_TileTypes,       nTiles,           1},
		{(void**)&map.m_TileAttrLists,   nTiles,           2},
		{(void**)&map.m_RegionTypes,     map.m_NRegions,   1},
		{(void**)&map.m_RegionXs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionYs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionWs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionHs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionAttrLists, map.m_NRegions,   2},
		{(void**)&map.m_AttrTypes,       map.m_NAttrs,     1},
		{(void**)&map.m_AttrNames,       map.m_NAttrs,     sizeof(AttrName)},
		{(void**)&map.m_AttrValues,      map.m_NAttrs,     sizeof(AttrValue)},
		{(void**)&map.m_AttrListLengths, map.m_NAttrLists, 1},
		{(void**)&map.m_AttrLists,       map.m_NAttrLists, sizeof(AttrList)}
	};
	
	map.m_Buffer = ZTGL::AllocBatch(allocs, sizeof(allocs) / sizeof(allocs[0]));
	if (!map.m_Buffer)
	{
		ZTGL::Error("map: failed to allocate batch buffer!");
		return 1;
	}
	
	map.m_TileTypes[0] = 0;
	map.m_TileAttrLists[0] = 0;
	
	return 0;
}

i32
Map::Read(OUT Map& map, FILE* file)
{
	// read and validate header.
	char magic[9] = {0};
	if (fread(magic, 1, 8, file) != 8)
	{
		ZTGL::Error("map: failed to read 8xu8 magic (header)!");
		return 1;
	}
	
	if (strcmp(magic, MAP_MAGIC))
	{
		ZTGL::Error("map: invalid magic (header) - %s!", magic);
		return 1;
	}
	
	u8 version;
	if (fread(&version, 1, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu8 version (header)!");
		return 1;
	}
	
	if (version != MAP_VERSION)
	{
		ZTGL::Error("map: unsupported version (header) - %u!", version);
		return 1;
	}
	
	// read and validate map size information.
	if (fread(&map.m_W, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 width!");
		return 1;
	}
	
	if (fread(&map.m_H, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 height!");
		return 1;
	}
	
	if (!map.m_W || !map.m_H)
	{
		ZTGL::Error("map: invalid wxh - %ux%u!", map.m_W, map.m_H);
		return 1;
	}
	
	if (fread(&map.m_NRegions, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 region count!");
		return 1;
	}
	
	if (fread(&map.m_NAttrs, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 attribute count!");
		return 1;
	}
	
	if (fread(&map.m_NAttrLists, 2, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu16 attribute list count!");
		return 1;
	}
	
	// allocate space for map data.
	u32 nTiles = (u32)map.m_W * map.m_H;
	ZTGL::AllocBatchDesc allocs[] =
	{
		{(void**)&map.m_TileTypes,       nTiles,           1},
		{(void**)&map.m_TileAttrLists,   nTiles,           2},
		{(void**)&map.m_RegionTypes,     map.m_NRegions,   1},
		{(void**)&map.m_RegionXs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionYs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionWs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionHs,        map.m_NRegions,   4},
		{(void**)&map.m_RegionAttrLists, map.m_NRegions,   2},
		{(void**)&map.m_AttrTypes,       map.m_NAttrs,     1},
		{(void**)&map.m_AttrNames,       map.m_NAttrs,     sizeof(AttrName)},
		{(void**)&map.m_AttrValues,      map.m_NAttrs,     sizeof(AttrValue)},
		{(void**)&map.m_AttrListLengths, map.m_NAttrLists, 1},
		{(void**)&map.m_AttrLists,       map.m_NAttrLists, sizeof(AttrList)}
	};
	map.m_Buffer = ZTGL::AllocBatch(allocs, sizeof(allocs) / sizeof(allocs[0]));
	if (!map.m_Buffer)
	{
		ZTGL::Error("map: failed to allocate batch buffer!");
		return 1;
	}
	
	// read tile data.
	if (fread(map.m_TileTypes, 1, nTiles, file) != nTiles)
	{
		ZTGL::Error("map: failed to read %uxu8 tile types!", nTiles);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_TileAttrLists, 2, nTiles, file) != nTiles)
	{
		ZTGL::Error("map: failed to read %uxu16 tile attribute lists!", nTiles);
		free(map.m_Buffer);
		return 1;
	}
	
	// read region data.
	if (fread(map.m_RegionTypes, 1, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxu8 region types!", map.m_NRegions);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_RegionXs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region x values!", map.m_NRegions);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_RegionYs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region y values!", map.m_NRegions);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_RegionWs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region w values!", map.m_NRegions);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_RegionHs, 4, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxf32 region h values!", map.m_NRegions);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_RegionAttrLists, 2, map.m_NRegions, file) != map.m_NRegions)
	{
		ZTGL::Error("map: failed to read %uxu16 region attribute lists!", map.m_NRegions);
		free(map.m_Buffer);
		return 1;
	}
	
	// read attribute data.
	if (fread(map.m_AttrTypes, 1, map.m_NAttrs, file) != map.m_NAttrs)
	{
		ZTGL::Error("map: failed to read %uxu8 attribute types!", map.m_NAttrs);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_AttrNames, sizeof(AttrName), map.m_NAttrs, file) != map.m_NAttrs)
	{
		ZTGL::Error("map: failed to read %uxm_attrname_t attribute names!", map.m_NAttrs);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_AttrValues, sizeof(AttrValue), map.m_NAttrs, file) != map.m_NAttrs)
	{
		ZTGL::Error("map: failed to read %uxm_attrval_t attribute names!", map.m_NAttrs);
		free(map.m_Buffer);
		return 1;
	}
	
	// read attribute list data.
	if (fread(map.m_AttrListLengths, 1, map.m_NAttrLists, file) != map.m_NAttrLists)
	{
		ZTGL::Error("map: failed to read %uxu8 attribute list lengths!", map.m_NAttrLists);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(map.m_AttrLists, sizeof(AttrList), map.m_NAttrLists, file) != map.m_NAttrLists)
	{
		ZTGL::Error("map: failed to read %uxm_attrlist_t attribute lists!", map.m_NAttrLists);
		free(map.m_Buffer);
		return 1;
	}
	
	// read and validate trailer.
	if (fread(magic, 1, 8, file) != 8)
	{
		ZTGL::Error("map: failed to read 8xu8 magic (trailer)!");
		free(map.m_Buffer);
		return 1;
	}
	
	if (strcmp(magic, MAP_MAGIC))
	{
		ZTGL::Error("map: invalid magic (trailer) - %s!", magic);
		free(map.m_Buffer);
		return 1;
	}
	
	if (fread(&version, 1, 1, file) != 1)
	{
		ZTGL::Error("map: failed to read 1xu8 version (trailer)!");
		free(map.m_Buffer);
		return 1;
	}
	
	if (version != MAP_VERSION)
	{
		ZTGL::Error("map: unsupported version (trailer) - %u!", version);
		free(map.m_Buffer);
		return 1;
	}
	
	return 0;
}

i32
Map::Write(FILE* file)
{
	// TODO: implement map write to file.
	return 1;
}

void
Map::Render()
{
#if 0
	i32 wndw, wndh;
	SDL_GetWindowSize(r_wnd, &wndw, &wndh);
	
	f32 minx, miny;
	r_screen2game(&minx, &miny, 0, 0);
	minx = 0.0f > minx ? 0.0f : minx;
	miny = 0.0f > miny ? 0.0f : miny;
	
	f32 maxx, maxy;
	r_screen2game(&maxx, &maxy, wndw, wndh);
	maxx = (u32)maxx + 1 < m_map.w ? (u32)maxx + 1 : m_map.w;
	maxy = (u32)maxy + 1 < m_map.h ? (u32)maxy + 1 : m_map.h;
	
	for (u32 x = minx; x < (u32)maxx; ++x)
	{
		for (u32 y = miny; y < (u32)maxy; ++y)
		{
			if (!m_map.tiletypes[y * m_map.w + x])
			{
				continue;
			}
			
			// TODO: draw tiles.
		}
	}
#endif
}

void
Map::RenderOutlines()
{
	// TODO: implement map render outlines.
}

}
