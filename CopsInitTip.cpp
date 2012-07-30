// CopsInitTip.cpp : implementation file
//

#include "stdafx.h"
#include "CopsInitTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopsInitTip dialog


CCopsInitTip::CCopsInitTip(CString strTip, CWnd* pParent /*=NULL*/)
	: CDialog(CCopsInitTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopsInitTip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		m_strTip = strTip;
}


void CCopsInitTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopsInitTip)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	 DDX_Text(pDX, IDC_STATIC_INT_TIP, m_strTip);
}


BEGIN_MESSAGE_MAP(CCopsInitTip, CDialog)
	//{{AFX_MSG_MAP(CCopsInitTip)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	 ON_BN_CLICKED(IDOK,OnButtonCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopsInitTip message handlers

BOOL CCopsInitTip::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//     m_bFirst = TRUE;
	// 	SetSkin(g_SetData.Setup_nSkinStyle);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCopsInitTip::OnButtonCancel()
{
	RasHangUp(m_hRasConn);
	m_hRasConn = NULL;
	CDialog::OnCancel();
}
