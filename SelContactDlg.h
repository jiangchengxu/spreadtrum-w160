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

//��ѡ����¼����Ϊ30��
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

    /*��ʼ��ListCtrl�ؼ�������PB��¼д��ListCtrl�ؼ�*/
    void InitListCtrl();
// Dialog Data
    //{{AFX_DATA(CSelContactDlg)
    enum { IDD = IDD_DIALOG_SELCONTACT };
        // NOTE: the ClassWizard will add data members here
    //IDC_LIST_SELCONTACT�ĳ�Ա����
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
    /*��Ӧ���˫���¼�*/
    afx_msg void OnDblclkListSelcontact(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    //��ȡһ���û�ѡ���PB��¼
    bool SelContact(int nItem);
    CPbData  *m_pPbData;
    CWnd* m_parent;
    EnSelContactType m_SelType;

	bool m_ReadDataCardAndUSIMStatus;  //��ȡUSIM��DATACARD�洢��ʱ�Ƿ���ִ���trueΪ��ȷ��falseΪ����
	int m_nStoreFlag;  // //����USIM�洢����DATACARD�洢����0ΪDATACARD,1ΪUSIM��
	CWaitDlg *m_pDlg;   //ʵ����CWaitDlg 
	int m_ReadDataFromDCState;  //���ڱ����ص������е����
	int m_nIndex;        //�ӵڶ�����ʼѭ����ȡ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_CSelContactDlg_H__8E5EF2BE_B217_4D5C_BECF_BB9AB0C8D188__INCLUDED_)
