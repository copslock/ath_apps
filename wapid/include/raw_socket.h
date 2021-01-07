/*****************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�raw_socket.h
* ժ    Ҫ��ʹ��ԭʼ�׽ӿڷ��ͺͽ������ݵ����
			 ������ʵ��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
******************************************************************/

#ifndef __RAW_SOCKET_H__
#define __RAW_SOCKET_H__
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include "auth.h"

#define ETH_P_WAPI  0x88B4				/*WAIЭ���*/
#define MAXMSG 2048

/*������*/
#define RAW_SOCK_ERR_CREATE  -1
#define RAW_SOCK_ERR_INVALID_DEV_NAME -2
#define RAW_SOCK_ERR_GET_IFR  -3
#define RAW_SOCK_ERR_GET_MAC  -4
#define RAW_SOCK_ERR_SELECT   -5
#define RAW_SOCK_ERR_RECV     -6
#define RAW_SOCK_ERR_SEND     -7

int open_raw_socket(int eth_protocol_type);
int get_device_index_by_raw_socket(char* dev_name, int sock);
int get_device_mac_by_raw_socket(char* dev_name, int sock, u8 *mac_out);
int get_device_mtu_by_raw_socket(char* dev_name, int sock, u16 *mtu_out);
int handle_recverr(int sock);

int send_rs_data(const void *data, int len, struct ethhdr *eh, apdata_info *APData);

int recv_rs_data(void * data, int buflen, struct ethhdr *_eh, int sk);

#endif  

