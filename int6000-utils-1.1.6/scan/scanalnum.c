/*====================================================================*
 *
 *   void scanalnum (SCAN * content);
 *
 *   scan.h
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANALNUM_SOURCE
#define SCANALNUM_SOURCE

#include <ctype.h>

#include "../scan/scan.h"

void scanalnum (SCAN * content) 

{
	while ((content->final < content->limit) && isalnum (*content->final)) 
	{
		content->final++;
	}
	return;
}

#endif

