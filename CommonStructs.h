/*
File: CommonStructs.h
Desc:
    ���õĽṹ����

Note:


*/

#ifndef _COMMNOSTRUCTS_H
#define _COMMNOSTRUCTS_H
typedef unsigned long ULONG_PTR;

#include "Ras.h"
#include "FeatureCustom.h"
#include <tchar.h>
#include "Constants.h"

/*************************************************************************/
/*
Name:    StPbRecord
Desc:    �绰��
*/
/*************************************************************************/
struct StPbRecord
{
	int index;
    TCHAR strName[PB_NAME_MAX+1];        //����
    TCHAR strMobile[PB_NUM_MAX+1];        //�ֻ�
    TCHAR strHome[PB_NUM_MAX+1];            //��ͥ
	TCHAR strGroup[PB_NUM_MAX+1];			//Ⱥ��
    TCHAR strCompany[PB_NUM_MAX+1];        //��˾
    TCHAR strEmail[PB_EMAIL_MAX+1];        //email
    TCHAR strMark[PB_MARK_MAX+1];        //��ע
    TCHAR strPicPath[MAX_PATH+1];
};

/*************************************************************************/
/*
Name:    StPbGroup
Desc:    �绰��Ⱥ��
*/
/*************************************************************************/
struct StPbGroup
{
	int index;
    TCHAR strGroupName[PB_NAME_MAX+1];        //Ⱥ����   
};



/*************************************************************************/
/*
Name:    PHONEBOOK_LISTCTRL_HEADERINDEX
Desc:    �绰���б�ؼ�ͷö��
*/
/*************************************************************************/
typedef enum 
{
    PHONEBOOK_LISTCTRL_HEADERINDEX_NAME,
    PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE,
    PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY,
    PHONEBOOK_LISTCTRL_HEADERINDEX_HOME,
	PHONEBOOK_LISTCTRL_HEADERINDEX_GROUP,
    PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL,
    PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK
}PHONEBOOK_LISTCTRL_HEADERINDEX;

/*************************************************************************/
/*
Name:    PHONEBOOK_LISTCTRL_HEADERINDEX
Desc:    �绰�����ݿ���ϵ���б�ؼ�ͷö��
Author:    ybb
*/
/*************************************************************************/
typedef enum 
{
    PHONEBOOK_LISTCTRLDC_HEADERINDEX_NAME,
    PHONEBOOK_LISTCTRLDC_HEADERINDEX_MOBILE,    
}PHONEBOOK_LISTCTRLDC_HEADERINDEX;

/*************************************************************************/
/*
Name:    SELCONTACT_LISTCTRL_HEADERINDEX
Desc:    ѡ����ϵ�˴����б�ؼ�ͷö��
Author:    ybb
*/
/*************************************************************************/
typedef enum 
{
    SELCONTACT_LISTCTRL_HEADERINDEX_NAME,
    SELCONTACT_LISTCTRL_HEADERINDEX_MOBILE,    
    SELCONTACT_LISTCTRL_HEADERINDEX_HOME,
    SELCONTACT_LISTCTRL_HEADERINDEX_COMPANY,
    SELCONTACT_LISTCTRL_HEADERINDEX_EMAIL,
    SELCONTACT_LISTCTRL_HEADERINDEX_REMARK,
	SELCONTACT_LISTCTRL_HEADERINDEX_MAX
}SELCONTACT_LISTCTRL_HEADERINDEX;

/*************************************************************************/
/*
Name:    CONTACTDLG_TYPE
Desc:    ����CContactDlg����ö��
*/
/*************************************************************************/
typedef enum 
{
    CONTACTDLG_TYPE_EXTRACT=-3, //��ȡ����
	CONTACTDLG_TYPE_MODIFY=-2,
	CONTACTDLG_TYPE_ADD=-1,
	CONTACTDLG_TYPE_NORMAL=0
}CONTACTDLG_TYPE;

/*************************************************************************/
/*
Name:    EnSmsState
Desc:    ����Ϣ״̬ö��

*/
/*************************************************************************/
typedef enum
{
    SMS_STATE_MT_NOT_READ,        
    SMS_STATE_MT_READ,
    SMS_STATE_MO_NOT_SENT,    
    SMS_STATE_MO_SENT,
    SMS_STATE_MAX
}EnSmsState;

/*************************************************************************/
/*
Name:    EnSmsKind
Desc:    ����Ϣ����ö��

*/
/*************************************************************************/
typedef enum
{
    SMS_KIND_MT,
    SMS_KIND_MO,
    SMS_KIND_ALL,
}EnSmsKind;

/*************************************************************************/
/*
Name:    EnSmsType
Desc:    ����Ϣ����ö��

*/
/*************************************************************************/
typedef enum
{
    SMS_TYPE_INBOX,        //�ռ���
    SMS_TYPE_OUTBOX,    //������
    SMS_TYPE_DRAFT,        //�ݸ���
	SMS_TYPE_ARCHIVE,   //�鵵
    SMS_TYPE_ALL
}EnSmsType;

/*************************************************************************/
/*
Name:    SMS_LISTCTRL_HEADERINDEX
Desc:    ����Ϣ�б�ؼ�ͷö��
*/
/*************************************************************************/
typedef enum
{
    SMS_LISTCTRL_HEADERINDEX_NAME,        //����
    SMS_LISTCTRL_HEADERINDEX_DATETIME,    //����
    SMS_LISTCTRL_HEADERINDEX_CONTENT,    //����
    SMS_LISTCTRL_HEADERINDEX_NUMBER    //����
}SMS_LISTCTRL_HEADERINDEX;

/*************************************************************************/
/*
Name:    StSmsRecord
Desc:    ����Ϣ��¼�ṹ
*/
/*************************************************************************/
#define SMS_RECORD_FLAG_NEW 0x01
#define SMS_RECORD_FLAG_CONCATENATE_SEGE 0x02
#define SMS_RECORD_FLAG_CONCATENATE_FULL 0x04
#define SMS_RECORD_FLAG_COPY_UE          0x08

struct StSmsRecord
{
    BYTE          flag;                              //���
    EnSmsState    state;                            //״̬
    char       szNumber[PB_NUM_MAX+2];        //����
    COleDateTime        timestamp;                        //ʱ���
	//CTime        timestamp;
//    USHORT      nRefCnt;                        //�����ű�ʶ��
    TCHAR      nRefCnt;                        //�����ű�ʶ��(flexi LMS)
    BYTE        nSeqCnt;                        //�����ŵ�ǰ�ֶκ�
    BYTE        nTotalCnt;                      //�������ֶܷκ�
    char        szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 2];    //��������
    char        szSCNumber[SMS_SC_NUM_MAX];  //��Ϣ���ĺ���
	int         SMSpriority;                  //�������ȼ�
	BOOL        m_NoATRspCDS;                  //���ͱ��棨FALSE��ʾΪ���ͱ��棬TRUEΪ�ǻ��ͱ��棩
	int         voicemail;                    //�Ƿ���������Ϣ TATA����
	int         segeindex[SMS_CONCAT_SEGMENT_MAX][1];//������δ��ȫʱ��¼δ��ȫ�Ĳ���λ�ã�δ��ȫ�Ĳ��ֻ��á�<...>�����棬
	                                              //��¼����ÿ�����ִ��ַ�����λ�ã�
};

struct StSmsCardRecord
{
    USHORT index;
    StSmsRecord record;
};

struct StRcvConcSmsRecord
{
	BOOL		bInuse;							//������÷�
	UINT		nTimerID;						//��ʱ��
    USHORT      nRefCnt;                        //�����ű�ʶ��
    BOOL        nSeqFlag[SMS_CONCAT_SEGMENT_MAX]; //�����ŵ�ǰ�ֶκ�
    BYTE        nTotalCnt;                      //�������ֶܷκ�
    TCHAR        szNumber[PB_NUM_MAX + 2];        //����
    COleDateTime       timestamp;                        //ʱ���
    TCHAR        szContent[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX*2 + 2];    //��������
    TCHAR        szSCNumber[SMS_SC_NUM_MAX +2];  //��Ϣ���ĺ���
};

/*************************************************************************/
/*
Name:    ID_STATIC_CONTROL_STYLE
Desc:    �ؼ�����ö��
*/
/*************************************************************************/
typedef enum
{
    ID_STATIC,
    ID_GROUP_BOX
}ID_STATIC_CONTROL_STYLE;

/*����ListCtrl����������Ϣ*/
struct ROWINFO{
    DWORD dwData;
    DWORD dwStates;
    CStringArray aTexts;
};

//�绰��������Ϣ�洢λ������
typedef enum {
    LOC_PC = 0,
    LOC_ME,
    LOC_UIM,
    LOC_MAX
} EnLocType;

//����Ϣ���ȴ洢λ��
typedef enum {
    SMS_PRESTOR_ME = 0, 
    SMS_PRESTOR_UIM, 
    SMS_PRESTOR_PC, 
    SMS_PRESTOR_MAX
} EnSmsPreStor;

//�绰��������Ϣ�洢λ�ù���
typedef enum {
    LOCFILTER_PC = 0, 
    LOCFILTER_ME, 
    LOCFILTER_UIM, 
    LOCFILTER_ALL, 
	LOCFILTER_MAX
} EnLocFilter;

//�绰��������Ϣ��������
typedef struct {
	EnLocType LocType;	//��Ϣ�洢����
	WORD nIndex;		//��Ϣ�ڴ����
} StDspEntityType;

//����Ϣģʽ
typedef enum {
    SMS_MODE_PDU = 0,
    SMS_MODE_TEXT,
    SMS_MODE_MAX
} EnSmsMode;

//�����ļ��ṹ
//�ַ��ͽṹ
struct stStringType 
{
    LPCTSTR lpSectionName;
    LPCTSTR lpKeyName;
    LPCTSTR lpDefault;    
    DWORD nSize;
};

//���ͽṹ
struct stIntType 
{
    LPCTSTR lpSectionName;
    LPCTSTR lpKeyName;
    int nDefault;
};

struct stSetData
{
   TCHAR Internet_szDefaultConnName[RAS_MaxEntryName];
    TCHAR Internet_szLocalPhoneNumber[RAS_MaxPhoneNumber];
    TCHAR Internet_szDeviceName[RAS_MaxDeviceName];
    TCHAR Internet_szDeviceType[RAS_MaxDeviceType];
    TCHAR Internet_szUserName[UNLEN];
    TCHAR Internet_szPassword[PWLEN]; 

    TCHAR General_szSkinFile[256];
    TCHAR Internet_szDefaultConnection[RAS_MaxEntryName];

    TCHAR Internet_szPCUIName[256]; 
	TCHAR Internet_szPCVOICEName[256]; //wj test
	TCHAR Main_szPrefixName[32];
	TCHAR Main_szURL[256];
	TCHAR Main_szEMail[256];
	TCHAR Main_szSplashBmp[256];
	TCHAR Main_szLogoBmp[256];
	#ifdef FEATURE_VERSION_ITELCO
		TCHAR Main_szTitleLogoIcon[256];   
	#endif
	TCHAR Main_szCompany[64];
	TCHAR Main_szMainIcon[64];
	TCHAR Main_szHelpFile[256];
	//default profile name
	TCHAR Internet_szProfileName[RAS_MaxEntryName];
	TCHAR Internet_szAPN[256];
	TCHAR Main_szUsbDiskFriendlyName[256];
	TCHAR Main_szAppName[64];
	TCHAR Setup_sz3GType[10];
	
    int Internet_dwFramingProtocol;
    int Internet_dwfOptions;
    int Internet_dwType;
    int Internet_dwCountryID;
    int Internet_dwCountryCode;
    int Internet_dwfNetProtocols;
    int Internet_dwSubEntries;
    int Internet_dwEncryptionType;
    int Internet_dwSubEntry;

    int Internet_nConnections;
    int Setup_nSkinStyle;
    int Setup_nPowerManagement;
	//have call function? 1:yes, 0:no
	int Main_nCall;
	//Language
	int Main_nLanguage;
	//Company ID
	int Main_nCompany;
	////DEL: Prefered Storage. 0: PC, 1: ME, 2: USIM
	//delete PC
	//so 0:ME, 1:USIM, 2:PC
	int Messages_nPreStor;
	//Validity Period.	0: 1 hour, 1: 6 hours, 2: 1 day,
	//					3: 3 days, 4: 1 week,  5: 1 year
	int Messages_nValPeriod;
	//Delivery report. 0: Off, 1: On
	int Messages_nDeliReport;
	//have help menu? 1:yes, 0:no
	int Main_nHelp;
	//PC Card Auto run. 0:Manual run; 1:Auto run when PC card inserted
	int Main_nAutoRun;

    //Launch application when Windows start up. 1:yes, 0:no
    int Setup_nAutoLaunchApp;
    //Connect internet while starting. 1:yes, 0:no
    int Setup_nAutoConnect;
    //Start main window minimized. 1:yes, 0:no
    int Setup_nStartMini;
    //Standby/Hibernate Setting. 1:Allow, 0:Prevent
    int Setup_nAllowSleep;

	// NDIS feature supported. 1:yes, 0:no
	int Setup_nNDIS;
    //Alert window when receiving new SMS. 1:yes, 0:no
    int Messages_nAlertWindow;
    //Alert Tone when receiving new SMS. 1:yes, 0:no
    int Messages_nAlertTone;
	//CDMA2000 SMS ���ȼ�����,0:Urgent, 1:Emergency, 2:General, 3:Interactive
	int Messages_nPriority;
	//CDMA2000 SMS ��Ч������
	int Messages_nDefDelivery;
    //SMS Alert sound file path
    TCHAR Messages_szSoundFile[MAX_PATH];
	//VoiceMailNumber
	TCHAR Messages_szVoiceMailNumber[40];

    int Internet_nPromptEnable;

    int Internet_nPromptYear;
    int Internet_nPromptMonth;
    int Internet_nPromptDay;
    int Internet_nPromptFluxOrDuration;
    int Internet_nPromptFluxVal;
    int Internet_nPromptDurationVal;

    //Auth type. 0: CHAP 1: PAP
    int Internet_nAuthType;
    
    TCHAR Main_szVID[20];
    TCHAR Main_szPID[20];

	int Internet_nPromptDisconnEnable;  //added by ljl 2007.8.30
	int Internet_nPromptPwdConfirm;      //added by ljl 2007.8.30
	TCHAR  Internet_szCurrentPwd[40];   //added by ljl 2007.8.30
	int Internet_nReachUsageLimit;      //added by ljl 2007.9.17
	TCHAR Internet_szLastTimeReset[40];  //added by Zhou Bin 2008.12.06

	TCHAR Call_szEmergencyNumbers[MAX_PATH];

	int Main_nSimLock;
};


//��������
typedef enum {
    NW_TYPE_NOSRV = 0, 
    NW_TYPE_GSM, 
    NW_TYPE_WCDMA, 
	NW_TYPE_EDGE, 
	NW_TYPE_CDMA2000,
    NW_TYPE_MAX
} EnNetWorkType;

//�������
typedef enum {
  NW_SRV_NONE = -1,
  NW_SRV_GSM,
  NW_SRV_GPRS,
  NW_SRV_EDGE,
  NW_SRV_UMTS,
  NW_SRV_HSDPA,
  NW_SRV_HSUPA,
  NW_SRV_CDMA1X,
  NW_SRV_CDMA_EVDO,
  NW_SRV_MAX
} EnNetWorkSrv;

//CallLog ��¼����״̬
typedef enum {
    NW_CALLLOG_NOSRV = 0, 
    NW_CALLLOG_GSM, 
    NW_CALLLOG_WCDMA, 
	NW_CALLLOG_CDMA1X,
	NW_CALLLOG_CDMA_EVDO,
    NW_CALLLOG_MAX
}EnNetCallLog;

//����״̬
typedef enum {
    ROAM_OFF = 0, 
    ROAM_ON, 
    ROAM_FLASHING, 
    ROAM_MAX
} EnRoamStatus;

//����״̬
typedef enum {
    SRVSTATUS_NONE = 0, 
    SRVSTATUS_LIMIT, 
    SRVSTATUS_FULL, 
    SRVSTATUS_MAX
} EnSrvStatus;

typedef enum {
    ICON_TYPE_RSSI = 0, 
    ICON_TYPE_NETWORK, 
    ICON_TYPE_ROAM, 
    ICON_TYPE_SMSUNREAD, 
    ICON_TYPE_SMSFULL, 
	ICON_TYPE_CALLPRIVACY,
    ICON_TYPE_PLMN,
    ICON_TYPE_PLMN2,
//	ICON_TYPE_NSRVLN,
//	ICON_TYPE_PLMN3,
    ICON_TYPE_HANDSET,
    ICON_TYPE_NWSRV, 
    ICON_TYPE_UPSPEED, 
    ICON_TYPE_DWSPEED, 
    ICON_TYPE_UPFLUX, 
    ICON_TYPE_DWFLUX, 
	ICON_TYPE_PS,
	ICON_TYPE_VOICEMAIL,
    ICON_TYPE_MAX,
} EnIconType;

//��Դ״ָ̬ʾ
typedef enum {
    POWER_OFF = 0,   //RF OFF
    POWER_ON,		 //RF ON
    POWER_MAX,
} EnPowerType;

//ͨ����¼
typedef enum {
    HISTKIND_MIS = 0,
    HISTKIND_RCV,
    HISTKIND_SND,
    HISTKIND_MAX
} EnHistKind;

typedef enum {
    HISTCOL_NAME,
    HISTCOL_NUMBER,
    HISTCOL_TIMESTAMP,
    HISTCOL_DURATION,
    HISTCOL_FREQUENCY,
    HISTCOL_MAX
} EnHistCol;

typedef struct {
    BYTE         nSeqNum;
    TCHAR         szNumber[PB_NUM_MAX+2];
    COleDateTime cTimeStamp;
    CTimeSpan    cDuration;
} stHistItem;

typedef struct {
    USHORT     nCount;
    stHistItem ItemData[HIST_ITEM_MAX];
} stHistList;

//������־
enum _eLogColV 
{
   LOGCONNTYPE = 0,
    LOGTIMESTAMP,
    LOGDURATION,
    LOGBYTEIN,
    LOGBYTEOUT,
    Log_COL_MAX
};

typedef struct _stLogItem 
{
	//delete nLogType
    int        nLogType;
    COleDateTime    cTimeStamp;
    CTimeSpan         cDuration;
    DWORD   dwByteIn;
    DWORD   dwByteOut;
} stLogItem;

//�����������
typedef struct _stThroughput
{
	DWORD dailyIn;
	DWORD dailyOut;
	DWORD monthlyIn;
	DWORD monthlyOut;
	DWORD yearlyIn;
	DWORD yearlyOut;
}stThroughput;

typedef struct _stLogList 
{
    stLogItem mItemData[LOG_ITEM_MAX];
    int count;
}stLogList;

//SMS��״̬
typedef enum {
    SMS_FULL_UNKNOWN = -1, 
    SMS_FULL_ME, 
    SMS_FULL_SM, 
    SMS_FULL_BOTH,
} EnSmsFullState;

//���������
typedef enum {
    CONNPOINT_CMNET = 0,	//CMNET
    CONNPOINT_CMWAP,		//CMWAP
    CONNPOINT_MAX,
} EnConnPointType;

//д����Ϣ����
typedef enum {
    SMS_WRITE_NONE = 0, 
    SMS_WRITE_NEW, 
    SMS_WRITE_REPLY, 
    SMS_WRITE_FORWORD, 
    SMS_WRITE_EDIT, 
    SMS_WRITE_MAX,
} EnSmsWriteType;

//�汾��ѯ����
typedef enum {
    VERSION_IMEI = 0, 
    VERSION_FIRMWARE, 
    VERSION_MAX,
} EnVersionType;

//Network Stat 
typedef enum
{
	NW_STAT_UNKNOWN = 0,
	NW_STAT_AVAILABLE,
	NW_STAT_CURRENT,
	NW_STAT_FORBIDDEN,
}EnNWStat;

//access technology selected
typedef enum
{
	NW_ACT_GSM = 0,
	NW_ACT_GSMCOMPACT,
	NW_ACT_UTRAN,
}EnNWACT;

//Network struct
struct StNetWork
{
	EnNWStat enNWStat;
	TCHAR szOperLong[16+1];
	//short format up to 8 characters, but need 16 characters
	TCHAR szOperShort[16+1];
	UINT nOperNum;
	EnNWACT enNWAct;
};

//��ǰprofile����RAS����NDIS
typedef enum {
    INTERNET_NDIS = 0, 
    INTERNET_RAS 
} EnInternetPreConf;

typedef  enum
{
   NDIS_STATE_CONNECT = 0,
   NDIS_STATE_ABORT,
   NDIS_STATE_DISCONNECT
} NdisStateRet;

typedef enum
{
    OPENPORT_RET_NOTFOUND = 0, 
    OPENPORT_RET_SUCC, 
    OPENPORT_RET_FAIL, 
    OPENPORT_RET_MAX
} EnOpenPortRet;
typedef enum
{
    AUTHPROTOCOL_CHAP = 0, 
    AUTHPROTOCOL_PAP, 
    AUTHPROTOCOL_MAX
} EnAuthProtocol;
struct StConnProfile
{
	BOOL bDefault;
    EnAuthProtocol AuthProtocol;
    TCHAR szEntryName[RAS_MaxEntryName + 1];
    TCHAR szPhoneNumber[RAS_MaxPhoneNumber + 1];
    TCHAR szUserName[UNLEN + 1];
    TCHAR szPassword[PWLEN + 1];
	TCHAR szAPN[CONNECTION_APN_LEN + 1];
	EnInternetPreConf NetPreConfig;
};
typedef enum
{
  CM_MODE_PREF_AUTOMATIC=2,
  CM_MODE_PREF_GSM_ONLY=13,
  CM_MODE_PREF_WCDMA_ONLY=14,
  CM_MODE_PREF_MAX
} EnCMModePref;

typedef enum
{
  CM_GW_ACQ_ORDER_PREF_AUTOMATIC=0,
  CM_GW_ACQ_ORDER_PREF_GSM_WCDMA=1,
  CM_GW_ACQ_ORDER_PREF_WCDMA_GSM=2,
  CM_GW_ACQ_ORDER_PREF_MAX
} EnGWAcqOrderPref;

typedef struct {
	UINT nNameMap;
	UINT nTel1Map;
	UINT nTel2Map;
	UINT nTel3Map;
	UINT nFaxMap;
	UINT nEmail1Map;
	UINT nEmail2Map;
	UINT nURLMap;
	UINT nICQMap;
	UINT nMemoMap;
} RecordMapping;

enum EolContactIndex {
		EFirstName = 0,
		ELastName,
		EFullName,
		ENickName,
		EBusinessTelephoneNumber,
		EBusiness2TelephoneNumber,
		EHomeTelephoneNumber,
		EHome2TelephoneNumber,
		EMobileTelephoneNumber,
		EPrimaryTelephoneNumber,
		EOtherTelephoneNumber,
		EAssistantTelephoneNumber,
		ECallbackTelephoneNumber,
		ECarTelephoneNumber,
		ECompanyMainTelephoneNumber,
		ERadioTelephoneNumber,
		ETTYTDDTelephoneNumber,
		EBusinessFaxNumber,
		EHomeFaxNumber,
		EOtherFaxNumber,
		ETelexNumber,
		EEmail1Address,
		EEmail2Address,
		EEmail3Address,
		EWebPage,
		EIMAddress,
		EBody,
		EnItemCount,
};

/* ndis support*/
#ifdef FEATURE_NDIS_SUPPORT
typedef enum{
	CONN_TYPE_NDIS = 0,
	CONN_TYPE_RAS,
	CONN_TYPE_MAX
}enConnType;
#endif
/*ndis support*/
#endif    //_COMMNOSTRUCTS_H