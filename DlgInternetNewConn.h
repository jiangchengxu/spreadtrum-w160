#if !defined(AFX_DLGINTERNETNEWCONN_H__0FB589E3_80FB_4072_ABAA_CA82284AD3DB__INCLUDED_)
#define AFX_DLGINTERNETNEWCONN_H__0FB589E3_80FB_4072_ABAA_CA82284AD3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInternetNewConn.h : header file
//
#include "NumEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgInternetNewConn dialog

class CDlgInternetNewConn : public CBaseDialog
{
// Construction
public:
    CDlgInternetNewConn(CWnd* pParent = NULL, INT nProfileType = 1);   // standard constructor
// Dialog Data
    //{{AFX_DATA(CDlgInternetNewConn)
	enum { IDD = IDD_DIALOG_INTERNET_NEWCONN };
	CNumEdit	m_NumEditPhoneNumber;
    CString    m_strConnName;
	CString	m_strUserName;
	CString	m_strPassWord;
	CString	m_strPhoneNumber;
	CString m_strAPN;
	BOOL	m_bSetDefault;
    int		m_nAuthRadio;
	//}}AFX_DATA

protected:
    CConnProfile *m_pConnProf;
    CConnProfile *m_pNdisConnProf;
	int m_nProfileType;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgInternetNewConn)
    virtual void OnOK();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
    // Generated message map functions
    //{{AFX_MSG(CDlgInternetNewConn)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINTERNETNEWCONN_H__0FB589E3_80FB_4072_ABAA_CA82284AD3DB__INCLUDED_)
