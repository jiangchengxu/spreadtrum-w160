#if !defined(AFX_DLGSIGNIN_H__48AB645E_1BC2_49E1_93AE_9D331D1E17CF__INCLUDED_)
#define AFX_DLGSIGNIN_H__48AB645E_1BC2_49E1_93AE_9D331D1E17CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSignIn.h : header file
//
#include "HSDPADlg.h"
#define  WM_DECLCK        (WM_USER+35)
/////////////////////////////////////////////////////////////////////////////
// CDlgSignIn dialog

class CDlgSignIn : public CBaseDialog
{
public:
    CHSDPADlg *m_pMWnd;
    HANDLE m_hSignInEvt;
    EnSyncInitFuncRetType OnSignin();
    //static void AtRespCLCKsc0(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    static void AtRespCPinVerify(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
// Construction
public:
    //CWnd *m_pParent;
    CDlgSignIn(CWnd* pParent = NULL);   // standard constructor
    ~CDlgSignIn();
    CPinEx* m_pHandlePin;

// Dialog Data
    //{{AFX_DATA(CDlgSignIn)
    enum { IDD = IDD_DIALOG_SIGNIN };
    CString	m_strKey;
    CString	m_strLeaveTime;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSignIn)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgSignIn)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnBtnSignin();
    //}}AFX_MSG
    afx_msg LRESULT DeClck(WPARAM wParam =0, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()

protected:
    HWND m_Hwnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSIGNIN_H__48AB645E_1BC2_49E1_93AE_9D331D1E17CF__INCLUDED_)
