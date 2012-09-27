/*
File: HSDPADlg.cpp
Desc:
    数据通讯的历史记录

Note:
$Log: $


*/

// DataServiceHistory.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "DataServiceHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataServiceHistory dialog


CDataServiceHistory::CDataServiceHistory(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CDataServiceHistory::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDataServiceHistory)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDataServiceHistory::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDataServiceHistory)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataServiceHistory, CBaseDialog)
    //{{AFX_MSG_MAP(CDataServiceHistory)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataServiceHistory message handlers
