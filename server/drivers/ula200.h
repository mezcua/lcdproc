/*  This is the LCDproc driver header for ULA-200 (http://www.elv.de)

    Author: bernhard.walle@gmx.de

    Copyright (C) 2006, Bernhard Walle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 */
#ifndef ULA200_H
#define ULA200_H

#include "lcd.h"

#ifndef bool
# define bool short
# define true 1
# define false 0
#endif

MODULE_EXPORT int  ula200_init(Driver *drvthis);
MODULE_EXPORT void ula200_close (Driver *drvthis);
MODULE_EXPORT int  ula200_width (Driver *drvthis);
MODULE_EXPORT int  ula200_height (Driver *drvthis);
MODULE_EXPORT void ula200_clear (Driver *drvthis);
MODULE_EXPORT void ula200_string (Driver *drvthis, int x, int y, const char string[]);
MODULE_EXPORT void ula200_chr (Driver *drvthis, int x, int y, char c);
MODULE_EXPORT void ula200_set_char (Driver *drvthis, int n, char *dat);
MODULE_EXPORT void ula200_backlight (Driver *drvthis, int on);
MODULE_EXPORT void ula200_flush (Driver *drvthis);
MODULE_EXPORT void ula200_vbar (Driver *drvthis, int x, int y, int len, int promille, int options);
MODULE_EXPORT void ula200_hbar (Driver *drvthis, int x, int y, int len, int promille, int options);
MODULE_EXPORT void ula200_num (Driver *drvthis, int x, int num);
MODULE_EXPORT int  ula200_icon(Driver *drvthis, int x, int y, int icon);

#endif /* ULA200_H */
