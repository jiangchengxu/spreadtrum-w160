#ifndef _AFX_HISTLISTCTRL_H_
#define _AFX_HISTLISTCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistListCtrl.h : header file
#include "HeaderCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CHistListCtrl window
class CHistListCtrl : public CListCtrl
{
public:
    CHeaderCtrlEx m_headerCtrl;
    CHistListCtrl();
    virtual ~CHistListCtrl();
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHistListCtrl)
protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
    // Generated message map functions
protected:
    //{{AFX_MSG(CHistListCtrl)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    //}}AFX_MSG
    afx_msg void OnMenuClicked(UINT nID);
    DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif //_AFX_HISTLISTCTRL_H_