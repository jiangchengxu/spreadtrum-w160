#if !defined(AFX_PINCLCKDLG_H__FCF71BF3_BE6E_4CDA_BFBB_56491EEB20D5__INCLUDED_)
#define AFX_PINCLCKDLG_H__FCF71BF3_BE6E_4CDA_BFBB_56491EEB20D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PinClckDlg.h : header file
//
#include "PinEx.h"
#include "TransparentStatic.h"
#include "Ds.h"
/////////////////////////////////////////////////////////////////////////////
// CPinClckDlg dialog

class CPinClckDlg : public CBaseDialog
{
public:

	//设置PIN码enalbe或disable时，用于和回调函数同步的事件
	HANDLE m_hPinEvt;  
	UINT m_nTag;
	CPinEx* m_pHandlePin;
    
// Construction
public:
    CPinClckDlg(CWnd* pParent = NULL);   // standard constructor
	CPinClckDlg(CWnd* pParent,UINT tag);
	~CPinClckDlg();
	int OnEnablePin();
	int OnDisablePin();
	static void AtRespCLCK(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
// Dialog Data
    //{{AFX_DATA(CPinClckDlg)
	enum { IDD = IDD_DIALOG_PINCLCK };
	//CString	m_cPinClckTip;
	CString	m_cRemainTime;
	CString	m_valKey;
	//}}AFX_DATA
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPinClckDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPinClckDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
	afx_msg void OnButtonSubmit();
	//}}AFX_MSG
	//afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINCLCKDLG_H__FCF71BF3_BE6E_4CDA_BFBB_56491EEB20D5__INCLUDED_)





















