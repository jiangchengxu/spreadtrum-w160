#if !defined(AFX_MENUBAR_H__82B69EB8_D338_4B92_876B_BA8357A2F016__INCLUDED_)
#define AFX_MENUBAR_H__82B69EB8_D338_4B92_876B_BA8357A2F016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuBar.h : header file
//
#include "MenuButtonST.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuBar window

class CMenuBar : public CWnd
{
// Construction
public:
	static CMenuBar& GetInstance();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFont(int nHeight=0, int nWidth=0, LPCTSTR name=NULL);
	BOOL PtInRect(POINT pt, CRect rc);
	void OnHookMove(WPARAM wParam, LPARAM lParam);
	void SetButtonItem(UINT item[], int count);
	static LRESULT CALLBACK MessageProc(int code, WPARAM wParam, LPARAM lParam);
	void TrackPopup(int nButton);
	BOOL SetMenu(UINT nMenu, HWND hParentWnd, BOOL bRepaint = TRUE);
	virtual ~CMenuBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CMenuBar();

private:
	HMENU m_hMenu;
	HWND m_hParentWnd;

	CFont *m_pFont;

	int m_nPopup;
	int m_nNextPop;

	UINT m_item[4];
	CMenuButtonST m_btnST[4];
	int m_itemCount;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUBAR_H__82B69EB8_D338_4B92_876B_BA8357A2F016__INCLUDED_)
