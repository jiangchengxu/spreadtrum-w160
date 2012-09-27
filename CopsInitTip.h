#if !defined(AFX_COPSINITTIP_H__876D2B7F_6D2A_4B88_B063_5A870FB7DE49__INCLUDED_)
#define AFX_COPSINITTIP_H__876D2B7F_6D2A_4B88_B063_5A870FB7DE49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopsInitTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopsInitTip dialog

class CCopsInitTip : public CDialog
{
// Construction
public:
    CCopsInitTip(CString strTip = _T(""), CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CCopsInitTip)
    enum { IDD = IDD_DIALOG_COPS_TIP };
    CString  m_strTip;
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCopsInitTip)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:


    // Generated message map functions
    //{{AFX_MSG(CCopsInitTip)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    virtual BOOL OnInitDialog();
    virtual void OnButtonCancel();
    DECLARE_MESSAGE_MAP()
public:
    HRASCONN m_hRasConn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPSINITTIP_H__876D2B7F_6D2A_4B88_B063_5A870FB7DE49__INCLUDED_)
