#if !defined(AFX_PBSELPOSITON_H__3E87B604_6A7B_4909_9B42_779899444D16__INCLUDED_)
#define AFX_PBSELPOSITON_H__3E87B604_6A7B_4909_9B42_779899444D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PbSelPositon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPbSelPositon dialog
#include "TransparentStatic.h"


class CPbSelPositon : public CBaseDialog
{
// Construction
public:
    CPbSelPositon(CString Name,CString Num ,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CPbSelPositon)
    enum { IDD = IDD_DIALOG_PBSELPOSITION };
    
    CTransparentStatic m_Mobile;
    CTransparentStatic m_Home;
    CTransparentStatic m_company;

    CTransparentStatic m_StaName;
    CTransparentStatic m_staNum;
    CTransparentStatic m_RadioSet;
    CTransparentStatic m_StaText;
    CTransparentStatic m_StaNameIn;
    CTransparentStatic m_staNumIn;

        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPbSelPositon)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPbSelPositon)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:            
    CButton            m_RMobile;
    CButton            m_RHome;
    CButton            m_RCompany;
    int                   m_Flag ;        //1：mobile;2:home;3:company.
    CButton            m_Check;
    int                m_nSet;     //复选框状态表示，1表示选中
protected:
    CString StrName;
    CString StrNum;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBSELPOSITON_H__3E87B604_6A7B_4909_9B42_779899444D16__INCLUDED_)
