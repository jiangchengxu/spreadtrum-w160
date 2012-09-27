// CBaseDialog.h : header file
//

#if !defined(AFX_BASEDLG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_)
#define AFX_BASEDLG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBaseDialog dialog

#include "SkinWin\SkinWin.h"
#include "SkinWin\SkinButton.h"

#include  <io.h>
#include  <stdio.h>

#include "resource.h"
#include "XInfoTip.h"
#include "SetupData.h"
class CBaseDialog : public CDialog
{
public:
    int m_nBkTag;
    CString CmGetAppPath();
    DECLARE_DYNAMIC(CBaseDialog)
// Construction
public:
    void RegisterForNetworkNotify(UINT nEventID);

    CBaseDialog(UINT nIDTemplate, CWnd* pParent = NULL);    // standard constructor

    virtual ~CBaseDialog();

    void InitDlgWithFlashEffect();

    virtual BOOL SubClassButton( HWND hwnd );

    //窗口启动效果完成后所要进行的动作，派生类可以覆盖
    virtual int ActionAfterInitDlgWithFlashEffect();

    //为底层回调的分发函数，凡是需要处理返回数据或者消息、通知的子类都必须重载此函数
    virtual int DealwithResponseProc(LPARAM lParam,   // application-defined value
                                     WPARAM wParam   // application-defined value
                                    );
    bool CompareChar(const TCHAR* chSource);  //用于判断电话号码中是否有非法字符。added by ljl
    bool CompareCharAnsi(const char* chSource);
public:
    BOOL SetSkin(int nSkinType);
    CRect m_OriginRect;    //原始尺寸
    BOOL m_bstatus;
    static bool m_bWinSize;//为真表示主窗口为默认大小，为假表示当前窗口为最大化显示
    CRect m_MaxRect;
    CRect g_MainDlgRect; //主窗口的最大化尺寸
    CRect g_MainDlgMinRect; //主窗口的默认的尺寸
    CRect g_TabDlgRect;  //TAB窗口的最大化尺寸

// Dialog Data
    //{{AFX_DATA(CBaseDialog)
//    enum { IDD = IDD_DIALOG1 };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBaseDialog)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON                    m_hIcon;
    CBitmap                    m_bmpBackground;            //背景图片

    BOOL                    m_blUseFlashEffect;            //是否启用启动效果
    BOOL                    m_blUseFlashEffectFinished;    //启动效果展示是否完毕

    //窗口位置
    int                        m_nWidth;
    int                        m_nHeight;
    int                        m_dx;
    int                        m_dy;

    CSkinWin                m_skinWin;        //skin win
    BOOL                    m_bFirst;        //first time call
    BOOL					m_bPreMsgBox;
    CObList                    m_wndList;

    CSkinButtonResource        m_btnres;        //skin button resource

    //CXInfoTip                m_Tip;
    HICON                    m_hIconInfo;

    //设置窗口的皮肤
    BOOL SetSkin(CString file);
    //BOOL SetSkin(CString file,int mlev);

    //为向底层传送命令字函数，所有命令传送必须通过本函数进行才能收到回调
    void PostCmd(CString strCmd);

    // Generated message map functions
    //{{AFX_MSG(CBaseDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnDestroy();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    //}}AFX_MSG
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()
private:
    CBrush m_brBkgnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEDLG_H__336A0773_407A_4803_94D1_9D27712706A9__INCLUDED_)
