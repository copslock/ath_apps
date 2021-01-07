/***************************************************************
* Copyright (c) 2001,���������ͨ��������ͨ�����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�structure.h
* ժ    Ҫ������WAI����Ľṹ
* 
* ��ǰ�汾��1.1
* ��    �ߣ����»�yhwang@iwncomm.com
* ������ڣ�2005��6��10��
*
* ȡ���汾��1.0 
* ԭ����  �����»�yhwang@iwncomm.com
* ������ڣ�2004��1��10��
***************************************************************/
#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__
#include "typedef.h"

#define  VERSIONNOW			0x0001	/*��ǰ�汾��*/
#define  WAI					0x01	
#define  RESERVEDDEF		0x0000	/*reserved����*/   

#define TRUE					(u8)1
#define FALSE				(u8)0

#define MAX_EXTENSION_TLV_ATTR   2		/*������չ���Եĸ���*/
#define MAX_BYTE_DATA_LEN		(u16)256 /* ����ֽ����ݳ��� */
#define COMM_DATA_LEN          		(u16)3000//1024/* ����ֽ����ݳ��� */
#define PUBKEY_LEN             		(u16)48  /* ��Կ���� */
#define HMAC_LEN                        		20  /*hmac ����*/

/*WAI ��������*/
#define  AUTHACTIVE				0x03	//���𼤻����
#define  STA_ACESS_REQUEST		0x04	//��������������
#define  STA_ACCESS_RESPONSE	0x05	//���������Ӧ����
#define  AP_AUTH_REQUEST		0x06	//֤�����������
#define  AP_AUTH_RESPONSE		0x07	//֤�������Ӧ����
#define  USK_NEG_REQUEST  		0x08	//��ԿЭ���������
#define  USK_NEG_RESPONSE		0x09	//��ԿЭ����Ӧ����
#define  USK_NEG_CONFIRM		0x0A	//��ԿЭ����Ӧ��
#define  MSK_ANNOUNCE         		0x0B 	//�㲥��Կͨ�����
#define  MSK_RESPONSE 			0x0C 	//�㲥��Կͨ����Ӧ����

/*֤�������*/
#define CERTIFVALID						0x00  /*֤����ȷ����*/
#define CERTIFISSUERUNKNOWN			0x01  /*�䷢�ߴ���(֤�鹫Կ�������뱾AS��ͬ) */     
#define CERTIFUNKNOWNCA				0x02  /*֤����ڲ������εĸ�֤��*/
#define CERTIFEXPIRED					0x03  /*֤�����*/
#define CERTIFSIGNERR					0x04  /*ǩ������ */
#define CERTIFREVOKED					0x05  /*֤���ѵ���*/
#define CERTIFBADUSE					0x06  /*֤��δ���涨��;ʹ��*/
#define CERTUNKNOWNREVOKESTATUS		0x07    /* ֤�����״̬δ֪*/
#define CERTUNKNOWNERROR     			0x08    /* ֤�����δ֪*/

#define  PACK_ERROR  0xffff

/*wapi�������ϣ�������BSS��û�п���IBSS*/
#define MAX_AKM_NO 2
#define MAX_UNICAST_NO 2
#define MAX_MULTICAST_NO 2
#define MAX_BKID_NO 2

/*
 * 1��֤�鼰����ؽṹ����	
 * 1) ͨ�����ݶ���
 */
struct _byte_data {
	u8  length;                      			/* ���� */
	u8  pad_value[3];                		/* ʵ�ֶ�������� */
	u8  data[MAX_BYTE_DATA_LEN];		/* ���� */
} ;
typedef struct _byte_data   byte_data;

struct _comm_data{
 	u32  length;                  			 /* ���� */
	u8  data[MAX_BYTE_DATA_LEN];  	/* ���� */
};
typedef struct _comm_data comm_data,*pcomm_data; 

typedef comm_data tsign;				/*ǩ��*/

struct _big_data{
 	u16  length;                  			 /* ���� */
	u16  pad_value;               			 /* ʵ�ֶ�������� */
	u8  data[COMM_DATA_LEN];  		/* ���� */
};
typedef struct _big_data big_data,*pbig_data;

/*
 * 2)�Ӵ��㷨sha��256��ǩ���㷨������һ���ṹ��
 */
#define MAX_PARA_LEN 20 
struct _para_alg {
	u8		para_flag;   				/*����*/
	u8		pad[3];
	u16		para_len;  		 		/*��������*/
	u16		pad16;
	u8		para_data[MAX_PARA_LEN];   /*��������OID */
} __ATTRIBUTE_PACK__;
typedef struct _para_alg para_alg, *ppara_alg;

/*WAIЭ����������ֶεĹ̶����ݶ���*/
/* a)��ʶFLAG*/
#define WAI_FLAG_BK_UPDATE 		0X01
#define WAI_FLAG_PRE_AUTH   		0X02
#define WAI_FLAG_AUTH_CERT 		0X04
#define WAI_FLAG_OPTION_BYTE 		0X08
#define WAI_FLAG_USK_UPDATE 	0X10	/*USK���±�ʶ*/
#define WAI_FLAG_STAKEY_SESSION	0X20
#define WAI_FLAG_STAKEY_DEL 		0X40
#define WAI_FLAG_REV		 		0X80
#define MAX_CERT_DATA_LEN 1000
#define MAX_ID_DATA_LEN 1000

typedef  u8 wai_fixdata_flag;

struct cert_bin_t
{
	unsigned int type;
	unsigned char *data;	
	unsigned int length;
};
typedef struct cert_bin_t item_data_t, resendbuf_st;

struct key_bin_t
{
	unsigned int OrderLen;
	item_data_t publickey;
	item_data_t privatekey;
};

/*b)֤��*/
typedef struct _wai_fixdata_cert {
	u16	cert_flag;
	struct cert_bin_t cert_bin;
}wai_fixdata_cert;



typedef struct _wai_fixdata_id{
	u16	id_flag;
	u16	id_len;
	u8 	id_data[MAX_ID_DATA_LEN];     /* ���� */
}wai_fixdata_id;

/*d)��ַ����*/
typedef struct _wai_fixdata_addid {
	u8	mac1[6];
	u8	mac2[6];
}wai_fixdata_addid;

/*WAIЭ����������ֶε��������ݶ���*/

/*ǩ���㷨*/
typedef struct _wai_fixdata_alg {
	u16		alg_length;
	u8		sha256_flag;
	u8		sign_alg;
	para_alg	sign_para;
}wai_fixdata_alg;

/*a)ǩ������*/
typedef struct _wai_attridata_auth_payload{
	u8				identifier;             	/*�������Ʊ�ʶ */
	u8				pad8[3];
	u16 				length;                 	/*����*/
	u16				pad16[2];
	wai_fixdata_id		 id;				/*���*/
	wai_fixdata_alg	alg;				/*ǩ���㷨*/
	comm_data		sign_data;		/*ǩ��*/
}wai_attridata_auth_payload;


/*b)֤����֤���*/
typedef struct _wai_attridata_auth_result{
	u8				identifier;				/*�������Ʊ�ʶ*/
	u8 				pad;
	u16 				length;                 		/*����*/
	u8				ae_challenge[32];		/*һ���������*/ 
	u8  				asue_challenge[32];	/*һ���������*/ 
	u8				auth_result1;			/*��֤���1*/
	wai_fixdata_cert 	cert1;				/*֤��1*/
	u8				auth_result2;			/*��֤���2*/
	u8 				pad8;
	wai_fixdata_cert	cert2;				/*֤��2*/
}wai_attridata_auth_result;

/*c)����б�*/
#define MAX_ID_NO 10
typedef struct _wai_attridata_id_list{
	u8		identifier;             	/* �������Ʊ�ʶ */
	u16 		length;                 	/* ����*/
	u8		rev;		
	u16  		id_no;			/*��ݸ���*/
	wai_fixdata_id	id[MAX_ID_NO];	/*���*/
}wai_attridata_id_list;


 /* 2��STA��AP֮��ļ���ṹ����*/
 /* 1) ͨ������ͷ������*/
 struct _packet_head {
	u16  version; 		/* �汾�� */
	u8	type;			/*WAI����*/
	u8  	sub_type;		/* ����������� */
	u16  	reserved;		/* ���� */
	u16  	data_len;		/* ���ݳ��� */ 
	u16 	group_sc;		/*�������*/
	u8   	frame_sc;		/*��Ƭ���*/
	u8 	flag;				/*��Ƭ��־*/
} ;
typedef struct _packet_head packet_head, *ppacket_head;

/*
 2) ���𼤻����
*/
/*
 * b) ���𼤻����
 */
 struct _auth_active {
 	wai_fixdata_flag	flag;				/*��ʶ�ֶ�*/
	u8				pad_vlue[3];      	/* ʵ�ֶ��������  */ 
	u8				ae_auth_flag[32];	/*�����ʶ*/
	wai_fixdata_id		asu_id;			/*ASU�����*/
	wai_fixdata_cert 	ae_cer;              	/* AE ��֤�� */
	para_alg			ecdh;			/*ecdh�㷨*/
};
typedef struct _auth_active    auth_active,  *pauth_active;

/*
 * 3) ��������������
 */
struct _sta_auth_request {
	wai_fixdata_flag	flag;				/*��ʶ�ֶ�*/
	u8				ae_auth_flag[32];	/*�����ʶ*/
	u8				asue_challenge[32];/*asue ��ս*/
	byte_data			key_data;		/*asue ��Կ����*/
	wai_fixdata_id		ae_id;			/*ae ���*/
	wai_fixdata_cert	asue_cert;		/*asue ֤��*/
	wai_attridata_id_list	asu_id_list;		/*asue ���ε�asu�б�*/
	para_alg			ecdh;
	wai_attridata_auth_payload	asue_sign;/*asue ǩ��*/
} ;
typedef struct _sta_auth_request    sta_auth_request;

/*
 * 4) ���������Ӧ����
*/
struct _cert_check_result {
	wai_attridata_auth_result 		cert_result;			/*������*/	
	wai_attridata_auth_payload	asue_trust_asu_sign;	/*ASUE ���ε�ASUǩ��*/
	wai_attridata_auth_payload	ae_trust_asu_sign;	/*AE ���ε�ASUǩ��*/
};
typedef struct _cert_check_result cert_check_result;

struct _auth_response {
	wai_fixdata_flag	flag;				/*��ʶ�ֶ�*/
	u8				pad_vlue[3];      	/* ʵ�ֶ��������  */ 
	u32				asue_challenge;	/*asue ��ս*/
	u32				ae_challenge;		/*ae��ս*/
	u8				access_result;	/*������*/
	u8				pad_vlue1[3];      	/* ʵ�ֶ��������  */ 
	byte_data			asue_key_data;	/*asue ��Կ����*/
	byte_data			ae_key_data;		/*ae ��Կ����*/
	wai_fixdata_id		ae_id;			/*ae ���*/
	wai_fixdata_id		asue_id;			/*asue ���*/
	cert_check_result 	cert_result;		/*���ϵ�֤����֤���*/
	wai_attridata_auth_payload	ae_sign;	/*ae ǩ��*/
} ;
typedef struct _auth_response auth_response;

/*
 * 5) ��ԿЭ���������
 */
struct _session_key_neg_request {
	wai_fixdata_flag	flag;		/*��ʶ�ֶ�*/
	u8		pad_vlue[3];		/* ʵ�ֶ��������*/ 
	u8		bkid[16];			/*����Կ���� */
	u8		uskid;			/*��Կ��ʶID*/
	u8		pad_vlue1[3];		/* ʵ�ֶ��������*/
	wai_fixdata_addid	addid;	/*��ַ���� */
	u8		ae_challenge[32];	/*ae��ս */
} ;
typedef struct _session_key_neg_request session_key_neg_request;

/*
 * 6) ��ԿЭ����Ӧ����
 */
/* ��ԿЭ����Ӧ����ṹ */
struct _session_key_neg_response {
	wai_fixdata_flag	flag;			/*��ʶ�ֶ�*/
	u8				pad_vlue[3]; 	/* ʵ�ֶ��������  */ 
	u8      			bkid[16];		/*����Կ���� */
	u8				uskid; 		/*��Կ��ʶID*/
	u8				pad_vlue1[3];	/* ʵ�ֶ��������  */
	wai_fixdata_addid	addid;		/*��ַ���� */
	u8				asue_challenge[32];  /*asue��ս */
	u8				ae_challenge[32];  /*ae��ս */
	u8				wie_asue[255];
	u8				mic[20];
} ;
typedef struct _session_key_neg_response session_key_neg_response;

/*�׼�*/
typedef struct _suite_haha {
	u8		oui[3];
	u8		suite_type;
}suite_haha;
/*������Ϣ�ֶ�*/
typedef struct _wapi_capability {
	u16	pre_auth:1;
	u16 	reserved:15;
}wapi_capability;

/*WAPI��ϢԪ��*/
typedef struct	_wapi_para_index { 
	u8			element_id;				/*ID*/
	u8			len;						/*����*/
	u16			ver;						/*�汾*/
	u16			akm_no;					/*AKM����*/
	suite_haha	akm_suite[MAX_AKM_NO];	/*AKM*/
	u16			unicast_key_no;			/*������Կ�׼����*/
	suite_haha	unicast_key_suite[MAX_UNICAST_NO];/*������Կ��*/
	suite_haha	multicast_key_suite;		/*�鲥��Կ�׼�*/	
	wapi_capability	capability;				/*������Ϣ*/
	u16		bkid_no;					/*BKID����*/
	u16		bkid_list[MAX_BKID_NO];	/*BKID�б�*/
}wapi_para_index;

/*
 * 7) ��ԿЭ��ȷ�Ϸ���
 */
 struct _session_key_neg_confirm {
	wai_fixdata_flag	flag;			/*��ʶ�ֶ�*/
	u8				pad_vlue[3];	/* ʵ�ֶ��������  */ 
	u8     			bkid[16];		/*����Կ���� */
	u8				uskid;		/*��Կ��ʶID*/
	u8				pad_vlue1[3];	/* ʵ�ֶ��������  */
	wai_fixdata_addid	addid;		/*��ַ���� */
	u8				asue_challenge[32];  /*asue��ս */
	wapi_para_index	wie_ae;
	u8				mic[20];
} ;
typedef struct _session_key_neg_confirm session_key_neg_confirm;

/*
 *8)�鲥��Կͨ�����
 */
struct _groupkey_notice {
	wai_fixdata_flag	flag;				/*��ʶ�ֶ�*/
	u8				notice_keyid;		/*ͨ����Կ��������0��1֮�䷭ת*/
	u8				uskid;              	 /*��Կ��ʶID*/
	u8				pad_vlue[1];      	/* ʵ�ֶ��������  */ 
	wai_fixdata_addid	addid;			/*��ַ���� */
	u8				g_sn[16];  		/*������� */
	u32				g_nonce[4];		/*��Կͨ���ʶ */
	byte_data			negotiate_data;	/*��Կͨ������ */
	u8				mic[20];  
} ;
typedef struct _groupkey_notice groupkey_notice;

/*
*9)�鲥��Կͨ����Ӧ����
*/
struct _groupkey_notice_response {
	wai_fixdata_flag	flag;				/*��ʶ�ֶ�*/
	u8				notice_keyid;		/*ͨ����Կ��������0��1֮�䷭ת*/
	u8				uskid;               	/*��Կ��ʶID*/
	u8				pad_vlue[1];      	/* ʵ�ֶ��������  */ 
	wai_fixdata_addid	addid;			/*��ַ���� */
	u32			g_nonce[4];		/*��Կͨ���ʶ */
	u8				mic[20];     
};
typedef struct _groupkey_notice_response groupkey_notice_response;


/*
 * 3) ֤�������Ӧ���ģ�
 */
 struct _ap_auth_response { 
	wai_fixdata_addid	addid;		/*��ַ���� */
	cert_check_result	cert_result;	/*���������Ӧ����*/	
} ;
typedef struct _ap_auth_response ap_auth_response;
#endif

