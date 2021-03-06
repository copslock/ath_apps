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
 *   pib.h - PIB version definitions and declarations;
 *
 *   The INT6000 PIB undergoes periodic revision as new features
 *   are added; versions are distiguished from one another by the 
 *   first two bytes;
 *
 *   this file declares PIB header structure for PIBs released to 
 *   date; it does not define the position of all PIB information
 *   because there must be some mystery to life;
 *
 *   call function pibpeek() to display a buffer holding an unkown 
 *   PIB structure;
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

#ifndef PIB_HEADER
#define PIB_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6 /* normally defined in ethernet.h or if_ether.h */
#endif

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"
#include "../tools/memory.h"
#include "../crypt/HPAVKey.h"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define PIB_HFID_LEN 64

#define PIB_SILENCE	(1 << 0)
#define PIB_VERBOSE 	(1 << 1)
#define PIB_BASENAME 	(1 << 2)
#define PIB_MAC 	(1 << 3)
#define PIB_MACINC 	(1 << 4)
#define PIB_NMK 	(1 << 5)
#define PIB_DAK 	(1 << 6)
#define PIB_MFGSTRING 	(1 << 7)
#define PIB_USER 	(1 << 8)
#define PIB_NETWORK 	(1 << 9)
#define PIB_CCO_MODE	(1 << 10)
#define PIB_NID 	(1 << 11)

#define PIB_CCOMODES 4
#define PIB_MDUROLES 2

#define PRESCALAR_OFFSET 0x0A10
#define PRESCALAR_MEMBER 1156

/*====================================================================*
 *   Basic PIB header;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed header_pib 

{
	uint8_t FWVERSION;
	uint8_t PIBVERSION;
	uint16_t RESERVED1;
	uint16_t PIBLENGTH;
	uint16_t RESERVED2;
	uint32_t CHECKSUM;
}

header_pib;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   As of PIB 1.4 a generic structure evolved; this is the start;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed simple_pib 

{
	uint16_t PIBVERSION;
	uint16_t RESERVED1;
	uint16_t PIBLENGTH;
	uint16_t RESERVED2;
	uint32_t CHECKSUM;
	uint8_t MAC [ETHER_ADDR_LEN];
	uint8_t DAK [HPAVKEY_DAK_LEN];
	uint16_t RESERVED3;
	uint8_t MFG [PIB_HFID_LEN];
	uint8_t NMK [HPAVKEY_NMK_LEN];
	uint8_t USR [PIB_HFID_LEN];
	uint8_t NET [PIB_HFID_LEN];
	uint8_t CCoSelection;
	uint8_t CexistModeSelect;
	uint8_t PLFreqSelect;
	uint8_t RESERVED4;
	uint8_t PreferredNID [HPAVKEY_NID_LEN];
	uint8_t AutoFWUpgradeable;
	uint8_t MDUConfiguration;
	uint8_t MDURole;
	uint8_t RESERVED5 [10];
	uint8_t StaticNetworkConfiguration [128];
	uint8_t InterfaceConfiguration [64];
}

simple_pib;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB1 

{
	uint8_t FWVersion;
	uint8_t PIBVersion;
	uint16_t Reserved1;
	uint16_t PIBLength;
}

PIB1;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v1.2; this is deprecated; 
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB1_2 

{
	uint8_t FWVersion;
	uint8_t PIBVersion;
	uint16_t Reserved1;
	uint8_t DAK [HPAVKEY_DAK_LEN];
	uint8_t NMK [HPAVKEY_NMK_LEN];
	uint8_t MAC [ETHER_ADDR_LEN];
	uint32_t FLG;
}

PIB1_2;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB Structure as of v1.3; this is deprecated;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB1_3 

{
	uint8_t FWVersion;
	uint8_t PIBVersion;
	uint16_t Reserved1;
	uint8_t MAC [ETHER_ADDR_LEN];
	uint8_t DAK [HPAVKEY_DAK_LEN];
	uint16_t Reserved2;
	uint8_t MFG [PIB_HFID_LEN];
	uint8_t NMK [HPAVKEY_NMK_LEN];
	uint8_t USR [PIB_HFID_LEN];
	uint8_t NET [PIB_HFID_LEN];
}

PIB1_3;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structure introduced as of v1.4
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed VersionHeader 

{
	uint8_t FWVersion;
	uint8_t PIBVersion;
	uint16_t Reserved1;
	uint16_t PIBLength;
	uint16_t Reserved2;
	uint32_t Checksum;
}

VersionHeader;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB Structure as of v1.4
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB1_4 

{
	struct VersionHeader VersionHeader;
	struct __packed 
	{
		uint8_t MAC [ETHER_ADDR_LEN];
		uint8_t DAK [HPAVKEY_DAK_LEN];
		uint16_t Reserved1;
		uint8_t MFG [PIB_HFID_LEN];
		uint8_t NMK [HPAVKEY_NMK_LEN];
		uint8_t USR [PIB_HFID_LEN];
		uint8_t NET [PIB_HFID_LEN];
		uint8_t CCoSelection;
		uint8_t CoexistModeSelection;
		uint8_t FreqSelection;
		uint8_t Reserved2;
	}
	LocalDeviceConfig;
}

PIB1_4;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structures introduced as of v1.5;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed LocalDeviceConfig 

{
	uint8_t MAC [ETHER_ADDR_LEN];
	uint8_t DAK [HPAVKEY_DAK_LEN];
	uint16_t Reserved1;
	uint8_t MFG [PIB_HFID_LEN];
	uint8_t NMK [HPAVKEY_NMK_LEN];
	uint8_t USR [PIB_HFID_LEN];
	uint8_t NET [PIB_HFID_LEN];
	uint8_t CCoSelection;
	uint8_t CoexistModeSelect;
	uint8_t PLFreqSelection;
	uint8_t Reserved2;
	uint8_t PreferredNID [HPAVKEY_NID_LEN];
	uint8_t AutoFWUpgradeable;
	uint8_t MDUConfiguration;
	uint8_t MDURole;
	uint8_t Reserved3 [10];
}

LocalDeviceConfig;
typedef struct __packed StaticNetworkConfig 

{
	uint8_t Reserved [128];
}

StaticNetworkConfig;
typedef struct __packed InterfaceConfig 

{
	uint8_t Reserved [96];
}

InterfaceConfig;
typedef struct __packed IGMPConfig 

{
	uint8_t Reserved [32];
}

IGMPConfig;
typedef struct __packed QoSParameters 

{
	uint8_t UnicastPriority;
	uint8_t McastPriority;
	uint8_t IGMPPriority;
	uint8_t AVStreamPriority;
	uint32_t PriorityTTL [4];
	uint8_t EnableVLANOver;
	uint8_t EnableTOSOver;
	uint16_t Reserved1;
	uint32_t VLANPrioTOSPrecMatrix;
	uint8_t Reserved2 [2020];
}

QoSParameters;
typedef struct __packed ToneNotchParameters 

{
	uint8_t Reserved [5120];
}

ToneNotchParameters;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v1.5
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB1_5 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
}

PIB1_5;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structures introduced as of v2.0
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed FeatureConfiguration 

{
	uint8_t Reserved [128];
}

FeatureConfiguration;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v2.0
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB2_0 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
}

PIB2_0;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structures introduced as of v3.0;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed V3_0Configuration 

{
	uint32_t AVLNMembership;
	uint32_t SimpleConnectTimeout;
	uint8_t EnableLEDThroughputIndicate;
	uint8_t MidLEDThroughputThreshold;
	uint8_t HighLEDThroughputThreshold;
	uint8_t Reserved1;
	uint32_t EnableUnicastQueriesToMembers;
	uint32_t DisableExpireGroupMulticastInterval;
	uint32_t DisableLEDTestLights;
	uint8_t GPIOMap [12];
	uint8_t Reserved [8];
}

V3_0Configuration;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.0;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_0 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
}

PIB3_0;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structures introduced as of v3.1;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed V3_1Configuration 

{
	uint8_t Reserved [128];
}

V3_1Configuration;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.1;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_1 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
	struct V3_1Configuration V3_1Configuration;
}

PIB3_1;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.2
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_2 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
	struct V3_1Configuration V3_1Configuration;
}

PIB3_2;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structure introduced as of v3.3;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed V3_3Configuration 

{
	uint8_t Reserved [64];
}

V3_3Configuration;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.3
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_3 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
	struct V3_1Configuration V3_1Configuration;
	struct V3_3Configuration V3_3Configuration;
}

PIB3_3;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.4
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_4 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
	struct V3_1Configuration V3_1Configuration;
	struct V3_3Configuration V3_3Configuration;
}

PIB3_4;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB sub-structure introduced as of v3.5
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed FeatureGroupEnablement 

{
	uint8_t Reserved [16];
}

FeatureGroupEnablement;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.5
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_5 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
	struct V3_1Configuration V3_1Configuration;
	struct V3_3Configuration V3_3Configuration;
	struct FeatureGroupEnablement FeatureGroupEnablement;
}

PIB3_5;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   PIB structure as of v3.6
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed PIB3_6 

{
	struct VersionHeader VersionHeader;
	struct LocalDeviceConfig LocalDeviceConfig;
	struct StaticNetworkConfig StaticNetworkConfig;
	struct InterfaceConfig InterfaceConfig;
	struct IGMPConfig IGMPConfig;
	struct QoSParameters QoSParameters;
	struct ToneNotchParameters ToneNotchParameters;
	struct FeatureConfiguration FeatureConfiguration;
	struct V3_0Configuration V3_0Configuration;
	struct V3_1Configuration V3_1Configuration;
	struct V3_3Configuration V3_3Configuration;
	struct FeatureGroupEnablement FeatureGroupEnablement;
}

PIB3_6;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

signed pibpeek (const byte memory []);
signed pibfile (struct _file_ * pib);
signed piblock (struct _file_ * pib);

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

