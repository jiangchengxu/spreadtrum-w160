// CBaseDialog.h : header file
//

#if !defined(AFX_TABBEDDIALOG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_)
#define AFX_TABBEDDIALOG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTabedDialog dialog
#include "ShadeButtonST.h"
#include "BtnST.h"
class CTabedDialog: public CBaseDialog
{
    DECLARE_DYNAMIC(CTabedDialog)
// Construction
public:
    void SetButton(CShadeButtonST* pButton);
    CShadeButtonST* GetButton();
    CShadeButtonST* m_pButton;
    CTabedDialog(UINT nIDTemplate, CWnd* pParent = NULL);    // standard constructor

    virtual ~CTabedDialog();
    virtual BOOL CanSwitchMe();
// Dialog Data
    //{{AFX_DATA(CTabedDialog)
//    enum { IDD = IDD_HSDPA_DIALOG };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabedDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CTabedDialog)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnDestroy();
    afx_msg HCURSOR OnQueryDragIcon();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDDIALOG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_)
