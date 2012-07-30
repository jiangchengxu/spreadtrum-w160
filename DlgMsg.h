#if !defined(AFX_DLGMSG_H__011C3566_0F38_496A_9ABE_4F050167EBC7__INCLUDED_)
#define AFX_DLGMSG_H__011C3566_0F38_496A_9ABE_4F050167EBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog

class CDlgMsg : public CBaseDialog
{
// Construction
public:
	CDlgMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMsg)
	enum { IDD = IDD_DIALOG_MESSAGE };
	CString	m_strMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMsg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSG_H__011C3566_0F38_496A_9ABE_4F050167EBC7__INCLUDED_)
