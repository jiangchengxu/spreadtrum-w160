#if !defined(AFX_DIAGNOSTICDLG_H__5FADB298_0148_4C41_8616_0A8F9F539BB6__INCLUDED_)
#define AFX_DIAGNOSTICDLG_H__5FADB298_0148_4C41_8616_0A8F9F539BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagnosticDlg.h : header file
//

#include "Ds.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticDlg dialog

class CDiagnosticDlg : public CBaseDialog
{
// Construction
public:
    CDiagnosticDlg(CWnd* pParent = NULL);   // standard constructor
    ~CDiagnosticDlg();
    BOOL AtSndCPBS(EnLocType loctype = LOC_UIM);
    static void AtRespCPBS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
// Dialog Data
    //{{AFX_DATA(CDiagnosticDlg)
    enum { IDD = IDD_DIALOG_DIAGNOSTICS };
    CString	  m_strIMEI;
    CString	  m_strFWVersion;
    CString	  m_strSWVersion;
    CString	  m_strPbmNum;
    CString	  m_strSmsNum;
    CComboBox m_cmbStorageType;

// 	CComboBox m_cmbMessages;
// 	CComboBox m_cmbContacts;
// 	CComboBox m_cmbImei;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDiagnosticDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDiagnosticDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    afx_msg LRESULT OnDiagnosticsNotify(WPARAM wParam, LPARAM lParam = 0);
// 	afx_msg void OnComboSelectMessage();
// 	afx_msg void OnComboSelectContact();
    afx_msg void OnComboSelStorageType();
    DECLARE_MESSAGE_MAP()
    void InitInterface();
private:
    USHORT m_nPbmMax;
    USHORT m_nPbmNum;
    HANDLE m_syncevent;

    typedef enum {
        SEND_AT_SET_STORE = 1,
        SEND_STORE_CHECK_USEDANDTOTAL,
    } SendATState;
    int m_SendATState;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGNOSTICDLG_H__5FADB298_0148_4C41_8616_0A8F9F539BB6__INCLUDED_)
