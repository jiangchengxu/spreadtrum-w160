// SliderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SliderCtrlEx.h"
#include "Ds.h"
//#include "CallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx

CSliderCtrlEx::CSliderCtrlEx()
{
    m_bDragging = FALSE;
    m_nVolValue=4;
    m_bVolSet = FALSE;
}

CSliderCtrlEx::~CSliderCtrlEx()
{
    delete m_bmpBack;
    delete m_bmpTic;
}


BEGIN_MESSAGE_MAP(CSliderCtrlEx, CSliderCtrl)
    //{{AFX_MSG_MAP(CSliderCtrlEx)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
    //}}AFX_MSG_MAP
    //ON_WM_CTLCOLOR_REFLECT()
    ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx message handlers

void CSliderCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
    if( m_bDragging )
        Invalidate(FALSE);
    CSliderCtrl::OnMouseMove(nFlags, point);
}
void CSliderCtrlEx::SetVolume(int nVol)
{
    //AT Command
    CString nVoltemp;//add by liub
    nVoltemp.Format(_T("%d"),nVol);//add by liub
    const TCHAR ATCLVL[]=_T("AT+CLVL=");
    const TCHAR DQuote[]=_T("\x0d\x00");
    TCHAR szAtBuf[512] = {0};
    char szAtAscBuf[512] = {0};
    CString strAtComm=ATCLVL;
    strAtComm+=nVoltemp;
//	strAtComm+=nVoltemp + _T("0");
    strAtComm+=DQuote;
    wcscpy(szAtBuf, strAtComm);
    WCharToChar(szAtBuf, szAtAscBuf);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf))) {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespVol, this);
    }
}

void CSliderCtrlEx::AtRespVol(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CString RetStr;
    return ;
}

void CSliderCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bDragging = FALSE;
    Invalidate(FALSE);
    CSliderCtrl::OnLButtonUp(nFlags, point);
    /////////////AT
    int nIndex;
    nIndex = GetPos();
    if(m_bVolSet) {
        SetVolume(nIndex);
    }
    /////////////
}

void CSliderCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDragging =TRUE;
    Invalidate(FALSE);
    CSliderCtrl::OnLButtonDown(nFlags, point);
}
void CSliderCtrlEx::SetRange(int nMin, int nMax, BOOL bRedraw)
{
    CSliderCtrl::SetRange(nMin,nMax,bRedraw);
    SetPos(nMin);
}
/*
HBRUSH CSliderCtrlEx::CtlColor(CDC* pDC, UINT nCtlColor)
{
    CBrush crBrush(RGB(238,241,243));
    return ((HBRUSH)crBrush);
}
*/
void CSliderCtrlEx::SetSkin(UINT uiBackGround, UINT uiTickImage/*,short nCursor*/)
{
    //m_hCursor = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(nCursor));
    m_bmpBack = new CMyBitmap();
    m_bmpBack->LoadBitmap(uiBackGround);
    m_bmpTic = new CMyBitmap();
    m_bmpTic->LoadBitmap(uiTickImage);

}

void CSliderCtrlEx::OnPaint()
{
    CPaintDC dc(this);
    int iMax,iMin,iTickWidth=10,iMarginWidth=16;

    GetRange(iMin,iMax);


    RECT rcBack,rcTick;
    GetClientRect(&rcBack);
    rcTick = rcBack;
    rcTick.left = ((rcBack.right-iMarginWidth)*(GetPos()))/(iMax - iMin)+iMarginWidth/2-iTickWidth/2;
    rcTick.right = rcTick.left + iTickWidth;

    m_bmpBack->Draw(&dc,&rcBack);
    m_bmpTic->DrawTransparent(&dc, rcTick.left,4,RGB(255,0,0));
}



BOOL CSliderCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    return TRUE;
    //AfxMessageBox("xxxxxxxxx");
    //return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);
}
