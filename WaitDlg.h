#if !defined(AFX_WAITDLG_H__87A2C2F6_3448_45F3_83F6_2EA9660978FA__INCLUDED_)
#define AFX_WAITDLG_H__87A2C2F6_3448_45F3_83F6_2EA9660978FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitDlg.h : header file
//
//#include "HSDPADlg.h"
/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog

class CWaitDlg : public CDialog
{
// Construction
public:
    /*CWaitDlg构造函数，nUpper代表进度条的最大值，nSetp代表每次递增的值*/
    CWaitDlg(CWnd* pParent = NULL,int nUpper = 100,int nSetp = 1);   // standard constructor
	~CWaitDlg();

// Dialog Data
    //{{AFX_DATA(CWaitDlg)
    enum { IDD = IDD_DIALOG_WAITING };
    CProgressCtrl    m_ProgressWait;
    CString    m_strPrompt;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWaitDlg)
    public:
    //预处理消息函数，用于拦截WM_CLOSE以外的所有消息
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    public:
    
    //设置进度条的范围和递增值
    void SetProgressRange(int nLower,int nUpper,int nStep = 1);

    //设置进度条结束
    void ProgressCompleted(int nPos);

    afx_msg LRESULT OnClose(WPARAM wParam, LPARAM lParam = 0);

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CWaitDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    LRESULT ProgressSet(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

public:

    //m_bSuccess用于标识窗口是否是正确且正常关闭的。
    bool m_bSuccess;

    //进度条的上限
    int m_nUpper;

    //进度条的下限
    int m_nLower;

    //进度条的递增值
    int m_nSetp;

    //IDC_ITEM的成员变量，用于显示当前处理的条数和总条数
    CString m_strItem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITDLG_H__87A2C2F6_3448_45F3_83F6_2EA9660978FA__INCLUDED_)
