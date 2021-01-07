/*****************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�debug.c
* ժ    Ҫ�����Ժ�����ʵ��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
******************************************************************/

#include <stdio.h>
#include <string.h>
#include "include/debug.h"
#include "include/structure.h"

void wapi_hex_dump(char *name, void *_str,int len)
{
	int i;
	unsigned char *str = (unsigned char *)_str;

	if(debug < 2)
		return;
	
	if(name != NULL)
		printf("[WAPID]:: %s(length=%d):", name, len);
	if(len >16)
		printf("	\n");
	for(i=0;i<len;i++)
	{	
		if((i)%16 == 0) printf("	");
		printf("%02X:",*str++);
		if(((i+1)%16==0) && ((i+1) <len))
			printf("	\n");
	}
	printf("\n");
}



void wapi_str_dump(char *name, void *_str,int len)
{
	int i;
	if(debug == 0)
		return;
	if(name != NULL)
		printf("[WAPID]:: %s(length=%d) : \n", name, len);
	for(i=0; i<len; i++){
		if((i)%64 == 0) printf("	");
		 printf("%c", *((unsigned char *)_str + i));
		 if((i+1)%64==0)
		 	printf("	\n");
	}
	printf("\n");
}


