#if !defined(AFX_DLGSUBVERSION_H__FCE589E6_4389_44EA_BAD7_BC996151CA8E__INCLUDED_)
#define AFX_DLGSUBVERSION_H__FCE589E6_4389_44EA_BAD7_BC996151CA8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubversion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSubversion dialog

class CDlgSubversion : public CBaseDialog
{
	// Construction
public:
	CDlgSubversion(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDlgSubversion)
	enum { IDD = IDD_DIALOG_SUBVERSION };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubversion)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgSubversion)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBVERSION_H__FCE589E6_4389_44EA_BAD7_BC996151CA8E__INCLUDED_)
