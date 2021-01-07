/*====================================================================*
 *
 *   size_t dataspec (const char * string,  void * memory, size_t extent);
 *
 *   memory.h
 *
 *   encode a memory region with a variable-length hexadecimal string;
 *   return the number of bytes encoded or terminate the program on 
 *   error; 
 *
 *   the number of octets in string must equal the memory extent or
 *   an error will occur; octets may be seperated by semi-colons; 
 *   empty octets are illegal;
 *
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef DATASPEC_SOURCE
#define DATASPEC_SOURCE

#include <ctype.h>
#include <errno.h>

#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"

size_t dataspec (const char * string,  void * memory, size_t extent) 

{
	const char * number = string;
	byte * origin = (byte *)(memory);
	byte * offset = (byte *)(memory);
	if (!number) 
	{
		error (1, EFAULT, "dataspec");
	}

#ifdef WIN32

	while (isspace (*number)) 
	{
		number++;
	}

#endif

	while ((*number) && (extent)) 
	{
		unsigned digit = 0;

#ifdef WIN32

		if (isspace (*number)) 
		{
			break;
		}

#endif

		if ((offset > origin) && (*number == HEX_EXTENDER)) 
		{
			number++;
		}
		if ((digit = todigit (*number++)) >= RADIX_HEX) 
		{
			error (1, EINVAL, "Have '%s' but want valid hex digit", string);
		}
		*offset = digit << 4;
		if (!*number) 
		{
			error (1, EINVAL, "Have '%s' but want extra hex digit", string);
		}
		if ((digit = todigit (*number++)) >= 0x10) 
		{
			error (1, EINVAL, "Have '%s' but want valid hex data", string);
		}
		*offset |= digit;
		offset++;
		extent--;
	}

#ifdef WIN32

	while (isspace (*number)) 
	{
		number++;
	}

#endif

	if (*number && !extent) 
	{
		error (1, EINVAL, "'%s' exceeds %d bytes", string, (unsigned)(offset - origin - extent));
	}
	if (*number) 
	{
		error (1, EINVAL, "String '%s' contains trash", string);
	}
	return (offset - origin);
}

#endif

