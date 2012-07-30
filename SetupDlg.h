#if !defined(AFX_SETUPDLG_H__458624A5_F454_4EC6_B8E1_F4F2E056E531__INCLUDED_)
#define AFX_SETUPDLG_H__458624A5_F454_4EC6_B8E1_F4F2E056E531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupDlg.h : header file
//
#include "TabedDialog.h"
#include "TransparentStatic.h"
#include "DlgSetupGeneral.h"    // Added by ClassView
#include "DlgSetupSkin.h"    // Added by ClassView
#include "TabCtrl\entabCtrl.h"
#include "DlgSetupPowerMng.h"	// Added by ClassView
#include "CommonStructs.h"    // Added by ClassView
#include "SetupData.h"
#include "DlgSetupNWSel.h"	// Added by ClassView
#include "SetupPreferDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

#define SETUP_NETWORK
//#define SETUP_SKIN

class CSetupDlg : public CTabedDialog
{
// Construction
public:
	CDlgSetupNWSel m_dlgSetupNWSel;
	CDlgSetupPowerMng m_dlgSetupPowerMng;
    CDlgSetupSkin m_dlgSetupSkin;
    CDlgSetupGeneral m_dlgSetupGeneral;
    CSetupPreferDlg m_dlgPreferences;
	
    CSetupDlg(CWnd* pParent = NULL);   // standard constructor
	//liub_add_begin
	int nCGDCONTType;	//0:CGDCONT set, 1:CGDCONT?, 2:CGDCONT=?
	int m_nPowerStatus; //电源管理状态，0：关闭。1：上电
	BOOL InitData();
  
   // virtual BOOL OnInitDialog();

	//接入点设置
	static void RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
	BOOL SndAtCGDCONT(CString strConnPoint);
	
	static void RspAtPowerCFUN(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtPowerCFUN(EnPowerType nPowerType);

	
	CPinEx* m_pHandlePinSetup;

	bool CheckSimStatSetup();
	bool LoadPinStatSetup();
	int LoadSimStatSetup(bool bReady);
	void CheckPINUnlockSetup();
//liub_add_end

// Dialog Data
    //{{AFX_DATA(CSetupDlg)
    enum { IDD = IDD_DIALOG_SETUP };
    CEnTabCtrl    m_tabCtrl;
	CListBox    m_ctlConnList;
	CString	m_strConnPoint;

    CString    m_strName;
    int        m_nLength;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSetupDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
	virtual BOOL CanSwitchMe();
// Implementation
public:
    // Generated message map functions
    //{{AFX_MSG(CSetupDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeTabSetup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCall();
	afx_msg void OnButtonProfiles();
	afx_msg void OnButtonEnablePIN();
	afx_msg void OnButtonDisablePIN();
	afx_msg void OnButtonModifyPIN();
	afx_msg void OnButtonUnlockPIN();
	afx_msg void OnButtonText();
	afx_msg void OnButtonNetWork();
	afx_msg void OnButtonPreference();
	//}}AFX_MSG
	void EnableButton(BOOL BShow);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPDLG_H__458624A5_F454_4EC6_B8E1_F4F2E056E531__INCLUDED_)
