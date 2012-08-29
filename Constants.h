/*
File: CommonStructs.h
Desc:
    ���õĳ�������

Note:

*/

#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#include "FeatureCustom.h"
/*
//buttons positions
const CPoint BTNONE_LOCATION(11, 3);
const CPoint BTNTWO_LOCATION(11, 3 + 51);
const CPoint BTNTHREE_LOCATION(11, 3 + 51*2);
const CPoint BTNFOUR_LOCATION(11, 3 + 51*3);
const CPoint BTNFIVE_LOCATION(11, 3 + 51*4);
const CPoint BTNSIX_LOCATION(11, 3 + 51*5); //11-2
*/

//button size
const int BUTTON_WIDTH = 75;
const int BUTTON_HEIGHT = 80;

#ifdef FEATURE_VERSION_NOMENU
	const int MENU_HEIGHT = 0;  
#else
	const int MENU_HEIGHT = 24;  
#endif

//PhoneBook Constants
const int PB_EMAIL_MAX = 200;
const int PB_MARK_MAX = 201;
const int PB_LINE_MAX = 824; /*һ����¼���ܳ��ȣ��������������룬�ʼ�����ע��ͼƬ·��(260)������1λ��������*/
const int PB_RECORD_MAX     = 500;
const int PB_GROUP_MAX =20;
const int DC_RECORD_MAX     = 200;
const int DATACARD_RECORD_MAX = 300;

const int PB_INDEX_MAX = 5;
const int PB_NAME_MAX = 32;
const int PB_NUM_MAX = 32;		//wyw_0423 mofify ԭ����40
const int PB_NAME_SIM_MAX = 14; //��λ��ASICC���NAME����Ϊ14
const int PB_NAME_PC_MAX = 16; //��λ��Unicode���NAME����Ϊ16
const int PB_NAME_UCS_MAX = 6; //��λ��Unicode���NAME����Ϊ6
#ifdef FEATURE_HAIER_PHONEBOOK
const int PB_NUM_SIM_MAX = 20; //sim ������ĳ���
#endif


//Sms Constants
const int SMS_RECORD_MAX = 500;
const int SMS_CHAR_MAX = 160;
const int SMS_CHINESE_CHAR_MAX = 70;
const int SMS_RECORDLINE_LENGTH = 256;
//const int SMS_GROUPSEND_MAX = 40;
const int SMS_GROUPSEND_MAX = 10;//Flexi����
const int SMS_ME_NUM_MAX = 512;
const int SMS_SM_NUM_MAX = 256;
const int SMS_SC_NUM_MAX = PB_NUM_MAX;

//const int SMS_CONCAT8_ASCII_MAX = 153;
//const int SMS_CONCAT8_GB_MAX = 67;

const int SMS_CONCAT8_ASCII_MAX = 153;//FLEXI ���� 
const int SMS_CONCAT8_GB_MAX = 63;//FLEXI ����

const int SMS_CONCAT16_ASCII_MAX = 151;
const int SMS_CONCAT16_GB_MAX = 66;
const int SMS_CONCAT_ASCII_MAX = SMS_CONCAT8_ASCII_MAX;
const int SMS_CONCAT_GB_MAX = SMS_CONCAT8_GB_MAX;
//const int SMS_CONCAT_SEGMENT_MAX = 5;
const int SMS_CONCAT_SEGMENT_MAX = 10;//FLEXI ����
const int SMS_RCVCONCBUF_MAX = 15;
const int SMS_RCVCONC_TIMEOUT = 90000; //90�볬ʱ
const int SMS_EXTRACTNUM_MAX = 10;
const int SMS_TOTAL_RECORD_MAX = SMS_RECORD_MAX+SMS_ME_NUM_MAX+SMS_SM_NUM_MAX;

//Call Constants
const int HIST_ITEM_MAX = 50;
const int LOG_ITEM_MAX  = 1000;
const int SEARCH_NODATA = -1;
const int RING_TIMES = 1;

//Internet
const int CONNECTION_PROFILE_MAX = 256;
const int CONNECTION_APN_LEN = 100;

//Company ID
//�����������Ŀ�������ļ��еĶ���һ��
#define COMP_ONDA		    2
#define COMP_ONDA_9508		3
#endif