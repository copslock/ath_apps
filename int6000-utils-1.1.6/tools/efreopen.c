/*====================================================================*
 *
 *   FILE *efreopen(const char *filename, const char *openmode, FILE *fp);
 *
 *   files.h
 *
 *   attempt to reopen a file using freopen(); return a valid file
 *   pointer on success; print an error message to stderr and then
 *   return NULL on failure;
 *
 *.  Motley Tools by Charlie Maier; cmaier@cmassoc.net;
 *:  modified sun 07 sep 03 by user root on host zeus;
 *;  Licensed under the GNU General Public License verion 2 or later;
 *
 *--------------------------------------------------------------------*/

#ifndef EFREOPEN_SOURCE
#define EFREOPEN_SOURCE

#include <stdio.h>
#include <errno.h>

#include "../tools/files.h"
#include "../tools/error.h"

FILE *efreopen (const char *filename, const char *openmode, FILE *fp) 

{
	if ((fp = freopen (filename, openmode, fp)) == (FILE *)(0)) 
	{
		error (0, errno, "%s", filename);
	}
	return (fp);
}

#endif

