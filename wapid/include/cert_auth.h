/*****************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�cert_auth.c
* ժ    Ҫ��֤����������غ���ԭ��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
******************************************************************/

#ifndef __CERT_AUTH_H__
#define __CERT_AUTH_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include "structure.h"
#include "auth.h"
#include "cert_info.h"
#include "key_neg.h"
#include "typedef.h"
//#include "prf.h"
#include "pack.h"
#include "debug.h"
#include "raw_socket.h"

void ap_pskbk_derivation(apdata_info *pap);

/*ȡ֤���еİ䷢������,���������ƺ����к�*/
int wai_fixdata_id_by_ident(struct cert_bin_t *cert_st, wai_fixdata_id *fixdata_id, u16 index);
/*������Կ��������*/
void ae_usk_derivation(struct auth_sta_info_t *sta_info, session_key_neg_response *key_res_buff);
/*�����ʼ��*/
int auth_initiate(asso_mt *passo_mt_info, apdata_info *pap);
/*���첢���ͼ��𼤻����*/
int ap_activate_sta(struct auth_sta_info_t *sta_info);

/*����STA���͵�֡
	a ��������������
	b ��ԿЭ����Ӧ����
	c �㲥��Կͨ����Ӧ����
*/
int wapi_process_from_sta(u8 *read_asue, int readlen, u8 *mac, apdata_info *pap);

/*����AS���͵�֤�������Ӧ����*/
int wapi_process_1_of_1_from_as(u8 *read_as, int readlen, apdata_info *pap);

/*���������Ӧ*/
int access_auth_res(struct auth_sta_info_t *sta_info,	u8 *temp, int temp_len );

/*�����������������*/
int access_auth_req (struct auth_sta_info_t *sta_info, const u8 *read_asue, 
						int readlen, u8 *sendto_as, int *psendtoas_len);

/*����֤���������"���ز���Ĭ����ͨ��ASU����֤��"*/
int certificate_auth_req (const sta_auth_request	*sta_auth_requ, 
							struct auth_sta_info_t *sta_info,
							u8 *sendto_as, int *psendtoas_len); 
	
/*����֤�������Ӧ*/
int certificate_auth_res(struct auth_sta_info_t **sta_info, u8 *read_as, int readlen , apdata_info *pap);

/*��װ�鲥��Կ*/
void set_mcastkey(apdata_info *pap, struct sta_msksa *msksa);
/*��װ������Կ*/
void set_ucastkey(apdata_info *pap,  struct auth_sta_info_t *sta_info);
/*����Driver����Ϣ*/
void wapi_process_for_driver(unsigned char *readapdrv, int readlen, apdata_info *pap);
int register_certificate(struct eloop_data *eloop);
#endif
