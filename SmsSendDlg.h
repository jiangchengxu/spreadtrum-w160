#if !defined(AFX_SENDSMS_H__2F8A0E8A_0562_414D_9A29_19555AD9EE75__INCLUDED_)
#define AFX_SENDSMS_H__2F8A0E8A_0562_414D_9A29_19555AD9EE75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsSend.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmsSendDlg dialog

class CSmsSendDlg : public CDialog
{
// Construction
public:
    CSmsSendDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSmsSendDlg)
    enum { IDD = IDD_DIALOG_SENDSMS };
    CProgressCtrl    m_SndProgress;
    CString    m_strSndTip;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsSendDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSmsSendDlg)
        // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDSMS_H__2F8A0E8A_0562_414D_9A29_19555AD9EE75__INCLUDED_)
