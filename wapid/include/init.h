
/*****************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�init.h
* ժ    Ҫ�� ϵͳ��ʼ����غ�����ԭ��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
******************************************************************/

#include "auth.h"
#define COMMTYPE_GROUP	8
#define PROC_NET_WIRELESS	"/proc/net/wireless"
#define PROC_SYS_DEV_WIFI	"/proc/net/dev"


int ap_initialize_wie(apdata_info *papdata);
#ifdef LSDK6
int ap_setdriver_wie(apdata_info *papdata, int mgmttype);
#endif
int ap_initialize_alg(apdata_info *papdata);

int ap_initialize(apdata_info *pap);

void ap_initialize_sta_table(apdata_info *pap);

/*��ʼ����������Կ�����׼�*/
int ap_initialize_akm(apdata_info *apdata);

int set_wapi(apdata_info *pap);

int check_file_valid(char *filename);
 int open_socket_for_asu() ;
int socket_open_for_ioctl();

int socket_open_for_netlink();
int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions);
void rtnl_close(struct rtnl_handle *rth);

void free_one_interface(struct wapid_interfaces *interfaces );
void free_all_interfaces(struct eloop_data *eloop);

#ifdef DOT11_WAPI_MIB
int ap_initialize_mib(apdata_info *pap);
int ap_setup_mib(apdata_info *pap);
#endif

