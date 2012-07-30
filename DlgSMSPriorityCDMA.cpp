// DlgSMSPriorityCDMA.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "DlgSMSPriorityCDMA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSMSPriorityCDMA dialog


CDlgSMSPriorityCDMA::CDlgSMSPriorityCDMA(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSMSPriorityCDMA::IDD, pParent)
{
	m_nBkTag = 1;
	m_nSMSPriority=0;
}


void CDlgSMSPriorityCDMA::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Radio(pDX,IDC_RADIO_SMS_GENERAL,m_nSMSPriority);
}


BEGIN_MESSAGE_MAP(CDlgSMSPriorityCDMA, CBaseDialog)
	//{{AFX_MSG_MAP(CDlgSMSPriorityCDMA)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSMSPriorityCDMA message handlers
BOOL CDlgSMSPriorityCDMA::OnInitDialog()
{
 CBaseDialog::OnInitDialog();
  	m_bFirst = TRUE;
 	//SetSkin(g_SetData.Setup_nSkinStyle);
	m_nSMSPriority=0;
  UpdateData(FALSE);
  return TRUE;
}

BOOL CDlgSMSPriorityCDMA::OnPaint()
{
	CBaseDialog::OnPaint();
}

