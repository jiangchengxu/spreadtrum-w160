#if !defined(AFX_DLGSETUPGENERAL_H__8AA951CE_AE79_4DA8_BFD3_FBCFE64A4171__INCLUDED_)
#define AFX_DLGSETUPGENERAL_H__8AA951CE_AE79_4DA8_BFD3_FBCFE64A4171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetupGeneral.h : header file
//

#include "CommonStructs.h"    // Added by ClassView
#include "SetupData.h"
#include "Ds.h"
#include "PinEx.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetupGeneral dialog

class CDlgSetupGeneral : public CBaseDialog
{
// Construction
public:
    int nCGDCONTType;	//0:CGDCONT set, 1:CGDCONT?, 2:CGDCONT=?
    int m_nPowerStatus; //电源管理状态，0：关闭。1：上电
    CPinEx* m_pHandlePin;
    BOOL InitData();
    CDlgSetupGeneral(CWnd* pParent = NULL);   // standard constructor

    virtual BOOL OnInitDialog();

    //接入点设置
    static void RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtCGDCONT(CString strConnPoint);

    static void RspAtPowerCFUN(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
    BOOL SndAtPowerCFUN(EnPowerType nPowerType);

    bool CheckSimStat();
    bool LoadPinStat();
    int LoadSimStat(bool bReady);
    void CheckPINUnlock();
// Dialog Data
    //{{AFX_DATA(CDlgSetupGeneral)
    enum { IDD = IDD_DIALOG_SETUP_GENERAL };
    CListBox    m_ctlConnList;
    CString	m_strConnPoint;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetupGeneral)
public:
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgSetupGeneral)
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg LRESULT OnUpdateGeneralDlg(WPARAM wParam, LPARAM lParam = 0);
    afx_msg void OnButtonEnablepin();
    afx_msg void OnButtonDisablePin();
    afx_msg void OnButtonModifypin();
    afx_msg void OnButtonUnlockpin();
    afx_msg void OnButtonSetupPowermngPoweron();
    afx_msg void OnButtonSetupPowermngPoweroff();
    //}}AFX_MSG
    void EnableButton(BOOL BShow);
    afx_msg LRESULT OnUpdatePowerDlg(WPARAM wParam, LPARAM lParam = 0);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUPGENERAL_H__8AA951CE_AE79_4DA8_BFD3_FBCFE64A4171__INCLUDED_)
