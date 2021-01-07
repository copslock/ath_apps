/*****************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�debug.c
* ժ    Ҫ�����Ժ���ԭ�Ͷ���
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
******************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int debug;
#define DPrintf if (!debug) ; else printf
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

void wapi_str_dump(char *name, void *_str,int len);
void wapi_hex_dump(char *name, void *_str,int len);

#endif //  #ifndef _MY_TRACEFUNC_H__

