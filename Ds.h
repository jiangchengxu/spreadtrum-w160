#ifndef _AFX_DS_H_
#define _AFX_DS_H_

#include "FeatureCustom.h"
#include "SerialPort\SerialPort.h"

#define PIN_LEN 4

#define AT_FLAG_CR 0x0D
#define AT_FLAG_LF 0x0A
#define AT_FLAG_S3 AT_FLAG_CR
#define AT_FLAG_S4 AT_FLAG_LF

#define AT_FLAG_MR '>'
#define AT_FLAG_SP 0x20

#define UPCASE(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c))

//AT命令结果码
typedef enum
{
#ifndef FEATURE_ATAMOI
    DSAT_OK = 0,
    DSAT_ERROR,
    DSAT_RING,
    DSAT_NO_CARRIER,
    DSAT_DIALTONE,
    DSAT_CME_ERROR,
    DSAT_CMS_ERROR,
	DSAT_CMS_ERROR_500,
    DSAT_CMTI,
    DSAT_CMT,
	DSAT_RVMFB,//add by liub
	DSAT_RVMFBUPDATE,
    DSAT_RSSI,
    DSAT_CSMSS, 
    DSAT_CPBSS, 
    DSAT_CLIP,
    DSAT_NWCHG, 
    DSAT_NWSRVCHG,
    DSAT_ROAMCHG,
    DSAT_HANDSET,
    DSAT_SIMREADY,
    DSAT_PLMNCHG,
    DSAT_SRVSTATUSCHG, 
    DSAT_CDS, 
	DSAT_CUSD,
	DSAT_PS,   
#else
    DSAT_OK = 0,
    DSAT_ERROR,
    DSAT_CME_ERROR,
    DSAT_CMS_ERROR,
	DSAT_CMS_ERROR_500,
    DSAT_ATAMOI,
    DSAT_RING,
    DSAT_NO_CARRIER,
    DSAT_DIALTONE,
    DSAT_CMTI, 
    DSAT_CMT, 
	DSAT_RVMFB,//add by liub
	DSAT_RVMFBUPDATE,
    DSAT_RSSI,
    DSAT_CSMSS, 
    DSAT_CPBSS, 
    DSAT_CLIP,
    DSAT_NWCHG, 
    DSAT_NWSRVCHG, 
    DSAT_ROAMCHG,
    DSAT_HANDSET,
    DSAT_SIMREADY,
    DSAT_PLMNCHG, 
    DSAT_SRVSTATUSCHG, 
    DSAT_CDS, 
	DSAT_CUSD,
	DSAT_PS,  
#endif
	DSAT_HVPRIV,
	DSAT_SIDLOCK,
#ifdef FEATURE_HAIER_CM
	DSAT_CEND,
	DSAT_HCEND,
	DSAT_CFNM,
	DSAT_SMEMFUL,
	DSAT_VMGFS,
	DSAT_CONN,
	DSAT_CANS,
#endif
    DSAT_MAX
} EnDsatResCode;


//AT命令结果码模式
typedef enum
{
    DSAT_MODE_DIGIT = 0,    //数字
    DSAT_MODE_STRING,        //字符串
    DSAT_MODE_MAX
} EnDsatResMode;

#define DSAT_STRING_ROW 30        //处理结果字符串最大数目
#define DSAT_STRING_COL 800        //处理结果字符串最大长度

typedef enum
{
    BGEVT_SMS = 0, 
    BGEVT_CALL, 
    BGEVT_CLIP,
    BGEVT_END, 
    BGEVT_ARRNUM,
} EnBGEvt;

#define BG_SMS_ARRNUM 50
#define BG_STRING_ROW 1
#define BG_STRING_COL DSAT_STRING_COL

//DS,BG线程使用的消息CMTI通知队列
typedef struct
{
    CRITICAL_SECTION cs;
    BYTE StrArr[BG_SMS_ARRNUM][BG_STRING_ROW][BG_STRING_COL];
    WORD StrNum[BG_SMS_ARRNUM];
    BYTE nReadIndex;
    BYTE nWriteIndex;
    WORD wRxCount;
} StBGSmsQueue;

extern HANDLE g_BGPassEvt;
extern HANDLE g_BGReadNewSmsEvt;
extern HANDLE g_BGEvtArr[BGEVT_ARRNUM];


//AT命令处理回调函数
#if 0
typedef void (*AtRespFunc)(CWnd* pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
#else
typedef void (*AtRespFunc)(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
#endif

//回调函数类型
typedef enum
{
    ATRESP_RING = 0,    /*来电提示*/
    ATRESP_CMTI,        /*来短信,非CLASS0*/
    ATRESP_CMT,         /*来短信，CLASS0*/
    ATRESP_NO_CARRIER,  /*无载波*/
    ATRESP_DIALTONE,    /*网络侧挂断语音呼叫或通话*/
    ATRESP_RSSI,        /*场强变化*/
    ATRESP_CSMSS,       /*SMS初始化结束通知*/
    ATRESP_CPBSS,       /*PBM初始化结束通知*/
    ATRESP_CLIP,        /*来电号码通知*/
    ATRESP_NWCHG,       /*网络类型变化通知*/
    ATRESP_NWSRVCHG,    /*网络服务变化通知*/
    ATRESP_ROAMCHG,     /*漫游状态变化通知*/
    ATRESP_HANDSETCHG,  /*耳机插拔通知*/
    ATRESP_SIMREADY,    /*SIM卡初始化完成通知*/
    ATRESP_PLMNCHG,     /*注册网络变化通知*/
    ATRESP_SRVSTATUSCHG,/*服务状态变化通知*/
    ATRESP_CDS,         /*消息发送报告*/
	ATRESP_RVMFB,       //VOICEMAIL ADD BY LIUB
	ATRESP_RVMFBUPDATA, //VOICEMAIL更新条数
    ATRESP_CUSD,         /*ussd*/
	ATRESP_PS,         /*数据业务*/
	ATRESP_HVPRIV,
	ATRESP_SIDLOCK,
    ATRESP_GENERAL_AT,  /*通用AT*/
    ATRESP_MAX
} EnAtRespFuncType;

//AT命令响应结构
#if 0
typedef struct
{
    AtRespFunc m_AtRespFunc; //处理AT响应的函数
    CWnd       *m_pWnd;         //处理AT响应的窗口
} StAtResp;
#else
typedef struct
{
    AtRespFunc m_AtRespFunc; //处理AT响应的函数
    LPVOID     m_pWnd;         //处理AT响应的窗口
} StAtResp;
#endif

//Ds处理的事件
typedef enum
{
    DSEVENT_ATRESP = 0,    //从串口收到数据
    DSEVENT_ATTIMEOUT,     //APP普通命令超时
    DSEVENT_END,           //结束线程
    DSEVENT_ARRNUM
} EnDsEventType;

//AT响应解析状态机
typedef enum
{
    STATE_START = 0,
    STATE_HEAD_S3,
    STATE_HEAD_S4,
    STATE_TAIL_S3,
    STATE_FIND_CONTENT,
    STATE_RESCODE_S3,
    STATE_FIND_MR,
    STATE_END
} EnDsatState;

extern const EnDsatResMode gc_dsatmode;
extern const char gc_dsatResCodeTbl[DSAT_MAX][DSAT_MODE_MAX][30];

extern HANDLE g_AtRespEvent;
extern HANDLE g_AppRegEvt;
extern HANDLE g_EndEvent;
extern HANDLE g_AtTimeout;

extern void AtTimeoutNotify();
#if 0
extern void RegisterAtRespFunc(EnAtRespFuncType type, AtRespFunc func, CWnd* pWnd);
#else
extern void RegisterAtRespFunc(EnAtRespFuncType type, AtRespFunc func, LPVOID pWnd);
#endif
extern void DeRegisterAtRespFunc(EnAtRespFuncType type);
extern void DsEventCreate();
extern void DsEventDestroy();
extern UINT DsThreadProc(LPVOID pParam);
extern UINT BGThreadProc(LPVOID pParam);
//Sms At related variables and functions

typedef enum {
    SMS_QCSMP_TYPE_MT = 0,
    SMS_QCSMP_TYPE_MO,
} EnSmsQcsmpType;

typedef enum {
    AT_SMS_QCPMS = 0, //存储设置
    AT_SMS_QCMGR,     //读
    AT_SMS_QCMGW,     //写
    AT_SMS_QCMGD,     //删
    AT_SMS_QCMGS,     //发
    AT_SMS_QCSCA,     //消息中心
    AT_SMS_QCMGF,     //消息模式
    AT_SMS_QCNMI,     //路由设置
    AT_SMS_QCSMP,     //写发配置
	AT_SMS_QHMSGL,    //短消息语言和编码设置(CDMA2000)//add by liub 
	AT_SMS_QHMSGP,    //短消息其他参数设置(CDMA2000)//add by liub 
    AT_SMS_MAX
} EnAtSmsType;


#ifdef FEATURE_HAIER_SMS
typedef enum{
	STATE_OA_NUMBER = 0,
	STATE_DAYTIME_YEAR,
	STATE_DAYTIME_MONTH,
	STATE_DAYTIME_DAY,
	STATE_DAYTIME_HOUR,
	STATE_DAYTIME_MINUTE,
	STATE_DAYTIME_SECOND,
	STATE_LANGUAGE,
	STATE_FORMAT,
	STATE_LENGTH,
	STATE_PRIORITY,
	STATE_SECURITY,
	STATE_TYPE,
	STATE_STATUS,
	STATE_SMS_END,
}EnAtSmsContextState;

const char gcstrAtSms[AT_SMS_MAX][15] = {
    "AT+CPMS=",
        "AT^HCMGR=",
        "AT^HCMGW=",
        "AT+CMGD=",
        "AT^HCMGS=",
        "AT+CSCA=",
        "AT+CMGF=",
        "AT+CNMI=",
        "AT+CSMP=",
		"AT^HSMSSS=",//add by liub for SMS CDMA2000
		"AT^HSMSSS=",//add by liub for SMS CDMA2000
};

const char gcstrResSms[AT_SMS_MAX][15] = {
    "+CPMS:",
        "^HCMGR:",
        "^HCMGW:",
        "+CMGD:",
        "^HCMGS:",
        "+CSCA:",
        "+CMGF:",
        "+CNMI:",
        "+CSMP:",
};
#else
const char gcstrAtSms[AT_SMS_MAX][15] = {
    "AT+CPMS=",
        "AT+CMGR=",
        "AT+CMGW=",
        "AT+CMGD=",
        "AT+CMGS=",
        "AT+CSCA=",
        "AT+CMGF=",
        "AT+CNMI=",
        "AT+CSMP=",
		"AT+HMSGL=",//add by liub for SMS CDMA2000
		"AT+HMSGP=",//add by liub for SMS CDMA2000
};

const char gcstrResSms[AT_SMS_MAX][15] = {
    "+CPMS: ",
        "+CMGR: ",
        "+CMGW: ",
        "+CMGD: ",
        "+CMGS: ",
        "+CSCA: ",
        "+CMGF: ",
        "+CNMI: ",
        "+CSMP: ",
};
#endif

const char gcstrSmsState[SMS_STATE_MAX][SMS_MODE_MAX][15] = {
    "0", "REC UNREAD",
        "1", "REC READ",
        "2", "STO UNSENT",
        "3", "STO SENT"
};
//add by liub for voicemail
const char gcstrVoicemailRes[2][50] = {
	"+CVMR: No voice mail",
	"+CVMR: No voice mail to view",
};

//Flexi需求，长短信拆分时显示Min@Max格式时可代替中间的@的字符集
// add by liub for Flexi
const char LMS_Flexi_MinMaxChar[10]= {
	'*','-','/',':','~','.','#','^','|','%',
};

const char LMS_Flexi_RcvConcNoAll[6] = {"<...>"};//flexi长短信占位省略符

const EnSmsMode gSmsMode = SMS_MODE_TEXT;

const char gcstrLoc[LOC_MAX][3] = {
    "PC", "ME", "SM"
};

const char gcstrSmsPrompt[5] = {"> "};
const char gccCtrl_Z = 0x1A;

const char gcstrNumType[2][4] = {
    "145", "129",
};

extern EnSmsState GetSmsStateFromStr(LPCTSTR str);
extern LPCSTR GetSmsStrFromState(EnSmsState state);
extern BOOL GetTimeFromStr(LPCSTR str, COleDateTime &time);

extern BOOL IsValidCallNumChar(UINT nChar);
extern BOOL IsValidSmsNumChar(UINT nChar);

//电源设置参数
const char gcstrSetPower[POWER_MAX][2] = 
{    
    "0", //电源关
    "1", //电源开
};

//网络服务名称
const UINT gcstrNetWorkSrv[NW_SRV_MAX+2] = 
{    
    IDS_UNKNOWN,     //NW_SRV_NONE ,
    IDS_CONNTYPE_GPRS,     //NW_SRV_GPRS,
    IDS_CONNTYPE_EDGE,     //NW_SRV_EDGE,
    IDS_CONNTYPE_HSDPA,    //NW_SRV_HSDPA,
};

//版本查询特殊号码
const TCHAR szVersionNum[VERSION_MAX][10] = {
    _T("*#06#"), 
    _T("*7638#"),
};

//版本对话框标题
const UINT szVersionTitle[VERSION_MAX] = {
    IDS_IMEI, 
    IDS_FIRMWARE_VERSION,
};
extern BOOL UnicodeIsHighStr(TCHAR *wchar);

extern BOOL CStringisHighStr(CString str);

extern BOOL IsAlphabetUnicode(const TCHAR *str);
extern int WCharToUnicode(const TCHAR *WChar, char* unicode);
extern int WCharToChar(const TCHAR *wchar, char* cstr);
//GB转UCS2处理函数
extern CString GBToUCS2(const char *strGb);
extern int  UCS2ToUCS2(CString strGb, CString strUcs2);
//UCS2转GB处理函数
extern CString UCS2ToGB(const CString &strUcs2);
#ifdef FEATURE_HAIER_SMS
extern char * WCharToGB(const wchar_t *strUcs2);
extern wchar_t * GBTOWChar(CString str);
#endif
extern CString BTToUCS2(const TCHAR*strGb);
extern void ASCToUCS2(const char* unicode, TCHAR* WChar);
extern BOOL ASCHEXToWchar(const char* ascii, TCHAR* WChar);
extern BOOL IsGsm7bitChar(const wchar_t uc);
//判断文本中的字符是否全部在GSM-7bit编码表中
extern BOOL IsAlphabet(const char *str);
//获得双字节字符集文本串的字符个数
extern USHORT GetUnicodeCharNum(const TCHAR *str);
extern USHORT GetACSIICharNum(const TCHAR *str);

extern void AtRespDummy(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

#ifdef FEATURE_SMS_PDUMODE
extern void DecodeNumFormSmsPDU(const char *pdunum, char *pOutNum);
extern void DecodeTimeFormSmsPDU(const char *pdutime, CTime *pOutTime);
extern void DecodeContentFromSmsPDU(const char *pduContent, const BYTE codeType, char *pContent);
extern void DecodeSmsPDU(const char *pdu, const USHORT len, StSmsRecord *pRecord);
#endif

extern BOOL ExtractConcatenateSmsPara(char *Para, USHORT *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt);
extern BOOL ExtractConcatenateSmsPara_Flexi(CString Para, TCHAR *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt);//flexi版本，解析长短信参数
#ifdef FEATURE_HAIER_SMS
extern BOOL ExtractConcatenateSmsPara_ChinaTel(char *Para, int *nRefCnt, int *nSeqCnt, int *nTotalCnt);
#endif
//Modified by Zhou Bin 2008.12.30
//extern BOOL DspConcatenateSmsPara(TCHAR *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt);
extern BOOL DspConcatenateSmsPara(char *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt);
extern BOOL SetConcatenateSmsPara(TCHAR*DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt);
extern BOOL SetConcatenateSmsParaA(char *DspBuf, USHORT nRefCnt, BYTE nSeqCnt, BYTE nTotalCnt, char MinMaxChar);
extern BOOL IsConcatenateSms(StSmsRecord *pSmsRecord);

#ifdef FEATURE_SMS_CONCATENATE 
extern BYTE gSmsRefCnt;
extern BOOL gSmsIsConcatenate;
extern BOOL gSmsIsAsciiCode;
extern BYTE gSmsTotalSege;
extern BYTE gSmsCurSege;
//Modified by Zhou Bin 2008.12.30
extern TCHAR gszSmsSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];
//extern TCHAR gszSmsSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];
extern void ResetSmsConcatenateData();
//extern BOOL DivideSmsConcatenate(const TCHAR *szRawContent);
extern BOOL DivideSmsConcatenate(const TCHAR *szRawContent);
#endif

//国家码列表
extern const char szCountryCodeArr[][10];
extern BOOL SmsAtCMGRRspProc(BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum, StSmsRecord &record, const EnSmsKind kind = SMS_KIND_ALL);

#define GSM0338_EXTENSIONS (2)
#define GSM0338_ALPHABET_SIZE (0x80)
extern const wchar_t GSM0338ToUCS2Array[GSM0338_EXTENSIONS][GSM0338_ALPHABET_SIZE];

extern StRcvConcSmsRecord gSmsRcvConcBUf[SMS_RCVCONCBUF_MAX];
extern void ResetSmsRcvConcBuf(BYTE nIndex);
extern int SaveSmsSegeToRcvConcBuf(const StSmsRecord &sege);
extern int MergeConcSmsSegeTogether(StSmsRecord &record);

extern BYTE gSmsTranRefCnt;
extern BOOL gSmsTranIsConcatenate;
extern BOOL gSmsTranIsAsciiCode;
extern BYTE gSmsTranTotalSege;
extern BYTE gSmsTranCurSege;
extern TCHAR gszSmsTranSege[SMS_CONCAT_SEGMENT_MAX][SMS_CONCAT_ASCII_MAX + 1];

extern void ResetSmsTranConcatenateData();
extern BOOL DivideSmsTranConcatenate(const TCHAR *szRawContent);

extern const BYTE gc_sms_validity_period[];
extern TCHAR gSmsCentreNum[SMS_SC_NUM_MAX+2];

// extern BYTE gSmsExtractNumCnt;
// extern TCHAR gSmsExtractNumBuf[SMS_EXTRACTNUM_MAX][PB_NUM_MAX+2];
// extern BYTE ExtractNumFromSmsContent(const TCHAR *szRawContent);


extern BYTE gSmsExtractNumCnt;
extern char gSmsExtractNumBuf[SMS_EXTRACTNUM_MAX][PB_NUM_MAX+2];
extern BYTE ExtractNumFromSmsContent(const char *szRawContent);

extern BOOL g_bSmsSyncComm;
extern USHORT g_ME_SmsCnt;
extern USHORT g_ME_SmsNum;
extern USHORT g_ME_SmsMax;
extern USHORT g_SM_SmsCnt;
extern USHORT g_SM_SmsNum;
extern USHORT g_SM_SmsMax;
extern StSmsCardRecord *g_ME_SmsRecord;
extern StSmsCardRecord *g_SM_SmsRecord;
extern StSmsRecord g_RcvSmsRecord;

extern void UE_InitSmsBuf();
extern void UE_ReleaseSmsBuf();
extern StSmsCardRecord UE_ReadSmsCardRecord(EnLocType type, WORD nIndex);
extern void UE_EditSmsCardRecord(EnLocType type, WORD nIndex, const StSmsRecord &record);
extern BOOL UE_DeleteSmsCardRecord(EnLocType type, WORD nIndex);
extern BOOL UE_AddSmsCardRecord(EnLocType type, const StSmsCardRecord &record);
extern BOOL UE_AddSmsCardRecord(EnLocType type, const StSmsRecord &record, const WORD &nIndex);
extern WORD UE_GetUnreadSmsNum(EnLocType loctype);
extern BOOL UE_ClearSmsFlag(EnLocType loctype, BYTE Clearflag, BOOL bAll = TRUE);
extern BOOL UE_SmsIsFull(EnLocType loctype);
extern int  UE_SmsFindCardRecord(EnLocType loctype, WORD nIndex);

extern int g_DataCardTotalNum;
extern int g_USimTotalNum; 
extern int g_DataCardUsedNum;
extern int g_USimUsedNum; 

extern StPbRecord* pUSIMPbRecord;
extern StPbRecord* pDataCardPbRecord;
extern bool gbPcCardInit;  /*PCCARD是否已读取数据至内存中，真为已读取*/
extern bool gbSimInit;     /*SIM是否已读取数据至内存中，真为已读取*/

extern BOOL g_bDataService;
extern BYTE g_nDataConnType;
extern BOOL g_bConnLimitTip;
extern CString    m_str_sms_Prompt;
extern int gCurAutoPref;

#endif //#ifndef _AFX_DS_H_