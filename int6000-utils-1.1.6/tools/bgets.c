/*====================================================================*
 *   
 *   Copyright (c) 2010, Atheros Communications Inc.
 *   
 *   Permission to use, copy, modify, and/or distribute this software 
 *   for any purpose with or without fee is hereby granted, provided 
 *   that the above copyright notice and this permission notice appear 
 *   in all copies.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL  
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *   
 *--------------------------------------------------------------------*/

/*====================================================================*
 *
 *   size_t bgets(uint8_t *bseg[], size_t bsegnum, buffer_t *b);
 *
 *   buffer.h 
 *
 *   This function provides buffer segment based on segment decstring.
 *  
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Intellon customers may modify and distribute the software 
 *   without obligation to Intellon. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *.  Intellon HomePlug AV Application Programming Interface;
 *:  Released 2007 by Intellon Corporation, Ocala FL USA;
 *;  For demontration and evaluation only; Not for production use;
 *
 *   Contributor(s): 
 *	A. Vasquez, alex.vasquez@intellon.com
 *
 *--------------------------------------------------------------------*/

#ifndef BGETS_SOURCE
#define BGETS_SOURCE

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "../tools/buffer.h"

size_t bgets (uint8_t *bseg [], size_t bsegnum, buffer_t *b) 

{
	size_t size;
	uint8_t *btemp;
	if (bseg == 0 || b == 0) 
	{
		errno = EINVAL;
		return 0;
	}
	*bseg = b->bstart + bsegnum * b->bseglen;

/* Check overflow */

	if ((*bseg - b->bstart + 1) > (signed)(b->blen)) 
	{
		errno = ERANGE;
		*bseg = b->bcurr = b->bend;
		return 0;
	}

/* Calculate size */

	b->bcurr = *bseg;
	b->bcurrsegnum = bsegnum;
	btemp = *bseg + b->bseglen;
	if (btemp > b->bend) 
	{
		size = b->bend - b->bcurr;
	}
	else 
	{
		size = b->bseglen;
	}
	return size;
}

#endif

