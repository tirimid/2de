// SPDX-License-Identifier: GPL-3.0-or-later

#define	MAP_MAGIC	"[2de]MAG"
#define	MAP_VERSION	1

#define	MAX_MAP_WIDTH			65535
#define	MAX_MAP_HEIGHT			65535
#define	MAX_MAP_REGIONS		65535
#define	MAX_MAP_ATTRS			65535
#define	MAX_MAP_ATTR_LISTS	65534 // -1 because attribute lists are 1-indexed.

// map file structure; start to end, data is packed.
//
// size (bytes)  field
//
// 8             magic string
// 1             version
//
// 2             map width in tiles
// 2             map height in tiles
// 2             number of regions
// 2             number of attributes
// 2             number of attribute lists
//
// T             tile types
// 2 * T         tile attribute lists
//
// R             region types
// 4 * R         region x values
// 4 * R         region y values
// 4 * R         region w values
// 4 * R         region h values
// 2 * R         region attribute lists
//
// A             attribute types
// 16 * A        attribute names
// 16 * A        attribute values
//
// L             attribute list lengths
// 32 * L        attribute lists
//
// 8             magic string (duplicate)
// 1             version (duplicate)
//
// note:
// * T = map width * map height
// * R = number of regions
// * A = number of attributes
// * L = number of attribute lists

namespace Map
{

enum AttrType : u8
{
	INT	= 0,
	FLOAT,
	STR,
	BOOL
};

struct AttrName
{
	char	m_Name[16];
};

union AttrValue
{
	i64	m_Int;
	f64	m_Float;
	char	m_String[16];
	bool	m_Bool;
};

struct AttrList
{
	u16	m_Attrs[16];
};

struct Map
{
	u16			m_W					{};
	u16			m_H					{};
	u16			m_NRegions			{};
	u16			m_NAttrs				{};
	u16			m_NAttrLists		{};
	
	void*			m_Buffer				{};
	
	u8*			m_TileTypes			{};
	u16*			m_TileAttrLists	{};
	
	u8*			m_RegionTypes		{};
	f32*			m_RegionXs			{};
	f32*			m_RegionYs			{};
	f32*			m_RegionWs			{};
	f32*			m_RegionHs			{};
	u16*			m_RegionAttrLists	{};
	
	u8*			m_AttrTypes			{};
	AttrName*	m_AttrNames			{};
	AttrValue*	m_AttrValues		{};
	
	u8*			m_AttrListLengths	{};
	AttrList*	m_AttrLists			{};
	
	static i32	New(OUT Map& map);
	static i32	Read(OUT Map& map, FILE* file);
	
	i32			Write(FILE* file);
	void			Render();
	void			RenderOutlines();
	void			Grow(u32 dx, u32 dy);
	void			RefitBounds();
};

void	Init();

}
