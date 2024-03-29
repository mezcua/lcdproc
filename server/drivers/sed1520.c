//////////////////////////////////////////////////////////////////////////
// This is a driver for 122x32 pixel graphic displays based on the      //
// SED1520 Controller connected to the parallel port. Check             //
// http://www.usblcd.de/lcdproc/ for where to buy                       //
// and how to build the hardware. This Controller has no built in       //
// character generator. Therefore all fonts and pixels are generated    //
// by this driver.                                                      //
//                                                                      //
// This driver is based on drv_base.c and hd44780.c.                    //
// The HD44780 font in sed1520fm.c was shamelessly stolen from          //
// Michael Reinelt / lcd4linux and is (C) 2000 by him.                  //
// The rest of fontmap.c and this driver is                             //
//                                                                      //
// Moved the delay timing code by Charles Steinkuehler to timing.h.     //
// Guillaume Filion <gfk@logidac.com>, December 2001                    //
//                                                                      //
// (C) 2001 Robin Adams ( robin@adams-online.de )                       //
//                                                                      //
// This driver is released under the GPL. See file COPYING in this      //
// package for further details.                                         //
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>
#include <time.h>
#include "port.h"
#include "timing.h"
#define uPause timing_uPause

#include "sed1520fm.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif


#ifndef DEFAULT_PORT
# define DEFAULT_PORT	0x378
#endif

#define CELLWIDTH	6
#define CELLHEIGHT	8

#define PIXELWIDTH	122
#define PIXELHEIGHT	32

#define WIDTH		((int) (PIXELWIDTH / CELLWIDTH))	/* 20 */
#define HEIGHT		((int) (PIXELHEIGHT / CELLHEIGHT))	/*  4 */

#define A0 0x08
#define CS2 0x04
#define CS1 0x02
#define WR 0x01
#define IODELAY 500

#include "shared/str.h"
#include "lcd.h"
#include "sed1520.h"
#include "report.h"


typedef struct driver_private_data {
    unsigned int port;

    unsigned char *framebuf;
} PrivateData;


// Vars for the server core
MODULE_EXPORT char *api_version = API_VERSION;
MODULE_EXPORT int stay_in_foreground = 0;
MODULE_EXPORT int supports_multiple = 0;
MODULE_EXPORT char *symbol_prefix = "sed1520_";

/////////////////////////////////////////////////////////////////
// writes command value to one or both sed1520 selected by chip
//
void
writecommand (unsigned int port, int value, int chip)
{
    port_out(port, value);
    port_out(port + 2, WR + CS1 - (chip & CS1) + (chip & CS2));
    port_out(port + 2, CS1 - (chip & CS1) + (chip & CS2));
    uPause(IODELAY);
    port_out(port + 2, WR + CS1 - (chip & CS1) + (chip & CS2));
    uPause(IODELAY);
}

/////////////////////////////////////////////////////////////////
// writes data value to one or both sed 1520 selected by chip
//
void
writedata (unsigned int port, int value, int chip)
{
    port_out(port, value);
    port_out(port + 2, A0 + WR + CS1 - (chip & CS1) + (chip & CS2));
    port_out(port + 2, A0 + CS1 - (chip & CS1) + (chip & CS2));
    uPause(IODELAY);
    port_out(port + 2, A0 + WR + CS1 - (chip & CS1) + (chip & CS2));
    uPause(IODELAY);
}

/////////////////////////////////////////////////////////////////
// selects a page (=row) on both sed1520s
//
void
selectpage (unsigned int port, int page)
{
    writecommand(port, 0xB8 + (page & 0x03), CS1 + CS2);
}

/////////////////////////////////////////////////////////////////
// selects a column on the sed1520s specified by chip
//
void
selectcolumn (unsigned int port, int column, int chip)
{
    writecommand(port, (column & 0x7F), chip);
}

/////////////////////////////////////////////////////////////////
// draws  char z from fontmap to the framebuffer at position
// x,y. These are zero-based textmode positions.
// The Fontmap is stored in rows while the framebuffer is stored
// in columns, so we need a little conversion.
//
void
drawchar2fb (unsigned char *framebuf, int x, int y, unsigned char z)
{
    int i, j;

    if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT))
	return;

    for (i = CELLWIDTH; i > 0; i--) {
	  int k = 0;

	  for (j = 0; j < CELLHEIGHT; j++)
		k |= ((fontmap[(int) z][j] >> (i-1)) & 0x01) << j;

	  framebuf[(y * PIXELWIDTH) + (x * CELLWIDTH) + (CELLWIDTH - i)] = k;
      }
}

/////////////////////////////////////////////////////////////////
// This initialises the stuff. We support supplying port as
// a command line argument.
//
MODULE_EXPORT int
sed1520_init (Driver *drvthis)
{
    PrivateData *p;

    /* Allocate and store private data */
    p = (PrivateData *) calloc(1, sizeof(PrivateData));
    if (p == NULL)
	return -1;
    if (drvthis->store_private_ptr(drvthis, p))
	return -1;


    /* Read config file */

    /* What port to use */
    p->port = drvthis->config_get_int(drvthis->name, "Port", 0, DEFAULT_PORT);

    /* End of config file parsing */

    if (timing_init() == -1) {
	report(RPT_ERR, "%s: timing_init() failed (%s)", drvthis->name, strerror(errno));
	return -1;
    }

    // Allocate our framebuffer
    p->framebuf = (unsigned char *) calloc(PIXELWIDTH * HEIGHT, sizeof(unsigned char));
    if (p->framebuf == NULL) {
	report(RPT_ERR, "%s: unable to allocate framebuffer", drvthis->name);
	// sed1520_close ();
	return -1;
      }

    // clear screen
    memset(p->framebuf, '\0', PIXELWIDTH * HEIGHT);

    // Initialize the Port and the sed1520s
    if (port_access(p->port) || port_access(p->port+2)) {
	report(RPT_ERR, "%s: unable to access port 0x%03X", drvthis->name, p->port);
	return -1;
    }

    port_out(p->port,0);
    port_out(p->port +2, WR + CS2);
    writecommand(p->port, 0xE2, CS1 + CS2);
    writecommand(p->port, 0xAF, CS1 + CS2);
    writecommand(p->port, 0xC0, CS1 + CS2);
    selectpage(p->port, 3);

    report(RPT_DEBUG, "%s: init() done", drvthis->name);

    return 1;
}

/////////////////////////////////////////////////////////////////
// Frees the frambuffer and exits the driver.
//
MODULE_EXPORT void
sed1520_close (Driver *drvthis)
{
    PrivateData *p = drvthis->private_data;

    if (p != NULL) {
	if (p->framebuf != NULL)
	    free(p->framebuf);

	free(p);
    }
    drvthis->store_private_ptr(drvthis, NULL);
}

/////////////////////////////////////////////////////////////////
// Returns the display width
//
MODULE_EXPORT int
sed1520_width (Driver *drvthis)
{
    //PrivateData *p = drvthis->private_data;
    return WIDTH;
}

/////////////////////////////////////////////////////////////////
// Returns the display height
//
MODULE_EXPORT int
sed1520_height (Driver *drvthis)
{
    //PrivateData *p = drvthis->private_data;
    return HEIGHT;
}

/////////////////////////////////////////////////////////////////
// Returns the display width
//
MODULE_EXPORT int
sed1520_cellwidth (Driver *drvthis)
{
    //PrivateData *p = drvthis->private_data;
    return CELLWIDTH;
}

/////////////////////////////////////////////////////////////////
// Returns the display height
//
MODULE_EXPORT int
sed1520_cellheight (Driver *drvthis)
{
    //PrivateData *p = drvthis->private_data;
    return CELLHEIGHT;
}

/////////////////////////////////////////////////////////////////
// Clears the LCD screen
//
MODULE_EXPORT void
sed1520_clear (Driver *drvthis)
{
    PrivateData *p = drvthis->private_data;

    memset(p->framebuf, '\0', PIXELWIDTH * HEIGHT);
}

/////////////////////////////////////////////////////////////////
//
// Flushes all output to the lcd...
//
MODULE_EXPORT void
sed1520_flush (Driver *drvthis)
{
    PrivateData *p = drvthis->private_data;
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
	  selectpage(p->port, i);

	  selectcolumn(p->port, 0, CS2) ;
	  for (j = 0; j < PIXELWIDTH/2; j++)
	      writedata(p->port, p->framebuf[j + (i * PIXELWIDTH)], CS2);

	  selectcolumn(p->port, 0, CS1) ;
	  for (j = PIXELWIDTH/2; j < PIXELWIDTH; j++)
	      writedata(p->port, p->framebuf[j + (i * PIXELWIDTH)], CS1);
      }
}

/////////////////////////////////////////////////////////////////
// Prints a string on the lc display, at position (x,y).  The
// upper-left is (1,1), and the lower right should be (20,4).
//
MODULE_EXPORT void
sed1520_string (Driver *drvthis, int x, int y, const char string[])
{
    PrivateData *p = drvthis->private_data;
    int i;

    x--;			// Convert 1-based coords to 0-based
    y--;

    for (i = 0; string[i] != '\0'; i++)
	drawchar2fb(p->framebuf, x + i, y, string[i]);
}

/////////////////////////////////////////////////////////////////
// Writes  char c at position x,y into the framebuffer.
// x and y are 1-based textmode coordinates.
//
MODULE_EXPORT void
sed1520_chr (Driver *drvthis, int x, int y, char c)
{
    PrivateData *p = drvthis->private_data;

    y--;
    x--;
    drawchar2fb(p->framebuf, x, y, c);
}

/////////////////////////////////////////////////////////////////
// This function draws a number num into the last 3 rows of the
// framebuffer at 1-based position x. It should draw a 4-row font,
// but methinks this would look a little stretched. When
// num=10 a colon is drawn.
// FIXME: make big numbers use less memory
//
MODULE_EXPORT void
sed1520_num (Driver *drvthis, int x, int num)
{
    PrivateData *p = drvthis->private_data;
    int z, c, i, s;
    x--;

    // return on illegal char or illegal position
    if ((x >= WIDTH) || (num < 0) || (num > 10))
	return;

    if (num == 10) {		// colon
	for (z = 0; z < 3; z++) {		// Zeilen a 8 Punkte
	    for (c = 0; c < 6; c++) {		// 6 columns
		s = 0;
		for (i = 0; i < 8; i++) {	// 8 bits aus zeilen
		    s >>= 1;
		    if (*(fontbigdp[(z * 8) + i] + c) == '.')
			s |= 0x80;
		}
		if ((x * CELLWIDTH + c >= 0) && (x * CELLWIDTH + c < PIXELWIDTH))
		    p->framebuf[((z + 1) * PIXELWIDTH) + (x * CELLWIDTH) + c] = s;
	    }
	}
    }
    else {			// digits 0 - 9
	for (z = 0; z < 3; z++) {		// Zeilen a 8 Punkte
	    for (c = 0; c < 18; c++) {		// 18 columns
		s = 0;
		for (i = 0; i < 8; i++) {	// 8 bits aus zeilen
		    s >>= 1;
		    if (*(fontbignum[num][z * 8 + i] + c) == '.')
			s |= 0x80;
		}
		if ((x * CELLWIDTH + c >= 0) && (x * CELLWIDTH + c < PIXELWIDTH))
		    p->framebuf[((z + 1) * PIXELWIDTH) + (x * CELLWIDTH) + c] = s;
	    }
	}
    }
}


/////////////////////////////////////////////////////////////////
// Changes the font of character n to a pattern given by *dat.
// HD44780 Controllers only posses 8 programmable chars. But
// we store the fontmap completely in RAM, so every character
// can be altered. !Important: Characters have to be redrawn
// by drawchar2fb() to show their new shape. Because we use
// a non-standard 6x8 font a *dat not calculated from
// width and height will fail.
//
MODULE_EXPORT void
sed1520_set_char (Driver *drvthis, int n, char *dat)
{
    //PrivateData *p = drvthis->private_data;
    int row, col;

    if (n < 0 || n > 255)
	return;
    if (!dat)
	return;

    for (row = 0; row < CELLHEIGHT; row++) {
	int i = 0;

	for (col = 0; col < CELLWIDTH; col++)
	    i = (i << 1) | (dat[(row * CELLWIDTH) + col] > 0);

	fontmap[n][row] = i;
    }
}


/////////////////////////////////////////////////////////////////
// Draws a vertical from the bottom up to the last 3 rows of the
// framebuffer at 1-based position x. len is given in pixels.
//
MODULE_EXPORT void
sed1520_old_vbar (Driver *drvthis, int x, int len)
{
    PrivateData *p = drvthis->private_data;
    int i, j, k;

    x--;

    for (j = 0; j < 3; j++) {
	k = 0;
	for (i = 0; i < CELLHEIGHT; i++) {
	    if (len > i)
		k |= (1 << (CELLHEIGHT-1 - i));
	}

	p->framebuf[((3 - j) * PIXELWIDTH) + (x * CELLWIDTH) + 0] = 0;
	p->framebuf[((3 - j) * PIXELWIDTH) + (x * CELLWIDTH) + 1] = 0;
	p->framebuf[((3 - j) * PIXELWIDTH) + (x * CELLWIDTH) + 2] = k;
	p->framebuf[((3 - j) * PIXELWIDTH) + (x * CELLWIDTH) + 3] = k;
	p->framebuf[((3 - j) * PIXELWIDTH) + (x * CELLWIDTH) + 4] = k;
	p->framebuf[((3 - j) * PIXELWIDTH) + (x * CELLWIDTH) + 5] = 0;
	len -= CELLHEIGHT;
    }
}


/////////////////////////////////////////////////////////////////
// Draws a horizontal bar from left to right at 1-based position
// x,y into the framebuffer. len is given in pixels.
//
MODULE_EXPORT void
sed1520_old_hbar (Driver *drvthis, int x, int y, int len)
{
    PrivateData *p = drvthis->private_data;
    int i;

    x--;
    y--;

    if ((y < 0) || (y >= HEIGHT) || (x < 0) || (len < 0) || ((x + (len / CELLWIDTH)) >= WIDTH))
	return;

    for (i = 0; i < len; i++)
	p->framebuf[(y * PIXELWIDTH) + (x * CELLWIDTH) + i] = 0x3C;  // set low 6 bits
}

/////////////////////////////////////////////////////////////////
// Reprogrammes character dest to contain an icon given by
// which. Calls set_char() to do this.
//
MODULE_EXPORT int
sed1520_icon (Driver *drvthis, int x, int y, int icon)
{
    //PrivateData *p = drvthis->private_data;
    static char heart_open[] = {
	1, 1, 1, 1, 1,
	1, 0, 1, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 1, 0, 1, 1,
	1, 1, 1, 1, 1 };

    static char heart_filled[] = {
	1, 1, 1, 1, 1,
	1, 0, 1, 0, 1,
	0, 1, 0, 1, 0,
	0, 1, 1, 1, 0,
	0, 1, 1, 1, 0,
	1, 0, 1, 0, 1,
	1, 1, 0, 1, 1,
	1, 1, 1, 1, 1 };

    switch (icon) {
	case ICON_BLOCK_FILLED:
	    sed1520_chr(drvthis, x, y, 255);
	    break;
	case ICON_HEART_FILLED:
	    sed1520_set_char(drvthis, 0, heart_filled);
	    sed1520_chr(drvthis, x, y, 0);
	    break;
	case ICON_HEART_OPEN:
	    sed1520_set_char(drvthis, 0, heart_open);
	    sed1520_chr(drvthis, x, y, 0);
	    break;
	default:
	    return -1;
    }
    return 0;
}

