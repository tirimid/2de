// SPDX-License-Identifier: GPL-3.0-or-later

#define INCXXD(name) \
	extern unsigned char const name[]; \
	extern unsigned const name##_len;

// data gnerated by xxd.
INCXXD(vcr_osd_mono_ttf)