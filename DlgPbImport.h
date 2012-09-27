#if !defined(AFX_PBIMPORT_H__D873D135_114E_452B_B4E1_8D1A05834F73__INCLUDED_)
#define AFX_PBIMPORT_H__D873D135_114E_452B_B4E1_8D1A05834F73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PbImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPbImport dialog

class CDlgPbImport : public CBaseDialog
{
// Construction
public:
    CDlgPbImport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgPbImport)
    enum { IDD = IDD_DIALOG_PBIMPORT };
    CButton m_RFile;
    CButton m_ROutlook;
    int m_nCheck;
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgPbImport)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:


    // Generated message map functions
    //{{AFX_MSG(CDlgPbImport)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBIMPORT_H__D873D135_114E_452B_B4E1_8D1A05834F73__INCLUDED_)
