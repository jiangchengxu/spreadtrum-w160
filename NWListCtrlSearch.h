#if !defined(AFX_NWLISTCTRLSEARCH_H__34012A33_B1C5_470B_9479_EBB3A6152B34__INCLUDED_)
#define AFX_NWLISTCTRLSEARCH_H__34012A33_B1C5_470B_9479_EBB3A6152B34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NWListCtrlSearch.h : header file
//
#include "HeaderCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CNWListCtrlSearch window

class CNWListCtrlSearch : public CListCtrl
{
// Construction
public:
	CHeaderCtrlEx m_headerCtrl;
	CNWListCtrlSearch();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNWListCtrlSearch)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNWListCtrlSearch();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNWListCtrlSearch)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	virtual void PreSubclassWindow();
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NWLISTCTRLSEARCH_H__34012A33_B1C5_470B_9479_EBB3A6152B34__INCLUDED_)
