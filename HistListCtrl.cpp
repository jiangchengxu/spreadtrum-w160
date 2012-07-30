// HistListCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "HSDPA.h"
#include "HistListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CHistListCtrl
CHistListCtrl::CHistListCtrl()
{
}

CHistListCtrl::~CHistListCtrl()
{
}

BEGIN_MESSAGE_MAP(CHistListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CHistListCtrl)
    ON_WM_CONTEXTMENU()
    //}}AFX_MSG_MAP
    ON_COMMAND_RANGE(ID_MHIST_RECALL, ID_MHIST_DELETEALL, OnMenuClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistListCtrl message handlers
void CHistListCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    if(GetNextItem(-1, LVNI_SELECTED) == -1)
        return;
    
    CMenu histMenu;
    histMenu.LoadMenu( IDR_MENU_HIST_RCLICK );
    
    CMenu* pPopup = histMenu.GetSubMenu( 0 );
    ASSERT_VALID( pPopup );
    
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                            point.x, point.y,
                            this, NULL);
}

void CHistListCtrl::OnMenuClicked(UINT nID)
{
    UINT arID[]={
    IDC_BUTTON_REPLY,
    IDC_BUTTON_ABSTRACT,
    IDC_BUTTON_SENDSMS,
    IDC_BUTTON_DELETE,
    IDC_BUTTON_DELETEALL
    };

    //¸¸´°¿Ú£ºCHistListCtrl->CEnTabCtrl->CCallHistoryDlg
    CWnd *pParent = GetParent()->GetParent();
    ASSERT(pParent != NULL);
    pParent->SendMessage(WM_HIST_MENUITEM_CLICK, arID[nID-32771], 0);
}

void CHistListCtrl::PreSubclassWindow() 
{
    m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
    CListCtrl::PreSubclassWindow();
}
