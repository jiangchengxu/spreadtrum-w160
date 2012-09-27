#if !defined(AFX_PBNEWGROUP_H__6846624A_9C0C_43E2_B4DC_9729B77660E4__INCLUDED_)
#define AFX_PBNEWGROUP_H__6846624A_9C0C_43E2_B4DC_9729B77660E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PBNewGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPBNewGroup dialog

class CPBNewGroupDlg : public CBaseDialog
{
// Construction
public:
    CPBNewGroupDlg(CWnd* pParent = NULL,CString groupName=_T(""));   // standard constructor

// Dialog Data
    //{{AFX_DATA(CPBNewGroup)
    enum { IDD = IDD_DIALOG_PBGROUP_NEW };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
    CListBox m_ListGroupMembers;
    CString m_strGroupName;
    int m_iEditFlag; //编辑还是添加
    CString m_strPreGPName; //修改之前的group name
    CEdit m_EditGroupName;

    int m_nGroupSelected;
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPBNewGroup)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    //}}AFX_VIRTUAL

// Implementation
protected:

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void initGroupList();
    afx_msg void OnButtonOK();
    afx_msg void OnButtonCancel();
    afx_msg void OnButtonEdit();
    afx_msg LRESULT OnRefreshGroup(WPARAM wParam, LPARAM lParam);
    BOOL GroupExisted();
    DECLARE_MESSAGE_MAP()

private:
    CPbData  *m_pPbData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBNEWGROUP_H__6846624A_9C0C_43E2_B4DC_9729B77660E4__INCLUDED_)
