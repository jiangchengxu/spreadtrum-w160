// TransparentStatic.cpp : implementation file
//

#include "stdafx.h"
#include "TransparentStatic.h"


// CTransparentStatic
IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)
CTransparentStatic::CTransparentStatic()
{
    //{{AFX_DATA_INIT(CTransparentStatic)
    StaticControlStyle = ID_STATIC;
    //}}AFX_DATA_INIT
}

CTransparentStatic::~CTransparentStatic()
{
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
    //{{AFX_MSG_MAP(CTransparentStatic)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
    //ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransparentStatic message handlers
#if 0
void CTransparentStatic::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Where to draw text
    CRect client_rect;
    GetClientRect(client_rect);

    // Get the caption
    CString szText;
    GetWindowText(szText);

    // Get the font
    CFont *pFont, *pOldFont;
    pFont = GetFont();
    pOldFont = dc.SelectObject(pFont);

    // Map "Static Styles" to "Text Styles"
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

    DWORD dwStyle = GetStyle(), dwText = 0;

    MAP_STYLE(    SS_RIGHT,            DT_RIGHT                    );
    MAP_STYLE(    SS_CENTER,            DT_CENTER                    );
    MAP_STYLE(    SS_CENTERIMAGE,        DT_VCENTER | DT_SINGLELINE    );
    MAP_STYLE(    SS_NOPREFIX,        DT_NOPREFIX                    );
    MAP_STYLE(    SS_WORDELLIPSIS,    DT_WORD_ELLIPSIS            );
    MAP_STYLE(    SS_ENDELLIPSIS,        DT_END_ELLIPSIS                );
    MAP_STYLE(    SS_PATHELLIPSIS,    DT_PATH_ELLIPSIS            );

    NMAP_STYLE(    SS_LEFTNOWORDWRAP |
                SS_CENTERIMAGE |
                SS_WORDELLIPSIS |
                SS_ENDELLIPSIS |
                SS_PATHELLIPSIS,    DT_WORDBREAK                );

    // Set transparent background
    dc.SetBkMode(TRANSPARENT);

    switch(StaticControlStyle)
    {
        case ID_STATIC:
            // Draw the text
            dc.DrawText(szText, client_rect, dwText);
            break;
        case ID_GROUP_BOX:
            // Draw 3D Ractangle
            dc.DrawEdge(client_rect,BDR_RAISEDINNER,BF_RECT);
            // Draw the text
            HFONT font=CreateFont(
            12,
            6,
            0,
            1,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,
            GB2312_CHARSET,
            OUT_CHARACTER_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FF_DECORATIVE,
            "宋体"
            );
    
            dc.SelectObject(font);
            //画背影
            POINT poin;
            poin.x=-2;
            poin.y=-2;
            dc.SetWindowOrg(poin);
            dc.SetTextColor(RGB(180,180,180));
            dc.DrawText(szText, client_rect, dwText);
            //写正文
            poin.x=0;
            poin.y=0;
            dc.SetWindowOrg(poin);
            dc.SetTextColor(RGB(0,90,0));
            dc.DrawText(szText, client_rect, dwText);            
            break;
    }

    // Select old font
    dc.SelectObject(pOldFont);
}
#endif
BOOL CTransparentStatic::OnEraseBkgnd(CDC* pDC) 
{
    return TRUE;
}
HBRUSH CTransparentStatic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    //HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);
    
    if (nCtlColor==CTLCOLOR_STATIC){
        UNUSED_ALWAYS(nCtlColor);
        pDC->SetBkMode(TRANSPARENT);
    }    
    return ((HBRUSH)GetStockObject(NULL_BRUSH));
    // TODO: Return a different brush if the default is not desired
    //return hbr;
}
