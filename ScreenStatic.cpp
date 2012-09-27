// ScreenStatic.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "ScreenStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CScreenStatic

CScreenStatic::CScreenStatic()
{
    m_bFillBk = TRUE;
}

CScreenStatic::~CScreenStatic()
{
}


BEGIN_MESSAGE_MAP(CScreenStatic, CStatic)
    //{{AFX_MSG_MAP(CScreenStatic)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenStatic message handlers

BOOL CScreenStatic::OnEraseBkgnd(CDC* pDC)
{
    CRect client;
//	//pDC->GetClipBox(&client);
    //////////////////////////////////////////////////////////////////////////delete by liub for static text color
// 	#ifdef FEATURE_UICOLOR_GREY
//     pDC->SetTextColor(RGB(100,100,100));
// 	#else
// 	pDC->SetTextColor(RGB(255,255,255));
// 	#endif
    //////////////////////////////////////////////////////////////////////////
    GetClientRect(&client);
    //pDC->FillSolidRect(client,RGB(18,106,138));

    CDC memDC;
    CBitmap *pOldBitmap;

    // Blit Background
    memDC.CreateCompatibleDC(pDC);
    pOldBitmap=memDC.SelectObject(&m_bmp);
    pDC->BitBlt(0,0,m_nWidth,m_nHeight,&memDC,0,0,SRCCOPY);
    memDC.SelectObject(pOldBitmap);

    return FALSE;
}

BOOL CScreenStatic::SetBitmap(UINT nIDResource)
{
    this->m_nBmpID = nIDResource;
    m_bmp.DeleteObject();
    if (!m_bmp.LoadBitmap(m_nBmpID))
        return FALSE;
    GetObject(m_bmp.GetSafeHandle(), sizeof(m_bmInfo), &m_bmInfo);

    m_nWidth = m_bmInfo.bmWidth;
    m_nHeight = m_bmInfo.bmHeight;
    return TRUE;
}

void CScreenStatic::OnPaint()
{
    CDC memDC;
    CPaintDC dc(this); // device context for painting

    CRect rect;
    GetClientRect(&rect);
    /*
        CBitmap *pOldBitmap;
        // Blit Background
        memDC.CreateCompatibleDC(&dc);
        pOldBitmap=memDC.SelectObject(&m_bmp);
        dc.BitBlt(0,0,m_nWidth,m_nHeight,&memDC,0,0,SRCCOPY);
        memDC.SelectObject(pOldBitmap);
    */
    // Draw Text
    dc.SetBkMode(TRANSPARENT);

    //dc.SetTextColor(RGB(255,255,255));//modify by liub for 修改模拟屏幕上字体颜色

    CRect textrect;

    int nLine,i;
    if (m_strText.GetLength()==0) {
        nLine=0;
    } else {
        //计算要刷新的行数
        nLine=1;
        for (i=0; i<m_strText.GetLength(); i++) {
            if (m_strText[i]=='\n') {
                nLine++;
            }
        }
        textrect.left=rect.left;
        textrect.top=rect.top;
        textrect.bottom=rect.top+16*nLine;
        textrect.right=rect.right;

        dc.DrawText(m_strText,-1,textrect, DT_VCENTER);
    }
}
void CScreenStatic::SetTextA(LPCSTR szText)
{
    m_strText=szText;
    RedrawWindow();
}

void CScreenStatic::SetText(LPCTSTR szText)
{
    m_strText=szText;
    RedrawWindow();
}
