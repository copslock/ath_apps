// dk_common.h - contains ui and os related function declarations

// Copyright (c) 2001 Atheros Communications, Inc., All Rights Reserved


// DESCRIPTION
// -----------
// Contains the function declarations of the ui and os related functions

#ifndef __INCdk_commonh
#define __INCdk_commonh

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus 

#include "mIds.h"

#ifndef SWIG
#include <stdio.h>
#include "wlantype.h"

#ifndef A_ASSERT
#ifdef KERPLUG
#include <crtdbg.h> 
#define A_ASSERT _ASSERT
#else

#ifndef DRAGON
#include <assert.h>
#define A_ASSERT assert
#endif
#endif

#endif

#if defined(Linux) || defined(__APPLE__)
#include "linuxdrv.h"
#endif
#if defined(WIN32)  && !defined(KERLPLUG)
#include "ntdrv.h"
#endif
#ifdef VXWORKS
#include "vxdrv.h"
#endif

//#if defined(PREDATOR) || defined(COBRA_AP) || defined(DRAGON)
#ifndef __APPLE__
#include "ar5211/ar5211reg.h"
#endif
//#else
//#include "ar5210reg.h"
//#endif

#define CMD_LEN_SIZE	2
#define MAX_BUF_SIZE        512
#define MAX_TRANSFER_SIZE   (8 * 1024)   // MAX_PCI_ENTRIES * sizeof(PCI_VALUES)
#define MIN_TRANSFER_SIZE 	512

#ifdef MDK_C
#ifdef RUN_TEST
#define DLL_EXPORT __declspec( dllimport )
#else
#define DLL_EXPORT __declspec( dllexport )
#endif
#else 
#define DLL_EXPORT
#endif

#ifdef ECOS

#ifndef AR6000

#include "ecosdrv.h"
#include <string.h>
#include <stdlib.h>
#include <cyg/error/errno.h>
#include <cyg/hal/hal_stub.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/io/io.h>
#include <cyg/kernel/kapi.h>

#else

#include "osapi.h"

#endif

#ifdef USB_COMN
#include "athusbdrv.h"
#else
typedef void * APP_HANDLE;
typedef void * DRV_HANDLE;
#endif

#include "hwext.h"

#ifndef AR6000

#define FILE A_INT8
#define TRUE 1
#define FALSE 0
#define A_MALLOC(a) (malloc(a))
#define A_FREE(a) (free(a))
#ifdef A_STRCPY
#undef A_STRCPY
#define A_STRCPY(a,b) (memcpy((a), (b), sizeof((b))))
#endif
#endif

#else   // AR6000

#define A_STRCPY A_MEMCPY

#endif  // !AR6000

#include "common_defs.h"

#if defined (DOS_CLIENT)
#undef WIN32
#include <tcp.h>
#define WIN32 1
#endif

#if 0
//#ifdef PHOENIX

#undef APP_HANDLE 
#undef DRV_HANDLE
#undef IN
#undef A_DATA_CACHE_INVAL

#define APP_HANDLE A_UINT32
#define DRV_HANDLE A_UINT32
#define IN

#define athUsbDrvInit(a, b, c, d, e, f, g) TRUE
#define athUsbDrvRecv(a, b, c) TRUE
#define athUsbDrvSend(a, b, c) TRUE
#define A_DATA_CACHE_INVAL(a, b) 


#endif


typedef struct osSockInfo {
	A_CHAR   hostname[128];
	A_UINT16  port_num;
	A_UINT32 ip_addr;
#if defined(ECOS) 
#if defined(USB_COMN) || defined(MBOX_COMN)
	DRV_HANDLE sockfd;
#endif //USB_COM
#if defined(SERIAL_COMN)
	cyg_io_handle_t sockfd;
#endif //SERIAL_COMN
#else //ECOS
	A_INT32 inHandle;
	A_INT32 outHandle;
#if defined (DOS_CLIENT)
	tcp_Socket sockfd;
#else
	A_INT32  sockfd;
#endif //DOS_CLIENT
#endif //ECOS
	A_UINT32 sockDisconnect;
	A_UINT32 sockClose;
} OS_SOCK_INFO, ART_SOCK_INFO;

#if defined(ART_BUILD ) || defined(__ATH_DJGPPDOS__) || defined(SOC_AP)
A_UINT16 uilog ( char *filename, A_BOOL append);
A_UINT16 uiWriteToLog ( char *string );       
#else
A_UINT16 uilog ( char *filename);
#endif

void uilogClose
(
	void
);

#if defined(ECOS)
//#define q_uiPrintf uiPrintf
static __inline void emptyfn(const char * format, ...) { }
#define q_uiPrintf 1 ? (void)0 : emptyfn

#ifndef AR6000
#else
#define uiPrintf A_PRINTF
#endif

#endif

#if !defined(ECOS)
/*
A_INT32 uiPrintf ( const char *format, ...);


A_INT32 q_uiPrintf ( const char *format, ...);

A_INT16 statsPrintf ( FILE *pFile, const char *format, ...);
*/
#endif
//#if defined(SERIAL_COMN) && !defined(ECOS)
//#endif



DLL_EXPORT void dk_quiet
(
    	A_UINT16 Mode // 0 for off, 1 for on
);

A_UINT32 map_file(A_STATUS *status, A_UCHAR **memPtr, A_UCHAR *filename);


A_STATUS osThreadCreate
(
	void            threadFunc(void * param), 
	void 		*param,
	A_CHAR*		threadName,
	A_INT32 	threadPrio,
	A_UINT32 	*threadId
);

void osThreadKill
(
	A_UINT32		threadId
);

ART_SOCK_INFO *osSockCreate
(
  char *pname
);


A_INT32 osSockRead
(
	OS_SOCK_INFO *pSockInfo,
	A_UINT8 *buf,
	A_INT32 len
);

A_INT32 osSockWrite
(
	OS_SOCK_INFO *pSockInfo,
	A_UINT8 *buf,
	A_INT32 len
);

void OSmemWrite
(
	A_UINT32 devNum,
    A_UINT32 physAddr,
	A_UCHAR	 *bytesWrite,
	A_UINT32 length
);

A_STATUS onlyOneRunning
(
	char *prog1, 
	char *prog2
);

OS_SOCK_INFO *osSockListen
(
	A_UINT32 acceptFlag,
	A_UINT16	port
);

OS_SOCK_INFO *osSockConnect
(
	char *pname
);

void osSockClose
(
	OS_SOCK_INFO *pSockInfo
);

OS_SOCK_INFO *osSockAccept
(
	OS_SOCK_INFO *pSockInfo
);

#ifdef ANWI
#define milliTime() (GetTickCount())
#else
A_UINT32 milliTime ( void);
#endif

void milliSleep
(	
	A_UINT32 millitime
);

A_UINT32 semInit
(
	void
); 

A_INT32 semLock
(
	A_UINT32 sem
);

A_INT32 semUnLock
(
	A_UINT32 sem
);

A_INT32 semClose
(
	A_UINT32 sem
);

#endif // SWIG

//event type definitions
#define INTERRUPT_F2    1
#define TIMEOUT         4
#define ISR_INTERRUPT   0x10
#define DEFAULT_TIMEOUT 0xff

#ifdef __ATH_DJGPPDOS__
#define ISR_INTERRUPT   0x10
#endif


// added this event id for the dk_client side. The event id assigned to  an event, 
// is dervied from the nextEventId stored in the dk_master data structure. 
// Events created in the client side cannot read this value. 
// So a fixed event id is used in such cases. This is used in the resetDevice function
// in devlib (for remote clients).
#define DEVLIB_EVENT_ID	0xfe

//feature ID definitions for MDK
#define COMPARE_PKTS	0
#define RECYCLE_RX		1
#define RECYCLE_TX		2
#define RECYCLE_ALT		3
#define TX_STATS		4
#define RX_STATS		5
#define INTERRUPT_STATS	6
#define LAST_FEATURE	6	//update as add more to be the highest feature ID
#define ALL_FEATURES	0xff

#define ENABLE_RECYCLE	0x00000001
#define ENABLE_STATS	0x00000002

/* statistics enable flag bits */
#define RX_NUM_PHY_ERRORS		0x7
#define RX_ALL					0x0000007f
#define RX_GOOD_PACKETS			0x00000001
#define RX_CRC_ERRORS			0x00000002
#define RX_DECRYPT_CRC_ERRORS	0x00000004
#define RX_PHY_ERROR			0x00000008
#define RX_SIGNAL_STRENGTH		0x00000010
#define RX_NUM_KNOWN_STATS		12   //This defines the number of known
					     //stats that get sent to dk master,
					     //the number of duplicate packet
					     //statistics gets added to it.
					     //consists of 8 phy stats, 3 misc 
#define RX_DUP_PACKETS			0x00000020
#define RX_THROUGHPUT			0x00000040

#define TX_ALL					0x0000003f
#define TX_GOOD_PACKETS			0x00000001
#define TX_EXCESS_RETRIES		0x00000002
#define TX_FIFO_UNDERRUN		0x00000004
#define TX_SHORT_RETRIES		0x00000008
#define TX_LONG_RETRIES			0x00000010
#define TX_ACK_SIG_STRENGTH		0x00000020
#define TX_NUM_BYTES_TX_STATS	8*4 + 32*2

//experimental DEV_IDs
#define DEV_BEANIE_MAUI2_EXPERIMENT 0xe011
#define DEV_E2_PCI          0x0101 
#define DEV_E5_PCI          0x0102
#define DEV_E7_PCI          0x0103
#define DEV_E7_PCI_PA       0x0104
#define DEV_E7_PC_PA        0x0004
#define DEV_E9_PCI_PA       0x0105
#define DEV_E9_PC_PA        0x0005
#define DEV_E9_PC_ANT       0x0006
#define DEV_AR5210_PCI      0x0107
#define DEV_AR5210_PC       0x0007
#define DEV_AR5210_AP       0x0207
#define DEV_AR5001          0x0010
#define DEV_AR5001_QMAC     0x0011
#define DEV_AR5001_QMAC_FPGA 0xf011
#define DEV_LEGACY          0x1107
#define DEV_11B_FPGA				0xf11b
#define DEV_OAHU_FPGA				0xf012
#define DEV_OAHU_DEF				0xff12
#define DEV_OAHU					0x0012
#define DEV_OAHU_TEST				0xe012
#define DEV_VENICE_FPGA				0xf013
#define DEV_VENICE_DEF				0xff13
#define DEV_VENICE   				0x0013
#define DEV_VENICE_DERBY			0x0014
#define DEV_HAINAN_SOM	     		0x0015
#define DEV_VENICE_DERBY_2			0x0016
#define DEV_HAINAN_DERBY_2			0x0017
#define DEV_HAINAN_SB_FPGA			0xf015
#define DEV_HAINAN_SB_DEF			0xff15
#define DEV_HAINAN_DERBY_FPGA		0xf016
#define DEV_HAINAN_DERBY_DEF		0xff16
#define DEV_GRIFFIN      			0x0018
#define DEV_EAGLE        			0x0019
#define DEV_PREDATOR      			0x00b0
#define DEV_PHOENIX      			0x00c0
#define DEV_CONDOR      			0x0020
#define DEV_DRAGON                  0x0022
#define DEV_OWL                     0x0023
#define DEV_OWL_PCIE                0x0024
#define DEV_SWAN					0x0025
#define DEV_NALA					0x0026
#define DEV_SOWL                    0x0027
#define DEV_SOWL_PCIE               0x0028
#define DEV_MERLIN                  0x0029
#define DEV_MERLIN_PCIE             0x002a
#define DEV_KITE_PCIE_default       0xff1c
#define DEV_KITE_PCIE               0x002b
#define DEV_KIWI                    0x002d
#define DEV_KIWI_PCIE               0x002e

#define MAX_DK_STA_NUM	32 
#define BUFF_BLOCK_SIZE			0x100  			/* size of a buffer block */

#define COM1_PORT_NUM       0
#define COM2_PORT_NUM       1
#define COM3_PORT_NUM       2
#define COM4_PORT_NUM		3
#define SOCK_PORT_NUM       33120
#define USB_PORT_NUM        999
#define MBOX_PORT_NUM        998
#define RECV_MBOX       0
#define SEND_MBOX       1
#define MBOX_BUF_SIZE   512

#ifndef COM_DEFS
#define COM_DEFS
// com port
#define READ_BUF_SIZE                   512
#define WRITE_BUF_SIZE                  512
//#define WRITE_BUF_SIZE                  32
#define COM_ERROR_GETHANDLE             1    
#define COM_ERROR_BUILDDCB              2
#define COM_ERROR_CONFIGDEVICE          4
#define COM_ERROR_CONFIGBUFFERS         8
#define COM_ERROR_SETDTR                16
#define COM_ERROR_CLEARDTR              32
#define COM_ERROR_PURGEBUFFERS          64
#define COM_ERROR_READ                  128 
#define COM_ERROR_WRITE                 256
#define COM_ERROR_MASK                  512
#define COM_ERROR_TIMEOUT               1024
#define COM_ERROR_INVALID_HANDLE        2048
#endif

// Device function ID
#define WMAC_FUNCTION  0
#define UART_FUNCTION  1
#define USB_FUNCTION   2
#define SDIO_FUNCTION   3

#define MDK_MAX_NUM_DEVICES	4
#define UART_FN_DEV_START_NUM  (UART_FUNCTION * MDK_MAX_NUM_DEVICES)
#define USB_FN_DEV_START_NUM  USB_FUNCTION
#define SDIO_FN_DEV_START_NUM  USB_FN_DEV_START_NUM

#define MAX_CODE_SIZE	0x16800   // 90k for USB Plus devices
//#define MAX_BOOT_DATA_WORDS (20*1024) // 80kB == 20k Words
#define MAX_BOOT_DATA_WORDS (MAX_CODE_SIZE/4)


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INCdk_commonh 
