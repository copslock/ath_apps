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
 *   signed WaitForAssoc (struct int6k * int6k);
 *
 *   int6k.h
 *
 *   poll the device using VS_NW_INFO messages and examine confirmation
 *   messages until the device reports that a network exists and has at
 *   least one station present on that network; return 0 if the device
 *   associates within the allotted time or -1 if it does not or if 
 *   transmission errors occur;
 *
 *   retry is number of times to poll the device before returing an 
 *   error to indicate that the device di not associate in the allotted
 *   time; timer is the time between poll attempts;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual
 *   for more information;
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

#ifndef WAITFORASSOC_SOURCE
#define WAITFORASSOC_SOURCE

#include <unistd.h>
#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/memory.h"

signed WaitForAssoc (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed station 
	{
		uint8_t DA [ETHER_ADDR_LEN];
		uint8_t TEI;
		uint8_t BDA [ETHER_ADDR_LEN];
		uint8_t AVGTX;
		uint8_t AVGRX;
	}
	* station;
	struct __packed network 
	{
		uint8_t NID [HPAVKEY_NID_LEN];
		uint8_t SNID;
		uint8_t TEI;
		uint8_t ROLE;
		uint8_t CCO_MACADDR [6];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station station [1];
	}
	* network;
	struct __packed networks 
	{
		uint8_t NUMAVLNS;
		struct network network [1];
	}
	* networks = (struct networks *) (&message->content);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	static byte broadcast [ETHER_ADDR_LEN] = 
	{
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF 
	};
	unsigned retry = int6k->retry;
	signed networkcount;
	signed stationcount;
	Request (int6k, "Wait for Assoc");
	while (retry--) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_NW_INFO | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		while (ReadMME (int6k, (VS_NW_INFO | MMTYPE_CNF)) > 0) 
		{
			network = (struct network *)(&networks->network);
			networkcount = networks->NUMAVLNS;
			while (networkcount-- > 0) 
			{
				station = (struct station *)(&network->station);
				stationcount = network->NUMSTAS;
				while (stationcount-- > 0) 
				{
					if (memcmp (station->DA, broadcast, sizeof (broadcast))) 
					{
						return (0);
					}
					station++;
				}
				network = (struct network *)(station);
			}
		}
		if (retry) 
		{
			SLEEP (int6k->timer);
		}
	}
	return (-1);
}

#endif

