// SmsListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SmsListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsListCtrl

CSmsListCtrl::CSmsListCtrl(BOOL bEnableRB) : m_bEnableRB(bEnableRB)
{
    m_DispFilter = SMS_TYPE_INBOX;
    loctype = LOC_PC;
}

CSmsListCtrl::~CSmsListCtrl()
{
}

BEGIN_MESSAGE_MAP(CSmsListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CSmsListCtrl)
    ON_WM_CONTEXTMENU()
    //}}AFX_MSG_MAP
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_SMS_READ, /*IDC_BUTTON_SMS_EXPORT*/IDC_BUTTON_SMS_RESUME, OnButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsListCtrl message handlers

void CSmsListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
    if(GetNextItem(-1, LVNI_SELECTED) == -1)
        return;

    if(!m_bEnableRB)
        return;

    CString strItemText;
    CMenu menu;
    menu.CreatePopupMenu();



    //strItemText.LoadString(IDS_SMS_READ);   //del by liub 1103 for SMS
    //menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_READ, strItemText);   //del by liub 1103 for SMS

    strItemText.LoadString(IDS_SMS_NEW);
    menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_NEW, strItemText);
    menu.AppendMenu(MF_SEPARATOR);
    if(m_DispFilter == SMS_TYPE_INBOX && loctype == LOC_PC) {
        strItemText.LoadString(IDS_SMS_REPLY);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_REPLY, strItemText);
        strItemText.LoadString(IDS_SMS_FORWARD);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_FORWARD, strItemText);
        strItemText.LoadString(IDS_SMS_ABSTRACT);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_ABSTRACT, strItemText);
        strItemText.LoadString(IDS_SMS_DELETE);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DELETE, strItemText);
        menu.AppendMenu(MF_SEPARATOR);
        strItemText.LoadString(IDS_SMS_ARCHIVE);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_ARCHIVE, strItemText);
        strItemText.LoadString(IDS_SMS_DIAL);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DIAL, strItemText);
//         strItemText.LoadString(IDS_SMS_SEARCH);
// 		menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_SEARCH, strItemText);
    }
    if (m_DispFilter == SMS_TYPE_OUTBOX && loctype == LOC_PC) {
        strItemText.LoadString(IDS_SMS_EDIT);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_EDIT, strItemText);
        strItemText.LoadString(IDS_SMS_FORWARD);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_FORWARD, strItemText);
        strItemText.LoadString(IDS_SMS_ABSTRACT);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_ABSTRACT, strItemText);
        strItemText.LoadString(IDS_SMS_DELETE);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DELETE, strItemText);
        menu.AppendMenu(MF_SEPARATOR);
//         strItemText.LoadString(IDS_SMS_SEARCH);
// 		menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_SEARCH, strItemText);
    }
    if (m_DispFilter == SMS_TYPE_DRAFT && loctype == LOC_PC) {
        strItemText.LoadString(IDS_SMS_EDIT);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_EDIT, strItemText);
        strItemText.LoadString(IDS_SMS_DELETE);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DELETE, strItemText);
//         strItemText.LoadString(IDS_SMS_SEARCH);
// 		menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_SEARCH, strItemText);

    }
    if (m_DispFilter == SMS_TYPE_ARCHIVE && loctype == LOC_PC) {
        strItemText.LoadString(IDS_SMS_RESUME);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_RESUME, strItemText);
        strItemText.LoadString(IDS_SMS_DELETE);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DELETE, strItemText);
//         strItemText.LoadString(IDS_SMS_SEARCH);
// 		menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_SEARCH, strItemText);
    }
    if (loctype == LOC_ME || loctype == LOC_UIM) {
        strItemText.LoadString(IDS_SMS_REPLY);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_REPLY, strItemText);
        strItemText.LoadString(IDS_SMS_FORWARD);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_FORWARD, strItemText);
        strItemText.LoadString(IDS_SMS_ABSTRACT);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_ABSTRACT, strItemText);
        menu.AppendMenu(MF_SEPARATOR);
        strItemText.LoadString(IDS_SMS_DIAL);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DIAL, strItemText);
        strItemText.LoadString(IDS_SMS_DELETE);
        menu.AppendMenu(MF_ENABLED, IDC_BUTTON_SMS_DELETE, strItemText);

    }
    menu.SetDefaultItem(IDC_BUTTON_SMS_READ, FALSE);
    GetCursorPos(&point);
    menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CSmsListCtrl::OnButtonClicked(UINT nID)
{
    //¸¸´°¿Ú£ºCSmsListCtrl->CEnTabCtrl->CSmsDlg
    CWnd *pParent = GetParent();//modify by liub 1104 for SMS
    ASSERT(pParent != NULL);
    pParent->SendMessage(WM_SMS_MENUITEM_CLICK, nID, 0);
}

void CSmsListCtrl::SetDspFilter(EnSmsType type)
{
    ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);

    m_DispFilter = type;
}

void CSmsListCtrl::SetLocType(EnLocType LocNow)
{
    ASSERT(LocNow >= LOC_PC && LocNow < LOC_MAX);

    loctype = LocNow;

}

void CSmsListCtrl::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd());
    CListCtrl::PreSubclassWindow();
}
