// SPDX-License-Identifier: GPL-3.0-or-later

#define M_MAGIC "[2de]MAG"
#define M_VERSION 1

#define M_MAXWIDTH 65535
#define M_MAXHEIGHT 65535
#define M_MAXREGIONS 65535
#define M_MAXATTRS 65535
#define M_MAXATTRLISTS 65534 // -1 because attribute lists are 1-indexed.

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

typedef enum m_attrtype
{
	M_INT = 0,
	M_FLOAT,
	M_STR,
	M_BOOL
} m_attrtype_t;

typedef struct m_attrname
{
	char name[16];
} m_attrname_t;

typedef union m_attrval
{
	i64 i;
	f64 f;
	char s[16];
	bool b;
} m_attrval_t;

typedef struct m_attrlist
{
	u16 attrs[16];
} m_attrlist_t;

typedef struct m_map
{
	u16 w, h;
	u16 nregs;
	u16 nattrs;
	u16 nattrlists;
	
	void *buf;
	
	u8 *tiletypes;
	u16 *tileattrlists;
	
	u8 *regtypes;
	f32 *regxs, *regys;
	f32 *regws, *reghs;
	u16 *regattrlists;
	
	u8 *attrtypes;
	m_attrname_t *attrnames;
	m_attrval_t *attrvals;
	
	u8 *attrlistlens;
	m_attrlist_t *attrlists;
} m_map_t;

extern m_map_t m_map;

i32 m_init(void);
i32 m_read(FILE *fp);
i32 m_write(FILE *fp);
void m_render(void);
void m_renderoutlines(void);
