// MenuBar.cpp : implementation file
//

#include "stdafx.h"
#include "MenuBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static HHOOK g_hMsgHook = NULL;

/////////////////////////////////////////////////////////////////////////////
// CMenuBar

CMenuBar::CMenuBar()
{
    m_hParentWnd = NULL;
    m_hMenu = NULL;

    for (int i=0; i<4; i++) {
        m_item[i] = -1;
    }

    m_itemCount = 0;

    m_nPopup = -1;
    m_nNextPop = -1;

    m_pFont = NULL;
}

CMenuBar::~CMenuBar()
{
    if (g_hMsgHook != NULL) {
        UnhookWindowsHookEx(g_hMsgHook);
        g_hMsgHook = NULL;
    }

    if (m_pFont) {
        m_pFont->DeleteObject();
        delete m_pFont;
        m_pFont = NULL;
    }
}


BEGIN_MESSAGE_MAP(CMenuBar, CWnd)
    //{{AFX_MSG_MAP(CMenuBar)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMenuBar message handlers

CMenuBar& CMenuBar::GetInstance()
{
    static CMenuBar menuBar;
    return menuBar;
}

BOOL CMenuBar::SetMenu(UINT nMenu, HWND hParentWnd, BOOL bRepaint)
{
    HINSTANCE	hInstResource	= NULL;

    // Destroy any previous menu
    if (m_hMenu) {
        ::DestroyMenu(m_hMenu);
        m_hMenu = NULL;
        m_hParentWnd = NULL;
    }

    // Load menu
    if (nMenu) {
        // Find correct resource handle
        hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nMenu), RT_MENU);
        // Load menu resource
        m_hMenu = ::LoadMenu(hInstResource, MAKEINTRESOURCE(nMenu));
        m_hParentWnd = hParentWnd;
        // If something wrong
        if (m_hMenu == NULL)
            return FALSE;
    }

    // Repaint the button
    if (bRepaint)
        FromHandle(m_hParentWnd)->Invalidate();

    return TRUE;
}

void CMenuBar::OnHookMove(WPARAM wParam, LPARAM lParam)
{
    CRect rect;
    POINT pt = ((MSG*)lParam)->pt;

    for (int i=0; i<4; i++) {
        FromHandle(m_hParentWnd)->GetDlgItem(m_item[i])->GetWindowRect(rect);
        if (m_nPopup != i && PtInRect(pt, rect)) {
            m_nNextPop = i;
            ::SendMessage(m_hParentWnd, WM_CANCELMODE, 0, 0);
            break;
        } else {
            m_nNextPop = -1;
        }
    }
}

LRESULT CALLBACK CMenuBar::MessageProc(int code, WPARAM wParam, LPARAM lParam)
{
    MSG *pMsg = (MSG*)lParam;

    if (pMsg->message == WM_MOUSEMOVE) {
        CMenuBar::GetInstance().OnHookMove(wParam, lParam);

        return 0;
    }
    return ::CallNextHookEx(g_hMsgHook, code, wParam, lParam);
}

void CMenuBar::TrackPopup(int nButton)
{
//   	if (nButton == m_nPopup && g_hMsgHook)
//   	{
//   		m_nPopup = -1;
//   		return;
//   	}

    while (nButton >= 0) {
        m_nPopup = nButton;

        //安装钩子
        g_hMsgHook = SetWindowsHookEx(WH_MSGFILTER, MessageProc, 0, GetCurrentThreadId());

        CRect rWnd;
        FromHandle(m_hParentWnd)->GetDlgItem(m_item[nButton])->GetWindowRect(rWnd);

        HMENU hSubMenu = ::GetSubMenu(m_hMenu, nButton);

        m_btnST[nButton].SetMenuPopup(TRUE);
        FromHandle(m_hParentWnd)->GetDlgItem(m_item[nButton])->Invalidate();

        DWORD dwRetValue = ::TrackPopupMenuEx(hSubMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY |
                                              TPM_RETURNCMD, rWnd.left, rWnd.bottom, m_hParentWnd, NULL);

        m_btnST[nButton].SetMenuPopup(FALSE);
        FromHandle(m_hParentWnd)->GetDlgItem(m_item[nButton])->Invalidate();

        //卸载钩子
        if (g_hMsgHook != NULL) {
            UnhookWindowsHookEx(g_hMsgHook);
            g_hMsgHook = NULL;
        }

        nButton = m_nNextPop;

        if (dwRetValue)
            ::PostMessage(m_hParentWnd, WM_COMMAND, MAKEWPARAM(dwRetValue, 0), (LPARAM)NULL);
    }
}

void CMenuBar::SetButtonItem(UINT item[], int count)
{
    m_itemCount = count;

    for (int i=0; i<count; i++) {
        m_item[i] = item[i];

        m_btnST[i].SubclassDlgItem(item[i], FromHandle(m_hParentWnd));
        m_btnST[i].DrawTransparent(TRUE);
    }
}

BOOL CMenuBar::PtInRect(POINT pt, CRect rc)
{
    if (pt.x > rc.left && pt.x < rc.right && pt.y > rc.top && pt.y <rc.bottom) {
        return TRUE;
    }
    return FALSE;
}

void CMenuBar::SetFont(int nHeight, int nWidth, LPCTSTR name)
{
    if (m_pFont) {
        m_pFont->DeleteObject();
        delete m_pFont;
        m_pFont = NULL;
    }

    LOGFONT lFont;
    FromHandle(m_hParentWnd)->GetFont()->GetLogFont(&lFont);

    if (nHeight) {
        lFont.lfHeight = nHeight;
    }
    if (nWidth) {
        lFont.lfWidth = nWidth;
    }

    m_pFont = new CFont();
    if (name) {
        m_pFont->CreateFont(lFont.lfHeight, lFont.lfWidth, lFont.lfEscapement, lFont.lfOrientation, lFont.lfWeight,
                            lFont.lfItalic, lFont.lfUnderline, lFont.lfStrikeOut, lFont.lfCharSet,
                            lFont.lfOutPrecision, lFont.lfClipPrecision, lFont.lfQuality, lFont.lfPitchAndFamily, name);
    } else {
        m_pFont->CreateFont(lFont.lfHeight, lFont.lfWidth, lFont.lfEscapement, lFont.lfOrientation, lFont.lfWeight,
                            lFont.lfItalic, lFont.lfUnderline, lFont.lfStrikeOut, lFont.lfCharSet,
                            lFont.lfOutPrecision, lFont.lfClipPrecision, lFont.lfQuality, lFont.lfPitchAndFamily, lFont.lfFaceName);
    }

    for (int i=0; i<m_itemCount; i++) {
        m_btnST[i].SetFont(m_pFont);
    }
}
