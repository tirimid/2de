// SPDX-License-Identifier: GPL-3.0-or-later

#define INCXXD(name) \
	extern unsigned char const name[]; \
	extern unsigned const name##_len;

#define INCRES(name) \
	{ \
		.data = name, \
		.size = &name##_len \
	}

typedef struct resdata
{
	u8 const *data;
	u32 const *size;
} resdata_t;

// data gnerated by xxd.
INCXXD(vcr_osd_mono_ttf)