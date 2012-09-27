// SelPosition.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SelPosition.h"
#include "ContactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPosition dialog


CSelPosition::CSelPosition(CWnd* pParent,const TCHAR *cNumber/*=NULL*/)
    : CBaseDialog(CSelPosition::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelPosition)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_nBkTag = 1;
    wcscpy(Number, cNumber);
}


void CSelPosition::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelPosition)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPosition, CBaseDialog)
    //{{AFX_MSG_MAP(CSelPosition)
    ON_BN_CLICKED(IDC_RADIO_MOBILE, OnRadioMobile)
    ON_BN_CLICKED(IDC_RADIO_HOME, OnRadioHome)
    ON_BN_CLICKED(IDC_RADIO_COMPANY, OnRadioCompany)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_ABSTRACT_NUM, OnAbstractNum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelPosition message handlers

BOOL CSelPosition::OnInitDialog()
{
    CBaseDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_bFirst = TRUE;

    m_pPbData   = ((CHSDPAApp *)AfxGetApp())->GetPbData();//add by liub
    //SetSkin(g_SetData.Setup_nSkinStyle);

    ((CButton*)GetDlgItem(IDC_RADIO_MOBILE))->SetCheck(1);
    OnRadioMobile();
    UpdateData(FALSE);

    this->GetFocus();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}



void CSelPosition::OnRadioMobile()
{
    // TODO: Add your control notification handler code here
    Position=1;
}

void CSelPosition::OnRadioHome()
{
    // TODO: Add your control notification handler code here
    Position=2;
}

void CSelPosition::OnRadioCompany()
{
    // TODO: Add your control notification handler code here
    Position=3;
}

void CSelPosition::OnOK()
{
    // TODO: Add extra validation here

    ShowWindow(SW_HIDE);
    CContactDlg dlg(this,CONTACTDLG_TYPE_EXTRACT,Position,Number);
    dlg.m_bSimCardFlag = FALSE;// save position,default is PC
    dlg.m_pPbData=m_pPbData;//PBGroup
    dlg.DoModal();
}

LRESULT CSelPosition::OnAbstractNum(WPARAM wParam, LPARAM lParam)
{
    //Cancel
    if(wParam == 1)
        ShowWindow(SW_NORMAL);
    else { //Ok
        if(GetParent())
            GetParent()->PostMessage(WM_ABSTRACT_NUM, 0);
        OnCancel();
    }
    return 0;
}

void CSelPosition::OnCancel()
{
    // TODO: Add extra cleanup here
    if(GetParent())
        GetParent()->PostMessage(WM_ABSTRACT_NUM, 1);
    CBaseDialog::OnCancel();
}
