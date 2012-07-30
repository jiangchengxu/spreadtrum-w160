#if !defined(AFX_DLGEDITGPMEMBER_H__8223C4B6_C6C4_4B2E_A379_7C92EED879B0__INCLUDED_)
#define AFX_DLGEDITGPMEMBER_H__8223C4B6_C6C4_4B2E_A379_7C92EED879B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditGPMember.h : header file
//

#include "MyListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEditGPMember dialog

class CDlgEditGPMember : public CBaseDialog
{
// Construction
public:
	CDlgEditGPMember(CWnd* pParent = NULL, CString groupName=_T(""));   // standard constructor


	enum { IDD = IDD_DIALOG_GPMEMBER };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_strGroupName;
	CMyListCtrl m_ListPhoneBook;
	CMyListCtrl m_ListGPMember;


protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEditGPMember)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void InitListPhoneBook();
	void InitListGPMember();
	void OnButtonToRight();
	void OnButtonToLeft();
	void OnButtonOK();
	void OnButtonView();
	LRESULT OnDblclkListPhonebook(WPARAM wParam, LPARAM lParam);
	LRESULT OnDblclkListGPMember(WPARAM wParam, LPARAM lParam);

		int m_nGroupSelected;
private:
	CPbData  *m_pPbData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITGPMEMBER_H__8223C4B6_C6C4_4B2E_A379_7C92EED879B0__INCLUDED_)
