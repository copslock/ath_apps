#ifndef _SPECTRAL_IOCTL_
#define _SPECTRAL_IOCTL_

/*
 * ioctl defines
 */
// This code needs to be synchronized with DFS 
#define	DFS_LAST_IOCTL  	16 


#define	IOCTL_SPECTRAL_SET_FFT_PEROID		( 1)
#define	IOCTL_SPECTRAL_SET_SCAN_PEROID      ( 2)
#define	IOCTL_SPECTRAL_SET_SCAN_COUNT       ( 3)
#define	IOCTL_SPECTRAL_SET_SHORT_RPT        ( 4)
#define IOCTL_SPECTRAL_INIT_RELATED_REG     ( 5) 
#define	IOCTL_SPECTRAL_GET_CONFIG			(DFS_LAST_IOCTL + 2)
#define	IOCTL_SPECTRAL_ENABLE_SCAN	        (DFS_LAST_IOCTL + 4) 
#define	IOCTL_SPECTRAL_DISABLE_SCAN	        (DFS_LAST_IOCTL + 5)
#define	IOCTL_SPECTRAL_ACTIVATE_SCAN	    (DFS_LAST_IOCTL + 6) 
#define	IOCTL_SPECTRAL_STOP_SCAN	        (DFS_LAST_IOCTL + 7)
#define	IOCTL_SPECTRAL_SET_DEBUG_LEVEL  	(DFS_LAST_IOCTL + 8) 
#define	IOCTL_SPECTRAL_IS_ACTIVE  	        (DFS_LAST_IOCTL + 9) 
#define	IOCTL_SPECTRAL_IS_ENABLED  	        (DFS_LAST_IOCTL + 10) 
#define IOCTL_SPECTRAL_DUMP_COUNT           (14) 
#define IOCTL_SPECTRAL_CLEAR_COUNT          (15) 
#define	IOCTL_SPECTRAL_START_CLASSIFY_SCAN	(16)
#define	IOCTL_SPECTRAL_STOP_CLASSIFY_SCAN	(17)
#define IOCTL_SPECTRAL_ENABLE_CLASSIFIER	(18)

#define	SPECTRAL_IOCTL_PARAM_NOVAL	-65535

#endif
