#if !defined(AFX_SETUPPREFERDLG_H__3C0788ED_52DA_45CA_A22B_FE7BC9EA45C9__INCLUDED_)
#define AFX_SETUPPREFERDLG_H__3C0788ED_52DA_45CA_A22B_FE7BC9EA45C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupPreferDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupPreferDlg dialog

class CSetupPreferDlg : public CBaseDialog
{
// Construction
public:
    CSetupPreferDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSetupPreferDlg)
    enum { IDD = IDD_DIALOG_SETUP_PREFER };
    //  CString     m_strSmsSoundFile;
// 	BOOL	m_bAlertTone;
// 	BOOL	m_bAlertWindow;
    BOOL	m_bConncet;
    BOOL	m_bLaunchApp;
    BOOL	m_bStartMini;
    int		m_nSleepRadio;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSetupPreferDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CSetupPreferDlg)
    virtual void OnOK();
    virtual void OnCancel();
//     afx_msg void OnButtonSmsSoundBrowse();
//     afx_msg void OnButtonSmsSoundPlay();
    afx_msg void OnButtonUsagelimit();
    afx_msg void OnButtonDiagnostics();
//    afx_msg void OnButtonDefault();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    void WriteDataToFile();
    void ReadDataFromFile();
    // CString GetDefaultSMSSoundFilePath();
private:
    BOOL AutoStartupApp(BOOL bFlag);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPPREFERDLG_H__3C0788ED_52DA_45CA_A22B_FE7BC9EA45C9__INCLUDED_)
