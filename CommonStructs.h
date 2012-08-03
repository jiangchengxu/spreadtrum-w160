/*
File: CommonStructs.h
Desc:
    公用的结构定义

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
Desc:    电话本
*/
/*************************************************************************/
struct StPbRecord
{
	int index;
    TCHAR strName[PB_NAME_MAX+1];        //姓名
    TCHAR strMobile[PB_NUM_MAX+1];        //手机
    TCHAR strHome[PB_NUM_MAX+1];            //家庭
	TCHAR strGroup[PB_NUM_MAX+1];			//群组
    TCHAR strCompany[PB_NUM_MAX+1];        //公司
    TCHAR strEmail[PB_EMAIL_MAX+1];        //email
    TCHAR strMark[PB_MARK_MAX+1];        //备注
    TCHAR strPicPath[MAX_PATH+1];
};

/*************************************************************************/
/*
Name:    StPbGroup
Desc:    电话本群组
*/
/*************************************************************************/
struct StPbGroup
{
	int index;
    TCHAR strGroupName[PB_NAME_MAX+1];        //群组名   
};



/*************************************************************************/
/*
Name:    PHONEBOOK_LISTCTRL_HEADERINDEX
Desc:    电话本列表控件头枚举
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
Desc:    电话本数据卡联系人列表控件头枚举
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
Desc:    选择联系人窗口列表控件头枚举
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
Desc:    弹出CContactDlg类型枚举
*/
/*************************************************************************/
typedef enum 
{
    CONTACTDLG_TYPE_EXTRACT=-3, //提取号码
	CONTACTDLG_TYPE_MODIFY=-2,
	CONTACTDLG_TYPE_ADD=-1,
	CONTACTDLG_TYPE_NORMAL=0
}CONTACTDLG_TYPE;

/*************************************************************************/
/*
Name:    EnSmsState
Desc:    短消息状态枚举

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
Desc:    短消息种类枚举

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
Desc:    短消息类型枚举

*/
/*************************************************************************/
typedef enum
{
    SMS_TYPE_INBOX,        //收件箱
    SMS_TYPE_OUTBOX,    //发件箱
    SMS_TYPE_DRAFT,        //草稿箱
	SMS_TYPE_ARCHIVE,   //归档
    SMS_TYPE_ALL
}EnSmsType;

/*************************************************************************/
/*
Name:    SMS_LISTCTRL_HEADERINDEX
Desc:    短消息列表控件头枚举
*/
/*************************************************************************/
typedef enum
{
    SMS_LISTCTRL_HEADERINDEX_NAME,        //姓名
    SMS_LISTCTRL_HEADERINDEX_DATETIME,    //日期
    SMS_LISTCTRL_HEADERINDEX_CONTENT,    //内容
    SMS_LISTCTRL_HEADERINDEX_NUMBER    //号码
}SMS_LISTCTRL_HEADERINDEX;

/*************************************************************************/
/*
Name:    StSmsRecord
Desc:    短消息记录结构
*/
/*************************************************************************/
#define SMS_RECORD_FLAG_NEW 0x01
#define SMS_RECORD_FLAG_CONCATENATE_SEGE 0x02
#define SMS_RECORD_FLAG_CONCATENATE_FULL 0x04
#define SMS_RECORD_FLAG_COPY_UE          0x08

struct StSmsRecord
{
    BYTE          flag;                              //标记
    EnSmsState    state;                            //状态
    char       szNumber[PB_NUM_MAX+2];        //号码
    COleDateTime        timestamp;                        //时间戳
	//CTime        timestamp;
//    USHORT      nRefCnt;                        //长短信标识号
    TCHAR      nRefCnt;                        //长短信标识号(flexi LMS)
    BYTE        nSeqCnt;                        //长短信当前分段号
    BYTE        nTotalCnt;                      //长短信总分段号
    char        szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 2];    //短信内容
    char        szSCNumber[SMS_SC_NUM_MAX];  //消息中心号码
	int         SMSpriority;                  //短信优先级
	BOOL        m_NoATRspCDS;                  //回送报告（FALSE表示为回送报告，TRUE为非回送报告）
	int         voicemail;                    //是否是语音消息 TATA需求
	int         segeindex[SMS_CONCAT_SEGMENT_MAX][1];//长短信未收全时记录未收全的部分位置（未收全的部分会用“<...>”代替，
	                                              //记录的是每个部分此字符串的位置）
};

struct StSmsCardRecord
{
    USHORT index;
    StSmsRecord record;
};

struct StRcvConcSmsRecord
{
	BOOL		bInuse;							//该项可用否
	UINT		nTimerID;						//定时器
    USHORT      nRefCnt;                        //长短信标识号
    BOOL        nSeqFlag[SMS_CONCAT_SEGMENT_MAX]; //长短信当前分段号
    BYTE        nTotalCnt;                      //长短信总分段号
    TCHAR        szNumber[PB_NUM_MAX + 2];        //号码
    COleDateTime       timestamp;                        //时间戳
    TCHAR        szContent[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX*2 + 2];    //短信内容
    TCHAR        szSCNumber[SMS_SC_NUM_MAX +2];  //消息中心号码
};

/*************************************************************************/
/*
Name:    ID_STATIC_CONTROL_STYLE
Desc:    控件类型枚举
*/
/*************************************************************************/
typedef enum
{
    ID_STATIC,
    ID_GROUP_BOX
}ID_STATIC_CONTROL_STYLE;

/*用于ListCtrl操作的行信息*/
struct ROWINFO{
    DWORD dwData;
    DWORD dwStates;
    CStringArray aTexts;
};

//电话本，短消息存储位置类型
typedef enum {
    LOC_PC = 0,
    LOC_ME,
    LOC_UIM,
    LOC_MAX
} EnLocType;

//短消息优先存储位置
typedef enum {
    SMS_PRESTOR_ME = 0, 
    SMS_PRESTOR_UIM, 
    SMS_PRESTOR_PC, 
    SMS_PRESTOR_MAX
} EnSmsPreStor;

//电话本，短消息存储位置过滤
typedef enum {
    LOCFILTER_PC = 0, 
    LOCFILTER_ME, 
    LOCFILTER_UIM, 
    LOCFILTER_ALL, 
	LOCFILTER_MAX
} EnLocFilter;

//电话本，短消息排序索引
typedef struct {
	EnLocType LocType;	//消息存储类型
	WORD nIndex;		//消息内存序号
} StDspEntityType;

//短消息模式
typedef enum {
    SMS_MODE_PDU = 0,
    SMS_MODE_TEXT,
    SMS_MODE_MAX
} EnSmsMode;

//配置文件结构
//字符型结构
struct stStringType 
{
    LPCTSTR lpSectionName;
    LPCTSTR lpKeyName;
    LPCTSTR lpDefault;    
    DWORD nSize;
};

//整型结构
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
	//CDMA2000 SMS 优先级设置,0:Urgent, 1:Emergency, 2:General, 3:Interactive
	int Messages_nPriority;
	//CDMA2000 SMS 有效期设置
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


//网络类型
typedef enum {
    NW_TYPE_NOSRV = 0, 
    NW_TYPE_GSM, 
    NW_TYPE_WCDMA, 
	NW_TYPE_EDGE, 
	NW_TYPE_CDMA2000,
    NW_TYPE_MAX
} EnNetWorkType;

//网络服务
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

//CallLog 记录网络状态
typedef enum {
    NW_CALLLOG_NOSRV = 0, 
    NW_CALLLOG_GSM, 
    NW_CALLLOG_WCDMA, 
	NW_CALLLOG_CDMA1X,
	NW_CALLLOG_CDMA_EVDO,
    NW_CALLLOG_MAX
}EnNetCallLog;

//漫游状态
typedef enum {
    ROAM_OFF = 0, 
    ROAM_ON, 
    ROAM_FLASHING, 
    ROAM_MAX
} EnRoamStatus;

//服务状态
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

//电源状态指示
typedef enum {
    POWER_OFF = 0,   //RF OFF
    POWER_ON,		 //RF ON
    POWER_MAX,
} EnPowerType;

//通话记录
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

//网络日志
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

//日月年的流量
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

//SMS满状态
typedef enum {
    SMS_FULL_UNKNOWN = -1, 
    SMS_FULL_ME, 
    SMS_FULL_SM, 
    SMS_FULL_BOTH,
} EnSmsFullState;

//接入点类型
typedef enum {
    CONNPOINT_CMNET = 0,	//CMNET
    CONNPOINT_CMWAP,		//CMWAP
    CONNPOINT_MAX,
} EnConnPointType;

//写短消息类型
typedef enum {
    SMS_WRITE_NONE = 0, 
    SMS_WRITE_NEW, 
    SMS_WRITE_REPLY, 
    SMS_WRITE_FORWORD, 
    SMS_WRITE_EDIT, 
    SMS_WRITE_MAX,
} EnSmsWriteType;

//版本查询类型
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

//当前profile属于RAS还是NDIS
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