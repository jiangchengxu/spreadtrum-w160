#if !defined(AFX_PROGRESSDLG_H__DE3288CC_821A_46F6_824A_0879D74482C2__INCLUDED_)
#define AFX_PROGRESSDLG_H__DE3288CC_821A_46F6_824A_0879D74482C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CDialog
{
// Construction
public:
    CProgressDlg(CWnd* pParent = NULL, int nUpper = 10, int nStep = 1);   // standard constructor
// Dialog Data
    //{{AFX_DATA(CProgressDlg)
    enum { IDD = IDD_DIALOG_PROGRESS };
    CProgressCtrl    m_Progress;
    //CString    m_str_sms_Prompt;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProgressDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    virtual BOOL OnInitDialog();
// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CProgressDlg)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    int m_nStep;
    int m_nUpper;
    int m_nLower;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__DE3288CC_821A_46F6_824A_0879D74482C2__INCLUDED_)
