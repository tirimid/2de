// SPDX-License-Identifier: GPL-3.0-or-later

#define	MIN(a, b)				((a) < (b) ? (a) : (b))
#define	MAX(a, b)				((a) > (b) ? (a) : (b))
#define	CLAMP(min, n, max)	(MIN((max), MAX((min), (n))))
#define	ARRAY_SIZE(array)		(sizeof(array) / sizeof(array[0]))
