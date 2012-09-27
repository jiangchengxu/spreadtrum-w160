#if !defined(AFX_DLGCONNECT_H__506C56C9_CA21_4DC4_B942_19836C6528AF__INCLUDED_)
#define AFX_DLGCONNECT_H__506C56C9_CA21_4DC4_B942_19836C6528AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgconnect.h : header file
//

#include "TabedDialog.h"//add by liub 1023
#include "ConnProfile.h"
#include "Ds.h"
#include "InternetConnTip.h"
#include "CopsInitTip.h"
#include "dlgconnect.h"



/////////////////////////////////////////////////////////////////////////////
// CDlgConnect dialog

class CDlgConnect : public CTabedDialog
{
// Construction
public:
    CDlgConnect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgConnect)
    enum { IDD = IDD_DIALOG_CONNECT };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    int nCGDCONTType;	//0:CGDCONT set, 1:CGDCONT?, 2:CGDCONT=?
    CString m_strLastAPN;
    //EnNetWorkSrv m_OldnetworkSrv;
    //EnNetWorkType m_Oldnetwork;
    EnNetCallLog m_Oldnetwork; //added by GL
    BOOL BApnSet; //是否已经设置过连接
    BOOL m_bSetDefault;

    CLogData *m_pLogData;

    CString  m_strConnStatus;
    CString	 m_strPassword;
    CString	 m_strUserName;
    CString	 m_strPhoneNumber;
    CString  m_strAPN;
    CString    m_strReceived;
    CString    m_strSent;

    DWORD m_TempLastTxOkCount;
    DWORD m_TempLastRxOkCount;
    DWORD TempdwByteIn;
    DWORD TempdwByteOut;

    int m_nLastTxOkCount;
    int m_nLastRxOkCount;
    CTime m_lastNdisTxReportDataTime;
    CTime m_lastNdisRxReportDataTime;

    double m_dNDISRate_Rec; //NDIS接收速率
    double m_dNDISRate_Send; //NDIS发送速率
    double m_dNDISData_Rec; //NDIS接收数据
    double m_dNDISData_Send; //NDIS发送数据

//	CInternetConnTip *m_pConnTipWait;
    CCopsInitTip *m_pConnTipWait;

#if WINVER>=0x500
    //#if WINVER>=0x0401
    RAS_STATS rsOldStatistics;    //用于记录连接状态
    RAS_STATS rsNewStatistics;    //用于记录新的连接状态

    RAS_STATS rsBeforeChangestatistics;
    COleDateTime m_BeforeChageTime;
    //RAS_STATS rsUMTSstatistics;
#endif


    CCopsInitTip *m_pCopsInitTip;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgConnect)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    //}}AFX_VIRTUAL

    BOOL     m_BAutoDisconnect; //为ture表示流量达到限制时的自动断连
// Implementation
protected:
    CComboBox  m_cmbConnectionName;

    HRASCONN m_hRasConn;
    RASDIALPARAMS rdParams;
    RASENTRY reRasEntry;

    //连接中的连接名称
    CString m_strActiveConnection;

    //连接中的Profile名称
    CString m_strActiveProfile;

    //通过密码验证拔号连接上的
    BOOL m_bPassPwdConfirm;

    CString    m_strDuration;

    //afx_msg LRESULT OnClickConnectButton(WPARAM wParam, LPARAM lParam);

    void OnButtonDial();

    afx_msg void OnDestroy();

#ifdef FEATURE_NDIS_SUPPORT
    afx_msg void OnRadioNdis();
    afx_msg void OnRadioRas();
#endif


    DECLARE_MESSAGE_MAP()

private:
    UINT m_uTime;
    UINT m_TempTime;
    NdisStateRet m_NdisState;

    BYTE m_nUpdateSec;//持续的未统计入流量的连接时间
    CConnProfile *m_pConnProf;
    CConnProfile *m_pNdisConnProf;

    CStdioFile m_file;

    CInternetConnTip *m_pConnTip;




    stLogItem m_stLogItem;
    stLogItem dTempItem;

    CString m_StrUserName;



public:

#ifdef FEATURE_NDIS_SUPPORT
    int m_connect_type;
#endif

    CRITICAL_SECTION m_LogFileCs;
//		CNetStatistics *m_pUsageDlg;

    void PromptCheckBeforeConnect();
    void WriteLog(CString);
    void OnTimer(UINT nIDEvent);

    LRESULT OnUpdateConnectStatus(WPARAM wParam, LPARAM lParam);
    LRESULT OnRasDialEvent(WPARAM wp, LPARAM lp);
    void OpenConnTip(CString strTip = _T(""));
    void UpdateConnTip(CString str);
    void CloseConnTip();
    afx_msg void OnButtonDisconnect();
    void PromptCheck(DWORD dwCurMByte, DWORD dwCurMin);
    void PromptNDISCheck(DWORD dwCurSec);
    void OnSelchangeComboConnectName();
    stThroughput GetThroughtput();

    afx_msg void OnClickConnectButton();
    afx_msg void OnModemInOut(WPARAM wParam, LPARAM lParam);
    BOOL ModemLockPrompt();



    //接入点设置
    static void RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtCGDCONT(CString strConnPoint);
    BOOL SyncSystemConnEntry(const StConnProfile &profile);
    BOOL CreateSystemConnEntry(const StConnProfile &profile);
    BOOL ModifySystemConnEntry(const StConnProfile &profile);
    BOOL IsNetServerChange();
    void StasticNdisLog();
    CString GetDurationTime();
    void onTxOkCount(int nTxOkCount);
    void onRxOkCount(int nRxOkCount);
    void getConnProfile();
    void resetConnProfile();
    void killTimers();



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONNECT_H__506C56C9_CA21_4DC4_B942_19836C6528AF__INCLUDED_)
