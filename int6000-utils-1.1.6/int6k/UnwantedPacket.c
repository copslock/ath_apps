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
 *   bool UnwantedPacket (const struct message * message, uint16_t MMTYPE);
 *
 *   message.h
 *
 *   inspect an Ethernet frame; return true if the frame appears to
 *   be a valid Intellon Vendor Specif Frame;
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
 *   liability for any consequences of its use. 
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef UNWANTEDPACKET_SOURCE
#define UNWANTEDPACKET_SOURCE

#include <stdint.h>

#include "../int6k/intellon.h"
#include "../int6k/homeplug.h"
#include "../int6k/message.h"
#include "../tools/endian.h"

signed UnwantedPacket (const void * memory, uint16_t MMTYPE) 

{
	struct message * message = (struct message *)(memory);
	if (message->ethernet.MTYPE != htons (HOMEPLUG_MTYPE)) 
	{
		return (-1);
	}
	if (message->intellon.MMV != INTELLON_MMV) 
	{
		return (-1);
	}
	if (message->intellon.MMTYPE != HTOLE16 (MMTYPE)) 
	{
		return (-1);
	}
	if (message->intellon.OUI [0] != 0x00) 
	{
		return (-1);
	}
	if (message->intellon.OUI [1] != 0xB0) 
	{
		return (-1);
	}
	if (message->intellon.OUI [2] != 0x52) 
	{
		return (-1);
	}
	return (0);
}

#endif

