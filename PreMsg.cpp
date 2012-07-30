// PreMsg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "PreMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PreMsg dialog


PreMsg::PreMsg(CWnd* pParent /*=NULL*/)
    //: CDialog(PreMsg::IDD, pParent)
{
    //{{AFX_DATA_INIT(PreMsg)
    m_rcText = CRect(0,0,0,0);
    m_crTextColor = RGB(0,0,0);
    //}}AFX_DATA_INIT
}


void PreMsg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(PreMsg)
    //}}AFX_DATA_MAP
}
#if 1
void PreMsg::SetText(LPCTSTR szText)
{
    m_strText=szText;	
    RedrawWindow();
}
BOOL PreMsg::SetBitmap(UINT nBitmapID,short red,short green,short blue)
{
    BITMAP bm;
    
    m_bitmap.DeleteObject();
    if (!m_bitmap.LoadBitmap(nBitmapID))
        return FALSE;
    
    GetObject(m_bitmap.GetSafeHandle(), sizeof(bm), &bm);
    m_nBitmapWidth=bm.bmWidth;
    m_nBitmapHeight=bm.bmHeight;
	//m_rcText.SetRect(0+100,0+20,bm.bmWidth-50,bm.bmHeight-10);
#ifdef FEATURE_VERSION_BLUECUBE
    m_rcText.SetRect(30,115,bm.bmWidth-30,bm.bmHeight-75);
#else
    m_rcText.SetRect(30,95,bm.bmWidth-30,bm.bmHeight-95);
#endif

    //显示坐标
    m_nxPos=(GetSystemMetrics(SM_CXFULLSCREEN)-bm.bmWidth)/2;
    m_nyPos=(GetSystemMetrics(SM_CYFULLSCREEN)-bm.bmHeight)/2;
    MoveWindow(m_nxPos, m_nyPos, m_nBitmapWidth, m_nBitmapHeight);
    /*
    CRect dlgRect;
    GetWindowRect(dlgRect);
    m_nxPos = (dlgRect.left-bm.bmWidth)/2;
    m_nyPos = (dlgRect.top-bm.bmHeight)/2;
    */
    if (red!=-1 && green!=-1 && blue!=-1)
    {
        m_hRegion=CreateRgnFromBitmap((HBITMAP)m_bitmap.GetSafeHandle(),RGB(red,green,blue));
        SetWindowRgn(m_hRegion, TRUE);
    }
    
    return TRUE;
}

BOOL PreMsg::SetBitmap(LPCTSTR szFileName,short red,short green,short blue)
{
    BITMAP bm;
    HBITMAP hBmp;
    
    hBmp=(HBITMAP)::LoadImage(AfxGetInstanceHandle(),szFileName,IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
    if (!hBmp)
	{
		return SetBitmap(IDB_SPLASH,255,0,255);
        //return FALSE;
	}
    
    m_bitmap.DeleteObject();
    m_bitmap.Attach(hBmp);
    
    GetObject(m_bitmap.GetSafeHandle(), sizeof(bm), &bm);
    m_nBitmapWidth=bm.bmWidth;
    m_nBitmapHeight=bm.bmHeight;
    //m_rcText.SetRect(0,0,bm.bmWidth,bm.bmHeight);
#ifdef FEATURE_VERSION_BLUECUBE
    m_rcText.SetRect(30,115,bm.bmWidth-30,bm.bmHeight-75);
#else
    m_rcText.SetRect(180,130,bm.bmWidth-30,bm.bmHeight-95);//modify by liub 启动画面字符位置
#endif
    
    m_nxPos=(GetSystemMetrics(SM_CXFULLSCREEN)-bm.bmWidth)/2;
    m_nyPos=(GetSystemMetrics(SM_CYFULLSCREEN)-bm.bmHeight)/2;

    if (red!=-1 && green!=-1 && blue!=-1)
    {
        m_hRegion=CreateRgnFromBitmap((HBITMAP)m_bitmap.GetSafeHandle(),RGB(red,green,blue));
        SetWindowRgn(m_hRegion,TRUE);
    }
    
    return TRUE;
}
HRGN PreMsg::CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color)
{
    if (!hBmp) return NULL;
    
    BITMAP bm;
    GetObject( hBmp, sizeof(BITMAP), &bm );    //get bitmap attributes
    
    CDC dcBmp;
    dcBmp.CreateCompatibleDC(GetDC());    //Creates a memory device context for the bitmap
    dcBmp.SelectObject(hBmp);            //selects the bitmap in the device context
    
    const DWORD RDHDR = sizeof(RGNDATAHEADER);
    const DWORD MAXBUF = 40;        // size of one block in RECTs
    // (i.e. MAXBUF*sizeof(RECT) in bytes)
    LPRECT    pRects;                                
    DWORD    cBlocks = 0;            // number of allocated blocks
    
    INT        i, j;                    // current position in mask image
    INT        first = 0;                // left position of current scan line
    // where mask was found
    bool    wasfirst = false;        // set when if mask was found in current scan line
    bool    ismask;                    // set when current color is mask color
    
    // allocate memory for region data
    RGNDATAHEADER* pRgnData = (RGNDATAHEADER*)new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
    memset( pRgnData, 0, RDHDR + cBlocks * MAXBUF * sizeof(RECT) );
    // fill it by default
    pRgnData->dwSize    = RDHDR;
    pRgnData->iType        = RDH_RECTANGLES;
    pRgnData->nCount    = 0;
    for ( i = 0; i < bm.bmHeight; i++ )
        for ( j = 0; j < bm.bmWidth; j++ ){
            // get color
            ismask=(dcBmp.GetPixel(j,bm.bmHeight-i-1)!=color);
            // place part of scan line as RECT region if transparent color found after mask color or
            // mask color found at the end of mask image
            if (wasfirst && ((ismask && (j==(bm.bmWidth-1)))||(ismask ^ (j<bm.bmWidth)))){
                // get offset to RECT array if RGNDATA buffer
                pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
                // save current RECT
                pRects[ pRgnData->nCount++ ] = CRect( first, bm.bmHeight - i - 1, j+(j==(bm.bmWidth-1)), bm.bmHeight - i );
                // if buffer full reallocate it
                if ( pRgnData->nCount >= cBlocks * MAXBUF ){
                    LPBYTE pRgnDataNew = new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
                    memcpy( pRgnDataNew, pRgnData, RDHDR + (cBlocks - 1) * MAXBUF * sizeof(RECT) );
                    delete pRgnData;
                    pRgnData = (RGNDATAHEADER*)pRgnDataNew;
                }
                wasfirst = false;
            } else if ( !wasfirst && ismask ){        // set wasfirst when mask is found
                first = j;
                wasfirst = true;
            }
        }
        dcBmp.DeleteDC();    //release the bitmap
        // create region
        /*  Under WinNT the ExtCreateRegion returns NULL (by Fable@aramszu.net) */
        //    HRGN hRgn = ExtCreateRegion( NULL, RDHDR + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
        /* ExtCreateRegion replacement { */
        HRGN hRgn=CreateRectRgn(0, 0, 0, 0);
        ASSERT( hRgn!=NULL );
        pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
        for(i=0;i<(int)pRgnData->nCount;i++)
        {
            HRGN hr=CreateRectRgn(pRects[i].left, pRects[i].top, pRects[i].right, pRects[i].bottom);
            VERIFY(CombineRgn(hRgn, hRgn, hr, RGN_OR)!=ERROR);
            if (hr) DeleteObject(hr);
        }
        ASSERT( hRgn!=NULL );
        /* } ExtCreateRegion replacement */
        
        delete pRgnData;
        return hRgn;
}
void PreMsg::DrawWindow(CDC *pDC)
{
    CDC memDC;
    CBitmap *pOldBitmap;
    
    // Blit Background
    memDC.CreateCompatibleDC(pDC);
    pOldBitmap=memDC.SelectObject(&m_bitmap);
    pDC->BitBlt(0,0,m_nBitmapWidth,m_nBitmapHeight,&memDC,0,0,SRCCOPY);
    
    memDC.SelectObject(pOldBitmap);
    
    // Draw Text
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(m_crTextColor);

#ifdef FEATURE_LANGUAGE_RTOL
	CFont* cfont = this->GetFont();
	LOGFONT logfont;	
	cfont->GetLogFont(&logfont);
	logfont.lfCharSet = ARABIC_CHARSET;	
	pDC->SelectObject(cfont);
#endif
    pDC->DrawText(m_strText,-1,m_rcText,DT_SINGLELINE /*| DT_CENTER*/ | DT_VCENTER);    
}
#endif
BEGIN_MESSAGE_MAP(PreMsg, CDialog)
    //{{AFX_MSG_MAP(PreMsg)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PreMsg message handlers

BOOL PreMsg::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}
void PreMsg::OnPaint() 
{
    CPaintDC dc(this);
    DrawWindow(&dc);
}

BOOL PreMsg::OnInitDialog() 
{
    //CDialog::OnInitDialog();
	CString strTitle;
    strTitle.LoadString(IDS_APPNAME);
	AddPrefixName(strTitle);
    SetWindowText(strTitle);
    
    //删除边框和标题
    SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER))); 
    
    //  Set the window region
#if 1
    //SetBitmap(IDB_SPLASH,255,0,255);
	SetBitmap(g_SetData.Main_szSplashBmp, 255 ,0 ,255);
    //SetWindowRgn(m_hRegion,TRUE);
#else
    FormatRgn("res\\splash.bmp");
#endif	

	//wyw add 2009.12.19
	//将此扩展界面移动到主窗口中心和适应图片的大小
	CRect mainRc, preRc;
	CWnd *pWnd = GetParent();
	if (pWnd)
	{
		pWnd->GetWindowRect(mainRc);
		int x=0, y=0;
		x = mainRc.left+(mainRc.Width()-m_nBitmapWidth)/2;
		y = mainRc.top+(mainRc.Height()-m_nBitmapHeight)/2;
		
		MoveWindow(x, y, m_nBitmapWidth, m_nBitmapHeight);
	}
	//wyw end 2009.12.19

    return TRUE;
}

void PreMsg::FormatRgn(LPCTSTR szFileName) 
{
#define TRANSPARENTCOLOR (RGB(255,0,255))
    BITMAP  m_Bitmap;        // Struct to hold info about the bitmap
    HBITMAP m_hBitmap;        // Handle of the bitmap
    // Load the image
    m_hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (m_hBitmap == NULL){
        MessageBox(_T("Error loading bitmap"));
        return;
    }
    //Get information about the bitmap..
    GetObject(m_hBitmap, sizeof(m_Bitmap), &m_Bitmap);    // Get info about the bitmap 
    // Put the bitmap into a memory device context
    CPaintDC dc(this);
    //get a memory dc object
    CDC dcMem;
    //create a compatible dc
    dcMem.CreateCompatibleDC(&dc);    // Select the bitmap into the in-memory DC
    //Select the bitmap into the dc
    CBitmap* pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(m_hBitmap));
    //Create a couple of region objects.
    CRgn crRgn, crRgnTmp;
    //create an empty region
    crRgn.CreateRectRgn(0, 0, 0, 0);
    //Create a region from a bitmap with transparency colour of Purple
    COLORREF crTransparent = TRANSPARENTCOLOR;    
    int iX = 0;
    int iY = 0;
    for (iY = 0; iY < m_Bitmap.bmHeight; iY++)
    {
        do
        {
            //skip over transparent pixels at start of lines.
            while (iX <= m_Bitmap.bmWidth && dcMem.GetPixel(iX, iY) == crTransparent)
                iX++;
            //remember this pixel
            int iLeftX = iX;
            //now find first non transparent pixel
            while (iX <= m_Bitmap.bmWidth && dcMem.GetPixel(iX, iY) != crTransparent)
                ++iX;
            //create a temp region on this info
            crRgnTmp.CreateRectRgn(iLeftX, iY, iX, iY+1);
            //combine into main region.
            crRgn.CombineRgn(&crRgn, &crRgnTmp, RGN_OR);
            //delete the temp region for next pass (otherwise you'll get an ASSERT)
            crRgnTmp.DeleteObject();
        }while(iX < m_Bitmap.bmWidth);
        iX = 0;
    }
    //Centre it on current desktop
    SetWindowRgn(crRgn, TRUE);
    iX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (m_Bitmap.bmWidth / 2);
    iY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (m_Bitmap.bmHeight / 2);
    SetWindowPos(&wndTopMost, iX, iY, m_Bitmap.bmWidth, m_Bitmap.bmHeight, NULL); 
    
    // Free resources.
    dcMem.SelectObject(pOldBitmap);    // Put the original bitmap back (prevents memory leaks)
    dcMem.DeleteDC();
    crRgn.DeleteObject();
}