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

//AT��������
typedef enum {
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
    DSAT_SPREADY,
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
    DSAT_HVPRIV,
    DSAT_SIDLOCK,
    DSAT_SIND,
    DSAT_MODE,
    DSAT_SYSINFO,
    DSAT_CREG,
    DSAT_CGREG,
    DSAT_ECIND,
    DSAT_MAX
} EnDsatResCode;


//AT��������ģʽ
typedef enum {
    DSAT_MODE_DIGIT = 0,    //����
    DSAT_MODE_STRING,        //�ַ���
    DSAT_MODE_MAX
} EnDsatResMode;

#define DSAT_STRING_ROW 30        //�������ַ��������Ŀ
#define DSAT_STRING_COL 800        //�������ַ�����󳤶�

typedef enum {
    BGEVT_SMS = 0,
    BGEVT_CALL,
    BGEVT_CLIP,
    BGEVT_END,
    BGEVT_ARRNUM,
} EnBGEvt;

#define BG_SMS_ARRNUM 50
#define BG_STRING_ROW 1
#define BG_STRING_COL DSAT_STRING_COL

//DS,BG�߳�ʹ�õ���ϢCMTI֪ͨ����
typedef struct {
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


//AT�����ص�����
typedef void (*AtRespFunc)(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

//�ص���������
typedef enum {
    ATRESP_RING = 0,    /*������ʾ*/
    ATRESP_CMTI,        /*������,��CLASS0*/
    ATRESP_CMT,         /*�����ţ�CLASS0*/
    ATRESP_NO_CARRIER,  /*���ز�*/
    ATRESP_DIALTONE,    /*�����Ҷ��������л�ͨ��*/
    ATRESP_RSSI,        /*��ǿ�仯*/
    ATRESP_SPREADY,       /*SMS PBM ��ʼ������֪ͨ*/
    ATRESP_CLIP,        /*�������֪ͨ*/
    ATRESP_NWCHG,       /*�������ͱ仯֪ͨ*/
    ATRESP_NWSRVCHG,    /*�������仯֪ͨ*/
    ATRESP_ROAMCHG,     /*����״̬�仯֪ͨ*/
    ATRESP_HANDSETCHG,  /*�������֪ͨ*/
    ATRESP_SIMREADY,    /*SIM����ʼ�����֪ͨ*/
    ATRESP_PLMNCHG,     /*ע������仯֪ͨ*/
    ATRESP_SRVSTATUSCHG,/*����״̬�仯֪ͨ*/
    ATRESP_CDS,         /*��Ϣ���ͱ���*/
    ATRESP_RVMFB,       //VOICEMAIL ADD BY LIUB
    ATRESP_RVMFBUPDATA, //VOICEMAIL��������
    ATRESP_CUSD,         /*ussd*/
    ATRESP_PS,         /*����ҵ��*/
    ATRESP_HVPRIV,
    ATRESP_SIDLOCK,
    ATRESP_SIND,
    ATRESP_MODE,
    ATRESP_SYSINFO,
    ATRESP_CREG,		/*��CGREG��ͬ*/
    ATRESP_ECIND,
    ATRESP_GENERAL_AT,  /*ͨ��AT*/
    ATRESP_MAX
} EnAtRespFuncType;

//AT������Ӧ�ṹ
typedef struct {
    AtRespFunc m_AtRespFunc; //����AT��Ӧ�ĺ���
    LPVOID     m_pWnd;         //����AT��Ӧ�Ĵ���
} StAtResp;

//Ds������¼�
typedef enum {
    DSEVENT_ATRESP = 0,    //�Ӵ����յ�����
    DSEVENT_ATTIMEOUT,     //APP��ͨ���ʱ
    DSEVENT_END,           //�����߳�
    DSEVENT_ARRNUM
} EnDsEventType;

//AT��Ӧ����״̬��
typedef enum {
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
extern void RegisterAtRespFunc(EnAtRespFuncType type, AtRespFunc func, LPVOID pWnd);
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
    AT_SMS_QCPMS = 0, //�洢����
    AT_SMS_QCMGR,     //��
    AT_SMS_QCMGW,     //д
    AT_SMS_QCMGD,     //ɾ
    AT_SMS_QCMGS,     //��
    AT_SMS_QCSCA,     //��Ϣ����
    AT_SMS_QCMGF,     //��Ϣģʽ
    AT_SMS_QCNMI,     //·������
    AT_SMS_QCSMP,     //д������
    AT_SMS_QHMSGL,    //����Ϣ���Ժͱ�������(CDMA2000)//add by liub
    AT_SMS_QHMSGP,    //����Ϣ������������(CDMA2000)//add by liub
    AT_SMS_QCMMS,	//������//add by jcx
    AT_SMS_MAX
} EnAtSmsType;

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
    "AT+CMMS=",
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
    "+CMMS: "
};

const char gcstrSmsState[SMS_STATE_MAX][SMS_MODE_MAX][15] = {
    "0", "REC UNREAD",
    "1", "REC READ",
    "2", "STO UNSENT",
    "3", "STO SENT",
    "4", "ALL"
};
//add by liub for voicemail
const char gcstrVoicemailRes[2][50] = {
    "+CVMR: No voice mail",
    "+CVMR: No voice mail to view",
};

//Flexi���󣬳����Ų��ʱ��ʾMin@Max��ʽʱ�ɴ����м��@���ַ���
// add by liub for Flexi
const char LMS_Flexi_MinMaxChar[10] = {
    '*', '-', '/', ':', '~', '.', '#', '^', '|', '%',
};

const char LMS_Flexi_RcvConcNoAll[6] = {"<...>"};//flexi������ռλʡ�Է�

#ifdef FEATURE_SMS_PDUMODE
const EnSmsMode gSmsMode = SMS_MODE_PDU;
#else
const EnSmsMode gSmsMode = SMS_MODE_TEXT;
#endif

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

//��Դ���ò���
const char gcstrSetPower[POWER_MAX][2] = {
    "0", //��Դ��
    "1", //��Դ��
};

//�����������
const UINT gcstrNetWorkSrv[NW_SRV_MAX+2] = {
    IDS_UNKNOWN,     //NW_SRV_NONE ,
    IDS_CONNTYPE_GPRS,     //NW_SRV_GPRS,
    IDS_CONNTYPE_EDGE,     //NW_SRV_EDGE,
    IDS_CONNTYPE_HSDPA,    //NW_SRV_HSDPA,
};

//�汾��ѯ�������
const TCHAR szVersionNum[VERSION_MAX][10] = {
    _T("*#06#"),
    _T("*7638#"),
};

//�汾�Ի������
const UINT szVersionTitle[VERSION_MAX] = {
    IDS_IMEI,
    IDS_FIRMWARE_VERSION,
};

/*
** Define a type for the type of network.
*/
typedef enum uinetwk_network_type_e
{
   UI_NETWK_TYPE_UNKNOWN_TYPE,
   UI_NETWK_TYPE_GSM_900,
   UI_NETWK_TYPE_DCS_1800,
   UI_NETWK_TYPE_PCS_1900,
   UI_NETWK_TYPE_GSM_SAT,
   UI_NETWK_TYPE_UMTS
} uinetwk_network_type_e_type;

/*
** Define a type that contains the networks, country code, network code
** and name.
*/
typedef struct uinetwk_network_info_s
{

   unsigned int                  mcc;
     /* Mobile Network Code                                */

   unsigned int                  mnc;
    /* Mobile Country Code                                 */

   uinetwk_network_type_e_type   network_type;

   char                         *short_name_ptr;
     /* Pointer to a null terminated string containing the */
     /* network's short name.                              */

   char                         *full_name_ptr;
     /* Pointer to a null terminated string containing the */
     /* network's full name.                               */

} uinetwk_network_info_s_type;

extern BOOL UnicodeIsHighStr(TCHAR *wchar);

extern BOOL CStringisHighStr(CString str);

extern BOOL IsAlphabetUnicode(const TCHAR *str);
extern int WCharToUnicode(const TCHAR *WChar, char* unicode);
extern int WCharToChar(const TCHAR *wchar, char* cstr);
//GBתUCS2������
extern CString GBToUCS2(const char *strGb);
extern int  UCS2ToUCS2(CString strGb, CString strUcs2);
//UCS2תGB������
extern CString UCS2ToGB(const CString &strUcs2);
extern CString BTToUCS2(const TCHAR*strGb);
extern void ASCToUCS2(const char* unicode, TCHAR* WChar);
extern BOOL ASCHEXToWchar(const char* ascii, TCHAR* WChar);
extern BOOL IsGsm7bitChar(const wchar_t uc);
//�ж��ı��е��ַ��Ƿ�ȫ����GSM-7bit�������
extern BOOL IsAlphabet(const char *str);
//���˫�ֽ��ַ����ı������ַ�����
extern USHORT GetUnicodeCharNum(const TCHAR *str);
extern USHORT GetACSIICharNum(const TCHAR *str);
extern char* strtrim(char * p);

extern void AtRespDummy(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

#ifdef FEATURE_SMS_PDUMODE
extern void DecodeFOFormSmsPDU(StSmsRecord *pRecord, char *fo);
extern int DecodeUDHFormSmsPDU(StSmsRecord *pRecord, char *fo);
extern void DecodeNumFormSmsPDU(const char *pdunum, char *pOutNum);
extern void DecodeTimeFormSmsPDU(const char *pdutime, COleDateTime *pOutTime);
extern void DecodeContentFromSmsPDU(const char *pduContent, const BYTE codeType, char *pContent);
extern void DecodeSmsPDU(const char *pdu, const USHORT len, StSmsRecord *pRecord);
extern int EncodeSmsPDU(char *pduOut, CString da, CString context);
extern int EncodeSCNumberForSmsPDU(char *sbuffer);
#endif

extern BOOL ExtractConcatenateSmsPara(char *Para, USHORT *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt);
extern BOOL ExtractConcatenateSmsPara_Flexi(CString Para, TCHAR *nRefCnt, BYTE *nSeqCnt, BYTE *nTotalCnt);//flexi�汾�����������Ų���
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

//�������б�
extern const char szCountryCodeArr[][10];
extern BOOL SmsAtCMGRRspProc(BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum, StSmsRecord &record, const EnSmsKind kind = SMS_KIND_ALL);

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
extern BOOL get_network_info(unsigned int mcc, unsigned int mnc, uinetwk_network_info_s_type  *info_ptr);

extern int g_DataCardTotalNum;
extern int g_USimTotalNum;
extern int g_DataCardUsedNum;
extern int g_USimUsedNum;

extern StPbRecord* pUSIMPbRecord;
extern StPbRecord* pDataCardPbRecord;
extern bool gbPcCardInit;  /*PCCARD�Ƿ��Ѷ�ȡ�������ڴ��У���Ϊ�Ѷ�ȡ*/
extern bool gbSimInit;     /*SIM�Ƿ��Ѷ�ȡ�������ڴ��У���Ϊ�Ѷ�ȡ*/

extern BOOL g_bDataService;
extern BYTE g_nDataConnType;
extern BOOL g_bConnLimitTip;
extern CString    m_str_sms_Prompt;
extern int gCurAutoPref;

#endif //#ifndef _AFX_DS_H_
