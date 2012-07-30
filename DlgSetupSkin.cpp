// DlgSetupSkin.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "DlgSetupSkin.h"
#include "SetupData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL SaveIniFile();

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupSkin dialog


CDlgSetupSkin::CDlgSetupSkin(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CDlgSetupSkin::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetupSkin)
    m_nStyle = -1;
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
}


void CDlgSetupSkin::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetupSkin)
    DDX_Radio(pDX, IDC_RADIO_SETUP_SKIN_DEFAULT, m_nStyle);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupSkin, CBaseDialog)
    //{{AFX_MSG_MAP(CDlgSetupSkin)
    ON_BN_CLICKED(IDC_RADIO_SETUP_SKIN_DEFAULT, OnRadioSetupSkin)
    ON_BN_CLICKED(IDC_RADIO_SETUP_SKIN_XP, OnRadioSetupSkin)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupSkin message handlers

void CDlgSetupSkin::OnRadioSetupSkin() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    WPARAM wParam = 0;
    LPARAM lParam = m_nStyle;
    ::SendMessage(GetActiveWindow()->GetSafeHwnd(),WM_CHANGE_SKIN, wParam,lParam);  
    g_SetData.Setup_nSkinStyle = m_nStyle;
    SaveIniFile();
}

//DEL void CDlgSetupSkin::OnButtonSetupSkinBrowseskinfile() 
//DEL {
//DEL     // TODO: —°‘ÒSkin File
//DEL //    CFileDialog dlgFile(TRUE);
//DEL //    if (dlgFile.DoModal()==IDOK)
//DEL //    {
//DEL //        m_strSkinFile=(CString)dlgFile.m_ofn.lpstrFile;
//DEL //        UpdateData(FALSE);
//DEL //    }
//DEL }

BOOL CDlgSetupSkin::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_nStyle = g_SetData.Setup_nSkinStyle;
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
