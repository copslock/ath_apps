/***************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�typedef.h
* ժ    Ҫ���Զ�������˵��
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
***************************************************************/

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
#ifdef __linux__
#include <endian.h>
#include <byteswap.h>
#endif
#include <stdint.h>
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned char   BOOL;

#endif

