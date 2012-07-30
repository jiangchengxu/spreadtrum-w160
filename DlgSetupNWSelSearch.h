#if !defined(AFX_DLGSETUPNWSELSEARCH_H__F625EE9F_5855_479C_96E5_40D0F20896E6__INCLUDED_)
#define AFX_DLGSETUPNWSELSEARCH_H__F625EE9F_5855_479C_96E5_40D0F20896E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupNWSelSearch.h : header file
//
#include "Ds.h"
#include "DlgMsg.h"
#include "DlgSetupNWSelMode.h"
#include "NWListCtrlSearch.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSelSearch dialog

class CDlgSetupNWSelSearch : public CBaseDialog
{
// Construction
public:
	CDlgSetupNWSelSearch(CWnd* pParent = NULL);   // standard constructor

	int nCOPSType;	//0:cops set, 1:cops?, 2:cops=?
	int nNWnumber;
	int nSetPREFOK;	//0:not refresh
	//1:need refresh, before PREF
					//2:PREF ok, before COPS

	int m_nMsgBeat;

	CDlgMsg* pdlgMsg;
	CDlgSetupNWSelMode *p_dlgSetupNWSelMode;

	int AnalyseNWList(CString strNWList);
    StNetWork* pstNetWork;

	bool bSetNWSucc;  //set network succ

	//COPS 
	static void RspAtCOPS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	BOOL SndAtCOPS(CString strCOPS);
	
	void GetCurAutoPrefer();
	void ManualUpdateIcon();

// Dialog Data
	//{{AFX_DATA(CDlgSetupNWSelSearch)
	enum { IDD = IDD_DIALOG_SETUP_NW_SEARCH };
	CNWListCtrlSearch m_NWListSearch;
	int	 m_nAutoMode;
	int		m_nSelMode;//add by liub 1028 for ctrllist
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetupNWSelSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetupNWSelSearch)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNwselSearchSelect();
	afx_msg void OnButtonNwselSearchRefresh();
	afx_msg void OnRadioNwselSearchAutosel();
	afx_msg void OnRadioNwselSearchMansel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnUpdateNWSelSearchDlg(WPARAM wParam, LPARAM lParam = 0);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPNWSELSEARCH_H__F625EE9F_5855_479C_96E5_40D0F20896E6__INCLUDED_)
