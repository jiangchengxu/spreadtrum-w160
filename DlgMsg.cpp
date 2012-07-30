// DlgMsg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog


CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMsg)
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
	m_nBkTag = 1;
}


void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsg)
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsg, CBaseDialog)
	//{{AFX_MSG_MAP(CDlgMsg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg message handlers
