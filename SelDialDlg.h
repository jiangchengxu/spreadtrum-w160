#if !defined(AFX_SELDIALDLG_H__7B796378_CCE6_4839_99B1_D04A456AC19A__INCLUDED_)
#define AFX_SELDIALDLG_H__7B796378_CCE6_4839_99B1_D04A456AC19A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelDialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelDialDlg dialog

class CSelDialDlg : public CBaseDialog
{
// Construction
public:
    CSelDialDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSelDialDlg)
	enum { IDD = IDD_DIALOG_SEL_NUMBER };
    CComboBox m_ComboNum;
	//}}AFX_DATA
    

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSelDialDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSelDialDlg)
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	void GetSelNumber(); 
    DECLARE_MESSAGE_MAP()

public:

    //���ڱ���Mobile��������
    CString m_strMobile;

    //���ڱ���Home��������
    CString m_strHome;

    //���ڱ���Company��������
    CString m_strCompany;
    
    //���ڱ����û�ѡ��ĺ�������
    CString m_strSelCallNum;

	//���ڱ���Name����
	CString m_strName;

};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELDIALDLG_H__7B796378_CCE6_4839_99B1_D04A456AC19A__INCLUDED_)
