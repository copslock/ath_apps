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
 *   void sdramtext(const struct config_ram * config_ram);
 *
 *   sdram.h
 *   
 *   print SDRAM configuration on stdout as an ASCII string; this
 *   format is suitable input for Intellon Windows Device Manager;
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef SDRAMTEXT_SOURCE
#define SDRAMTEXT_SOURCE

#include <stdio.h>
#include <unistd.h>

#include "../ram/sdram.h"
#include "../tools/memory.h"

void sdramtext (struct config_ram * config_ram) 

{
	printf ("%08X", htonl (config_ram->SDRAMSIZE));
	printf ("%08X", htonl (config_ram->SDRAMCONF));
	printf ("%08X", htonl (config_ram->SDRAMTIM0));
	printf ("%08X", htonl (config_ram->SDRAMTIM1));
	printf ("%08X", htonl (config_ram->SDRAMCNTRL));
	printf ("%08X", htonl (config_ram->SDRAMREF));
	printf ("%08X", htonl (config_ram->MACCLOCK));
	printf ("%08X", htonl (config_ram->SDRAMRSVD));
	return;
}

#endif

