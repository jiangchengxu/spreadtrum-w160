#if !defined(AFX_SEARCHCONTACTDLG_H__FE82BC15_D531_476B_829D_F0EB6433C9D9__INCLUDED_)
#define AFX_SEARCHCONTACTDLG_H__FE82BC15_D531_476B_829D_F0EB6433C9D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchContactDlg.h : header file
//
#include "TransparentStatic.h"
#include "NumEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CSearchContactDlg dialog

class CSearchContactDlg : public CDialog
{
// Construction
public:
    CSearchContactDlg(CWnd* pParent = NULL);   // standard constructor

	~CSearchContactDlg();
// Dialog Data
    //{{AFX_DATA(CSearchContactDlg)
    enum { IDD = IDD_DIALOG_SEARCH };
    	
	CNumEdit m_EditSerachNum;
    CString m_strName;
    CString m_strMobile;
    CString m_strFindNum;

        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSearchContactDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSearchContactDlg)
    virtual BOOL OnInitDialog();
    
    afx_msg void OnButtonSearch();
    afx_msg void OnButtonAhead();
    afx_msg void OnButtonNext();
    //}}AFX_MSG
//    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

private:
    CPbData  *m_pPbData;
    int TotalIndexNum;
    int CurIndexNum;
    int* m_IndexArr;  //存放满足条件的下标
	int m_PBListTotalCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHCONTACTDLG_H__FE82BC15_D531_476B_829D_F0EB6433C9D9__INCLUDED_)
