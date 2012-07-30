#if !defined(AFX_DLGSETUPSKIN_H__0A1C51BC_FFA9_4508_857C_633D491089A6__INCLUDED_)
#define AFX_DLGSETUPSKIN_H__0A1C51BC_FFA9_4508_857C_633D491089A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupSkin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupSkin dialog

class CDlgSetupSkin : public CBaseDialog
{
// Construction
public:
    CDlgSetupSkin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgSetupSkin)
    enum { IDD = IDD_DIALOG_SETUP_SKIN };
    int        m_nStyle;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetupSkin)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgSetupSkin)
    afx_msg void OnRadioSetupSkin();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPSKIN_H__0A1C51BC_FFA9_4508_857C_633D491089A6__INCLUDED_)
