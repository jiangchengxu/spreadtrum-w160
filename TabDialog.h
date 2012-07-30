//////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABDIALOG_H__B0253332_216E_4954_832A_200D7B58D3DC__INCLUDED_)
#define AFX_TABDIALOG_H__B0253332_216E_4954_832A_200D7B58D3DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDialog.h : header file
//
#include <afxtempl.h>
#include "SkinWin\SkinWin.h"
#include "SkinWin\SkinButton.h"

#include "Common_DataCrad/resource.h"
#include "CallDlg.h"


#include "TabedDialog.h"

#include "ShadeButtonST.h"
#include "BtnST.h"
#include "ScreenStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CTabDialog dialog
typedef enum TABDIALOGSTYLE
{
    TABDIALOGSTYLE_VERICAL = 1,    //TAB��ť��������
    TABDIALOGSTYLE_HORIZONTAL,    //TAB��ť��������
};

typedef enum TABDIALOG_MOVEDIRECTION
{
    TABDIALOG_MOVEDIRECTION_LEFTTORIGHT = 1,    //����
    TABDIALOG_MOVEDIRECTION_RIGHTTOLEFT,    //�ҵ���
    TABDIALOG_MOVEDIRECTION_UPTODOWN,    //�ϵ���
    TABDIALOG_MOVEDIRECTION_DOWNTOUP,    //�µ���
};

class CTabDialog : public CBaseDialog
{
    DECLARE_DYNAMIC(CTabDialog)
// Construction
public:
    CTabDialog(UINT nID, CWnd* pParent /*= NULL*/);   // standard constructor
    ~CTabDialog();
    virtual CWnd* GetChildDlgContainer();
	void ReLoadLogo(CString strPLMN);
	void InitTestBoxShow();
  
// Dialog Data
    //{{AFX_DATA(CTabDialog)
   // enum { IDD = IDD_TABDLG};
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
	BOOL InCalling;

    CShadeButtonST m_btn1;
	CShadeButtonST m_btn2;
    CShadeButtonST m_btn3;
    CShadeButtonST m_btn4;
    CShadeButtonST m_btn5;
    CShadeButtonST m_btn6; 
    CShadeButtonST m_btn7;
	CShadeButtonST m_btn8;
	CShadeButtonST m_btn9;
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabDialog)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support    
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CTabDialog)
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    afx_msg LRESULT OnSwitchMe(WPARAM wParam = 0, LPARAM lParam = 0);
    afx_msg LRESULT OnDial(WPARAM wParam, LPARAM lParam = 0);
    afx_msg LRESULT ChangeSkin(WPARAM wParam, LPARAM lParam = 0);
	afx_msg LRESULT WindowMax(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT WindowRestore(WPARAM wParam = 0, LPARAM lParam = 0);

    DECLARE_MESSAGE_MAP()

public:
    BOOL AddPage(CTabedDialog* pDialog, CWnd* pButton);
    BOOL AddPage(CTabedDialog* pDialog, UINT nButtonID);
    virtual void InitPagesShow();
    BOOL SwitchDlg(CTabedDialog* pDlg);
	void InitMenuMap();

//	CScreenStatic m_TabStaticBKground;//add by liub 1030 for tabstatic background 
private:
	CWnd* m_pCurBtn; //��ǰ��ť
	BOOL m_moveEnding;
    void InitButtonsShow();
    void InitDialogShow();

    void HideAllPages();

    void MoveTabDlg() ;
public:    
    TABDIALOGSTYLE m_nStyle;    //1:TAB��ť�������£�2��TAB��ť��������
    BOOL m_bMoveWindow;            //�����Ƿ����ƶ���Ч����ʾ
    TABDIALOG_MOVEDIRECTION    m_nMoveDirection;
protected:
    int m_nTotalPage;
    CMap<int, int&, CTabedDialog*, CTabedDialog*> m_DialogMap;    //�������жԻ����ָ��
    CMap<int, int&, CWnd*, CWnd*> m_ButtonMap;                    //�������а�ť��ָ��
    CTabedDialog *m_pCurWnd;//��ǰ����    
    
    int m_ispeed;//�˶��ٶ�
    int m_wndwidth;//�Ӵ���Ŀ��
    int m_wndheight;
    CPoint m_bottomright;//�Ӵ������½ǵ�����
    CPoint m_topleft;//�Ӵ������Ͻǵ�����
    CRect m_CurWndRect;
	int m_Currentbutton; //��¼��ǰ��ʾ��BUTTON����

	CMap<int,int,int,int> m_MenuMap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDIALOG_H__B0253332_216E_4954_832A_200D7B58D3DC__INCLUDED_)
