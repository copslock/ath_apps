
/**************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�alg_comm.c
* ժ    Ҫ���������AP֤���״̬�ĺ���
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
*************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "proc.h"
#include "certupdate.h"
#include "debug.h"

/*����AP֤���״̬*/
int update_cert_status(char *fileconfig)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char get_cert_status[255] ={0,};
	int res = 0;

	/*�������ļ��ж�ȡ֤��״̬*/
	prop_count=load_prop(SEP_EQUAL,fileconfig,properties);
	get_prop("CERT_STATUS", get_cert_status, properties, prop_count);
	free_prop(properties,prop_count);

	/*�ж�֤��״̬�Ƿ�����Ч*/
	if(atoi(get_cert_status) != 1)
	{
		/*��״̬�޸�Ϊ��Ч*/
		res = !save_cert_status(fileconfig, "1");
	}
	return res;
}

/*����֤��״̬*/
int save_cert_status(char *fileconfig, char *cert_flag)
{
	int res = 0;
	res = !save_global_conf(SEP_EQUAL,fileconfig,"",  "CERT_STATUS",cert_flag);
	if(res != 0)
	{
		DPrintf("open file %s error\n", fileconfig);
	}
	return res;
}

