#if !defined(AFX_MODIFYPINDLG_H__2048756A_8673_401C_B880_D0FDB65D83D9__INCLUDED_)
#define AFX_MODIFYPINDLG_H__2048756A_8673_401C_B880_D0FDB65D83D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyPinDlg.h : header file
//
#include "PinEx.h"
#include "TransparentStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CModifyPinDlg dialog

class CModifyPinDlg : public CBaseDialog
{
public:
    HANDLE m_hCPwdEvt;
    CPinEx* m_pHandlePin;
    UINT m_nTag;


// Construction
public:
    CModifyPinDlg(CWnd* pParent,UINT tag);
    ~CModifyPinDlg();
    int  OnPinModify();
    int  OnPinUnlock();
    BOOL CheckCorrect();
    static void AtRespCPwd(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
// Dialog Data
    //{{AFX_DATA(CModifyPinDlg)
    enum { IDD = IDD_DIALOG_MODIFY_PIN };
    CString	m_pinConf;
    CString	m_pinCurr;
    CString	m_pinNew;
    CString m_cRemainTime;
    //CString	m_strLockTip;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CModifyPinDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CModifyPinDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //afx_msg void OnPaint();
    afx_msg void OnBtnMpinOk();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    HWND m_Hwnd;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYPINDLG_H__2048756A_8673_401C_B880_D0FDB65D83D9__INCLUDED_)










































