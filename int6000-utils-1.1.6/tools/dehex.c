/*====================================================================*
 *
 *   size_t dehex (char const buffer [], size_t length, void * memory, size_t extent);
 *
 *   memory.h
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef DEHEX_SOURCE
#define DEHEX_SOURCE

#include <stdint.h>

#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/types.h"

size_t dehex (char const buffer [], size_t length, void * memory, size_t extent) 

{
	register char * string = (char *)(buffer);
	register byte * offset = (byte *)(memory);
	length >>= 1;
	while ((length--) && (extent--)) 
	{
		*offset |= (todigit (*string++) & 0x0F) << 4;
		*offset |= (todigit (*string++) & 0x0F) << 0;
		offset++;
	}
	return (string - buffer);
}

#endif

