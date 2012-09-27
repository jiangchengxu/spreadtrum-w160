#if !defined(AFX_DLGSETUPPOWERMNG_H__6F371F18_5D89_49B1_8B03_5D8E1D92613B__INCLUDED_)
#define AFX_DLGSETUPPOWERMNG_H__6F371F18_5D89_49B1_8B03_5D8E1D92613B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupPowerMng.h : header file
//

#include "Ds.h"
#include "NumEdit.h"
#include "CommonStructs.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CDlgSetupPowerMng dialog

class CDlgSetupPowerMng : public CBaseDialog
{
// Construction
public:

    BOOL SetOK;
    CString strLastSCA;
    BOOL InitData();
    int m_nPowerStatus; //电源管理状态，0：关闭。1：上电
    CDlgSetupPowerMng(CWnd* pParent = NULL);   // standard constructor

    //SCA set
    static void RspAtCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtCSCA(CString strSCA);

    //CPMS
    static void RspAtCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtCPMS(CString strSPMS);

    //CNMI
    static void RspAtCNMI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtCNMI(int para1, int para2);

    static void RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtSmsQCSCA();

    //HMSGP
    BOOL SndAtSmsQHMSGP();//设置短消息的其他参数（<发送报告>,<短消息类型>,<消息头>,<优先级>）
    static void RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

    //CSMP  add by liub <teleservice id>,<短消息有效期的格式>,<短消息有效期值>,<短消息推迟传递有效期的格式>,<短消息推迟传递有效期值>
    BOOL SndAtSmsQCSMP();
    static void RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

    //SMSsetting begin
    BOOL m_bAlertWindowSmsSet;
    BOOL m_bAlertToneSMSSet;
    void WriteDataToFile();
    void ReadDataFromFile();
    void DefaultSet();
    CString GetDefaultSMSSoundFilePath();


// Dialog Data
    //{{AFX_DATA(CDlgSetupPowerMng)
    enum { IDD = IDD_DIALOG_SETUP_MESSAGES };
    CNumEdit	m_NumEditSCA;
    CString     m_strSmsSoundFileSMSTone;//add by liub 1104 for SMSSettings
    CString	m_strSCA;
    CString m_strVoicemailNumber;//add by liub
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetupPowerMng)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CComboBox    m_cmbDefDelivery;
    CComboBox    m_cmbPrefStor;
    CComboBox    m_cmbValPeriod;
    CComboBox    m_cmbDeliReport;
    CComboBox    m_Priority;// CDMA2000 SMSSettings
    // Generated message map functions
    //{{AFX_MSG(CDlgSetupPowerMng)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnButtonSmsSoundBrowseSMSSet(); //add by liub 1104 for SMSSettings
    afx_msg void OnButtonSmsSoundPlaySMSSet();//add by liub 1104 for SMSSettings
    //}}AFX_MSG
    afx_msg LRESULT OnUpdatePowerDlg(WPARAM wParam, LPARAM lParam = 0);
    afx_msg void OnButtonSetupMsgSet();
    DECLARE_MESSAGE_MAP()
private:
    EnLocType m_PreLocType;
    EnSmsPreStor m_PreStor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPPOWERMNG_H__6F371F18_5D89_49B1_8B03_5D8E1D92613B__INCLUDED_)
