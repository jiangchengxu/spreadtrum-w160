#if !defined(AFX_SMSEDIT_H__17F1ECBE_58D8_41DD_94E4_AFB7EC5C8243__INCLUDED_)
#define AFX_SMSEDIT_H__17F1ECBE_58D8_41DD_94E4_AFB7EC5C8243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmsEdit window

class CSmsEdit : public CEdit
{
// Construction
public:
	CSmsEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmsEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmsEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSmsEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSEDIT_H__17F1ECBE_58D8_41DD_94E4_AFB7EC5C8243__INCLUDED_)
