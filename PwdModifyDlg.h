#if !defined(AFX_PWDMODIFYDLG_H__49814395_CC94_428E_9C00_DB9AF6DF2A54__INCLUDED_)
#define AFX_PWDMODIFYDLG_H__49814395_CC94_428E_9C00_DB9AF6DF2A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PwdModifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPwdModifyDlg dialog

class CPwdModifyDlg : public CDialog
{
// Construction
public:
	CPwdModifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPwdModifyDlg)
	enum { IDD = IDD_DIALOG_INTERNET_PWDMODIFY };
	CString	m_strPwdCurr;
	CString	m_strPwdVerify;
	CString	m_strPwdNew;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPwdModifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPwdModifyDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void WritePwdtoINIFile();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PWDMODIFYDLG_H__49814395_CC94_428E_9C00_DB9AF6DF2A54__INCLUDED_)
