#if !defined(AFX_SETUPCALLDLG_H__755C0CC5_CA2A_4415_966E_FD98DC857BAF__INCLUDED_)
#define AFX_SETUPCALLDLG_H__755C0CC5_CA2A_4415_966E_FD98DC857BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CSetupCallDlg dialog

class CSetupCallDlg : public CBaseDialog
{
	// Construction
public:

	CSetupCallDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupCallDlg();

	BOOL AtSndPrivacy();
	static void AtRespPrivacy(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

	BOOL AtSndSetPrivacy(int nPrivacy);
	static void AtRespSetPrivacy(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

	HANDLE m_hCallSetupEvt;
	int m_nPrivacy;
	int m_nPrivacySet;
	
	// Dialog Data
	//{{AFX_DATA(CSmsTemplateDlg)
	enum { IDD = IDD_DIALOG_SETUP_CALL };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmsTemplateDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSmsTemplateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioPrivacyOn();
	afx_msg void OnRadioPrivacyOff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSTEMPLATEDLG_H__755C0CC5_CA2A_4415_966E_FD98DC857BAF__INCLUDED_)
