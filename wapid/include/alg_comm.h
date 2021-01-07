/**************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�alg_comm.h
* ժ    Ҫ������WAPID���õĵ��㷨ʵ�ֺ�����ԭ��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
*************************************************************/

#ifndef _ALG_COMM_H
#define _ALG_COMM_H

void update_gnonce(unsigned int *gnonce, int type);
int overflow(unsigned int * gnonce);
void add(unsigned int *a, unsigned int b, unsigned short len);
void	smash_random(unsigned char *buffer, int len );
void get_random(unsigned char *buffer, int len);
void * MEMCPY(void *dbuf, const void *srcbuf, int len) ;

#endif
