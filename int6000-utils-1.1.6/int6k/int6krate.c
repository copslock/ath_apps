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
 *   int6krate.c - Intellon INT6x00 Phy Rate Monitor
 *
 *   this program sends and receives raw ethernet frames and so needs
 *   root priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   things right;
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
 *      Abdel Younes <younes@leacom.fr>
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
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../int6k/int6k.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k/Confirm.c"
#include "../int6k/Display.c"
#include "../int6k/chipset.c"
#include "../int6k/devices.c"
#include "../int6k/Display.c"
#include "../int6k/Failure.c"
#include "../int6k/NetworkInfo.c"
#include "../int6k/NetworkTraffic.c"
#include "../int6k/Traffic.c"
#include "../int6k/PHYRates.c"
#include "../int6k/ResetDevice1.c"
#include "../int6k/Request.c"
#include "../int6k/ReadMME.c"
#include "../int6k/SendMME.c"
#include "../int6k/VersionInfo.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/ProtocolHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/checkfilename.c"
#include "../tools/checksum32.c"
#include "../tools/error.c"
#include "../tools/fdchecksum32.c"
#include "../tools/hexstring.c"
#include "../tools/alert.c"
#include "../tools/synonym.c"
#include "../tools/synonym.c"
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
 *   void manager (struct int6k *int6k, signed loop, signed wait);
 *   
 *   perform operations in logical order despite any order specfied 
 *   on the command line; for example, read PIB before writing PIB; 
 *
 *   operation order is controlled by the order of "if" statements 
 *   shown here; the entire operation sequence can be repeated with
 *   an optional pause between each iteration;
 * 
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *--------------------------------------------------------------------*/

void manager (struct int6k *int6k, signed loop, signed wait) 

{
	signed pass = loop;
	while (pass-- > 0) 
	{
		if (_anyset (int6k->flags, INT6K_VERSION)) 
		{
			VersionInfo (int6k);
		}
		if (_anyset (int6k->flags, INT6K_MINIMUM_TRAFFIC)) 
		{
			Traffic (int6k->channel, int6k->timeperiod, 50);
		}
		if (_anyset (int6k->flags, INT6K_NETWORK_TRAFFIC)) 
		{
			NetworkTraffic (int6k);
		}
		if (_anyset (int6k->flags, INT6K_NETWORK)) 
		{
			PHYRates (int6k);
		}
		if (_anyset (int6k->flags, INT6K_RESET_DEVICE)) 
		{
			ResetDevice (int6k);
		}
		if ((!pass) || (!wait)) 
		{
			continue;
		}
		if (_anyset (int6k->flags, INT6K_VERBOSE)) 
		{
			alert ("pause %d seconds %d of %d times", wait, loop - pass, loop);
		}
		fflush (stdout);
		sleep (wait);
	}
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv[]);
 *   
 *   parse command line, populate int6k structure and perform selected 
 *   operations; show help summary if asked; see getoptv and putoptv
 *   to understand command line parsing and help summary display; see
 *   int6k.h for the definition of struct int6k; 
 *
 *   the command line accepts multiple MAC addresses and the program 
 *   performs the specified operations on each address, in turn; the
 *   address order is significant but the option order is not; the
 *   default address is a local broadcast that causes all devices on
 *   the local H1 interface to respond but not those at the remote
 *   end of the powerline;
 *
 *   the default address is 00:B0:52:00:00:01; omitting the address
 *   will automatically address the local device; some options will
 *   cancel themselves if this makes no sense;
 *
 *   the default interface is eth1 because most people use eth0 as 
 *   their principle network connection; you can specify another 
 *   interface with -i or define environment string PLC to make
 *   that the default interface and save typing;
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
		"d:ei:l:nqrRtTvw:x",
		"device [device] [...]",
		"Intellon INT6x00 Device Manager for PHY Rates",
		"d n\ttraffic duration is (n) seconds per leg [3]",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"n\tnetwork TX/RX information",
		"l n\tloop n times",
		"q\tquiet mode",
		"r\trequest device information",
		"R\treset device with VS_RS_DEV",
		"t\tgenerate network traffic",
		"T\tgenerate network traffic",
		"v\tverbose mode",
		"w n\twait n seconds",
		"x\texit on error",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	signed loop = 1;
	signed wait = 0;
	signed c;
	optind = 1;
	opterr = 1;
	if (getenv (PLCDEVICE)) 
	{
#if defined (WINPCAP) || defined (LIBPCAP)
		channel.id = atoi (getenv (PLCDEVICE));
#else
		channel.name = strdup (getenv (PLCDEVICE));
#endif
	}
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'd':
			int6k.timeperiod = (unsigned)(uintspec (optarg, 1, 60));
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
		case 'l':
			loop = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'n':
			_setbits (int6k.flags, INT6K_NETWORK);
			break;
		case 'q':
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'r':
			_setbits (int6k.flags, INT6K_VERSION);
			break;
		case 'R':
			_setbits (int6k.flags, INT6K_RESET_DEVICE);
			break;
		case 't':
			_setbits (int6k.flags, INT6K_MINIMUM_TRAFFIC);
			break;
		case 'T':
			_setbits (int6k.flags, INT6K_NETWORK_TRAFFIC);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		case 'w':
			wait = (unsigned)(uintspec (optarg, 0, 3600));
			break;
		case 'x':
			_setbits (int6k.flags, INT6K_BAILOUT);
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

	openchannel (&channel);
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}
	if (!argc) 
	{
		manager (&int6k, loop, wait);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		manager (&int6k, loop, wait);
		argv++;
		argc--;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

