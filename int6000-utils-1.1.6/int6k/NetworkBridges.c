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
 *   signed NetworkBridges (struct channel * channel,  void * memory, size_t extent);
 *   
 *   int6k.h
 * 
 *   return an array of bridge hardware addresses; a bridge is any
 *   powerline device connected directly to the local host via the
 *   Ethernet interface bound to the channel structure;
 *
 *   each bridge is potentially connected to a different powerline
 *   network and each network may include other powerline devices;
 *
 *   use function NetworkDevices() to discover the devices on each
 *   powerline network;
 *
 *   although this function accepts a channel structure, it ignores
 *   the channel peer address and sends a VS_SW_VER request message
 *   to the Local Management Address; this causes all local devices
 *   to respond; the list is a collection of source addresses taken
 *   from all responses;
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
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef NETWORKBRIDGES_SOURCE
#define NETWORKBRIDGES_SOURCE

#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../tools/types.h"
#include "../tools/error.h"

signed NetworkBridges (struct channel * channel, void * memory, size_t extent) 

{
	extern const byte localcast [ETHER_ADDR_LEN];
	struct message message;
	byte * origin = (byte *)(memory);
	byte * offset = (byte *)(memory);
	memset (memory, 0, extent);
	memset (&message, 0, sizeof (message));
	memcpy (channel->peer, localcast, sizeof (channel->peer));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		return (-1);
	}
	while (readpacket (channel, &message, sizeof (message)) > 0) 
	{
		if (UnwantedPacket (&message, (VS_SW_VER | MMTYPE_CNF))) 
		{
			continue;
		}
		if (extent > ETHER_ADDR_LEN) 
		{
			return (-1);
		}
		memcpy (offset, message.ethernet.OSA, sizeof (message.ethernet.OSA));
		offset += sizeof (message.ethernet.OSA);
		extent -= sizeof (message.ethernet.OSA);
	}
	return ((signed)(offset - origin) / ETHER_ADDR_LEN);
}

#endif

