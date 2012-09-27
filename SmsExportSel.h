#if !defined(AFX_SMSEXPORTSEL_H__DA0E78CF_41B3_4D39_91FE_A1FF4C1BF0A6__INCLUDED_)
#define AFX_SMSEXPORTSEL_H__DA0E78CF_41B3_4D39_91FE_A1FF4C1BF0A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsExportSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmsExportSel dialog

class CSmsExportSel : public CBaseDialog
{
// Construction
public:
    CSmsExportSel(CWnd* pParent = NULL);   // standard constructor
    CString GetSmsStateText(EnSmsState state);
// Dialog Data
    //{{AFX_DATA(CSmsExportSel)
    enum { IDD = IDD_DIALOG_SMS_EXPORT };
    int        m_nSmsType;
    //}}AFX_DATA
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsExportSel)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSmsExportSel)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CSmsData  * m_pSmsData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSEXPORTSEL_H__DA0E78CF_41B3_4D39_91FE_A1FF4C1BF0A6__INCLUDED_)
