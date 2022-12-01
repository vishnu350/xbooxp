/*
	This file is part of xbooxp. It makes use of io.dll, which is free, and
	available at http://www.geekhideout.com/iodll.shtml

    xbooxp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xbooxp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xbooxp.  If not, see <http://www.gnu.org/licenses/>.
*/
#if defined(_WIN32) || defined(_WIN64)
#include "windows.h"

typedef void (WINAPI *PORTOUT) (short int Port, char Data);
typedef void (WINAPI *PORTWORDOUT)(short int Port, short int Data);
typedef void (WINAPI *PORTDWORDOUT)(short int Port, int Data);
typedef char (WINAPI *PORTIN) (short int Port);
typedef short int (WINAPI *PORTWORDIN)(short int Port);
typedef int (WINAPI *PORTDWORDIN)(short int Port);
typedef void (WINAPI *SETPORTBIT)(short int Port, char Bit);
typedef void (WINAPI *CLRPORTBIT)(short int Port, char Bit);
typedef void (WINAPI *NOTPORTBIT)(short int Port, char Bit);
typedef short int (WINAPI *GETPORTBIT)(short int Port, char Bit);
typedef short int (WINAPI *RIGHTPORTSHIFT)(short int Port, short int Val);
typedef short int (WINAPI *LEFTPORTSHIFT)(short int Port, short int Val);
typedef short int (WINAPI *ISDRIVERINSTALLED)();

extern PORTOUT PortOut;
extern PORTWORDOUT PortWordOut;
extern PORTDWORDOUT PortDWordOut;
extern PORTIN PortIn;
extern PORTWORDIN PortWordIn;
extern PORTDWORDIN PortDWordIn;
extern SETPORTBIT SetPortBit;
extern CLRPORTBIT ClrPortBit;
extern NOTPORTBIT NotPortBit;
extern GETPORTBIT GetPortBit;
extern RIGHTPORTSHIFT RightPortShift;
extern LEFTPORTSHIFT LeftPortShift;
extern ISDRIVERINSTALLED IsDriverInstalled;

extern int LoadIODLL();

PORTOUT PortOut;
PORTWORDOUT PortWordOut;
PORTDWORDOUT PortDWordOut;
PORTIN PortIn;
PORTWORDIN PortWordIn;
PORTDWORDIN PortDWordIn;
SETPORTBIT SetPortBit;
CLRPORTBIT ClrPortBit;
NOTPORTBIT NotPortBit;
GETPORTBIT GetPortBit;
RIGHTPORTSHIFT RightPortShift;
LEFTPORTSHIFT LeftPortShift;
ISDRIVERINSTALLED IsDriverInstalled;

HMODULE hio;

void UnloadIODLL() {
	FreeLibrary(hio);
}

int LoadIODLL() {
	hio = LoadLibrary("io");
	if (hio == NULL) return 1;

	PortOut = (PORTOUT)GetProcAddress(hio, "PortOut");
	PortWordOut = (PORTWORDOUT)GetProcAddress(hio, "PortWordOut");
	PortDWordOut = (PORTDWORDOUT)GetProcAddress(hio, "PortDWordOut");
	PortIn = (PORTIN)GetProcAddress(hio, "PortIn");
	PortWordIn = (PORTWORDIN)GetProcAddress(hio, "PortWordIn");
	PortDWordIn = (PORTDWORDIN)GetProcAddress(hio, "PortDWordIn");
	SetPortBit = (SETPORTBIT)GetProcAddress(hio, "SetPortBit");
	ClrPortBit = (CLRPORTBIT)GetProcAddress(hio, "ClrPortBit");
	NotPortBit = (NOTPORTBIT)GetProcAddress(hio, "NotPortBit");
	GetPortBit = (GETPORTBIT)GetProcAddress(hio, "GetPortBit");
	RightPortShift = (RIGHTPORTSHIFT)GetProcAddress(hio, "RightPortShift");
	LeftPortShift = (LEFTPORTSHIFT)GetProcAddress(hio, "LeftPortShift");
	IsDriverInstalled = (ISDRIVERINSTALLED)GetProcAddress(hio, "IsDriverInstalled");

	atexit(UnloadIODLL);

	return 0;
}

#elif defined(__linux__)
#include <stdlib.h>
#include <sys/io.h>

/* ioport(port)         permit access to an I/O port.
 * port                 I/O address to enable
 */
void
ioport(int port)
{
    if (ioperm(port, 1, 1)) {perror("ioperm"); exit(1);}
}

void PortInit(short int Port)
{
  printf ("PortInit: %04X\n",Port & 0xFFFF);

  if ((Port & 0xFFFF) < 0x400)
  {
    printf ("using ioperm\n");
    ioport(Port);
    ioport(Port+1);
    ioport(Port+2);
  }
  else
  {
    printf ("using iopl\n");
    if (iopl(3)) exit(1); /* needs root priviledges */
  }
}

void PortOut(short int Port, char Data)
{
  outb(Data, Port);
}

char PortIn(short int Port)
{
  return inb(Port);
}

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <sys/syscall.h>
#include <machine/sysarch.h>
#include <machine/pio.h>

/* ioport(port)         permit access to an I/O port.
 * port                 I/O address to enable
 */
void
ioport(int port)
{
  u_long iomap[32];
  struct i386_set_ioperm_args ioperm;

  ioperm.iomap = iomap;
  syscall(SYS_sysarch, I386_GET_IOPERM, (char *) &ioperm);
  iomap[port >> 5] &= ~(1 << (port & 0x1f));
  syscall(SYS_sysarch, I386_SET_IOPERM, (char *) &ioperm);
}

void PortInit(short int Port)
{
  ioport(Port);
  ioport(Port+1);
  ioport(Port+2);
}

void PortOut(short int Port, char Data)
{
  outb(Port, Data);
}

char PortIn(short int Port)
{
  return inb(Port);
}

#elif defined(__OpenBSD__)

#include <sys/types.h>
#include <machine/sysarch.h>
#include <machine/pio.h>

/*
 * Modifies the I/O permission bitmap for the current process, so that the
 * process could read/write the I/O port 'port'.
 */
void
ioport(int port)
{
  u_long iomap[32];

  if (i386_get_ioperm(iomap) != 0)
    fprintf(stderr,"i386_get_permio: %d",errno);

  iomap[((unsigned short) port) / (4 * 8)] &= ~(((u_long) 1) << (((unsigned short) port) % (4 * 8)));

  if (i386_set_ioperm(iomap) != 0)
    fprintf(stderr,"i386_set_permio: %d",errno);
}

void PortInit(short int Port)
{
  ioport(Port);
  ioport(Port+1);
  ioport(Port+2);
}

void PortOut(short int Port, char Data)
{
  outb(Port, Data);
}

char PortIn(short int Port)
{
  return inb(Port);
}

#elif defined(__FreeBSD__)
#include <sys/types.h>
#include <machine/cpufunc.h>
#include <machine/sysarch.h>

#if (__FreeBSD_version > 500000)
static FILE * port_access_handle = NULL;
#endif

void
ioport(int port)
{
/* Get access to a specific port */
#if (__FreeBSD_version > 500000)
  if( port_access_handle
    || (port_access_handle = fopen("/dev/io", "rw")) != NULL )
  {
    if (i386_set_ioperm(port, 1, 1) != 0)
    {
      fprintf(stderr,"i386_set_permio: %d",errno);
    }
  }
  else
  {
    fprintf(stderr,"fopen /dev/io: %d",errno);
  };
#else
  if (i386_set_ioperm(port, 1, 1) != 0)
  {
    fprintf(stderr,"i386_set_permio: %d",errno);
  }
#endif
}

void PortInit(short int Port)
{
  ioport(Port);
  ioport(Port+1);
  ioport(Port+2);
}

void PortOut(short int Port, char Data)
{
  outb(Port, Data);
}

char PortIn(short int Port)
{
  return inb(Port);
}

#endif
