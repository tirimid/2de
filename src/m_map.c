// SPDX-License-Identifier: GPL-3.0-or-later

m_map_t m_map;

i32
m_read(FILE *fp)
{
	char magic[9] = {0};
	if (fread(magic, 1, 8, fp) != 8)
	{
		z_err("map: failed to read 8xu8 magic!");
		return 1;
	}
	
	if (strcmp(magic, M_MAGIC))
	{
		z_err("map: invalid magic - %s!", magic);
		return 1;
	}
	
	u8 version;
	if (fread(&version, 1, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu8 version!");
		return 1;
	}
	
	if (version != M_VERSION)
	{
		z_err("map: unsupported version - %u!", version);
		return 1;
	}
	
	u16 w;
	if (fread(&w, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 width!");
		return 1;
	}
	
	u16 h;
	if (fread(&h, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 height!");
		return 1;
	}
	
	if (!w || !h)
	{
		z_err("map: invalid wxh - %ux%u!", w, h);
		return 1;
	}
	
	u16 nregs;
	if (fread(&nregs, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 region count!");
		return 1;
	}
	
	u16 nattrs;
	if (fread(&nattrs, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 attribute count!");
		return 1;
	}
	
	u16 nattrlists;
	if (fread(&nattrlists, 2, 1, fp) != 1)
	{
		z_err("map: failed to read 1xu16 attribute list count!");
		return 1;
	}
	
	// TODO: read dynamically sized data.
	
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
	// TODO: implement.
}
