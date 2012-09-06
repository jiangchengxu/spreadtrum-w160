// HSDPADlg.h : header file
//

#if !defined(AFX_HSDPADLG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_)
#define AFX_HSDPADLG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_

#include "CommonStructs.h"  // Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHSDPADlg dialog

#include "BaseDialog.h"
#include "TabDialog.h"

#include "SkinWin\SkinWin.h"
#include "SkinWin\SkinButton.h"

#include "CallDlg.h"
#include "SmsDlg.h"
#include "PhoneBookDlg.h"
#include "SetupDlg.h"

#include "dlgconnect.h"
#include "NetStatistics.h"
#include "ScreenStatic.h"
#include "PopDlg.h"
#include "PinEx.h"
#include "PinClckDlg.h"
#include "ModifyPinDlg.h"
#include "ProgressDlg.h"

//////////////////////////////////////////////////////////////////////////
#include "ConnectionPage.h"
#include "ProfilesPage.h"
#include "StatisticsPage.h"
#include "StatusPage.h"

#include "TSQueue.h"
#include "Notice.h"

#include "UserMsgIdDef.h"
#include "ReadfalshSMS.h"
//////////////////////////////////////////////////////////////////////////

#define IDT_NETWORK_INFO    (WM_USER+0x011a)

typedef enum {
    SYNCINITFUNCRET_DONE,
    SYNCINITFUNCRET_SND_ERR,
    SYNCINITFUNCRET_RSP_TO,
    SYNCINITFUNCRET_MAX
} EnSyncInitFuncRetType;

typedef enum {
    SYNCINITFUNCID_STARTPOI = 0,
    SYNCINITFUNCID_SSAM = SYNCINITFUNCID_STARTPOI,
    // SYNCINITFUNCID_CFUN,
    //SYNCINITFUNCID_ROAM,
    // SYNCINITFUNCID_COPSFORMAT,

    SYNCINITFUNCID_COPS,
    // SYNCINITFUNCID_NWRAT,

    SYNCINITFUNCID_CSQ,//liub
    SYNCINITFUNCID_CMGF,

//  SYNCINITFUNCID_CSCA,

    SYNCINITFUNCID_CSDH,
    ///  SYNCINITFUNCID_CPMS,
    SYNCINITFUNCID_CNMI,
//  SYNCINITFUNCID_CLIP,
    SYNCINITFUNCID_CSCS,
    SYNCINITFUNCID_CGMR,
    SYNCINITFUNCID_HMSGP,
    SYNCINITFUNCID_CSMP,

    SYNCINITFUNCID_CGSN,//test
//   SYNCINITFUNCID_SYSCONFIG,
//  SYNCINITFUNCID_PS,
    SYNCINITFUNCID_NULL,
    SYNCINITFUNCID_MAX,
} EnSyncInitFuncID;

#define SYNCINIT_TIMEOUT_SHORT    30000
#define SYNCINIT_TIMEOUT_LONG     60000         //wyw_0122 modify  以前是90000
#define UNICODE


class CHSDPADlg : public CBaseDialog, public NoticeSubscriber
{
public:
    int xSize;//add by liub 1124 (显示器分辨率X)
    int ySize;//add by liub 1124 (显示器分辨率y)

    int VoicemailIndex;
    int SmsSyncNumAllOK;
    CString tempvoicemailASCUS;
    BOOL bInitstatus;
    StSmsRecord CFTTempRecord;
    StSmsRecord VoicemailRecord;

    int SMS_Priority;
    BOOL VoiceMail;
    CString VoiceMailCount;//add by liub for voicemail
    static CReadFalshSMS *m_pReadFalshSMSDlg;
    typedef EnSyncInitFuncRetType(CHSDPADlg::*SyncFuncType)();
    CPopDlg* m_pSmsPopDlg;
    CPopDlg* m_pCallPopDlg;
    BOOL   m_bPinProtected;
    CPinEx m_cHandlePin;
    CRITICAL_SECTION m_csSyncInitMask;
    BOOL m_bSyncInitMask;
    BOOL m_bSimReady;
    void RegSyncInitFunc();
    BOOL m_bInComSms;
    BOOL m_bInComCall;
    BOOL m_bInitMask;
    BOOL m_bEnableClip;
    TCHAR m_szSyncInitInfo[SYNCINITFUNCID_MAX][2][32];
    void UpdateUnreadSmsIcon();
    void UpdateFullSmsIcon();
    void ResetSyncData();
    WORD GetTotalSmsNum(EnSmsType type, EnLocFilter locFilter);
    SyncFuncType m_pSyncFuncTbl[SYNCINITFUNCID_MAX];
    BOOL m_bPBSS;
    BOOL m_bSMSS;
    bool m_bussdflag;

    int m_nSimLock;
    HANDLE m_hSimLockEvt;

    EnSyncInitFuncRetType SndAtPowerCFUN(EnPowerType nPowerType);
    EnSyncInitFuncRetType SndAtPowerCFUNQ(EnPowerType nPowerType);
    EnSyncInitFuncRetType SndAtCMEE();
    BOOL AtSendCVMI();//add by liub for voicemail
    BOOL m_bPCONLINE;

    TCHAR m_szPLMN[512];
    EnRoamStatus m_RoamStatus;
    EnSrvStatus m_SrvStatus;
    EnNetWorkType m_NetworkType;
    EnNetWorkSrv m_NetworkSrv;
    EnNetCallLog m_NetCallLog;
    EnSyncInitFuncRetType AtSndCNMI();
    EnSyncInitFuncRetType AtSndCMGF();
    EnSyncInitFuncRetType AtSndCSCA();
    BOOL SyncInitFunc(int nStatus = 0);
    HANDLE m_hSyncInitEvt;
    HANDLE m_hSyncSmsInitEvt;
    HANDLE m_hSyncCVMIEvt;

    HANDLE m_hSyncPconlineInitEvt;

    HANDLE m_hSyncPbmInitEvt;
    HANDLE m_hSyncSimInitEvt;
    BOOL m_bPS;
    BOOL m_nNwSrv;
    CScreenStatic m_StatusIcomBkgroud;//add by liub 1030 for Status Icon background
public:
    void RegisterDsAutoMsgRsp();
    static void AtRespRing(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSimReady(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespHandSetCHG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespNoCarrier(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespDialTone(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCMTI(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCMT(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespRVMFB(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum); //add by liub for voicemail
    static void AtRespCVMI(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum); //add by liub for voicemail
    static void AtRespRVMFBUPDATA(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum); //add by liub for voicemail
    static void AtRespRSSI(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespNWCHG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespPLMNCHG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespNWSRVCHG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespROAMCHG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSPREADY(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSIDLOCK(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSIND(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespMODE(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSYSINFO(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCREG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCLIP(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespECIND(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCOPS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCOPSFormat(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespNWRAT(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespROAM(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCSQ(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSSAM(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespATE0(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCLVL(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCSDH(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCPINQ(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespReadNewSms(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCLCKsc2(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespQSMSS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespQPCONLINE(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespQSIDLOCK(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespQPHBS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCFUN(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSPPRAS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCNMI(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCMGF(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCSCA(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespClip(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCSCS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCPMS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCGMR(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCGSN(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSYSCONFIG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespPHMOD(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespSRVSTATUSCHG(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCDS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

    static void AtResSndBeforeClose(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespHVPRIV(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

    static void AtRespQCPDSI(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespPS(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

    static void RspAtSmsQCMGD(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCMGR(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    //add by liub
    static void RspAtSmsQHMSGP(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtSmsQCSMP(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

    static void RspAtPowerCFUN(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtPowerCFUNQ(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void RspAtCMEE(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtSmsQCMGR(USHORT index);
    BOOL SndAtSmsQCMGD(USHORT index);
    BOOL ProgressOpen(int nUpper = 0, int nStep = 0);
    BOOL ProgressClose();
    BOOL ProgressStep();
    BOOL ProgressSet(int nLower, int nUpper, int nStep);

    EnSyncInitFuncRetType AtSndCNMI(int para1, int para2);
    EnSyncInitFuncRetType AtSndCMGF(EnSmsMode mode);

    //EnSyncInitFuncRetType AtSndCSCA(char *pSmsCentreNum);

    EnSyncInitFuncRetType AtSndATE0();
    EnSyncInitFuncRetType AtSndCLVL();
    EnSyncInitFuncRetType AtSndCSDH();
    EnSyncInitFuncRetType AtSndCPINQ();
    EnSyncInitFuncRetType AtSndQSMSS();
    EnSyncInitFuncRetType AtSndQPHBS();
    EnSyncInitFuncRetType AtSndCOPS();
    EnSyncInitFuncRetType AtSndSICLOCK();
    EnSyncInitFuncRetType AtSndPCONLINE();
    EnSyncInitFuncRetType AtSndOPCONLINE();

    EnSyncInitFuncRetType AtSndCOPSFormat();
    EnSyncInitFuncRetType AtSndNWRAT();
    EnSyncInitFuncRetType AtSndROAM();
    EnSyncInitFuncRetType AtSndCSQ();
    EnSyncInitFuncRetType AtSndSSAM();
    EnSyncInitFuncRetType AtSndSPPRAS();
    EnSyncInitFuncRetType AtSndCPMS();


    EnSyncInitFuncRetType AtSndCFUN();
    EnSyncInitFuncRetType AtSndCLIP();
    EnSyncInitFuncRetType AtSndCSCS();
    //CGMR firmware version
    EnSyncInitFuncRetType AtSndCGMR();
    //CGMR IMEI
    EnSyncInitFuncRetType AtSndCGSN();
    EnSyncInitFuncRetType AtSndSYSCONFIG();

    EnSyncInitFuncRetType CHSDPADlg::AtSndQCPDSI();

    //add by liub
    EnSyncInitFuncRetType SndAtSmsQHMSGP();
    EnSyncInitFuncRetType SndAtSmsQCSMP();

    BOOL AtSndBeforeClose();
    BOOL AtSndCOPSEx();
    static void AtRespCOPSEx(LPVOID pWnd, BYTE(*strArr)[DSAT_STRING_COL], WORD wStrNum);

    CHSDPADlg(CWnd* pParent = NULL);    // standard constructor
    virtual ~CHSDPADlg();
    virtual BOOL SubClassButton(HWND hwnd);
    virtual int ActionAfterInitDlgWithFlashEffect();
    virtual int DealwithResponseProc(LPARAM lParam, WPARAM wParam);
    /************************************************************************/
    /* for closing ports in sleep mode                                     */
    /************************************************************************/
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);



    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    TSQueue<NoticeRCP> m_noticeRCPs;

    void SelectDevice(std::string& deviceName);
    void DeselectDevice();
    void SendRequest(std::string& msgStr);
    void OnPublisherNotify
    (
        const Publisher<NoticeRCP>& publisher,
        const NoticeRCP& noticeRCP
    );

    bool OpenUserGuide(const char* relativePath);
    void ProcessMsgNotice(NoticeRCP noticeRCP);
    void ProcessReqMsgNotice(MessageRCP msgRCP, uint32 msgUID);
    void ProcessRspMsgNotice(MessageRCP msgRCP, uint32 msgUID);
    void ProcessIndMsgNotice(MessageRCP msgRCP, uint32 msgUID);
    void ProcessWDSStartReq(MessageRCP msgRCP);
    void ProcessWDSDeleteProfileReq(MessageRCP msgRCP);
    void ProcessWDSGetProfileSettingsReq(MessageRCP msgRCP);
    void ProcessWDSGetDefaultSettingsReq(MessageRCP msgRCP);
    void ProcessWDSSetEventReportRsp(MessageRCP msgRCP);
    void ProcessDMSSetEventReportRsp(MessageRCP msgRCP);
    void ProcessNASSetEventReportRsp(MessageRCP msgRCP);
    void ProcessWDSEventReportInd(MessageRCP msgRCP);
    void ProcessDMSEventReportInd(MessageRCP msgRCP);
    void ProcessNASEventReportInd(MessageRCP msgRCP);
    void ProcessDMSGetPowerStateRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceMfrRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceModelIdRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceRevIdRsp(MessageRCP msgRCP);
    void ProcessDMSGetMsisdnRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceSerialNumbersRsp(MessageRCP msgRCP);
    void ProcessDMSGetDeviceCapRsp(MessageRCP msgRCP);
    void ProcessNASGetSignalStrengthRsp(MessageRCP msgRCP);
    void ProcessNASGetServingSystemRsp(MessageRCP msgRCP);
    void ProcessNASServingSystemInd(MessageRCP msgRCP);
    void ProcessWDSAbortRsp(MessageRCP msgRCP);
    void ProcessWDSStartRsp(MessageRCP msgRCP);
    void ProcessWDSStopRsp(MessageRCP msgRCP);
    void ProcessWDSPktSrvcStatusInd(MessageRCP msgRCP);
    void ProcessWDSGetCurrentChannelRateRsp(MessageRCP msgRCP);
    void ProcessWDSCreateProfileRsp(MessageRCP msgRCP);
    void ProcessWDSDeleteProfileRsp(MessageRCP msgRCP);
    void ProcessWDSModifyProfileSettingsRsp(MessageRCP msgRCP);
    void ProcessWDSGetProfileListRsp(MessageRCP msgRCP);
    void ProcessWDSGetProfileSettingsRsp(MessageRCP msgRCP);
    void ProcessWDSGetDefaultSettingsRsp(MessageRCP msgRCP);
    void ProcessStatusNotice(NoticeRCP noticeRCP);
    void ProcessDeviceNotice(NoticeRCP noticeRCP);
    void LogMessage(MessageRCP msgRCP);
    void ReportMsgFailed(std::string name, uint16 error, bool isMsgBox);

    ConnectionPage  m_connectionPage;
    ProfilesPage    m_profilesPage;
    StatisticsPage  m_statisticsPage;
    StatusPage      m_statusPage;

    BOOL m_ProcessDeviceNotice;
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


// Dialog Data
    //{{AFX_DATA(CHSDPADlg)
    enum { IDD = IDD_HSDPA_DIALOG };
    CScreenStatic   m_cHandSet;
    CScreenStatic   m_cRoam;
    CScreenStatic   m_cUEType;
    CScreenStatic   m_cRssiInfo;
    CScreenStatic   m_cSmsUnrdInfo;
    CScreenStatic   m_cSmsFullInfo;
    CScreenStatic   m_cPlmnInfo;
    CScreenStatic   m_cCallPrivacyInfo;
    CScreenStatic   m_cPlmnInfo2;
    CScreenStatic   m_cNWSrvType;
    CScreenStatic   m_cUpSpeed;
    CScreenStatic   m_cDwSpeed;
    CScreenStatic   m_cUpFlux;
    CScreenStatic   m_cDwFlux;
    CScreenStatic   m_cVoiceSms;//add by liub for voicemail
    //}}AFX_DATA
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHSDPADlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
// Implementation
public:
    void SetBottomIconPos();
    EnCMModePref m_CMModePref;
    EnGWAcqOrderPref m_GWAcqOrderPref;
    BOOL ProcSmsRcvConcTimeout(UINT nTimerID);
    BOOL ProcSmsRcvConcTimeout_Flexi(UINT nTimerID);
    BOOL SearchAndInsertSmsSege(StSmsRecord &record);
    void CtlAllSmsRcvConcTimer(BOOL bOpen);
    void RcvNewSmsProc(EnLocType loctype, StSmsRecord &record, WORD nIndex = 0);
    void ConvertIMEI();
    void ForegroundWnd();
    BYTE                    m_nRingTimes;
    //char                    *SkinStyle;
    HICON                   m_hIcon;
    HCURSOR                 m_hAniCursor;
    CSmsDlg*                m_pSmsDlg;
    CSetupDlg*              m_pSetupDlg;
    CSetupDlg*              m_pHelpDlg;
    CPhoneBookDlg*          m_pPhoneBookDlg;
    CCallDlg*               m_pCallDlg;
//    CInternetDlg*           m_pInternetDlg;
    CDlgConnect*           m_pBrowseDlg;
    //CInternetDlg*           m_pBrowseDlg;
    CDlgConnect*             m_pConnectDlg;
    CNetStatistics*          m_pUsageDlg;  //Throughput statistics
    CNetStatistics*         m_pUsageDlg1;

    CTabDialog*             m_pTabDialog;
    NOTIFYICONDATA          m_nid;            // struct for Shell_NotifyIcon args
    //用于区分是否为RESET时关闭的端口，为真则是，为假则表示非RESET时的端口关闭，此时应关闭应用程序
    bool m_bPortStatus;

    /* 1-CMT(new message); 2-CPT(blank message &call back); 3-CFT(Flash message); 4-CDS(回送报告) 5-VOCEMAIL;
       在析构函数中暂时默认设置为1，每次来短信必须根据下位机头信息更改值*/
    int SMS_type_CPTCMTCFTCDS_VOICEMAIL;
    //IMEI
    CString m_strIMEI;

    //Firmware version
    CString m_strFWVersion;

    //Software version
    CString m_strSWVersion;




    int m_nHotKeyID;

    BOOL InitTabDialog();
    BOOL AddPagesToTabDialog();
    BOOL AddPageToTabDialog(CTabedDialog* pDlg, UINT nDLGID, RECT ButtonRect, UINT nButtonTextID, UINT nIButtonDD);
    // Generated message map functions
    //{{AFX_MSG(CHSDPADlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
    afx_msg void OnAppAbout();
    afx_msg void OnAppTrayMaxi();
    afx_msg void OnClose();
    //}}AFX_MSG
    afx_msg LRESULT OnDial(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT ChangeSkin(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT SelContactIndex(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnAddContact(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnPopDlg(WPARAM wParam = 0 , LPARAM lParam = 0);
    afx_msg LRESULT SwitchTo(WPARAM wParam , LPARAM lParam);
    afx_msg LRESULT PbReDraw(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT CheckPinStat(WPARAM wParam = 0 , LPARAM lParam = 0);
    afx_msg LRESULT PinVerify(WPARAM wParam = 0 , LPARAM lParam = 0);
    afx_msg LRESULT UpdateIcon(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSmsFullNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSmsInitNotify(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCreateProgress(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT WindowRestore(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT WindowMax(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SearchContactInList(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT GetListTotalCount(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnNewNotice(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnInternetSetup(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnInitSMSAndPB(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnHotKey(WPARAM wParam = 0, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
private:
    void GetVersionInfo();
    void UpdateSmsDlg();
    void UpdateInboxWhenRcvNewSms(EnLocType loctype, StSmsRecord &record, WORD nIndex = 0, BOOL bAdd = TRUE, BOOL bCopyUE = FALSE);
    int m_nNewSmsIndex;
    char m_szNewSmsLoc[5];
    EnLocType m_NewSmsLoc;
    CSerialPort *m_pComm;
    CPbData     *m_pPbData;
    EnSmsFullState m_SmsFullState;
    USHORT ME_SmsNum, ME_SmsMax, SM_SmsNum, SM_SmsMax;
    USHORT ME_SmsCnt, SM_SmsCnt;
    USHORT m_nReadIndex;
    CSmsData *m_pSmsData;
    int m_nStep;
    int m_nUpper;
    int m_nLower;
    CProgressDlg* pDlg;

    CMyBitmap m_bmpBkStatus;        //wyw_0207 add 下面的状态栏的背景图片
    HANDLE m_hMonitorThread;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSDPADLG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_)
