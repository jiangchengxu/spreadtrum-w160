#if !defined(AFX_SELPOSITION_H__62A2FB0C_886C_4880_BB8A_2072DB60A991__INCLUDED_)
#define AFX_SELPOSITION_H__62A2FB0C_886C_4880_BB8A_2072DB60A991__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelPosition.h : header file
//
//#include "TransparentStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CSelPosition dialog

class CSelPosition : public CBaseDialog
{
// Construction
public:
    CSelPosition(CWnd* pParent,const TCHAR *cNumber);   // standard constructor

    CPbData   *m_pPbData;//PBGroup
// Dialog Data
    //{{AFX_DATA(CSelPosition)
    enum { IDD = IDD_DIALOG_SELPOSITION };
    // NOTE: the ClassWizard will add data members here
//    CTransparentStatic m_static_strTitle;
//    CTransparentStatic m_static_strMobile;
//    CTransparentStatic m_static_strHome;
//    CTransparentStatic m_static_strCompany;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSelPosition)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSelPosition)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnRadioMobile();
    afx_msg void OnRadioHome();
    afx_msg void OnRadioCompany();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    afx_msg LRESULT OnAbstractNum(WPARAM wParam=0, LPARAM lParam=0);
private:
    int Position;
    TCHAR Number[PB_NUM_MAX+1];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELPOSITION_H__62A2FB0C_886C_4880_BB8A_2072DB60A991__INCLUDED_)
