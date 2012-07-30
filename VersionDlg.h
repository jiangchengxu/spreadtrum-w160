#if !defined(AFX_VERSIONDLG_H__EA64E5CF_3010_42C2_8729_E1017844C653__INCLUDED_)
#define AFX_VERSIONDLG_H__EA64E5CF_3010_42C2_8729_E1017844C653__INCLUDED_

#include "CommonStructs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VersionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVersionDlg dialog

class CVersionDlg : public CBaseDialog
{
// Construction
public:
	CVersionDlg(CWnd* pParent = NULL, EnVersionType type = VERSION_IMEI);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVersionDlg)
	enum { IDD = IDD_DIALOG_VERSION };
	CString	m_strVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVersionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVersionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	EnVersionType m_VersionType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERSIONDLG_H__EA64E5CF_3010_42C2_8729_E1017844C653__INCLUDED_)
