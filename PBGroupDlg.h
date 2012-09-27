#if !defined(AFX_PBNEWGROUPDLG_H__8658F167_9561_4E26_8233_254B21F954EB__INCLUDED_)
#define AFX_PBNEWGROUPDLG_H__8658F167_9561_4E26_8233_254B21F954EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "BaseDialog.h"

// PBNewGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPBGroupDlg dialog

class CPBGroupDlg : public CBaseDialog
{
// Construction
public:
    CPBGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CPBGroupDlg)
    enum { IDD = IDD_DIALOG_PBGROUP };

    CListBox m_ListGroup;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPBGroupDlg)

    int m_nGroupSelected;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL


// Implementation
protected:

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnButtonNewGP();
    afx_msg void OnButtonEdit();
    afx_msg void OnButtonDelete();
    afx_msg virtual void OnCancel();
    afx_msg LRESULT OnRefreshGroup(WPARAM wParam=0, LPARAM lParam=0);
    void initGroupList();

    DECLARE_MESSAGE_MAP()

private:
    CPbData  *m_pPbData;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBNEWGROUPDLG_H__8658F167_9561_4E26_8233_254B21F954EB__INCLUDED_)
