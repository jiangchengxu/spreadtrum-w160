// PbImport.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "DlgPbImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPbImport dialog


CDlgPbImport::CDlgPbImport(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CDlgPbImport::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgPbImport)
    // NOTE: the ClassWizard will add member initialization here
    m_nBkTag = 1;
    //}}AFX_DATA_INIT
    m_nCheck = -1;
}


void CDlgPbImport::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPbImport)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_RADIO_FILE, m_RFile);
    DDX_Control(pDX, IDC_RADIO_OUTLOOK, m_ROutlook);
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CDlgPbImport, CBaseDialog)
    //{{AFX_MSG_MAP(CDlgPbImport)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPbImport message handlers

BOOL CDlgPbImport::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;
    //SetSkin(g_SetData.Setup_nSkinStyle);

    // TODO: Add extra initialization here
    m_RFile.SetCheck(1);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPbImport::OnOK()
{
    // TODO: Add extra validation here
    if( 1 == m_RFile.GetCheck()) {
        m_nCheck = 1;
    } else {
        m_nCheck = 2;
    }
    CBaseDialog::OnOK();
}


