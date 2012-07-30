#if !defined(AFX_PWDCONFIRMDLG_H__C4979D17_3842_4403_BE00_A2D75EEEBE73__INCLUDED_)
#define AFX_PWDCONFIRMDLG_H__C4979D17_3842_4403_BE00_A2D75EEEBE73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PwdConfirmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPwdConfirmDlg dialog

class CPwdConfirmDlg : public CDialog
{
// Construction
public:
	CPwdConfirmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPwdConfirmDlg)
	enum { IDD = IDD_DIALOG_INTERNET_PWDCONFIRM };
	CString	m_strCurrPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPwdConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPwdConfirmDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PWDCONFIRMDLG_H__C4979D17_3842_4403_BE00_A2D75EEEBE73__INCLUDED_)
