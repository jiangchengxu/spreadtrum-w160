#if !defined(AFX_DLGSETUPNWSEL_H__365CC672_D6F5_4D6B_A898_91A7E4A64ED1__INCLUDED_)
#define AFX_DLGSETUPNWSEL_H__365CC672_D6F5_4D6B_A898_91A7E4A64ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupNWSel.h : header file
//
#include "NWListCtrl.h"
#include "Ds.h"
#include "DlgMsg.h"
#include "TabCtrl/EnTabCtrl.h"
#include "DlgSetupNWSelMode.h"
#include "DlgSetupNWSelSearch.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSel dialog

class CDlgSetupNWSel : public CBaseDialog
{
// Construction
public:
//	bool CopsState;
	int m_nMsgBeat;
	int nSetPREFOK;	//0:not refresh
					//1:need refresh, before PREF
					//2:PREF ok, before COPS

	int nCOPSType;	//0:cops set, 1:cops?, 2:cops=?

	

	int nNWnumber;
	bool bSetNWSucc;  //set network succ
//	CDlgMsg* pdlgMsg;
    CDlgSetupNWSelMode m_dlgSetupNWSelMode;//networktab
	CDlgSetupNWSelSearch m_dlgSetupNWSelSearch;
//	BOOL InitData();
//	int AnalyseNWList(CString strNWList);
	CDlgSetupNWSel(CWnd* pParent = NULL);   // standard constructor
	~CDlgSetupNWSel();

	StNetWork* pstNetWork;
	//PHPREF 
//	static void RspAtPHPREF(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
//	BOOL SndAtPHPREF(CString strPHPREF);
	//COPS 
//	static void RspAtCOPS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
//	BOOL SndAtCOPS(CString strCOPS);

//	void GetCurAutoPrefer();
//	void ManualUpdateIcon();

	void InitTabCtrl();
// Dialog Data
	//{{AFX_DATA(CDlgSetupNWSel)
	enum { IDD = IDD_DIALOG_SETUP_NWSEL };
	CNWListCtrl	m_lstNW;
	int		m_nSelMode;
	int		m_nAutoMode;
	CEnTabCtrl  m_tabHistNW;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetupNWSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CImageList m_ilTabs;

	// Generated message map functions
	//{{AFX_MSG(CDlgSetupNWSel)
	virtual void OnOK();
    virtual void OnCancel();
	afx_msg void OnSelchangeTabHistNW(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPNWSEL_H__365CC672_D6F5_4D6B_A898_91A7E4A64ED1__INCLUDED_)
