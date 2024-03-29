/* $id$
 * Low level I/O functions taken from led-stat.txt
 * Jan 22 95 copyright damianf@wpi.edu
 *
 * DOS part (tested only with DOS version of GCC, DJGPP)
 * by M.Prinke <m.prinke@trashcan.mcnet.de> 3/97
 *
 * FreeBSD support by Guillaume Filion and Philip Pokorny, copyright 05/2001
 *
 * NetBSD & OpenBSD port by Guillaume Filion, copyright 12/2001 and 02/2002
 *
 * (Better) FreeBSD port by Guillaume Filion, copyright 05/2002
 *
 * Improved support for old Linux (glibc1 and libc5) by Guillaume Filion, 12/2002
 *
 * Access to ports over 0x3FF by Joris Robijn, 04/2003
 *
 */

/*
This file defines 6 static inline functions for port I/O:

static inline int port_in (unsigned short port);
	Read a byte from port
	Returns the content of the byte.

static inline void port_out (unsigned short port, unsigned char val);
	Write a char(byte) 'val' to port.
	Returns nothing.

static inline int port_access (unsigned short port);
	Get access to a specific port
	Returns 0 if successful, -1 if failed

static inline int port_deny (unsigned short port);
	Close access to a specific port
	Returns 0 if successful, -1 if failed

static inline int port_access_multiple (unsigned short port, unsigned short count)
	Get access multiple to ports at once.
	Returns 0 if successful, -1 if failed

static inline int port_deny_multiple (unsigned short port, unsigned short count)
	Close access to multiple ports at once.
	Returns 0 if successful, -1 if failed

If you make modifications to this file: References to the LPT port should
not be in this file but in lpt-port.h ...
*/

#ifndef PORT_H
#define PORT_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

/*  ------------------------------------------------------------- */
/*  Use ioperm, inb and outb in <sys/io.h> (Linux) */
/*  And iopl for higher addresses of PCI LPT cards */
#if defined HAVE_IOPERM

/* Glibc2 and Glibc1 */
# ifdef HAVE_SYS_IO_H 
#  include <sys/io.h>
# endif

/* Libc5 */
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif

/*  Read a byte from port */
static inline int port_in (unsigned short port) {
	return inb(port);
}

/*  Write a byte 'val' to port */
static inline void port_out (unsigned short port, unsigned char val) {
	outb(val, port);
}

/*  Get access to a specific port */
static inline int port_access (unsigned short port) {
	if (port <= 0x3FF) {
		return ioperm(port, 1, 255);
	} else {
#ifdef HAVE_IOPL
		/* Is there a better way to do this ? */
		static short int iopl_done = 0;
		if (iopl_done) return 0;
		iopl_done = 1;
		return iopl(3);
#else
		return -1; /* Error, can't access the requested port */
#endif
	}
}

/*  Close access to a specific port */
static inline int port_deny (unsigned short port) {
	if (port <= 0x3FF) {
		return ioperm(port, 1, 0);
	}
	/* We can't simply close access acquired with iopl */
	return 0;
}

/*  Get access to multiple ports at once */
static inline int port_access_multiple (unsigned short port, int count) {
	if (port+count-1 <= 0x3FF) {
		return ioperm(port, count, 255);
	} else {
#ifdef HAVE_IOPL
		return port_access(port+count);
		/* to use the iopl part there... */
#else
		return -1;
#endif
	}
	return 0;
}

/*  Close access to multiple ports at once */
static inline int port_deny_multiple (unsigned short port, int count) {
	if (port+count-1 <= 0x3FF) {
		return ioperm(port, count, 0);
	}
	/* We can't simply close access acquired with iopl */
	return 0;
}

/*  ------------------------------------------------------------- */
/*  Use i386_get_ioperm, i386_set_ioperm, inb and outb from <machine/pio.h> (NetBSD&OpenBSD) */
#elif defined HAVE_I386_IOPERM_NETBSD && defined HAVE_MACHINE_PIO_H && defined HAVE_MACHINE_SYSARCH_H && defined HAVE_SYS_TYPES_H
#include <sys/types.h>
#include <machine/pio.h>
#include <machine/sysarch.h>

/*  Read a byte from port */
static inline int port_in (unsigned short port) {
	return inb(port);
}

/*  Write a byte 'val' to port */
static inline void port_out (unsigned short port, unsigned char val) {
	outb(port, val);
}

static inline void setaccess(u_long * map, u_int bit, int allow) {
	u_int           word;
	u_int           shift;
	u_long          mask;

	word = bit / 32;
	shift = bit - (word * 32);

	mask = 0x000000001 << shift;
	if (allow)
		map[word] &= ~mask;
	else
		map[word] |= mask;
}

/*  Get access to a specific port */
static inline int port_access (unsigned short port) {
	u_long          iomap[32];

	if (i386_get_ioperm(iomap) == -1) return -1;

	setaccess(iomap, port  , 1);

	if (i386_set_ioperm(iomap) == -1) return -1;

	return 0;
}

/*  Close access to a specific port */
static inline int port_deny (unsigned short port) {
	u_long          iomap[32];

	if (i386_get_ioperm(iomap) == -1) return -1;

	setaccess(iomap, port, 0);

	if (i386_set_ioperm(iomap) == -1) return -1;

	return 0;
}

/*  Get access to multiple ports at once */
static inline int port_access_multiple (unsigned short port, unsigned short count) {
	u_long          iomap[32];
	unsigned short  i;

	if (i386_get_ioperm(iomap) == -1) return -1;

	for (i=0; i<count; i++) {
		setaccess(iomap, port + i, 1);
	}

	if (i386_set_ioperm(iomap) == -1) return -1;

	return 0;
}

/*  Close access to multiple ports at once */
static inline int port_deny_multiple (unsigned short port, unsigned short count) {
	u_long          iomap[32];
	unsigned short  i;

	if (i386_get_ioperm(iomap) == -1) return -1;

	for (i=0; i<count; i++) {
		setaccess(iomap, port + i, 0);
	}

	if (i386_set_ioperm(iomap) == -1) return -1;

	return 0;
}

/*#endif // defined HAVE_I386_IOPERM_NETBSD && defined HAVE_MACHINE_PIO_H && defined HAVE_MACHINE_SYSARCH_H
-------------------------------------------------------------
Use i386_get_ioperm, i386_set_ioperm from <machine/sysarch.h> and inb and outb from <machine/cpufunc.h> (FreeBSD) */
#elif defined HAVE_I386_IOPERM_FREEBSD && defined HAVE_MACHINE_CPUFUNC_H && defined HAVE_MACHINE_SYSARCH_H && defined HAVE_SYS_TYPES_H
#include <sys/types.h>
#include <sys/param.h>
#include <machine/cpufunc.h>
#include <machine/sysarch.h>

#if (__FreeBSD_version > 500000)
static FILE * port_access_handle = NULL;
#endif

/* Read a byte from port */
static inline int port_in (unsigned short port) {
        return inb(port);
}

/* Write a byte 'val' to port */
static inline void port_out (unsigned short port, unsigned char val) {
        outb(port,val);
}

/* Get access to a specific port */
static inline int port_access (unsigned short port) {
#if (__FreeBSD_version > 500000)
	if( port_access_handle
	    || (port_access_handle = fopen("/dev/io", "rw")) != NULL ) {
        	return i386_set_ioperm(port, 1, 1);
	} else {
		return( -1 );  /*  Failure */
	};
#else
        return i386_set_ioperm(port, 1, 1);
#endif
}

/* Get access to multiple ports at once */
static inline int port_access_multiple (unsigned short port, unsigned short count) {
#if (__FreeBSD_version > 500000)
	if( port_access_handle
	    || (port_access_handle = fopen("/dev/io", "rw")) != NULL ) {
        	return i386_set_ioperm(port, count, 1);
	} else {
		return( -1 );  /*  Failure */
	};
#else
        return i386_set_ioperm(port, count, 1);
#endif
}

/* Close access to a specific port */
static inline int port_deny (unsigned short port) {
        return i386_set_ioperm(port, 1, 0);
}

/* Close access to multiple ports at once */
static inline int port_deny_multiple (unsigned short port, unsigned short count) {
        return i386_set_ioperm(port, count, 0);
}

#else
/*  ------------------------------------------------------------- */
/*  Last chance! Use /dev/io and i386 ASM code (BSD4.3 ?) */

/*  Read a byte from port */
static inline int port_in (unsigned short port) {
	unsigned char value;
	__asm__ volatile ("inb %1,%0":"=a" (value)
							:"d" ((unsigned short) port));
	return value;
}

/*  Write a byte 'val' to port */
static inline void port_out (unsigned short port, unsigned char val) {
	__asm__ volatile ("outb %0,%1\n"::"a" (val), "d" (port)
		 );
}

/*  Get access to a specific port */
static inline int port_access (unsigned short port) {
	static FILE *  port_access_handle = NULL ;

	if( port_access_handle
	    || (port_access_handle = fopen("/dev/io", "rw")) != NULL ) {
		return( 0 );  /*  Success */
	} else {
		return( -1 );  /*  Failure */
	};

	return -1;
}

/*  Close access to a specific port */
static inline int port_deny (unsigned short port) {
	/* Can't close /dev/io... */
	return 0;
}

/*  Get access to multiple ports at once */
static inline int port_access_multiple (unsigned short port, unsigned short count) {
	return port_access (port); /* /dev/io gives you access to all ports. */
}

/*  Close access to multiple ports at once */
static inline int port_deny_multiple (unsigned short port, unsigned short count) {
	/*  Can't close /dev/io... */
	return 0;
}

#endif

/*
#else
#include <pc.h>

static inline int
port_in (int port)
{
	unsigned char value;
	value = inportb ((unsigned short) port);
	return (int) value;
}

static inline void
port_out (unsigned int port, unsigned char val)
{
	outportb ((unsigned short) port, val);
}
#endif
*/

#endif /*  PORT_H */
