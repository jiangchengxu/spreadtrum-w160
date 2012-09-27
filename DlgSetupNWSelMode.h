#if !defined(AFX_DLGSETUPNWSELMODE_H__096E1C64_C594_4976_8311_BBE318CF8854__INCLUDED_)
#define AFX_DLGSETUPNWSELMODE_H__096E1C64_C594_4976_8311_BBE318CF8854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupNWSelMode.h : header file
//
#include "Ds.h"
#include "DlgMsg.h"

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSelMode dialog

class CDlgSetupNWSelMode : public CBaseDialog
{
// Construction
public:
    CDlgSetupNWSelMode(CWnd* pParent = NULL);   // standard constructor

    int m_nMsgBeat;
    int nCOPSType;	//0:cops set, 1:cops?, 2:cops=?


    int nSetPREFOK;	//0:not refresh
    //1:need refresh, before PREF
    //2:PREF ok, before COPS
    bool bSetNWSucc;  //set network succ

    CDlgMsg* pdlgMsg;
    //StNetWork* pstNetWork;

    BOOL InitData();
    //PHPREF
    static void RspAtPHPREF(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);//add by liub 1028 for SENDAT
    BOOL SndAtPHPREF(CString strPHPREF);//add by liub 1028 for SENDAT
// Dialog Data
    //{{AFX_DATA(CDlgSetupNWSelMode)
    enum { IDD = IDD_DIALOG_SETUP_NW_MODE };
    int		m_nSelMode;
    int	 m_nAutoMode;//add by liub 1027 for NWSELMODE
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetupNWSelMode)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgSetupNWSelMode)
    // NOTE: the ClassWizard will add member functions here
    afx_msg void OnButtonNwselSet();
    afx_msg LRESULT OnUpdateNWSelDlgMode(WPARAM wParam, LPARAM lParam = 0);
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPNWSELMODE_H__096E1C64_C594_4976_8311_BBE318CF8854__INCLUDED_)
