#ifndef AFX_MSGBOX_H_
#define AFX_MSGBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopDlg dialog
class CMsgBox : public CBaseDialog
{
public:
    DECLARE_DYNAMIC(CMsgBox)
    UINT m_nRetTag;
    UINT m_nRet;
    UINT m_nType;
    //HANDLE m_hMsgEvt;
    // Construction
public:
    CMsgBox(CWnd* pParent = NULL);   // standard constructor
    CMsgBox(CWnd* pParent,LPCTSTR lpszPrompt,UINT nType, UINT nIDPrompt);
    /*
    BOOL Create(UINT nID, CWnd* pWnd){
    	return CBaseDialog::Create(nID,pWnd);
    }
    void PostNcDestroy(){
        delete this;
    }
    void OnCancel(){
        DestroyWindow();
    }
    */
    ///////////////////////////////////////////
    virtual ~CMsgBox();
    // Dialog Data
    //{{AFX_DATA(CMsgBox)
    enum { IDD = IDD_DIALOG_MSGBOX };
    CString m_strInfo;
    //}}AFX_DATA
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgBox)
public:
    virtual int DoModal();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual int OnCommand(WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CMsgBox)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnCancel();
    afx_msg void OnBtnOk();
    //}}AFX_MSG
    //virtual void OnOK();
    //virtual void OnCancel();
    DECLARE_MESSAGE_MAP()
};

#endif
