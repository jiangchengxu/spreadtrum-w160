#if !defined(AFX_DLGSMSPRIORITYCDMA_H__0C81E8F4_CB0E_4BD4_96E5_AE495735E438__INCLUDED_)
#define AFX_DLGSMSPRIORITYCDMA_H__0C81E8F4_CB0E_4BD4_96E5_AE495735E438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSMSPriorityCDMA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSMSPriorityCDMA dialog

class CDlgSMSPriorityCDMA : public CBaseDialog
{
// Construction
public:
    CDlgSMSPriorityCDMA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDlgSMSPriorityCDMA)
    enum { IDD = IDD_DIALOG_SMSPRIORITY_CDMA };


    int m_nSMSPriority;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support



protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgSMSPriorityCDMA)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSMSPRIORITYCDMA_H__0C81E8F4_CB0E_4BD4_96E5_AE495735E438__INCLUDED_)
