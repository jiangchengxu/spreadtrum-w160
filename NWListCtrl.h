#if !defined(AFX_NWLISTCTRL_H__20EBA9D3_8AB5_478C_AC2C_DAD984AC6CBD__INCLUDED_)
#define AFX_NWLISTCTRL_H__20EBA9D3_8AB5_478C_AC2C_DAD984AC6CBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NWListCtrl.h : header file
//
#include "HeaderCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CNWListCtrl window

class CNWListCtrl : public CListCtrl
{
// Construction
public:
	CHeaderCtrlEx m_headerCtrl;
	CNWListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNWListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNWListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNWListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NWLISTCTRL_H__20EBA9D3_8AB5_478C_AC2C_DAD984AC6CBD__INCLUDED_)
