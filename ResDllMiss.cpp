// ResDllMiss.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "ResDllMiss.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResDllMiss dialog


CResDllMiss::CResDllMiss(CWnd* pParent /*=NULL*/)
	: CDialog(CResDllMiss::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResDllMiss)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CResDllMiss::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResDllMiss)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResDllMiss, CDialog)
	//{{AFX_MSG_MAP(CResDllMiss)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResDllMiss message handlers
