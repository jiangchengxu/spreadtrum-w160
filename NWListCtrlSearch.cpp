// NWListCtrlSearch.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "NWListCtrlSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNWListCtrlSearch

CNWListCtrlSearch::CNWListCtrlSearch()
{
}

CNWListCtrlSearch::~CNWListCtrlSearch()
{
}


BEGIN_MESSAGE_MAP(CNWListCtrlSearch, CListCtrl)
    //{{AFX_MSG_MAP(CNWListCtrlSearch)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNWListCtrlSearch message handlers
void CNWListCtrlSearch::PreSubclassWindow()
{
    m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
    CListCtrl::PreSubclassWindow();
}