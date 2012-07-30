#if !defined(AFX_CONTACTDLG_H__0ACBC2AE_F3B6_42FC_8B32_00663186F390__INCLUDED_)
#define AFX_CONTACTDLG_H__0ACBC2AE_F3B6_42FC_8B32_00663186F390__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactDlg.h : header file
//
#include "TransparentStatic.h"
#include "NumEdit.h"

#include "ExDataExch.h"
#include "PbData.h"
/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog
//#define Name_UCS_MAX 16
//#define Name_ASI_MAX 12


class CContactDlg : public CBaseDialog
{
// Construction
public:

    /*CContactDlg���캯��������InsertIndex����-1ʱ��ʾ��ӡ�����Ϣģ����ȡ�����ӹ��ܣ�
    InsertIndex����-2��ʾ�޸��ӹ��ܡ�
    Pos��cNumber��ʾ����Ϣģ����ȡ�����ӹ��ܺ���Ӧ��ŵ�λ�ã�mobile/home/company����ѡ�񣩺ͺ������ݡ�*/
    CContactDlg(CWnd* pParent = NULL,int InsertIndex = -1,int Pos = 0,TCHAR *cNumber = _T("\0"));   // standard constructor

// Dialog Data
    //{{AFX_DATA(CContactDlg)
	enum { IDD = IDD_DIALOG_CONTACT };
    CString m_strName;
    CString    m_strMobile;
    CString    m_strHome;
	CString    m_strGroup;
	CString    m_strGroupBeforechange;
    CString    m_strCompany;
    CString    m_strEmail;
    CString    m_strMark;
	CString    m_strPicPath;/*��ŵ�ǰ��ʾ��ͼƬ��ŵ�·��*/
	CString    m_strPicOldPath;/*��Ÿ���ǰ��ͼƬ��ŵ�·��*/
	//int        nIndex;      /*���ڱ�ʶ�༭�����б��༭��Ϣ��index*/
	CEdit   m_EditName;
    CNumEdit   m_EditMobile;
    CNumEdit   m_EditHome;
    CNumEdit   m_EditCompany;
	CEdit   m_EditEMail;
	CEdit   m_EditMark;
	CComboBox m_ComboGroup;
	bool       bLoadPic;    //���ڱ�ʶ�Ƿ������ͼƬ,trueΪ����,Ĭ��Ϊfalse.
	CPbData*   m_pPbData;
	BOOL m_bSimCardFlag;  //��ʶ�Ƿ���SIM���е���ϵ��
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContactDlg)
    public:

    //��ϢԤ�������������Ҽ�����
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
	

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CContactDlg)
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditCompany();
	afx_msg void OnUpdateEditMobile();
	afx_msg void OnUpdateEditHome();
	afx_msg void OnLoadPic();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	BOOL isChineseChar(const char* ch);
	BOOL isChineseCharUnicode(const TCHAR* ch);
	bool CheckName(const char* ch);
	bool CheckNameUnicode(const TCHAR* ch);
	bool CheckNumber(CString strNum);
	bool DisplayPicture(CString strPath);
	CString GetPicDefPath();
	CString GetPicNewPath();
	CString GetSysTime();
	BOOL CopyPicFile(CString strNowPath,CString strNewPath);
	bool CompareChar(const TCHAR* chSource);
    DECLARE_MESSAGE_MAP()

private:

    /*iInsertIndex����������������ӹ���ģ�飬����InsertIndex����-1ʱ��ʾ��ӡ�����Ϣģ����ȡ�����ӹ��ܣ�
      InsertIndex����-2��ʾ�޸��ӹ��ܡ�*/
    int iInsertIndex;

    /*Pos��ʾ����Ϣģ����ȡ�����ӹ��ܺ���Ӧ��ŵ�λ�ã�mobile/home/company����ѡ��*/
    int iPos;

    /*cNumber��ʾ����Ϣģ����ȡ�����ӹ��ܴ������ĺ������ݡ�*/
    TCHAR strNumber[PB_NUM_MAX + 1];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__0ACBC2AE_F3B6_42FC_8B32_00663186F390__INCLUDED_)
