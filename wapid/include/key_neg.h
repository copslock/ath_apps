/*****************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�key_neg.c
* ժ    Ҫ��USKЭ��MSKͨ����غ�����ԭ��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
******************************************************************/

#ifndef __KEY_NEG_H__
#define __KEY_NEG_H__


//#include "cert_auth.h"
//#include "prf.h"
//#include "ecc192_x962.h"
//#include "sha256_api.h"
#include "auth.h"
#include "structure.h"

/*����Rekey�еĵ�����ԿЭ������*/
void usk_rekey_handle(struct auth_sta_info_t *sta_info);
int msk_rekey_handle(apdata_info *pap);

/*������ԿЭ������*/
void	usk_negotiation_req(struct auth_sta_info_t *sta_info);
/*������ԿЭ����Ӧ����*/
int usk_negotiation_res(void *read_asue, int readlen,  struct auth_sta_info_t *sta_info);
/*������ԿЭ��ȷ�Ϸ���*/
void usk_negotiation_confirmation(struct auth_sta_info_t *sta_info);
/*�鲥ͨ��*/
void msk_announcement_tx(struct auth_sta_info_t *sta_info, u8 *sendto_asue);
/*�鲥ͨ����Ӧ*/
int  msk_announcement_res(struct auth_sta_info_t *sta_info, u8 *read_asue, int readlen);
/*������STA�����鲥ͨ��*/
void send_msk_announcement_to_all(apdata_info *pap);
/*��Driver �����鲥ͨ����*/
int notify_groupnotice_to_apdriver(apdata_info *pap);
/*ECDH�㷨��ʼ��*/
int wai_initialize_ecdh(para_alg *ecdh);
int wai_copy_ecdh(para_alg *ecdh_a, para_alg *ecdh_b);
int wai_compare_ecdh(para_alg *ecdh_a, para_alg *ecdh_b);


#endif
