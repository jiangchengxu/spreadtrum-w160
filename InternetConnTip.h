#if !defined(AFX_INTERNETCONNTIP_H__4AFEE40A_5259_4A3F_9D1D_D3E7A54F4232__INCLUDED_)
#define AFX_INTERNETCONNTIP_H__4AFEE40A_5259_4A3F_9D1D_D3E7A54F4232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InternetConnTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInternetConnTip dialog

class CInternetConnTip : public CDialog
{
// Construction
public:
    CInternetConnTip(CString strTip = _T(""), CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CInternetConnTip)
    enum { IDD = IDD_DIALOG_INTERNET_TIP };
    CString  m_strTip;
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CInternetConnTip)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CInternetConnTip)
    virtual BOOL OnInitDialog();
    virtual void OnButtonCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    HRASCONN m_hRasConn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNETCONNTIP_H__4AFEE40A_5259_4A3F_9D1D_D3E7A54F4232__INCLUDED_)
