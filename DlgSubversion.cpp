// DlgSubversion.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSubversion.h"
#include "BaseDialog.h"
#include "Hsdpadlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubversion dialog

#define VK_S 83

/////////////////////////////////////////////////////////////////////////////
// CDlgSubversion dialog


CDlgSubversion::CDlgSubversion(CWnd* pParent /*=NULL*/)
: CBaseDialog(CDlgSubversion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubversion)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nBkTag = 1;    
}


void CDlgSubversion::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubversion)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubversion, CBaseDialog)
//{{AFX_MSG_MAP(CDlgSubversion)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubversion message handlers

BOOL CDlgSubversion::OnInitDialog() 
{    
    CBaseDialog::OnInitDialog();
	
	CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	UnregisterHotKey(pMainWnd->GetSafeHwnd(),0);
	return TRUE;
}


void CDlgSubversion::OnCancel()
{
	CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	RegisterHotKey(pMainWnd->GetSafeHwnd(),0,MOD_CONTROL|MOD_ALT|MOD_SHIFT,VK_S);
	CBaseDialog::OnCancel();
	
}