/*====================================================================*
 *
 *   void memswap ( void * memory1,  void * memory2, size_t extent);
 *
 *   memory.h
 *   
 *   exchange the contents of one memory region with that of another; 
 *   return no value; 
 *
 *   one application for this function is to exchange the source and
 *   destination addresses in an Ethernet frame to form a response
 *   message;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *   
 *--------------------------------------------------------------------*/

#ifndef MEMSWAP_SOURCE
#define MEMSWAP_SOURCE

#include "../tools/memory.h"
 
void memswap ( void * memory1,  void * memory2, size_t extent) 

{
	register byte * byte1 = (byte *)(memory1);
	register byte * byte2 = (byte *)(memory2);
	if (memory1 != memory2) while (extent--) 
	{
		byte byte = *byte1;
		*byte1++ = *byte2;
		*byte2++ = byte;
	}
	return;
}

#endif
 

