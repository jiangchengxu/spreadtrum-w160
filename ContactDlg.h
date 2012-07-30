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

    /*CContactDlg构造函数，其中InsertIndex等于-1时表示添加、短消息模块提取号码子功能，
    InsertIndex等于-2表示修改子功能。
    Pos和cNumber表示短消息模块提取号码子功能号码应存放的位置（mobile/home/company三种选择）和号码内容。*/
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
	CString    m_strPicPath;/*存放当前显示的图片存放的路径*/
	CString    m_strPicOldPath;/*存放更新前的图片存放的路径*/
	//int        nIndex;      /*用于标识编辑功能中被编辑信息的index*/
	CEdit   m_EditName;
    CNumEdit   m_EditMobile;
    CNumEdit   m_EditHome;
    CNumEdit   m_EditCompany;
	CEdit   m_EditEMail;
	CEdit   m_EditMark;
	CComboBox m_ComboGroup;
	bool       bLoadPic;    //用于标识是否更换了图片,true为更换,默认为false.
	CPbData*   m_pPbData;
	BOOL m_bSimCardFlag;  //标识是否是SIM卡中的联系人
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContactDlg)
    public:

    //消息预处理函数，拦截右键操作
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

    /*iInsertIndex变量用于区别调用子功能模块，其中InsertIndex等于-1时表示添加、短消息模块提取号码子功能，
      InsertIndex等于-2表示修改子功能。*/
    int iInsertIndex;

    /*Pos表示短消息模块提取号码子功能号码应存放的位置（mobile/home/company三种选择）*/
    int iPos;

    /*cNumber表示短消息模块提取号码子功能传过来的号码内容。*/
    TCHAR strNumber[PB_NUM_MAX + 1];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__0ACBC2AE_F3B6_42FC_8B32_00663186F390__INCLUDED_)
