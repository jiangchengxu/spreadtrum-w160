// NWListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "NWListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNWListCtrl

CNWListCtrl::CNWListCtrl()
{
}

CNWListCtrl::~CNWListCtrl()
{
}


BEGIN_MESSAGE_MAP(CNWListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CNWListCtrl)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNWListCtrl message handlers
void CNWListCtrl::PreSubclassWindow()
{
    m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
    CListCtrl::PreSubclassWindow();
}