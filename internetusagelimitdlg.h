#if !defined(AFX_INTERNETUSAGELIMITDLG_H__1B24A76F_8640_4409_9E8A_387AAC5E2976__INCLUDED_)
#define AFX_INTERNETUSAGELIMITDLG_H__1B24A76F_8640_4409_9E8A_387AAC5E2976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InternetUsageLimitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInternetUsageLimitDlg dialog

class CInternetUsageLimitDlg : public CDialog
{
// Construction
public:
    CInternetUsageLimitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CInternetUsageLimitDlg)
    enum { IDD = IDD_DIALOG_INTERNET_USAGELIMIT };
    int		m_nFluxOrDuration;
    BOOL	m_bPrompt;
    BOOL	m_bDisconnect;
    BOOL	m_bPassword;
    DWORD	m_nHours;
    DWORD	m_nMinutes;
    DWORD	m_nFlux;
    COleDateTime	m_DateTime;


    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CInternetUsageLimitDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CInternetUsageLimitDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnRadio1();
    afx_msg void OnRadio2();
    virtual void OnOK();
    afx_msg void OnCheckDisconn();
    afx_msg void OnCheckPWD();
    afx_msg void OnModifyPWD();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void ResetToDefault();
    void ReadFromFile();
    void WriteToFile();
    void ChangePWDCheckBoxStatus();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNETUSAGELIMITDLG_H__1B24A76F_8640_4409_9E8A_387AAC5E2976__INCLUDED_)
