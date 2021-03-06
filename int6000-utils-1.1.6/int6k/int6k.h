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
 *   int6k.h - Device Manager Definitions and Declarations;
 *
 *   this file contains constants and definitions for the Intellon
 *   Linux Device Manager; components declared here are probably not
 *   reusable in other applications;
 *
 *   the major feature is struct int6k, used to communcate between
 *   the main program and modules, and the module declarations;
 *
 *   functions Request(), Confirm() and Failure() are variations on
 *   the generic error() function; they understand struct int6k and
 *   can extract information from it when called; 
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef INT6K_HEADER
#define INT6K_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#ifndef IHP_HEADER
#include "../int6k/homeplug.h"
#include "../int6k/intellon.h"
#endif

#include "../ether/channel.h"
#include "../int6k/message.h"
#include "../int6k/chipset.h"
#include "../int6k/problem.h"
#include "../tools/types.h"
#include "../crypt/HPAVKey.h"
#include "../nvm/nvm.h"

/*====================================================================*
 *   sizes and offsets;                
 *--------------------------------------------------------------------*/

/*! Runtime firmware prior to 3.2 store the PIB at a higher address 
 *  than current versions; Add compiler option -DLEGACY_PIBOFFSET if 
 *  you are use legacy firmware;
 */

#define LEGACY_PIBOFFSET 0x01F00000
#define INT6K_PIBOFFSET 0x01000000
#define INT6K_BLOCKSIZE 1024
#define INT6K_VERSTRING 0x80

/*====================================================================*
 *   device manager flagword bits; 
 *--------------------------------------------------------------------*/

#define INT6K_BAILOUT           (1 << 0)
#define INT6K_SILENCE           (1 << 1)
#define INT6K_VERBOSE           (1 << 2)
#define INT6K_ANALYSE		(1 << 3)
#define INT6K_WAITFORRESET      (1 << 4)
#define INT6K_WAITFORSTART      (1 << 5)
#define INT6K_WAITFORASSOC      (1 << 6)
#define INT6K_RESET_DEVICE      (1 << 7)
#define INT6K_FACTORY_DEFAULTS  (1 << 8)
#define INT6K_DOWNLOADALL	INT6K_FACTORY_DEFAULTS /* for int6ktest */
#define INT6K_GRAPH		INT6K_FACTORY_DEFAULTS /* for int6ktone */
#define INT6K_NETWORK           (1 << 9)
#define INT6K_OPEN_SESSION      INT6K_NETWORK /* for int6kmod */
#define INT6K_VERSION           (1 << 10)
#define INT6K_WRITE_MODULE      INT6K_VERSION /* for int6kmod */
#define INT6K_MANUFACTURER      (1 << 11)
#define INT6K_READ_MODULE       INT6K_MANUFACTURER /* for int6kmod */
#define INT6K_READ_MAC          (1 << 12)
#define INT6K_MINIMUM_TRAFFIC	INT6K_READ_MAC /* for int6krate */
#define INT6K_READ_PIB          (1 << 13)
#define INT6K_NETWORK_TRAFFIC 	INT6K_READ_PIB /* for int6krate */
#define INT6K_READ_PIB_INFO     (1 << 14)
#define INT6K_WRITE_MAC         (1 << 15)
#define INT6K_LINK_STATS	INT6K_WRITE_MAC /* for int6kstat */
#define INT6K_WRITE_PIB         (1 << 16)
#define INT6K_TONE_MAP		INT6K_WRITE_PIB /* for int6kstat */
#define INT6K_FLASH_DEVICE      (1 << 17)
#define INT6K_COMMIT_MODULE     INT6K_FLASH_DEVICE /* for int6kmod */
#define INT6K_QUICK_FLASH	(1 << 18)
#define INT6K_SETLOCALKEY       (1 << 19)
#define INT6K_SETREMOTEKEY      (1 << 20)
#define INT6K_SDRAM_INFO        (1 << 21)
#define INT6K_SDRAM_CONFIG      (1 << 22)
#define INT6K_NVRAM_INFO        (1 << 23)
#define INT6K_ATTRIBUTES        (1 << 24)
#define INT6K_PUSH_BUTTON       (1 << 25)
#define INT6K_READ_CFG          (1 << 26)
#define INT6K_RD_MOD_WR_PIB     (1 << 27)
#define INT6K_WATCHDOG_REPORT	(1 << 28)
#define INT6K_HOST_ACTION       (1 << 29)
#define INT6K_RX_TONEMAPS	INT6K_HOST_ACTION /* for int6kstat */

/*====================================================================*
 *    program constants; 
 *--------------------------------------------------------------------*/

#define PLCDEVICE "PLC"

#define HARDWAREID 0
#define HOSTACTION 0
#define PUSHBUTTON 1
#ifndef IHP_HEADER
#define MODULECODE (VS_MODULE_MAC | VS_MODULE_PIB)
#else
#define MODULECODE (MACSW_MODID | PIB_MODID)
#endif
#define READACTION 0
#define TIMEPERIOD 3

#if defined (WIN32)
#define _TIMER 1000
#define _RETRY 5
#define _DELAY 60
#define SLEEP(timer) Sleep(timer)
#else
#define _TIMER 1000000
#define _RETRY 5
#define _DELAY 60
#define SLEEP(timer) usleep(timer)
#endif

#define SECONDS(timer,retry) ((timer)*(retry)/_TIMER)

#define INT6K_STATE 0
#define INT6K_PAUSE 0
#define INT6K_TIMER (_TIMER/_RETRY)
#define INT6K_RETRY (_DELAY*_RETRY)
#define INT6K_INDEX 0
#define INT6K_COUNT 1
#define INT6K_FLAGS 0

#define DEVICES 3

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

extern const struct _term_ devices [];

/*====================================================================*
 *
 *   the int6k structure holds everything needed to perform device 
 *   manager operations except for the frame buffer, allocated at
 *   runtime; 
 *
 *   the channel structure hold information needed to open, read,
 *   write and close a raw socket; it differs in detail depending
 *   on constants WINPCAP and LIBPCAP; 
 *
 *   character array address[] holds a decoded ethernet address for 
 *   display purposes because humans cannot read; 
 *
 *   byte array LMA[] holds the Intellon Local Broadcast Address 
 *   because it is used in so many places;
 *
 *   byte arrays NMK[] and DAK[] hold encryption keys used by some
 *   operations;
 *
 *   the socket _file_ structure holds the descriptor and interface
 *   name of the host NIC where the name is eth0, eth1, ... or ethn;
 *
 *   the three _file_ structures, CFG, NVM, and PIB hold descriptors
 *   and filenames for files written to the device; 
 *
 *   the three _file_ structres cfg, nvm and pib hold 
 *   descriptors and filenames of files read from the device;
 *
 *   integers retry and timer are used by program int6kwait;
 *
 *   integers index, count and pause control command line looping 
 *   and waiting;
 *
 *   flag_t flags contains bits that define program operations and
 *   control utility program flow;
 *
 *--------------------------------------------------------------------*/

typedef struct int6k 

{
	struct channel * channel;
	struct message * message;
	signed packetsize;
	uint8_t LDA [ETHER_ADDR_LEN];
	uint8_t RDA [ETHER_ADDR_LEN];
	uint8_t NMK [HPAVKEY_NMK_LEN];
	uint8_t DAK [HPAVKEY_DAK_LEN];
	struct _file_ CFG;
	struct _file_ cfg;
	struct _file_ NVM;
	struct _file_ nvm;
	struct _file_ PIB;
	struct _file_ pib;
	struct _file_ XML;
	struct _file_ rpt;
	uint8_t hardwareID;
	uint8_t hostaction;
	uint8_t modulecode;
	uint8_t pushbutton;
	uint8_t readaction;
	uint8_t timeperiod;
	unsigned state;
	unsigned pause;
	unsigned timer;
	unsigned retry;
	unsigned index;
	flag_t flags;

#ifdef IHPAPI_HEADER

	ihpapi_result_t * result;
	unsigned options;

#endif

}

INT6K;

/*====================================================================*
 *   functions; (without API)
 *--------------------------------------------------------------------*/

signed Attributes (struct int6k *);
signed BootDevice (struct int6k *);
signed DiagnosticNetworkProbe (struct int6k *);
signed EmulateHost (struct int6k *);
signed EmulateHost64 (struct int6k *);
signed FactoryDefaults (struct int6k *);
bool FirmwarePacket (struct int6k *);
signed Flash (struct int6k *);
signed FlashDevice (struct int6k *);
signed FlashNVM (struct int6k *);
signed FlashPTS (struct int6k *);
signed HostActionResponse (struct int6k *);
signed Identity (struct int6k *);
signed InitDevice (struct int6k *);
signed LinkStats (struct int6k *);
signed LinkStatistics (struct int6k *);
signed Loopback (struct int6k *, void * memory, size_t extent);
signed MfgString (struct int6k *);
signed NetworkInfo (struct int6k *);
signed NetworkTraffic (struct int6k *);
signed NVRAMInfo (struct int6k *);
signed PHYRates (struct int6k *);
signed PushButton (struct int6k *);
signed ReadNVM (struct int6k *);
signed ReadPIB (struct int6k *);
signed ResetDevice (struct int6k *);
signed ResetAndWait (struct int6k *);
signed Reset (struct int6k *);
signed SDRAMInfo (struct int6k *);
signed SetNMK (struct int6k *);
signed SignalToNoise (struct int6k *);
signed SlaveMembership (struct int6k *);
signed StartDevice (struct int6k *);
signed StartFirmware (struct int6k *, struct header_nvm *);
signed ToneMaps (struct int6k *);
signed UpgradeDevice (struct int6k *);
signed VersionInfo (struct int6k *);
signed WaitForAssoc (struct int6k *);
signed WaitForReset (struct int6k *);
signed WaitForStart (struct int6k *, char string [], signed length);
signed WaitForStop (struct int6k *);
signed WatchdogReport (struct int6k *);
signed WriteCFG (struct int6k *);
signed WriteNVM (struct int6k *);
signed WriteMEM (struct int6k *, struct _file_ * file, uint32_t offset, uint32_t length);
signed WritePIB (struct int6k *);

/*====================================================================*
 *   functions; 
 *--------------------------------------------------------------------*/

signed FlashMOD (struct channel *, uint8_t module);
signed WriteMOD (struct channel *, uint8_t module, const void * memory, size_t extent);
signed NetworkInformation (struct channel *);
signed NetworkTopology (struct channel *);
signed NetworkBridges (struct channel *,  void * memory, size_t extent);
signed NetworkDevices (struct channel *,  void * memory, size_t extent);
signed Traffic (struct channel * channel, unsigned period, unsigned repeat);

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

signed SendMME (struct int6k *);
signed ReadMME (struct int6k *, uint16_t MMTYPE);
signed ReadMFG (struct int6k *, uint16_t MMTYPE);

/*====================================================================*
 *   functions; (with API)
 *--------------------------------------------------------------------*/

#ifdef IHPAPI_HEADER

signed Flash2 (struct int6k *);
signed HostActionWait2 (struct int6k * int6k);
signed Identity2 (struct int6k *);
signed MfgString2 (struct int6k *);
signed NetworkInfo2 (struct int6k *);
signed ReadNVM2 (struct int6k *);
signed ReadPIB2 (struct int6k *);
signed ResetDevice2 (struct int6k *);
signed FactoryDefaults2 (struct int6k * int6k);
signed SetKey2 (struct int6k *);
signed SetSDRAM2 (struct int6k * int6k);
signed Upgrade2 (struct int6k *);
signed WriteNVM2 (struct int6k *);
signed WritePIB2 (struct int6k *);
signed VersionInfo2 (struct int6k *);
signed ConfigBlock2 (struct int6k *);
signed RdModWrPIB2 (struct int6k *);
signed ReadMME2 (struct int6k *);

#endif

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

#ifdef __GNUC__

__attribute__ ((format (printf, 2, 3))) 

#endif

void Display (struct int6k * int6k, const char *format, ...);

#ifdef __GNUC__

__attribute__ ((format (printf, 2, 3))) 

#endif

void Request (struct int6k * int6k, const char *format, ...);

#ifdef __GNUC__

__attribute__ ((format (printf, 2, 3))) 

#endif

void Confirm (struct int6k * int6k, const char *format, ...);

#ifdef __GNUC__

__attribute__ ((format (printf, 2, 3))) 

#endif

void Failure (struct int6k * int6k, const char *format, ...);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

