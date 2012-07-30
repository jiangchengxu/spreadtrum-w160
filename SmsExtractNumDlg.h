#if !defined(AFX_SMSEXTRACTNUMDLG_H__5D168CDB_204D_41D2_8AB7_C56FF73735E6__INCLUDED_)
#define AFX_SMSEXTRACTNUMDLG_H__5D168CDB_204D_41D2_8AB7_C56FF73735E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsExtractNumDlg.h : header file
//

#include "resource.h"
#include "mylistctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSmsExtractNumDlg dialog

class CSmsExtractNumDlg : public CBaseDialog
{
// Construction
public:
	CSmsExtractNumDlg(LPCTSTR lpNum, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmsExtractNumDlg)
	enum { IDD = IDD_DIALOG_SMS_EXTRACTNUM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    CMyListCtrl m_listCtrl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmsExtractNumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmsExtractNumDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnDblclkListExtractNum(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    virtual void OnOK();
    afx_msg LRESULT OnAbstractNum(WPARAM wParam=0, LPARAM lParam=0);
	DECLARE_MESSAGE_MAP()
private:
	void InitListCtrl();
    TCHAR m_szNumber[PB_NUM_MAX + 2];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSEXTRACTNUMDLG_H__5D168CDB_204D_41D2_8AB7_C56FF73735E6__INCLUDED_)
