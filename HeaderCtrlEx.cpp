// HeaderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HeaderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

CHeaderCtrlEx::CHeaderCtrlEx()
{
//	m_bHeader = TRUE;
    m_bHeader = FALSE;
    m_nSortCol = -1;
    m_nOffset = 6;
    //DrawHeader();

}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}
int CHeaderCtrlEx::SetSortImage(int nCol, BOOL bAsc)
{
    int nPrevCol = m_nSortCol;

    m_nSortCol = nCol;
    m_bSortAsc = bAsc;

    // Change the item to ownder drawn
    HD_ITEM hditem;

    hditem.mask = HDI_BITMAP | HDI_FORMAT;
    GetItem( nCol, &hditem );
    if (hditem.hbm == NULL) {
        hditem.fmt |= HDF_OWNERDRAW;
        SetItem( nCol, &hditem );

        // Invalidate header control so that it gets redrawn
        Invalidate();
    }

    return nPrevCol;
}
#if 0
void CHeaderCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;

    dc.Attach( lpDrawItemStruct->hDC );

    // Get the column rect
    CRect rcLabel( lpDrawItemStruct->rcItem );

    // Save DC
    int nSavedDC = dc.SaveDC();

    // Set clipping region to limit drawing within column
    CRgn rgn;
    rgn.CreateRectRgnIndirect( &rcLabel );
    dc.SelectObject( &rgn );
    rgn.DeleteObject();

    // Draw the background
    dc.FillRect(rcLabel, &CBrush(::GetSysColor(COLOR_3DFACE)));
    dc.SetBkMode(TRANSPARENT);

    // Get the column text and format
    TCHAR buf[256];
    HD_ITEM hditem;

    hditem.mask = HDI_TEXT | HDI_FORMAT;
    hditem.pszText = buf;
    hditem.cchTextMax = 255;

    GetItem( lpDrawItemStruct->itemID, &hditem );

    // Determine format for drawing column label
    UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP
                   | DT_VCENTER | DT_END_ELLIPSIS ;

    if( hditem.fmt & HDF_CENTER)
        uFormat |= DT_CENTER;
    else if( hditem.fmt & HDF_RIGHT)
        uFormat |= DT_RIGHT;
    else
        uFormat |= DT_LEFT;

    // Adjust the rect if the mouse button is pressed on it
    if( lpDrawItemStruct->itemState == ODS_SELECTED ) {
        rcLabel.left++;
        rcLabel.top += 2;
        rcLabel.right++;
    }

    // Adjust the rect further if Sort arrow is to be displayed
    if( lpDrawItemStruct->itemID == (UINT)m_nSortCol ) {
        rcLabel.right -= 3 * m_nOffset;
    }

    rcLabel.left += m_nOffset;
    rcLabel.right -= m_nOffset;

    // Draw column label
    if( rcLabel.left < rcLabel.right ) {
        dc.DrawText(buf,-1,rcLabel, uFormat);
    }

    // Draw the Sort arrow
    if( lpDrawItemStruct->itemID == (UINT)m_nSortCol ) {
        CRect rcIcon( lpDrawItemStruct->rcItem );

        // Set up pens to use for drawing the triangle
        CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
        CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
        CPen *pOldPen = dc.SelectObject( &penLite );

        if( m_bSortAsc ) {
            // Draw triangle pointing upwards
            dc.MoveTo( rcIcon.right - 2*m_nOffset, m_nOffset-1);
            dc.LineTo( rcIcon.right - 3*m_nOffset/2, rcIcon.bottom - m_nOffset );
            dc.LineTo( rcIcon.right - 5*m_nOffset/2-2, rcIcon.bottom - m_nOffset );
            dc.MoveTo( rcIcon.right - 5*m_nOffset/2-1, rcIcon.bottom - m_nOffset-1 );

            dc.SelectObject( &penShad );
            dc.LineTo( rcIcon.right - 2*m_nOffset, m_nOffset-2);
        } else {
            // Draw triangle pointing downwords
            dc.MoveTo( rcIcon.right - 3*m_nOffset/2, m_nOffset-1);
            dc.LineTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset + 1 );
            dc.MoveTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset );

            dc.SelectObject( &penShad );
            dc.LineTo( rcIcon.right - 5*m_nOffset/2-1, m_nOffset -1 );
            dc.LineTo( rcIcon.right - 3*m_nOffset/2, m_nOffset -1);
        }

        // Restore the pen
        dc.SelectObject( pOldPen );
    }

    // Restore dc
    dc.RestoreDC( nSavedDC );

    // Detach the dc before returning
    dc.Detach();
}
void CHeaderCtrlEx::DrawHeader()
{
    CDC* pDC = GetDC();

    CRect rci;
    GetWindowRect(&rci);
    ScreenToClient(&rci);

    // Cover up thick 3D border.
    pDC->Draw3dRect(rci, ::GetSysColor(COLOR_3DFACE),
                    ::GetSysColor(COLOR_3DFACE));
    rci.DeflateRect(1,1);
    pDC->Draw3dRect(rci, ::GetSysColor(COLOR_3DFACE),
                    ::GetSysColor(COLOR_3DFACE));

    // Draw flat style border around entire header.
    rci.InflateRect(1,1);
    pDC->Draw3dRect(rci, ::GetSysColor(COLOR_3DHILIGHT),
                    ::GetSysColor(COLOR_3DSHADOW));

    // Create the pens for further cover-up.
    CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
    CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
    CPen penFace(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
    CPen *pOldPen = pDC->SelectObject( &penLite );

    pDC->SelectObject(&penFace);
    pDC->MoveTo(rci.right-1, 2);
    pDC->LineTo(rci.right-1, rci.bottom-2);

    // Set up the header item struct.
    HD_ITEM hdi;
    memset (&hdi, 0, sizeof(HD_ITEM));
    hdi.fmt  = HDF_STRING | HDF_LEFT | HDF_OWNERDRAW;
    hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT;
    int cx = 0;

    // For each header item found, do further cover up.
    for (int i = 0; i < GetItemCount(); ++i) {
        GetItem(i, &hdi);
        cx += hdi.cxy;

        pDC->SelectObject(&penShad);
        pDC->MoveTo(cx, 2);
        pDC->LineTo(cx, rci.bottom-2);

        pDC->SelectObject(&penLite);
        pDC->MoveTo(cx+1, 2);
        pDC->LineTo(cx+1, rci.bottom-2);

        pDC->SelectObject(&penFace);
        pDC->MoveTo(cx-1, 2);
        pDC->LineTo(cx-1, rci.bottom-2);

        pDC->SelectObject(&penFace);
        pDC->MoveTo(cx-2, 2);
        pDC->LineTo(cx-2, rci.bottom-2);
    }

    // Restore the pen and release device context.
    pDC->SelectObject( pOldPen );
    ReleaseDC(pDC);

}
#else

void CHeaderCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;

    dc.Attach( lpDrawItemStruct->hDC );

    // Get the column rect
    CRect rcLabel( lpDrawItemStruct->rcItem );

    // Save DC
    int nSavedDC = dc.SaveDC();

    // Set clipping region to limit drawing within column
    CRgn rgn;
    rgn.CreateRectRgnIndirect( &rcLabel );
    dc.SelectObject( &rgn );
    rgn.DeleteObject();

    // Draw the background
    dc.FillRect(rcLabel, &CBrush(HDCTRL_BK_COLOR));
    dc.SetBkMode(TRANSPARENT);

    // Get the column text and format
    TCHAR buf[256];
    HD_ITEM hditem;

    hditem.mask = HDI_TEXT | HDI_FORMAT;
    hditem.pszText = buf;
    hditem.cchTextMax = 255;

    GetItem( lpDrawItemStruct->itemID, &hditem );

    // Determine format for drawing column label
    UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP
                   | DT_VCENTER | DT_END_ELLIPSIS ;

    if( hditem.fmt & HDF_CENTER)
        uFormat |= DT_CENTER;
    else if( hditem.fmt & HDF_RIGHT)
        uFormat |= DT_RIGHT;
    else
        uFormat |= DT_LEFT;

    // Adjust the rect if the mouse button is pressed on it
    if( lpDrawItemStruct->itemState == ODS_SELECTED ) {
        rcLabel.left++;
        rcLabel.top += 2;
        rcLabel.right++;
    }

    // Adjust the rect further if Sort arrow is to be displayed
    if( lpDrawItemStruct->itemID == (UINT)m_nSortCol ) {
        rcLabel.left += 2 * m_nOffset;
    }
    rcLabel.left +=  m_nOffset;
    rcLabel.right -= m_nOffset;

    // Draw column label
    if( rcLabel.left < rcLabel.right ) {
        dc.DrawText(buf,-1,rcLabel, uFormat);
    }

    //»­ÅÅÐò¼ýÍ·
    if( lpDrawItemStruct->itemID == (UINT)m_nSortCol ) {
        CRect rcIcon( lpDrawItemStruct->rcItem );
        rcIcon.left += 3*m_nOffset+2;
        // Set up pens to use for drawing the triangle
        CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
        CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
        CPen *pOldPen = dc.SelectObject( &penLite );

        if( m_bSortAsc ) {
            // Draw triangle pointing upwards
            dc.MoveTo( rcIcon.left - 2*m_nOffset, m_nOffset-1);
            dc.LineTo( rcIcon.left - 3*m_nOffset/2, rcIcon.bottom - m_nOffset );
            dc.LineTo( rcIcon.left - 5*m_nOffset/2-2, rcIcon.bottom - m_nOffset );
            dc.MoveTo( rcIcon.left - 5*m_nOffset/2-1, rcIcon.bottom - m_nOffset-1 );

            dc.SelectObject( &penShad );
            dc.LineTo( rcIcon.left - 2*m_nOffset, m_nOffset-2);
        } else {
            // Draw triangle pointing downwords
            dc.MoveTo( rcIcon.left - 3*m_nOffset/2, m_nOffset-1);
            dc.LineTo( rcIcon.left - 2*m_nOffset-1, rcIcon.bottom - m_nOffset + 1 );
            dc.MoveTo( rcIcon.left - 2*m_nOffset-1, rcIcon.bottom - m_nOffset );

            dc.SelectObject( &penShad );
            dc.LineTo( rcIcon.left - 5*m_nOffset/2-1, m_nOffset -1 );
            dc.LineTo( rcIcon.left - 3*m_nOffset/2, m_nOffset -1);
        }

        // Restore the pen
        dc.SelectObject( pOldPen );
    }

    // Restore dc
    dc.RestoreDC( nSavedDC );

    // Detach the dc before returning
    dc.Detach();
}
void CHeaderCtrlEx::DrawHeader()
{
    CDC* pDC = GetDC();

    CRect rci;
    GetWindowRect(&rci);
    ScreenToClient(&rci);

    // Cover up thick 3D border.
    pDC->Draw3dRect(rci, HDCTRL_BK_COLOR,HDCTRL_BK_COLOR);
    rci.DeflateRect(1,1);
    pDC->Draw3dRect(rci, HDCTRL_BK_COLOR,HDCTRL_BK_COLOR);

    // Draw flat style border around entire header.
    rci.InflateRect(1,1);
    pDC->Draw3dRect(rci, HDCTRL_BK_COLOR,HDCTRL_BK_COLOR);

    // Create the pens for further cover-up.
    CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
    CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
    CPen penFace(PS_SOLID, 1, HDCTRL_BK_COLOR);
    CPen *pOldPen = pDC->SelectObject( &penLite );

    pDC->SelectObject(&penFace);
    pDC->MoveTo(rci.right-1, 2);
    pDC->LineTo(rci.right-1, rci.bottom-2);

    // Set up the header item struct.
    HD_ITEM hdi;
    memset (&hdi, 0, sizeof(HD_ITEM));
    hdi.fmt  = HDF_STRING | HDF_LEFT | HDF_OWNERDRAW;
    hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT;
    int cx = 0;

    //shang
    TCHAR buf[256];
    hdi.pszText = buf;
    hdi.cchTextMax = 255;
    ///////


    // For each header item found, do further cover up.
    for (int i = 0; i < GetItemCount(); ++i) {
        GetItem(i, &hdi);
        cx += hdi.cxy;

        ///////////////////
        hdi.fmt |= HDF_OWNERDRAW;
        hdi.fmt |= HDF_STRING;
        hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT;
        //strcpy(buf,"hello");

        SetItem(i,&hdi);
        /////////////////

        pDC->SelectObject(&penShad);
        pDC->MoveTo(cx, 2);
        pDC->LineTo(cx, rci.bottom-2);

        pDC->SelectObject(&penLite);
        pDC->MoveTo(cx+1, 2);
        pDC->LineTo(cx+1, rci.bottom-2);

        pDC->SelectObject(&penFace);
        pDC->MoveTo(cx-1, 2);
        pDC->LineTo(cx-1, rci.bottom-2);

        pDC->SelectObject(&penFace);
        pDC->MoveTo(cx-2, 2);
        pDC->LineTo(cx-2, rci.bottom-2);


    }

    // Restore the pen and release device context.
    pDC->SelectObject( pOldPen );
    ReleaseDC(pDC);
}
#endif
BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
    //{{AFX_MSG_MAP(CHeaderCtrlEx)
    ON_WM_PAINT()
    ON_WM_WINDOWPOSCHANGING()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx message handlers

void CHeaderCtrlEx::OnPaint()
{
    Default();
    if (m_bHeader)
        DrawHeader();
}

void CHeaderCtrlEx::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    CHeaderCtrl::OnWindowPosChanging(lpwndpos);
    Invalidate();
}

BOOL ItemCompareFunc::IsDecNumber(const CString& str, double& f)
{
    if (str.IsEmpty())
        return FALSE;

    LPTSTR p;
    f = _tcstod(str, &p);
    return (*p == _T('\0') || (*p == _T('%') && p[1] == _T('\0')));
}

int ItemCompareFunc::DecNumberCompare(double f1, double f2)
{
    if(f1 < f2)
        return -1;

    if(f1 > f2)
        return 1;

    return 0;
}
BOOL ItemCompareFunc::IsDate(const CString& str, COleDateTime& date)
{
    return date.ParseDateTime(str);
}

int ItemCompareFunc::DateCompare(const COleDateTime& date1, const COleDateTime& date2)
{
    if (date1 < date2)
        return -1;

    if (date1 > date2)
        return 1;

    return 0;
}


