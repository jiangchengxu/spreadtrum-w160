#if !defined(AFX_CSelContactDlg_H__8E5EF2BE_B217_4D5C_BECF_BB9AB0C8D188__INCLUDED_)
#define AFX_CSelContactDlg_H__8E5EF2BE_B217_4D5C_BECF_BB9AB0C8D188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSelContactDlg.h : header file
//
#include "TabedDialog.h"
#include "MyListCtrl.h"
#include "WaitDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSelContactDlg dialog

//可选最大记录条数为30条
#define SEL_MAX 30

typedef enum {
    SELCONTACT_SMSNUMBER = 0, 
    SELCONTACT_SMSCONTENT, 
    SELCONTACT_CALL, 
} EnSelContactType;

class CSelContactDlg : public CDialog
{
// Construction
public:
#if 0
	static void AtRespReadState(CWnd* pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
#else
	static void AtRespReadState(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);
#endif
	
    static void AtRespReadState(CWnd* pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum);

	~CSelContactDlg();
	void MallocDlgMem(int nUpper = 100,int nStep =1);
	void ReadDataFromDCOrCard(int flag);
    CSelContactDlg(CWnd* pParent, EnSelContactType SelType);

    /*初始化ListCtrl控件，并将PB记录写入ListCtrl控件*/
    void InitListCtrl();
// Dialog Data
    //{{AFX_DATA(CSelContactDlg)
    enum { IDD = IDD_DIALOG_SELCONTACT };
        // NOTE: the ClassWizard will add data members here
    //IDC_LIST_SELCONTACT的成员变量
    CMyListCtrl    m_lstSelContact;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSelContactDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CSelContactDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    /*响应左键双击事件*/
    afx_msg void OnDblclkListSelcontact(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    //获取一条用户选择的PB记录
    bool SelContact(int nItem);
    CPbData  *m_pPbData;
    CWnd* m_parent;
    EnSelContactType m_SelType;

	bool m_ReadDataCardAndUSIMStatus;  //读取USIM或DATACARD存储器时是否出现错误。true为正确，false为错误
	int m_nStoreFlag;  // //区分USIM存储器和DATACARD存储器，0为DATACARD,1为USIM。
	CWaitDlg *m_pDlg;   //实例化CWaitDlg 
	int m_ReadDataFromDCState;  //用于标明回调函数中的入口
	int m_nIndex;        //从第二条开始循环读取数据
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_CSelContactDlg_H__8E5EF2BE_B217_4D5C_BECF_BB9AB0C8D188__INCLUDED_)
