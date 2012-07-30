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
    /*CWaitDlg���캯����nUpper��������������ֵ��nSetp����ÿ�ε�����ֵ*/
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
    //Ԥ������Ϣ��������������WM_CLOSE�����������Ϣ
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    public:
    
    //���ý������ķ�Χ�͵���ֵ
    void SetProgressRange(int nLower,int nUpper,int nStep = 1);

    //���ý���������
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

    //m_bSuccess���ڱ�ʶ�����Ƿ�����ȷ�������رյġ�
    bool m_bSuccess;

    //������������
    int m_nUpper;

    //������������
    int m_nLower;

    //�������ĵ���ֵ
    int m_nSetp;

    //IDC_ITEM�ĳ�Ա������������ʾ��ǰ�����������������
    CString m_strItem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITDLG_H__87A2C2F6_3448_45F3_83F6_2EA9660978FA__INCLUDED_)
