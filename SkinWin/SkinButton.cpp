// SkinButton.cpp : implementation file
//

#include "stdafx.h"
#include "SkinButton.h"
#include "UtilsSkin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinButton
IMPLEMENT_DYNAMIC(CSkinButton, CButton)

CSkinButton::CSkinButton()
{
    m_res = NULL;
    m_bMouseIn = m_bDown = FALSE;
    m_backcolor = ::GetSysColor(COLOR_BTNFACE);
    m_textcolor = ::GetSysColor(COLOR_BTNTEXT);

}

CSkinButton::~CSkinButton()
{
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
    //{{AFX_MSG_MAP(CSkinButton)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_ERASEBKGND()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_ENABLE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinButton message handlers
// extern CString GetPathName( const TCHAR * filename );
// extern CString GetFileName( const TCHAR * filename, int ext = 0);
// extern TCHAR *next_token( TCHAR *buf, TCHAR *token, TCHAR *stopchars );

BOOL CSkinButtonResource::LoadSkin(const TCHAR *skinfile, const CString& strControlType)
{
    static const TCHAR * ps = _T("Buttons");
    TCHAR buf[1000];
    CString path = GetPathName( skinfile );

    //wyw
    if (m_bInited) {
        m_bmpButton.DeleteObject();
    }

//    GetPrivateProfileString( ps, "Bitmap", "", buf, 1000, skinfile );
    GetPrivateProfileString( ps, strControlType, _T(""), buf, 1000, skinfile );
    if ( *buf == 0 || !m_bmpButton.LoadBitmap( path + _T("/")+ GetFileName( buf,1 )) )
        return FALSE;

    m_TopHeight = GetPrivateProfileInt( ps, _T("TopHeight"), 0, skinfile );
    m_BottomHeight = GetPrivateProfileInt( ps, _T("BottomHeight"), 0, skinfile );
    m_LeftWidth = GetPrivateProfileInt( ps, _T("LeftWidth"), 0, skinfile );
    m_RightWidth = GetPrivateProfileInt( ps, _T("RightWidth"), 0, skinfile );

    m_bTrans = GetPrivateProfileInt( ps, _T("Trans"), 0, skinfile );


    m_bInited = TRUE;
    return TRUE;
}

BOOL CSkinButtonResource::DrawAImage(CDC *pDC, CRect r, CRect sr)
{
    int w = sr.Width();
    int h = sr.Height();
    int tw = r.Width();
    int th = r.Height();
    int x = sr.left;
    int y = sr.top;

    m_bmpButton.Draw( pDC, 0, 0, CRect(x, y, x + m_LeftWidth, y + m_TopHeight) );
    m_bmpButton.Draw( pDC, tw - m_RightWidth, 0, CRect(x + w-m_RightWidth, y, x + w +m_RightWidth,y+m_TopHeight) );
    m_bmpButton.Draw( pDC, 0, th-m_BottomHeight, CRect(x, y+h-m_BottomHeight, x+m_LeftWidth, y+h) );
    m_bmpButton.Draw( pDC, tw - m_RightWidth, th-m_BottomHeight, CRect(x+w -m_RightWidth, y+h-m_BottomHeight, x+w, y+h) );

    m_bmpButton.StretchDraw( pDC, CRect( m_LeftWidth, 0, tw - m_RightWidth, m_TopHeight ),
                             CRect( x + m_LeftWidth, y, x + w-m_RightWidth, y+m_TopHeight ) );
    m_bmpButton.StretchDraw( pDC, CRect( m_LeftWidth, th-m_BottomHeight, tw - m_RightWidth, th ),
                             CRect( x + m_LeftWidth, y+h-m_BottomHeight, x + w-m_RightWidth, y+h ) );

    m_bmpButton.StretchDraw( pDC, CRect( 0, m_TopHeight, m_LeftWidth, th - m_BottomHeight ),
                             CRect( x, y+m_TopHeight, x + m_LeftWidth , y+h-m_BottomHeight  ) );

    m_bmpButton.StretchDraw( pDC, CRect( tw - m_RightWidth, m_TopHeight, tw, th - m_BottomHeight  ),
                             CRect( x + w-m_RightWidth, y+m_TopHeight, x+w, y+h-m_BottomHeight ) );

    m_bmpButton.StretchDraw( pDC, CRect( m_LeftWidth, m_TopHeight, tw - m_RightWidth, th - m_BottomHeight ),
                             CRect( x + m_LeftWidth, y + m_TopHeight, x + w-m_RightWidth, y+h-m_BottomHeight ) );

    return TRUE;
}

BOOL CSkinButtonResource::DrawImage(CDC *pDC, CRect r, int state)
{
    int w = m_bmpButton.Width();
    int h = m_bmpButton.Height();
    if ( state == 0 ) {
        //nomral
        pDC->Draw3dRect( r, RGB(0,0,255), RGB(0, 0, 255 ) );
        //pDC->TextOut( 0, 0, "normal");
        DrawAImage( pDC, r, CRect( 0, 0, w/5, h ) );
    } else if ( state == 1 ) {
        //hover
        pDC->Draw3dRect( r, RGB(0,255,0), RGB(0, 255, 0 ) );
        //pDC->TextOut( 0, 0, "hover");
        DrawAImage( pDC, r, CRect( 3 * w/5, 0, 4 * w/5, h ) );
    } else if ( state == 2 ) {
        //down
        pDC->Draw3dRect( r, RGB(255,0,0), RGB(255, 0, 0 ) );
        //pDC->TextOut( 0, 0, "down");
        DrawAImage(pDC, r,  CRect( w/5, 0, 2 * w/5, h ) );
    } else if ( state == 3 ) {
        //normal with focus
        pDC->Draw3dRect( r, RGB(255,255,0), RGB(255, 255, 0 ) );
        //pDC->TextOut( 0, 0, "focus");
        DrawAImage( pDC, r, CRect( 3 * w/5, 0, 4 * w/5, h ) );
    } else if ( state == 4 ) {

        pDC->Draw3dRect( r, RGB(255,255,0), RGB(255, 255, 0 ) );

        DrawAImage( pDC, r, CRect( 2 * w/5, 0, 3 * w/5, h ) );
    }
    return TRUE;
}


BOOL CSkinButton::DrawImage(CMyBitmap &bmp)
{
    m_bFocus = GetFocus()->GetSafeHwnd() == m_hWnd;
    m_bEnable = !(GetWindowLong(m_hWnd,GWL_STYLE) & WS_DISABLED);

    CBitmap *obmp;
    CDC memDC;
    CClientDC dc(0);
    CRect r;
    GetClientRect(r);

    memDC.CreateCompatibleDC(&dc);
    bmp.CreateCompatibleBitmap(&dc, r.Width(), r.Height() );
    obmp = memDC.SelectObject( &bmp );

    if ( !m_bEnable ) {
        m_res->DrawImage( &memDC, r, 4 );
    } else if ( m_bMouseIn &&m_bDown ) {
        m_res->DrawImage( &memDC, r, 2);
    } else if ( m_bMouseIn ) {
        m_res->DrawImage( &memDC, r, 1 );
    } else if ( m_bFocus ) {
        m_res->DrawImage( &memDC, r, 3 );
    } else {
        m_res->DrawImage( &memDC, r,0  );
    }
    memDC.SelectObject( obmp );

    //wyw
    memDC.DeleteDC();

    return TRUE;

}

void CSkinButton::OnPaint()
{
    if ( !m_res || !m_res->m_bInited ) {
        Default();
        return;
    }
    //Note:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // to draw button, cliprgn is needed
    //
    CPaintDC dc(this); // device context for painting
    CRect r;
    CMyBitmap bmp;
    GetClientRect(r);
    DrawImage(bmp);

    //wyw
#if 0
    SetWindowRgn(bmp.CreateRgnFromFile(RGB(255,0,255)), TRUE );
#else
    HRGN hRgnTemp = bmp.CreateRgnFromFile(RGB(255,0,255));
    SetWindowRgn(hRgnTemp, TRUE );
#endif

    SetWindowRgn(bmp.CreateRgnFromFile(RGB(255,0,255)), TRUE );


    HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
    GetWindowRgn(hRgn);
    ::SelectClipRgn(dc.GetSafeHdc(), hRgn);
    bmp.Draw( &dc, r );
    dc.SelectClipRgn(NULL);
    DeleteObject(hRgn);

    //dc.BitBlt( 0, 0, r.Width(), r.Height(), &memDC, 0, 0, SRCCOPY);
    DrawText( &dc );

    //wyw
#if 1
    ::DeleteObject(hRgnTemp);
    bmp.DeleteObject();
#endif

    // TODO: Add your message handler code here
    // Do not call CButton::OnPaint() for painting messages
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
    GetWindowRgn(hRgn);

    if ( PtInRegion( hRgn, point.x, point.y )) {
#if 0
        if ( !m_bMouseIn) {
            m_bMouseIn = TRUE;
            Invalidate();
            UpdateWindow();
            SetCapture();
        }
#endif

        if (!m_bMouseIn)
            Invalidate();
        m_bMouseIn = TRUE;
//		UpdateWindow();
        SetCapture();

    } else {
        if ( m_bMouseIn) {
            m_bMouseIn = FALSE;
            Invalidate();
            if ( !m_bDown )
                ReleaseCapture();
        }

//		UpdateWindow();
    }
    DeleteObject(hRgn);
}

void CSkinButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    st = GetTickCount();
    m_bDown = TRUE;
    m_bMouseIn = TRUE;

    Invalidate();
    UpdateWindow();
    //SetFocus();

}

void CSkinButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    ed = GetTickCount();
    ReleaseCapture();

    if ( m_bMouseIn && m_bDown) {
        m_bMouseIn = FALSE;
        m_bDown = FALSE;
        Invalidate();
        UpdateWindow();
        GetParent()->SendMessage( WM_COMMAND, GetDlgCtrlID(), (LPARAM)m_hWnd );
    } else {
        m_bDown = FALSE;
        Invalidate();
        UpdateWindow();
    }

}

void CSkinButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    //CButton::OnLButtonDblClk(nFlags, point);
}



void CSkinButton::OnSetFocus(CWnd* pOldWnd)
{
    //CButton::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here

}

void CSkinButton::OnKillFocus(CWnd* pNewWnd)
{
    //CButton::OnKillFocus(pNewWnd);
    ReleaseCapture();
    // TODO: Add your message handler code here

}

BOOL CSkinButton::DrawText(CDC *pDC )
{
    CRect r;
    CString str;
    GetClientRect( r );
    GetWindowText( str );


    CFont *ofont;
    ofont = pDC->SelectObject( GetParent()->GetFont() );

    if ( m_bEnable )
        pDC->SetTextColor(m_textcolor);
    else
        pDC->SetTextColor(RGB(100,100,100));
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText( str, r, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

    pDC->SelectObject(ofont);
    return TRUE;
}




BOOL CSkinButton::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    return TRUE;
}

void CSkinButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    if ( nChar == VK_SPACE )
        OnLButtonDown( 0, 0 );
    //CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSkinButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    if ( nChar == VK_SPACE )
        OnLButtonUp( 0, 0 );

    //CButton::OnKeyUp(nChar, nRepCnt, nFlags);
}


BOOL CSkinButton::GetRegion()
{
    CMyBitmap bmp;
    CRgn rgn;
    rgn.CreateEllipticRgn( 00, 00, 50, 50 );
    SetWindowRgn((HRGN)rgn.Detach(), TRUE );

    //wyw
#if 1
    rgn.DeleteObject();
#endif

    return TRUE;
}

BOOL CSkinButton::EnableWindow( BOOL bEnable)
{
    CWnd::EnableWindow( bEnable );
    OnPaint();
    return TRUE;
}

void CSkinButton::OnEnable(BOOL bEnable)
{
//    CButton::OnEnable(bEnable);
    if ( bEnable )
        ModifyStyle( WS_DISABLED, 0, 0);
    else
        ModifyStyle( 0, WS_DISABLED, 0);
    // TODO: Add your message handler code here
    Invalidate();
    UpdateWindow();
}

void CSkinButton::PreSubclassWindow()
{
    CButton::PreSubclassWindow();

//    ModifyStyle(0, BS_OWNERDRAW);    // make the button owner drawn
}