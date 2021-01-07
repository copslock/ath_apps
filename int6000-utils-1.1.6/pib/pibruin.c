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
 *   pibruin.c - Intellon Classification Rule Insert Utility;
 *
 *   This inserts classification rules into pib files from a rule
 *   description file;
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
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

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
#include "../int6k/rules.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/lookup.c"
#include "../tools/reword.c"
#include "../tools/hexstring.c"
#include "../tools/hexdecode.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/memout.c"
#include "../tools/assist.c"
#include "../tools/bytespec.c"
#include "../tools/basespec.c"
#include "../tools/ipv4spec.c"
#include "../tools/ipv6spec.c"
#include "../tools/endian.c"
#include "../tools/emalloc.c"
#include "../tools/todigit.c"
#include "../tools/expect.c"
#include "../tools/filepart.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#include "../pib/piblock.c"
#endif

#ifndef MAKEFILE
#include "../int6k/rules.c"
#include "../int6k/ParseRule.c"
#endif

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push,1)
#endif

typedef struct __packed classifier_pib 

{
	uint32_t CR_ID;
	uint32_t CR_OPERAND;
	uint8_t CR_VALUE [16];
}

classifier_pib;
struct __packed auto_connection 

{
	uint8_t MACTION;
	uint8_t MOPERAND;
	uint16_t NUM_CLASSIFIERS;
	struct classifier_pib CLASSIFIER [3];
	struct cspec cspec;
	uint8_t RSVD [14];
}

auto_connection;
struct __packed classifier_priority_map 

{
	uint32_t Priority;
	struct classifier_pib CLASSIFIER;
}

classifier_priority_map;

#ifndef __GNUC__
#pragma pack (pop)
#endif

#define PIB_MAX_AUTOCONN 16
#define PIB_MAX_PRIORITY_MAPS 8

#define PIB_AUTOCONN_COUNT_OFFSET 0x22C
#define PIB_PRIORITY_COUNT_OFFSET 0x228

#define PIB_AUTOCONN_OFFSET 0x310
#define PIB_PRIORITY_MAPS_OFFSET 0x230

signed handle_rule (char *str, int argc, struct auto_connection *autoconn, struct classifier_priority_map * priority_map, int *autoconn_count, int *priority_count);

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
	static const char *optv [] = 
	{
		"eqv",
		"pibfile < rules",
		"Intellon Classification Rule Insert Utility",
		"e\tredirect stderr messages to stdout",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	struct auto_connection auto_connection [PIB_MAX_AUTOCONN];
	struct classifier_priority_map classifier_priority_map [PIB_MAX_PRIORITY_MAPS];
	flag_t flags = (flag_t)(0);
	struct _file_ pib;
	int autoconn_count = 0;
	int priority_count = 0;
	char line [1024];
	int len;
	int wc;
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'q':
			_setbits (flags, PIB_SILENCE);
			break;
		case 'v':
			_setbits (flags, PIB_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (!argc) 
	{
		error (1, 0, "must specify PIB file");
	}
	pib.name = *argv;
	memset (&auto_connection, 0, sizeof (auto_connection));
	memset (&classifier_priority_map, 0, sizeof (classifier_priority_map));
	len = 0;
	wc = 0;
	while ((c = getchar ()) != EOF) 
	{
		if (isspace (c)) 
		{
			continue;
		}
		if (c == '#') 
		{
			while ((c = getchar ()) != EOF && c != '\n') 
			{
				continue;
			}
			continue;
		}
		len = 0;
		wc = 0;
		do 
		{
			if (isspace (c)) 
			{
				while (c != '\n' && (c = getchar ()) != EOF && isspace (c)) 
				{
					continue;
				}
				if (len > 0) 
				{
					++wc;
				}
				if (c != '\n' && c != '#') 
				{
					line [len++] = ' ';
					if (len == sizeof (line) - 1) 
					{
						error (1, 0, "rule too long");
					}
				}
			}
			if (c == '\n' || c == '#') 
			{
				line [len] = '\0';
				handle_rule (line, wc, auto_connection, classifier_priority_map, &autoconn_count, &priority_count);
				len = 0;
				wc = 0;
				if (c == '#') 
				{
					ungetc (c, stdin);
				}
				break;
			}
			line [len++] = c;
			if (len == sizeof (line) - 1) 
			{
				error (1, 0, "rule too long");
			}
		}
		while ((c = getchar ()) != EOF);
	}
	if (len > 0) 
	{
		line [len] = '\0';
		handle_rule (line, wc, auto_connection, classifier_priority_map, &autoconn_count, &priority_count);
	}
	if ((pib.file = open (pib.name, O_BINARY|O_RDWR, FILE_FILEMODE)) == -1) 
	{
		error (1, errno, "%s", pib.name);
	}
	if (pibfile (&pib)) 
	{
		error (1, errno, "Bad PIB file: %s", pib.name);
	}
	if (lseek (pib.file, PIB_AUTOCONN_OFFSET, SEEK_SET) != PIB_AUTOCONN_OFFSET) 
	{
		error (1, errno, "could not seek to AutoConnections");
	}
	if (write (pib.file, &auto_connection, sizeof (auto_connection)) != sizeof (auto_connection)) 
	{
		error (1, errno, "could not write AutoConnections");
	}
	if (lseek (pib.file, PIB_AUTOCONN_COUNT_OFFSET, SEEK_SET) != PIB_AUTOCONN_COUNT_OFFSET) 
	{
		error (1, errno, "could not seek to AutoConnection count");
	}
	if (write (pib.file, &autoconn_count, sizeof (autoconn_count)) != sizeof (autoconn_count)) 
	{
		error (1, errno, "could not write AutoConnection count");
	}
	if (lseek (pib.file, PIB_PRIORITY_MAPS_OFFSET, SEEK_SET) != PIB_PRIORITY_MAPS_OFFSET) 
	{
		error (1, errno, "could not seek to Priority Map");
	}
	if (write (pib.file, &classifier_priority_map, sizeof (classifier_priority_map)) != sizeof (classifier_priority_map)) 
	{
		error (1, errno, "could not write Priority Map");
	}
	if (lseek (pib.file, PIB_PRIORITY_COUNT_OFFSET, SEEK_SET) != PIB_PRIORITY_COUNT_OFFSET) 
	{
		error (1, errno, "could not seek to PriorityMaps count");
	}
	if (write (pib.file, &priority_count, sizeof (priority_count)) != sizeof (priority_count)) 
	{
		error (1, errno, "could not write PriorityMaps count");
	}
	piblock (&pib);
	close (pib.file);
	exit (0);
}

signed handle_rule (char *str, int argc, struct auto_connection *autoconn, struct classifier_priority_map * priority_map, int *autoconn_count, int *priority_count) 

{
	const char **argv;
	char **mem;
	unsigned i;
	signed c;
	size_t len;
	struct rule rule;
	struct cspec cspec;
	static const char *optv [] = 
	{
		"T:V:",
		"",
		"Intellon Classification Rule Insert Utility",
		"T\ttag",
		"V\tversion",
		(const char *) (0) 
	};
	extern const char * program_name;
	mem = emalloc ((argc + 2) * sizeof (char *));
	argv = (const char **)(mem);
	*argv = program_name;
	++argv;
	*argv = str;
	++argv;
	len = strlen (str);
	for (i = 1; i < len; ++i) 
	{
		if (str [i] == ' ') 
		{
			str [i] = '\0';
			*argv = &str [i + 1];
			++argv;
		}
	}
	*argv = NULL;
	optind = 1;
	opterr = 1;
	argv = (const char **)(mem);
	memset (&rule, 0, sizeof (rule));
	memset (&cspec, 0, sizeof (cspec));
	while ((c = getoptv (argc, (const char **)(argv), optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'T':
			cspec.VLAN_TAG = (uint32_t)(basespec (optarg, 16, sizeof (cspec.VLAN_TAG)));
			cspec.VLAN_TAG = htonl (cspec.VLAN_TAG);
			break;
		case 'V':
			cspec.CSPEC_VERSION = (uint16_t)(basespec (optarg, 10, sizeof (cspec.CSPEC_VERSION)));
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	ParseRule (&argc, (const char ***) &argv, &rule, &cspec);

/* Now migrate the rule into the correct PIB structure. */

	if (rule.NUM_CLASSIFIERS > 1 || rule.MACTION == ACTION_STRIPTX || rule.MACTION == ACTION_STRIPRX || rule.MACTION == ACTION_TAGTX || *priority_count >= PIB_MAX_PRIORITY_MAPS) 
	{
		if (*autoconn_count >= PIB_MAX_AUTOCONN) 
		{
			error (1, 0, "too many rules");
		}
		autoconn = &autoconn [*autoconn_count];
		autoconn->CLASSIFIER [0].CR_ID = 0xFF;
		autoconn->CLASSIFIER [1].CR_ID = 0xFF;
		autoconn->CLASSIFIER [2].CR_ID = 0xFF;
		autoconn->MACTION = rule.MACTION;
		autoconn->MOPERAND = rule.MOPERAND;
		autoconn->NUM_CLASSIFIERS = rule.NUM_CLASSIFIERS;
		for (i = 0; i < rule.NUM_CLASSIFIERS; ++i) 
		{
			autoconn->CLASSIFIER [i].CR_ID = rule.CLASSIFIER [i].CR_ID;
			autoconn->CLASSIFIER [i].CR_OPERAND = rule.CLASSIFIER [i].CR_OPERAND;
			memcpy (&autoconn->CLASSIFIER [i].CR_VALUE, &rule.CLASSIFIER [i].CR_VALUE, sizeof (autoconn->CLASSIFIER [i].CR_VALUE));
		}
		memcpy (&autoconn->cspec, &rule.cspec, sizeof (autoconn->cspec));
		++(*autoconn_count);
	}
	else 
	{
		if (*priority_count >= PIB_MAX_PRIORITY_MAPS) 
		{
			error (1, 0, "too many rules");
		}
		priority_map = &priority_map [*priority_count];
		priority_map->Priority = rule.MACTION;
		priority_map->CLASSIFIER.CR_ID = rule.CLASSIFIER [0].CR_ID;
		priority_map->CLASSIFIER.CR_OPERAND = rule.CLASSIFIER [0].CR_OPERAND;
		memcpy (&priority_map->CLASSIFIER.CR_VALUE, &rule.CLASSIFIER [0].CR_VALUE, sizeof (priority_map->CLASSIFIER.CR_VALUE));
		++(*priority_count);
	}
	free (mem);
	return 0;
}

