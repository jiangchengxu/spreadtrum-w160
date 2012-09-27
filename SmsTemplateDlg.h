#if !defined(AFX_SMSTEMPLATEDLG_H__755C0CC5_CA2A_4415_966E_FD98DC857BAF__INCLUDED_)
#define AFX_SMSTEMPLATEDLG_H__755C0CC5_CA2A_4415_966E_FD98DC857BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsTemplateDlg.h : header file
//
#define WM_SELCONTENT WM_USER+145
#define WM_ADDCONTENT WM_USER+146

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateEditDlg dialog

class CSmsTemplateEditDlg : public CBaseDialog
{
    // Construction
public:
    CString m_strEditContent;

    int m_nType;

    void OnButtonClear();
    CSmsTemplateEditDlg(const TCHAR *pSzText = NULL, CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CSmsTemplateDlg)
    enum { IDD = IDD_DIALOG_TEMPLATE_EDIT };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsTemplateDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSmsTemplateDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateDlg dialog

class CSmsTemplateDlg : public CBaseDialog
{
// Construction
public:
    void OnListDBLCLK(NMHDR* pNMHDR, LRESULT* pResult);
    void OnTemplateAddOrEdit(WPARAM wp, LPARAM lp);
    void OnButtonDelete();
    void OnButtonEdit();
    void OnButtonNew();
    void OnButtonInsert();
    BOOL InitList();
    CSmsTemplateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSmsTemplateDlg)
    enum { IDD = IDD_DIALOG_SMS_TEMPLATE };
    // NOTE: the ClassWizard will add data members here
    CListCtrl	m_listCtrl;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSmsTemplateDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSmsTemplateDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnListSel(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CSmsData *m_pSmsTemplateData;

    int m_nIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSTEMPLATEDLG_H__755C0CC5_CA2A_4415_966E_FD98DC857BAF__INCLUDED_)
