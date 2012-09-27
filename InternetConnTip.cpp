// InternetConnTip.cpp : implementation file
//

#include "stdafx.h"
#include "InternetConnTip.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInternetConnTip dialog


CInternetConnTip::CInternetConnTip(CString strTip, CWnd* pParent /*=NULL*/)
    : CDialog(CInternetConnTip::IDD, pParent)
{
    //{{AFX_DATA_INIT(CInternetConnTip)
    // NOTE: the ClassWizard will add member initialization here
//	m_nBkTag = 1;
    //}}AFX_DATA_INIT
    m_strTip = strTip;
}


void CInternetConnTip::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CInternetConnTip)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_STATIC_INT_TIP, m_strTip);
}


BEGIN_MESSAGE_MAP(CInternetConnTip, CDialog)
    //{{AFX_MSG_MAP(CInternetConnTip)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK,OnButtonCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetConnTip message handlers

BOOL CInternetConnTip::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
//     m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CInternetConnTip::OnButtonCancel()
{
    CHSDPADlg *pDlg = (CHSDPADlg *)AfxGetMainWnd();
    RasHangUp(m_hRasConn);
    m_hRasConn = NULL;
    pDlg->m_pConnectDlg->killTimers();
    CDialog::OnCancel();
}
