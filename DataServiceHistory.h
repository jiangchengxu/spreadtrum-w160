#if !defined(AFX_DATASERVICEHISTORY_H__F0062B52_71D9_4076_88DC_012E6AC7790E__INCLUDED_)
#define AFX_DATASERVICEHISTORY_H__F0062B52_71D9_4076_88DC_012E6AC7790E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataServiceHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataServiceHistory dialog

class CDataServiceHistory : public CBaseDialog
{
// Construction
public:
    CDataServiceHistory(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDataServiceHistory)
    enum { IDD = IDD_DIALOG_DATASERVICEHISTORY };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDataServiceHistory)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDataServiceHistory)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASERVICEHISTORY_H__F0062B52_71D9_4076_88DC_012E6AC7790E__INCLUDED_)
