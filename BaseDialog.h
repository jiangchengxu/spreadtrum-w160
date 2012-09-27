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

    //��������Ч����ɺ���Ҫ���еĶ�������������Ը���
    virtual int ActionAfterInitDlgWithFlashEffect();

    //Ϊ�ײ�ص��ķַ�������������Ҫ���������ݻ�����Ϣ��֪ͨ�����඼�������ش˺���
    virtual int DealwithResponseProc(LPARAM lParam,   // application-defined value
                                     WPARAM wParam   // application-defined value
                                    );
    bool CompareChar(const TCHAR* chSource);  //�����жϵ绰�������Ƿ��зǷ��ַ���added by ljl
    bool CompareCharAnsi(const char* chSource);
public:
    BOOL SetSkin(int nSkinType);
    CRect m_OriginRect;    //ԭʼ�ߴ�
    BOOL m_bstatus;
    static bool m_bWinSize;//Ϊ���ʾ������ΪĬ�ϴ�С��Ϊ�ٱ�ʾ��ǰ����Ϊ�����ʾ
    CRect m_MaxRect;
    CRect g_MainDlgRect; //�����ڵ���󻯳ߴ�
    CRect g_MainDlgMinRect; //�����ڵ�Ĭ�ϵĳߴ�
    CRect g_TabDlgRect;  //TAB���ڵ���󻯳ߴ�

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
    CBitmap                    m_bmpBackground;            //����ͼƬ

    BOOL                    m_blUseFlashEffect;            //�Ƿ���������Ч��
    BOOL                    m_blUseFlashEffectFinished;    //����Ч��չʾ�Ƿ����

    //����λ��
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

    //���ô��ڵ�Ƥ��
    BOOL SetSkin(CString file);
    //BOOL SetSkin(CString file,int mlev);

    //Ϊ��ײ㴫�������ֺ�������������ͱ���ͨ�����������в����յ��ص�
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
