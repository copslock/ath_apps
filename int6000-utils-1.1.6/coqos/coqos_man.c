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
 *   coqos_man.c - Manage MCSMA streams
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
 *	Bill Wike <bill.wike@intellon.com>
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../int6k/int6k.h"
#include "../coqos/coqos.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/typename.c"
#include "../tools/error.c"
#include "../tools/synonym.c"
#endif

#ifndef MAKEFILE
#include "../int6k/Request.c"
#include "../int6k/Confirm.c"
#include "../int6k/Failure.c"
#include "../int6k/Display.c"
#include "../int6k/ReadMME.c"
#include "../int6k/SendMME.c"
#include "../int6k/devices.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#include "../ether/channel.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

/*====================================================================*
 *
 *   signed con_man (struct int6k * int6k, uint16_t TOT_BW_USED);
 *
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Bill Wike <bill.wike@intellon.com>
 *
 *--------------------------------------------------------------------*/

signed con_man (struct int6k * int6k, uint16_t TOT_BW_USED) 

{

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_con_info_req 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t REQ_TYPE;
		uint32_t RSVD;
		uint16_t CSPEC_VER;
		uint16_t CID;
		uint16_t TOT_BW_USED;
	}
	* request = (struct vs_con_info_req *)(int6k->message);
	struct __packed vs_con_info_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t REQ_TYPE;
		uint8_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RSVD;
		uint8_t NUM_CONN;
		struct conn_info CONN_INFO [64];
	}
	* confirm = (struct vs_con_info_cnf *)(int6k->message);
	struct conn_info * conn_info =(struct conn_info*)(confirm->CONN_INFO);
	struct vs_con_info_cnf *indicate = (struct vs_con_info_cnf *)(int6k->message);
	struct __packed vs_mod_conn_req 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t REQ_ID;
		uint32_t RSVD;
		uint16_t CID;
		uint8_t MOD_CTRL;
	}
	* mc_request = (struct vs_mod_conn_req *)(int6k->message);
	struct __packed vs_mod_conn_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t REQ_ID;
		uint8_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RSVD;
	}
	* mc_confirm = (struct vs_mod_conn_cnf *)(int6k->message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	int i;
	int bwused;
	int lowest;
	uint16_t lowcid;
	while (1) 
	{
		Request (int6k, "COQOS connection information (requesting bandwidth notification)");
		memset (int6k->message, 0, sizeof (struct message));
		EthernetHeader (&int6k->message->ethernet, int6k->channel->peer, int6k->channel->host);
		IntellonHeader (&int6k->message->intellon, (VS_CON_INFO | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		request->REQ_TYPE = 0x04;
		request->CID = 0x00;
		request->CSPEC_VER = 0x01;
		request->TOT_BW_USED = TOT_BW_USED;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
			return (-1);
		}
		while (ReadMME (int6k, (VS_CON_INFO | MMTYPE_CNF)) <= 0);
		if (confirm->MSTATUS) 
		{
			Failure (int6k, "Could not set up bandwidth notification.");
		}
		else 
		{
			Confirm (int6k, "Set up bandwidth monitoring.");
			break;
		}
		sleep (1);
	}
	while (1) 
	{

/*
 * Wait for indications saying too much bandwidth is
 * in use.
 */

		while (ReadMME (int6k, (VS_CON_INFO | MMTYPE_IND)) <= 0);
		if (indicate->MSTATUS) 
		{
			Failure (int6k, "Connection Info Indicate error");
			continue;
		}
		printf ("Number of Connections: %d\n", indicate->NUM_CONN);
		conn_info = (struct conn_info *)(confirm->CONN_INFO);
		bwused = 0;
		lowest = conn_info->CONN_COQOS_PRIO;
		lowcid = conn_info->CID;
		for (i = 0; i < indicate->NUM_CONN; i++) 
		{
			bwused += conn_info->BW_USED;
			if (conn_info->CONN_COQOS_PRIO < lowest) 
			{
				lowest = conn_info->CONN_COQOS_PRIO;
				lowcid = conn_info->CID;
			}
			conn_info++;
		}
		printf ("Total bandwidth used: %d\n", bwused);
		if (bwused <= TOT_BW_USED) 
		{
			continue;
		}
		printf ("Bandwidth limit exceeded dropping: %04X\n", lowcid);
		Request (int6k, "COQOS Modify Connection (suspend)");
		memset (int6k->message, 0, sizeof (struct message));
		EthernetHeader (&int6k->message->ethernet, int6k->channel->peer, int6k->channel->host);
		IntellonHeader (&int6k->message->intellon, (VS_MOD_CONN | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		mc_request->MOD_CTRL = 0x00;
		mc_request->CID = lowcid;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
			return (-1);
		}
		while (ReadMME (int6k, (VS_MOD_CONN | MMTYPE_CNF)) <= 0);
		if (mc_confirm->MSTATUS) 
		{
			Failure (int6k, "Could not suspend stream.");
			continue;
		}
		printf ("Connection ID: %04X suspended\n", lowcid);
		sleep (5);

/* flush out old data */

		while (ReadMME (int6k, (VS_CON_INFO | MMTYPE_IND)) > 0);
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv[]);
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"ei:qv",
		"limit [device] [...]",
		"CoQos Stream Utility",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"q\tquiet - suppress routine messages",
		"v\tverbose messages",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	uint16_t limit;
	signed c;
	if (getenv (PLCDEVICE)) 
	{
#if defined (WINPCAP) || defined (LIBPCAP)
		channel.id = atoi (getenv (PLCDEVICE));
#else
		channel.name = strdup (getenv (PLCDEVICE));
#endif
	}
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;

#ifndef WIN32

/*
 *      raw packets require root priviledges on linux; one does not have to be
 *      root when this program is installed setuid using 'chown root:root' and 
 *      'chmod 4555';
 */

	if (geteuid ()) 
	{
		error (1, EPERM, INT6K_NEEDROOT);
	}

#endif

	if (!argc) 
	{
		error (1, ECANCELED, "No limit given");
	}
	limit = (uint16_t)(uintspec (*argv++, 10, 90000));
	argc--;
	openchannel (&channel);
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}
	if (!argc) 
	{
		con_man (&int6k, limit);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		con_man (&int6k, limit);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

