// MsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "MsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNAMIC(CMsgBox, CBaseDialog)
/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog
CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
    :CBaseDialog(CMsgBox::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMsgBox)
    m_strInfo = _T("");
    m_nBkTag = 1;
    m_blUseFlashEffect = FALSE;
    m_bPreMsgBox = TRUE;
    //}}AFX_DATA_INIT
    m_nRet= IDCANCEL;
}
CMsgBox::CMsgBox(CWnd* pParent,LPCTSTR lpszPrompt,UINT nType, UINT nIDPrompt)
    :CBaseDialog(CMsgBox::IDD, pParent)
{
    m_strInfo = lpszPrompt;
    m_nBkTag = 1;
    m_blUseFlashEffect = FALSE;
    m_bPreMsgBox = TRUE;
    m_nRetTag = 0;
    //m_nRet = IDCANCEL;
    m_nType = nType;
}
CMsgBox::~CMsgBox()
{

}

void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgBox)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Text(pDX, IDC_STATIC_MSGBOX_INFO, m_strInfo);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgBox, CBaseDialog)
    //{{AFX_MSG_MAP(CMsgBox)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
    ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopDlg message handlers

BOOL CMsgBox::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    m_bPreMsgBox = TRUE;

//	SetSkin(g_SetData.Setup_nSkinStyle);

    //m_hMsgEvt = CreateEvent(NULL, FALSE, FALSE, NULL);

    if((m_nType&MB_TYPEMASK)==MB_OK) {
        m_nRetTag = 0;
        SetDlgItemText(IDC_BTN_OK,_T("OK"));
        GetDlgItem(IDC_BTN_OK)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_HIDE);
    } else if((m_nType&MB_TYPEMASK)==MB_OKCANCEL) {
        m_nRetTag = 0;
        SetDlgItemText(IDC_BTN_OK,_T("OK"));
        SetDlgItemText(IDC_BTN_CANCEL,_T("Cancel"));
        GetDlgItem(IDC_BTN_OK)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_SHOW);
    } else if((m_nType&MB_TYPEMASK)==MB_RETRYCANCEL) {
        SetDlgItemText(IDC_BTN_OK,_T("Retry"));
        SetDlgItemText(IDC_BTN_CANCEL,_T("Cancel"));
        GetDlgItem(IDC_BTN_OK)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_SHOW);
    } else if((m_nType&MB_TYPEMASK)==MB_YESNO) {
        m_nRetTag = 1;
        SetDlgItemText(IDC_BTN_OK,_T("Yes"));
        SetDlgItemText(IDC_BTN_CANCEL,_T("No"));
        GetDlgItem(IDC_BTN_OK)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_SHOW);
    }
    return TRUE;
}
/*
void CMsgBox::OnOK()
{
	CBaseDialog::OnOK();
	m_nRet= IDOK;
	//SetEvent(m_hMsgEvt);
}

void CMsgBox::OnCancel()
{
	CBaseDialog::OnCancel();
	m_nRet= IDCANCEL;
	//SetEvent(m_hMsgEvt);
}
*/
void CMsgBox::OnBtnCancel()
{
    CBaseDialog::OnCancel();
    m_nRet=(m_nRetTag==0)?IDCANCEL:IDNO;

}

void CMsgBox::OnBtnOk()
{
    CBaseDialog::OnOK();
    m_nRet=(m_nRetTag==0)?IDOK:IDYES;
}

int CMsgBox::DoModal()
{
    // TODO: Add your specialized code here and/or call the base class
    CBaseDialog::DoModal();
    return m_nRet;
}

int CMsgBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
    m_nRet=(m_nRetTag==0)?IDCANCEL:IDNO;
    CBaseDialog::OnCommand(wParam, lParam);
    return 	m_nRet;
}
