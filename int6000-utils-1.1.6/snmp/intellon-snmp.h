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
 *   intellon-snmp.h
 *   
 *   Implement the IHPAV-OPERATIONS MIB.
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
 *      Help also from mib2c.
 *
 *--------------------------------------------------------------------*/

#ifndef INTELLON_H
#define INTELLON_H

#include "DeviceList.h"

#define NNN 128

struct operationsTable_entry 

{

/* Index values */

	long deviceIndex;

/* Column values */

	char deviceMacAddress [NNN];
	size_t deviceMacAddress_len;
	char old_deviceMacAddress [NNN];
	size_t old_deviceMacAddress_len;
	char deviceID [NNN];
	size_t deviceID_len;
	char deviceFirmwareRevision [NNN];
	size_t deviceFirmwareRevision_len;
	long deviceReset;
	long old_deviceReset;
	long deviceInitialize;
	long old_deviceInitialize;
	long devicePriorityVLANTagsEnable;
	long old_devicePriorityVLANTagsEnable;
	long devicePriorityVLANTag0;
	long old_devicePriorityVLANTag0;
	long devicePriorityVLANTag1;
	long old_devicePriorityVLANTag1;
	long devicePriorityVLANTag2;
	long old_devicePriorityVLANTag2;
	long devicePriorityVLANTag3;
	long old_devicePriorityVLANTag3;
	long devicePriorityVLANTag4;
	long old_devicePriorityVLANTag4;
	long devicePriorityVLANTag5;
	long old_devicePriorityVLANTag5;
	long devicePriorityVLANTag6;
	long old_devicePriorityVLANTag6;
	long devicePriorityVLANTag7;
	long old_devicePriorityVLANTag7;
	long devicePriorityTrafficClassEnable;
	long old_devicePriorityTrafficClassEnable;
	long devicePriorityTOSBitsEnable;
	long old_devicePriorityTOSBitsEnable;
	long devicePriorityTOSBits0;
	long old_devicePriorityTOSBits0;
	long devicePriorityTOSBits1;
	long old_devicePriorityTOSBits1;
	long devicePriorityTOSBits2;
	long old_devicePriorityTOSBits2;
	long devicePriorityTOSBits3;
	long old_devicePriorityTOSBits3;
	long devicePriorityTOSBits4;
	long old_devicePriorityTOSBits4;
	long devicePriorityTOSBits5;
	long old_devicePriorityTOSBits5;
	long devicePriorityTOSBits6;
	long old_devicePriorityTOSBits6;
	long devicePriorityTOSBits7;
	long old_devicePriorityTOSBits7;
	long devicePriorityTTLCap0;
	long old_devicePriorityTTLCap0;
	long devicePriorityTTLCap1;
	long old_devicePriorityTTLCap1;
	long devicePriorityTTLCap2;
	long old_devicePriorityTTLCap2;
	long devicePriorityTTLCap3;
	long old_devicePriorityTTLCap3;
	long devicePriorityDefaultIGMP;
	long old_devicePriorityDefaultIGMP;
	long devicePriorityDefaultUnicast;
	long old_devicePriorityDefaultUnicast;
	long devicePriorityDefaultIGMPManagedMulticast;
	long old_devicePriorityDefaultIGMPManagedMulticast;
	long devicePriorityDefaultMulticastBroadcast;
	long old_devicePriorityDefaultMulticastBroadcast;
	long devicePriorityDSCPEnable;
	long old_devicePriorityDSCPEnable;
	char deviceNMK [NNN];
	size_t deviceNMK_len;
	char old_deviceNMK [NNN];
	size_t old_deviceNMK_len;
	long deviceResettoFactoryDefault;
	long old_deviceResettoFactoryDefault;
	char deviceHFID [NNN];
	size_t deviceHFID_len;
	char old_deviceHFID [NNN];
	size_t old_deviceHFID_len;
	long deviceMDUEnable;
	long old_deviceMDUEnable;
	long deviceMDURole;
	long old_deviceMDURole;
	long deviceAVNetworkCCoSelection;
	long old_deviceAVNetworkCCoSelection;
	long deviceFirmwareUpgrade;
	long old_deviceFirmwareUpgrade;
	u_long deviceTransmittedPhyRate;
	u_long deviceReceivedPhyRate;
	u_long deviceTransmittedToneMaps;
	u_long deviceReceivedToneMaps;
	u_long deviceTransmittedMPDUGoodCount;
	u_long deviceReceivedMPDUGoodCount;
	u_long deviceTransmittedMPDUBadCount;
	u_long deviceReceivedMPDUBadCount;
	u_long deviceTransmittedMPDUCollisionCount;
	u_long deviceTransmittedPDGoodCount;
	u_long deviceReceivedPDGoodCount;
	u_long deviceTransmittedPDBadCount;
	u_long deviceReceivedPDBadCount;
	u_long deviceReceivedFECGoodCount;
	u_long deviceReceivedFECBadCount;

/* Illustrate using a simple linked list */

	int valid;
	struct operationsTable_entry *next;
};

/* function declarations */

void init_intellon (void);
void init_deviceCount (void);
void init_table_operationsTable (void);
struct operationsTable_entry * operationsTable_createEntry (long, struct device *d);
void operationsTable_remove (struct operationsTable_entry *);
Netsnmp_Node_Handler operationsTable_handler;
Netsnmp_First_Data_Point operationsTable_get_first_data_point;
Netsnmp_Next_Data_Point operationsTable_get_next_data_point;

/* column number definitions for table operationsTable */

#define COLUMN_DEVICEINDEX		1

#define COLUMN_DEVICEMACADDRESS		2
#define COLUMN_DEVICEID			3
#define COLUMN_DEVICEFIRMWAREREVISION	4

#define COLUMN_DEVICERESET		5

#define COLUMN_DEVICEINITIALIZE		6

#define COLUMN_DEVICEPRIORITYVLANTAGSENABLE		7

#define COLUMN_DEVICEPRIORITYVLANTAG0		8

#define COLUMN_DEVICEPRIORITYVLANTAG1		9

#define COLUMN_DEVICEPRIORITYVLANTAG2		10

#define COLUMN_DEVICEPRIORITYVLANTAG3		11

#define COLUMN_DEVICEPRIORITYVLANTAG4		12

#define COLUMN_DEVICEPRIORITYVLANTAG5		13

#define COLUMN_DEVICEPRIORITYVLANTAG6		14

#define COLUMN_DEVICEPRIORITYVLANTAG7		15

#define COLUMN_DEVICEPRIORITYTRAFFICCLASSENABLE		16

#define COLUMN_DEVICEPRIORITYTOSBITSENABLE		17

#define COLUMN_DEVICEPRIORITYTOSBITS0		18

#define COLUMN_DEVICEPRIORITYTOSBITS1		19

#define COLUMN_DEVICEPRIORITYTOSBITS2		20

#define COLUMN_DEVICEPRIORITYTOSBITS3		21

#define COLUMN_DEVICEPRIORITYTOSBITS4		22

#define COLUMN_DEVICEPRIORITYTOSBITS5		23

#define COLUMN_DEVICEPRIORITYTOSBITS6		24

#define COLUMN_DEVICEPRIORITYTOSBITS7		25

#define COLUMN_DEVICEPRIORITYTTLCAP0		26

#define COLUMN_DEVICEPRIORITYTTLCAP1		27

#define COLUMN_DEVICEPRIORITYTTLCAP2		28

#define COLUMN_DEVICEPRIORITYTTLCAP3		29

#define COLUMN_DEVICEPRIORITYDEFAULTIGMP		30

#define COLUMN_DEVICEPRIORITYDEFAULTUNICAST		31

#define COLUMN_DEVICEPRIORITYDEFAULTIGMPMANAGEDMULTICAST		32

#define COLUMN_DEVICEPRIORITYDEFAULTMULTICASTBROADCAST		33

#define COLUMN_DEVICEPRIORITYDSCPENABLE		34

#define COLUMN_DEVICENMK		35

#define COLUMN_DEVICERESETTOFACTORYDEFAULT		36

#define COLUMN_DEVICEHFID		37

#define COLUMN_DEVICEMDUENABLE		38

#define COLUMN_DEVICEMDUROLE		39

#define COLUMN_DEVICEAVNETWORKCCOSELECTION		40

#define COLUMN_DEVICEFIRMWAREUPGRADE		41

#define COLUMN_DEVICETRANSMITTEDPHYRATE		42

#define COLUMN_DEVICERECEIVEDPHYRATE		43

#define COLUMN_DEVICETRANSMITTEDTONEMAPS		44

#define COLUMN_DEVICERECEIVEDTONEMAPS		45

#define COLUMN_DEVICETRANSMITTEDMPDUGOODCOUNT		46

#define COLUMN_DEVICERECEIVEDMPDUGOODCOUNT		47

#define COLUMN_DEVICETRANSMITTEDMPDUBADCOUNT		48

#define COLUMN_DEVICERECEIVEDMPDUBADCOUNT		49

#define COLUMN_DEVICETRANSMITTEDMPDUCOLLISIONCOUNT		50

#define COLUMN_DEVICETRANSMITTEDPDGOODCOUNT		51

#define COLUMN_DEVICERECEIVEDPDGOODCOUNT		52

#define COLUMN_DEVICETRANSMITTEDPDBADCOUNT		53

#define COLUMN_DEVICERECEIVEDPDBADCOUNT		54

#define COLUMN_DEVICERECEIVEDFECGOODCOUNT		55

#define COLUMN_DEVICERECEIVEDFECBADCOUNT		56

#endif

