#if !defined(AFX_GETOUTLOOKPBDLG_H__92E40DCD_D5F2_4CBF_BDF5_E7180AE82D55__INCLUDED_)
#define AFX_GETOUTLOOKPBDLG_H__92E40DCD_D5F2_4CBF_BDF5_E7180AE82D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetOutlookPBDlg.h : header file
//

#include "afxtempl.h"
#include "msoutl.h"
#include "PbData.h"


/////////////////////////////////////////////////////////////////////////////
// CGetOutlookPBDlg dialog

class CGetOutlookPBDlg : public CBaseDialog
{
// Construction
public:
    CGetOutlookPBDlg(CWnd* pParent = NULL);   // standard constructor

    void SetContactListPtr(CPbData *pListPtr);
// Dialog Data
    //{{AFX_DATA(CGetOutlookPBDlg)
    enum { IDD = IDD_DIALOG_PBGETOUTLOOK };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGetOutlookPBDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CGetOutlookPBDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnButtonBrowse();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    BOOL InitOutlookContactApp();
    BOOL GetContactItem(_ContactItem contact);
    void SetOulContactValue(_ContactItem contact);
    void InitRecordMapping();
    int ReadMAPIFolder(MAPIFolder pFolder);
    void ClearOulContactValue();
    bool CheckContactRule(StPbRecord *pPBRecord);
    bool CheckPbName(const char* ch);
    bool CheckPbNumber(CString strNum);
    bool  CheckPbNameUnicode(const TCHAR* ch);
    BOOL isChsCharUnicode(const TCHAR* ch);
    BOOL isChsChar(const char* ch);

protected:
    CPbData *pContactsPtr;
    _Application m_pApp;  //outlook”¶”√≥Ã–Ú
    _NameSpace m_pNS;
    MAPIFolder m_pFolder;
    RecordMapping m_RM;
    CStringArray m_OulContact;
};

inline void CGetOutlookPBDlg::SetContactListPtr(CPbData *pListPtr)
{
    pContactsPtr = pListPtr;
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETOUTLOOKPBDLG_H__92E40DCD_D5F2_4CBF_BDF5_E7180AE82D55__INCLUDED_)
