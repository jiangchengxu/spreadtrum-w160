#if !defined(AFX_SMSLISTCTRL_H__C500F2B5_33CC_44EE_B3EB_C32CA3035D72__INCLUDED_)
#define AFX_SMSLISTCTRL_H__C500F2B5_33CC_44EE_B3EB_C32CA3035D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsListCtrl.h : header file
//

#include "CommonStructs.h"
#include "HeaderCtrlEx.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSmsListCtrl window

class CSmsListCtrl : public CListCtrl
{
// Construction
public:
    CSmsListCtrl(BOOL bEnableRB = FALSE);

// Attributes
public:
    CHeaderCtrlEx m_headerCtrl; //列表头控件

// Operations
public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsListCtrl)
protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    void SetDspFilter(EnSmsType type);
    void SetLocType(EnLocType loctype);
    virtual ~CSmsListCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(CSmsListCtrl)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    //}}AFX_MSG
    afx_msg void OnButtonClicked(UINT nID);

    DECLARE_MESSAGE_MAP()
private:
    BOOL      m_bEnableRB;
    EnSmsType m_DispFilter;
    EnLocType loctype;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSLISTCTRL_H__C500F2B5_33CC_44EE_B3EB_C32CA3035D72__INCLUDED_)
