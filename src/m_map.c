// SPDX-License-Identifier: GPL-3.0-or-later

m_map_t m_map;

i32
m_init(void)
{
	m_map.w = 1;
	m_map.h = 1;
	
	u32 ntiles = (u32)m_map.w * m_map.h;
	z_allocbatch_t allocs[] =
	{
		{(void **)&m_map.tiletypes, ntiles, 1},
		{(void **)&m_map.tileattrlists, ntiles, 2},
		{(void **)&m_map.regtypes, m_map.nregs, 1},
		{(void **)&m_map.regxs, m_map.nregs, 4},
		{(void **)&m_map.regys, m_map.nregs, 4},
		{(void **)&m_map.regws, m_map.nregs, 4},
		{(void **)&m_map.reghs, m_map.nregs, 4},
		{(void **)&m_map.regattrlists, m_map.nregs, 2},
		{(void **)&m_map.attrtypes, m_map.nattrs, 1},
		{(void **)&m_map.attrnames, m_map.nattrs, sizeof(m_attrname_t)},
		{(void **)&m_map.attrvals, m_map.nattrs, sizeof(m_attrval_t)},
		{(void **)&m_map.attrlistlens, m_map.nattrlists, 1},
		{(void **)&m_map.attrlists, m_map.nattrlists, sizeof(m_attrlist_t)}
	};
	m_map.buf = z_allocbatch(allocs, sizeof(allocs) / sizeof(allocs[0]));
	if (!m_map.buf)
	{
		z_err("map: failed to allocate batch buffer!");
		return 1;
	}
	
	m_map.tiletypes[0] = 0;
	m_map.tileattrlists[0] = 0;
	
	return 0;
}

i32
m_read(FILE *fp)
{
	// read and validate header.
	char magic[9] = {0};
	if (fread(magic, 1, 8, fp) != 8)
	{
		z_err("map: failed to read 8xu8 magic (header)!");
		return 1;
	}
	
	if (strcmp(magic, M_MAGIC))
	{
		z_err("map: invalid magic (header) - %s!", magic);
		return 1;
	}
	
	u8 version;
	if (fread(&version, 1, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu8 version (header)!");
		return 1;
	}
	
	if (version != M_VERSION)
	{
		z_err("map: unsupported version (header) - %u!", version);
		return 1;
	}
	
	// read and validate map size information.
	if (fread(&m_map.w, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 width!");
		return 1;
	}
	
	if (fread(&m_map.h, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 height!");
		return 1;
	}
	
	if (!m_map.w || !m_map.h)
	{
		z_err("map: invalid wxh - %ux%u!", m_map.w, m_map.h);
		return 1;
	}
	
	if (fread(&m_map.nregs, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 region count!");
		return 1;
	}
	
	if (fread(&m_map.nattrs, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 attribute count!");
		return 1;
	}
	
	if (fread(&m_map.nattrlists, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 attribute list count!");
		return 1;
	}
	
	// allocate space for map data.
	u32 ntiles = (u32)m_map.w * m_map.h;
	z_allocbatch_t allocs[] =
	{
		{(void **)&m_map.tiletypes, ntiles, 1},
		{(void **)&m_map.tileattrlists, ntiles, 2},
		{(void **)&m_map.regtypes, m_map.nregs, 1},
		{(void **)&m_map.regxs, m_map.nregs, 4},
		{(void **)&m_map.regys, m_map.nregs, 4},
		{(void **)&m_map.regws, m_map.nregs, 4},
		{(void **)&m_map.reghs, m_map.nregs, 4},
		{(void **)&m_map.regattrlists, m_map.nregs, 2},
		{(void **)&m_map.attrtypes, m_map.nattrs, 1},
		{(void **)&m_map.attrnames, m_map.nattrs, sizeof(m_attrname_t)},
		{(void **)&m_map.attrvals, m_map.nattrs, sizeof(m_attrval_t)},
		{(void **)&m_map.attrlistlens, m_map.nattrlists, 1},
		{(void **)&m_map.attrlists, m_map.nattrlists, sizeof(m_attrlist_t)}
	};
	m_map.buf = z_allocbatch(allocs, sizeof(allocs) / sizeof(allocs[0]));
	if (!m_map.buf)
	{
		z_err("map: failed to allocate batch buffer!");
		return 1;
	}
	
	// read tile data.
	if (fread(m_map.tiletypes, 1, ntiles, fp) != ntiles)
	{
		z_err("map: failed to read %uxu8 tile types!", ntiles);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.tileattrlists, 2, ntiles, fp) != ntiles)
	{
		z_err("map: failed to read %uxu16 tile attribute lists!", ntiles);
		free(m_map.buf);
		return 1;
	}
	
	// read region data.
	if (fread(m_map.regtypes, 1, m_map.nregs, fp) != m_map.nregs)
	{
		z_err("map: failed to read %uxu8 region types!", m_map.nregs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.regxs, 4, m_map.nregs, fp) != m_map.nregs)
	{
		z_err("map: failed to read %uxf32 region x values!", m_map.nregs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.regys, 4, m_map.nregs, fp) != m_map.nregs)
	{
		z_err("map: failed to read %uxf32 region y values!", m_map.nregs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.regws, 4, m_map.nregs, fp) != m_map.nregs)
	{
		z_err("map: failed to read %uxf32 region w values!", m_map.nregs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.reghs, 4, m_map.nregs, fp) != m_map.nregs)
	{
		z_err("map: failed to read %uxf32 region h values!", m_map.nregs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.regattrlists, 2, m_map.nregs, fp) != m_map.nregs)
	{
		z_err("map: failed to read %uxu16 region attribute lists!", m_map.nregs);
		free(m_map.buf);
		return 1;
	}
	
	// read attribute data.
	if (fread(m_map.attrtypes, 1, m_map.nattrs, fp) != m_map.nattrs)
	{
		z_err("map: failed to read %uxu8 attribute types!", m_map.nattrs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.attrnames, sizeof(m_attrname_t), m_map.nattrs, fp) != m_map.nattrs)
	{
		z_err("map: failed to read %uxm_attrname_t attribute names!", m_map.nattrs);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.attrvals, sizeof(m_attrval_t), m_map.nattrs, fp) != m_map.nattrs)
	{
		z_err("map: failed to read %uxm_attrval_t attribute names!", m_map.nattrs);
		free(m_map.buf);
		return 1;
	}
	
	// read attribute list data.
	if (fread(m_map.attrlistlens, 1, m_map.nattrlists, fp) != m_map.nattrlists)
	{
		z_err("map: failed to read %uxu8 attribute list lengths!", m_map.nattrlists);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(m_map.attrlists, sizeof(m_attrlist_t), m_map.nattrlists, fp) != m_map.nattrlists)
	{
		z_err("map: failed to read %uxm_attrlist_t attribute lists!", m_map.nattrlists);
		free(m_map.buf);
		return 1;
	}
	
	// read and validate trailer.
	if (fread(magic, 1, 8, fp) != 8)
	{
		z_err("map: failed to read 8xu8 magic (trailer)!");
		free(m_map.buf);
		return 1;
	}
	
	if (strcmp(magic, M_MAGIC))
	{
		z_err("map: invalid magic (trailer) - %s!", magic);
		free(m_map.buf);
		return 1;
	}
	
	if (fread(&version, 1, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu8 version (trailer)!");
		free(m_map.buf);
		return 1;
	}
	
	if (version != M_VERSION)
	{
		z_err("map: unsupported version (trailer) - %u!", version);
		free(m_map.buf);
		return 1;
	}
	
	return 0;
}

i32
m_write(FILE *fp)
{
	// TODO: implement map write to file.
	return 1;
}

void
m_render(void)
{
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
}

void
m_renderoutlines(void)
{
	// TODO: implement.
}
