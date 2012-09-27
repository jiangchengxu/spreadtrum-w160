#if !defined(AFX_DLGNETPROFILENEWCDMA_H__D29CB822_1057_4D68_B79D_889B273AC465__INCLUDED_)
#define AFX_DLGNETPROFILENEWCDMA_H__D29CB822_1057_4D68_B79D_889B273AC465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNetProfileNewCDMA.h : header file
//

#include "NumEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNetProfileNewCDMA dialog

class CDlgNetProfileNewCDMA : public CDialog
{
// Construction
public:
    CDlgNetProfileNewCDMA(CWnd* pParent = NULL, INT nProfileType = 1);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgNetProfileNewCDMA)
    enum { IDD = IDD_DIALOG_PROFILE_NEWCDMA2000 };
    CNumEdit	m_NumEditPhoneNumber;
    CString    m_strConnName;
    CString	m_strUserName;
    CString	m_strPassWord;
    CString	m_strPhoneNumber;
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

#endif // !defined(AFX_DLGNETPROFILENEWCDMA_H__D29CB822_1057_4D68_B79D_889B273AC465__INCLUDED_)
