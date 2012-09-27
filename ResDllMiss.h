#if !defined(AFX_RESDLLMISS_H__E1E5D7D7_7A1D_47EE_B79F_AF2818328356__INCLUDED_)
#define AFX_RESDLLMISS_H__E1E5D7D7_7A1D_47EE_B79F_AF2818328356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResDllMiss.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResDllMiss dialog

class CResDllMiss : public CDialog
{
// Construction
public:
    CResDllMiss(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CResDllMiss)
    enum { IDD = IDD_DIALOG_RESDLLNOTFOUND };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CResDllMiss)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CResDllMiss)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESDLLMISS_H__E1E5D7D7_7A1D_47EE_B79F_AF2818328356__INCLUDED_)
