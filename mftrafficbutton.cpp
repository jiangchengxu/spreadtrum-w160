/*******************************************

	MFTrafficButton


********************************************/

// MFTrafficButton.cpp : implementation file
//


#include "stdafx.h"
#include "MFTrafficButton.h"
#include "MemDC.h"
#include <math.h>

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif
#include <commctrl.h>
#include <intshcut.h>
#include <wininet.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// MFTrafficButton

MFTrafficButton::MFTrafficButton()
{
    brushInitalized = FALSE;
    interfaceCallBack = NULL;
    useAdaptiveScale = FALSE;
    gridxstartpos = 0;
    gridystartpos = 0;
    gridxresolution		=	GRIDXRESOLUTION;
    gridyresolution		=	GRIDYRESOLUTION;
    gridscrollxspeed	=	GRIDSCROLLXSPEED;
    gridscrollyspeed	=	GRIDSCROLLYSPEED;
    plotgranularity		=	PLOTGRANULATRITY;
    netupdatespeed		=	NETUPDATESPEED;
    gridupdatespeed		=	GRIDUPDATESPEED;
    ToggleStatusText	=	"local maximum";
    drawFlag=FALSE;

    //wyw
    TrafficStats = NULL;
}

MFTrafficButton::~MFTrafficButton()
{
    //wyw
    if (TrafficStats) {
        delete []TrafficStats;
        TrafficStats = NULL;
    }
}


BEGIN_MESSAGE_MAP(MFTrafficButton, CButton)
    //{{AFX_MSG_MAP(MFTrafficButton)
    ON_WM_TIMER()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MFTrafficButton message handlers



void MFTrafficButton::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class

//	CButton::PreSubclassWindow();

    // make sure we are an owner draw button
    ModifyStyle( 0 , BS_OWNERDRAW );

    this->GetWindowRect(&TrafficDrawRectangle);
    this->GetWindowRect(&TrafficDrawUpdateRectangle);
    ScreenToClient(&TrafficDrawUpdateRectangle);
    ScreenToClient(&TrafficDrawRectangle);

    TGSize.cx = TrafficDrawRectangle.right - TrafficDrawRectangle.left;
    TGSize.cy = TrafficDrawRectangle.bottom - TrafficDrawRectangle.top;

    initalized = FALSE;
    MaxTrafficAmount = 0.0;
    SelectedInterface = 0;

//	smallFont.CreateFont(-6,0,0,0,FW_THIN,FALSE, FALSE, FALSE, DEFAULT_CHARSET,
    smallFont.CreateFont(-10,0,0,0,FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, _T("Arial"));

    red		= RGB(255,125,125);
    green	= RGB(125,255,125);
    cyan	= RGB(0,255,255);
    darkblue= RGB(0,0,75);
    darkgray= RGB(50,50,50);
    white	= RGB(255,255,255);
    black	= RGB(0,0,0);
    lightgreen	= RGB(156,255,156);
    darkgreen	= RGB(32,64,32);

    greenbrush.CreateSolidBrush(green);
    redbrush.CreateSolidBrush(red);

    GridPen.CreatePen(PS_SOLID ,1 , darkgreen);

    GetWindowRect(&TrafficDrawRectangle);
    ScreenToClient(&TrafficDrawRectangle);
    //	TrafficDrawRectangle.left += 2;
    //	TrafficDrawRectangle.right -= 3;
    //	TrafficDrawRectangle.bottom -= 4;

    TGSize.cx = TrafficDrawRectangle.right - TrafficDrawRectangle.left;
    TGSize.cy = TrafficDrawRectangle.bottom - TrafficDrawRectangle.top;


    TrafficEntries = TGSize.cx / plotgranularity;

    //wyw
    if (TrafficStats) {
        delete []TrafficStats;
        TrafficStats = NULL;
    }

    TrafficStats = new TRAFFICENTRY[TrafficEntries+1];

    for(DWORD x=0; x<TrafficEntries; x++) {
        TrafficStats[x].connected = TRUE;//FALSE;
        TrafficStats[x].value	= 0.0;//(float)x * (float)TrafficEntries*0.05;//;10.0 + 10.0*(sin(5.0*(float)x*3.14/180.0));
    }

    MaxTrafficAmount = 0.0;
    SelectedInterface = 0;

    CRgn rectRgn, ellRgn, finalRgn;
    rectRgn.CreateRectRgn(0,0,TGSize.cx, TGSize.cy);
    ShapeWNDRegion.CreateRectRgn(0,0,TGSize.cx, TGSize.cy);;
    ShapeDCRegion.CreateRectRgn(0,0,TGSize.cx, TGSize.cy);;

    int x1,x2,y1,y2,xe,ye,xs,ys;
    int xof, yof;
    int r;
    xs = TGSize.cx;
    ys = TGSize.cy;
    x1 = 0;
    y1 = 0;
    x2 = xs;
    y2 = ys;
    xe = 0;//Radius of edge
    ye = 0;//Radius of edge
    xof = (int)( (float)xs*0.0);
    yof = (int)( (float)ys*0.0);
    r = ellRgn.CreateRoundRectRgn(x1,y1,x2,y2,xe,ye);
    r = ellRgn.OffsetRgn(-xof, -yof);
    r = ShapeWNDRegion.CombineRgn(&rectRgn, &ellRgn,RGN_AND );
    r = ShapeDCRegion.CombineRgn(&rectRgn, &ellRgn,RGN_AND );

    this->SetWindowRgn((HRGN)ShapeWNDRegion, TRUE);
    initalized = TRUE;

    SetTimer(GRIDTIMER,	gridupdatespeed,	0);
// 	SetTimer(NETTIMER,	netupdatespeed,		0);

}


void MFTrafficButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your message handler code here and/or call default
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
    ASSERT( lpDrawItemStruct != NULL );


    CDC * pDC   = CDC::FromHandle( lpDrawItemStruct -> hDC );
    int erg = pDC->SelectClipRgn(&ShapeDCRegion,RGN_OR);



    CRect rect = lpDrawItemStruct -> rcItem;
    UINT state = lpDrawItemStruct -> itemState;
    UINT nStyle = GetStyle( );

    int nSavedDC = pDC -> SaveDC( );



    // Create the brush for the color bar
    if(brushInitalized == FALSE) {
        CBitmap bmp;
        CMemDC *memDC = new CMemDC(pDC);

        RECT clipRect;
        if(drawFlag)//如果窗口移动结束，则取当前绘图区域
            memDC->GetClipBox(&clipRect);

        if(clipRect.right - clipRect.left > 1) {
            bmp.CreateCompatibleBitmap(memDC,plotgranularity, TGSize.cy);
            CBitmap *pOld = memDC->SelectObject(&bmp);

            CSize bmps = bmp.GetBitmapDimension();

            // Need for scaling the color to the size of button
            double factor = 255.0 / (float)TGSize.cy;

            BYTE r,g,b;
            for(int x = 0; x<TGSize.cy; x++) {
                g = (BYTE)(255-factor*x);
                r = (BYTE)(factor*x);
                b = (BYTE)64;
                memDC->SetPixelV(0,x,RGB(r,g,b));
                memDC->SetPixelV(1,x,RGB(r,g,b));
            }
            //	memDC->SelectObject(pOld);
            pDC->SelectObject(pOld);

            colorbrush.CreatePatternBrush(&bmp);

            //wyw
            bmp.DeleteObject();

            brushInitalized = TRUE;
        }
        //wyw
        delete memDC;
    }
    if(initalized == TRUE) {
        COLORREF backcolor = GetSysColor(COLOR_BTNFACE);

        CBrush brush;
        CMemDC *memDC = new CMemDC(pDC);

        RECT clipRect;
        memDC->GetClipBox(&clipRect);
        memDC->FillSolidRect(&clipRect,backcolor);

        CFont *oldFont;
        int xp, yp, xx, yy;
        orgBrushOrigin = memDC->GetBrushOrg();

        oldFont = memDC->SelectObject(&smallFont);

        double scale = (double)TGSize.cy / (double)MaxTrafficAmount;

        yp = TrafficDrawRectangle.bottom;
        xp = TrafficDrawRectangle.left;

        RECT fillrect;

        CString tmp;

        // Fill the background
        back = memDC->GetBkColor();
        brush.CreateSolidBrush(darkblue);//back);
        memDC->FillRect(&TrafficDrawRectangle, &brush);

        // draw the grid
        int xgridlines, ygridlines;

        xgridlines = TGSize.cx / gridxresolution;
        ygridlines = TGSize.cy / gridyresolution;
        CPen* oldPen = memDC->SelectObject(&GridPen);
        // Create the vertical lines
        for (int x=0; x<= xgridlines; x++) {
            memDC->MoveTo(x*gridxresolution + gridxstartpos, 0			);
            memDC->LineTo(x*gridxresolution + gridxstartpos, TGSize.cy	);
        }
        // And the horizontal lines
        for (int y=0; y<= ygridlines; y++) {
            memDC->MoveTo(0			, gridystartpos + TGSize.cy - y*gridyresolution - 2);
            memDC->LineTo(TGSize.cx	, gridystartpos + TGSize.cy - y*gridyresolution - 2);
        }

        gridxstartpos += gridscrollxspeed;
        gridystartpos += gridscrollyspeed;
        if(gridxstartpos < 0				) gridxstartpos = gridxresolution;
        if(gridxstartpos > gridxresolution	) gridxstartpos = 0;
        if(gridystartpos < 0				) gridystartpos = gridyresolution;
        if(gridystartpos > gridyresolution	) gridystartpos = 0;


        memDC->SelectObject(oldPen );

        for(DWORD cnt=0; cnt<TrafficEntries; cnt++) {
            xx = xp + cnt*plotgranularity;
            double traffic = TrafficStats[cnt].value;
            yy = yp - (int)(TrafficStats[cnt].value * scale);

            // Just paint if we are connected...
            if(TrafficStats[cnt].connected == TRUE) {
                fillrect.bottom = yp;
                fillrect.top	= yy;
                fillrect.left	= xx;
                fillrect.right	= xx+plotgranularity;
                memDC->SetBrushOrg(xx,yp);
                if(TrafficStats[cnt].value > 0.0) {
                    //	CBrush brushBar(RGB(0, 255, 0));
                    memDC->FillRect(&fillrect, &colorbrush);
                    memDC->SetPixelV(xx, yy, cyan);

                }
            }
        }


        // last print the textual statistic
        tmp.Format(_T("%8.1f"),TrafficStats[TrafficEntries-1].value);
        COLORREF textcolor = memDC->GetTextColor();
        int bkmode = memDC->GetBkMode();
        memDC->SetBkMode(TRANSPARENT);
        memDC->SetTextColor(darkblue);
        memDC->TextOut(6,5,AllTraffic);
        memDC->SetTextColor(cyan);
        memDC->TextOut(5,5,AllTraffic);
        memDC->SetTextColor(textcolor);
        memDC->SetBkMode(bkmode);

        memDC->SelectObject(oldFont);
        memDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);


        delete memDC;

    }
    pDC -> RestoreDC( nSavedDC );
}

// if you have the Microsoft platform SDK, uncomment the following statement
// and insert Wininet.lib in the linker section of the compiler
// #define _I_HAVE_PLATFORM_SDK_INSTALLED_

void MFTrafficButton::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if(nIDEvent == NETTIMER) {

// If you have installed the Microsoft Platform SDK be happy, because you can use the feature
// added below..
#ifdef _I_HAVE_PLATFORM_SDK_INSTALLED_

        DWORD flag, reserved;
        BOOL erg;
        flag = 0;//INTERNET_CONNECTION_OFFLINE ;
        reserved = 0;
        TCHAR connectionname[1024];
        //	erg = InternetGetConnectedState(&flag, reserved);
        erg =  InternetGetConnectedStateEx(
                   &flag,	//OUT LPDWORD lpdwFlags,
                   (LPTSTR)&connectionname,//OUT LPTSTR lpszConnectionName,
                   1024,//IN DWORD dwNameLen,
                   0//IN DWORD dwReserved
               );

        isOnline = erg;
#else
        isOnline = TRUE;
#endif
        // Get current traffic
        double traffic;
        DWORD  totaltraffic;

        if(isVista()) {
            traffic = m_cTrafficClass.GetTrafficVista(SelectedInterface);
            currentTotalTraffic=m_cTrafficClass.CurrentTotalTraffic;//本次连接总流量
        } else {
            traffic = m_cTrafficClass.GetTraffic(SelectedInterface);
            totaltraffic = m_cTrafficClass.GetInterfaceTotalTraffic(SelectedInterface);
            currentTotalTraffic=m_cTrafficClass.CurrentTotalTraffic;//本次连接总流量

        }




        double delta1;
        double divisor = (1000.0/(double)NETUPDATESPEED);
        delta1 = (double)(traffic * divisor) / 1024.0;
        m_dCurrTraffic=(double)(currentTotalTraffic*divisor)/1024.0;

        TotalTraffic.Format(_T("%.2f KB"),(double)(totaltraffic*divisor)/1024.0);

        CurrentTraffic.Format(_T("%.2f KB/sec"),delta1);
        CurrentTotalTraffic.Format(_T("%.2f KB"),m_dCurrTraffic);


        // Should we recalculate the local maximum per session or per display?
        if(useAdaptiveScale==TRUE) {
            MaxTrafficAmount = 0.0;
        }
        // Shift whole array 1 step to left and calculate local maximum
        for(DWORD x=0; x<TrafficEntries; x++) {
            TrafficStats[x].connected = TrafficStats[x+1].connected;
            TrafficStats[x].value	= TrafficStats[x+1].value;
            if(TrafficStats[x].value > MaxTrafficAmount)
                MaxTrafficAmount = TrafficStats[x].value;
        }
        if(isOnline == TRUE) {
            TrafficStats[TrafficEntries].connected = TRUE;
            TrafficStats[TrafficEntries].value = traffic;
            if(TrafficStats[TrafficEntries].value > MaxTrafficAmount)
                MaxTrafficAmount = TrafficStats[TrafficEntries].value;
        } else {
            TrafficStats[TrafficEntries].connected = FALSE;
            TrafficStats[TrafficEntries].value = traffic;
            if(TrafficStats[TrafficEntries].value > MaxTrafficAmount)
                MaxTrafficAmount = TrafficStats[TrafficEntries].value;
        }

        double delta2;
        divisor = (1000.0/(double)NETUPDATESPEED);
        delta2 = (double)(MaxTrafficAmount * divisor) / 1024.0;
        MaximalTraffic.Format(_T("%.2f KB/sec"),delta2);
        AllTraffic.Format(_T("%.2f / %.2f KB/sec"),delta1, delta2);

    }

    // Force a redraw
    if(drawFlag) {
        Invalidate(FALSE);
    }

    CButton::OnTimer(nIDEvent);
}

/*
	Create little popup
*/
/*
void MFTrafficButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
#define STARTINDEX	2000
#define ENDINDEX	2999
#define SET_SCALING	3000

	HMENU hMenu , hPopup;
	long retId = 0;
	int InterfaceIndex = STARTINDEX;
	POINT pt;

	hMenu = CreatePopupMenu( );			  //  Create a new popup menu
	if( hMenu )
	{
		//  If the menu was created, add the item text
		CString tmp, tmp2;
		double tottraff = (double)m_cTrafficClass.GetInterfaceTotalTraffic(SelectedInterface) / (1024.0*1024.0);
		m_cTrafficClass.GetNetworkInterfaceName(&tmp, SelectedInterface);
		tmp2.Format(_T("%s : %.1f MB"), tmp, tottraff);

		AppendMenu( hMenu , MF_STRING , 1001 , tmp2 );
		AppendMenu( hMenu , MF_SEPARATOR , 1000 , NULL );
		// Submenu
		hPopup = CreatePopupMenu( );			  //  Create a new popup menu
		if( hPopup )
		{
			CString tmp;
			int cnt;
			for(cnt=0; cnt<=m_cTrafficClass.GetNetworkInterfacesCount(); cnt++)
			{
				CString tmp2;
				double tottraff = (double)m_cTrafficClass.GetInterfaceTotalTraffic(cnt) / (1024.0*1024.0);
				m_cTrafficClass.GetNetworkInterfaceName(&tmp, cnt);

				tmp2.Format(_T("%s : %.1f MB"), tmp, tottraff);
				AppendMenu( hPopup , MF_STRING , InterfaceIndex++ , tmp2 );
			}
			AppendMenu( hMenu , MF_POPUP , ( UINT )hPopup , _T("Select Interface") );
			AppendMenu( hMenu , MF_SEPARATOR , 1000 , NULL );
			AppendMenu( hMenu , MF_STRING , SET_SCALING , _T("Toggle scaling to ") + 	ToggleStatusText);
		}
	}

	GetCursorPos( & pt );			 //  Get the cursor position and
	retId = TrackPopupMenu( hMenu , TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD ,  pt.x , pt.y , 0 , this -> m_hWnd , NULL );  //  start the menu
	DestroyMenu( hMenu );				//  Kill the menu when done
	if( retId == 0 )
	{
		return;
	}
	else
	{
		if(retId == SET_SCALING)
		{
			if(useAdaptiveScale == TRUE)
			{
				useAdaptiveScale = FALSE;
				ToggleStatusText = "global maximum";
			}
			else
			{
				useAdaptiveScale = TRUE;
				ToggleStatusText = "local maximum";
			}
		}
		if((retId>=STARTINDEX) && (retId<ENDINDEX) )
		{
			SelectedInterface = retId-STARTINDEX;
			InterfaceHasChanged();
			ReInit(SelectedInterface);
		}
	}

	CButton::OnRButtonDown(nFlags, point);
}
*/

/*
	Button size has changed, we need an update of statistics.
	I am not sure if that works, because never used this...
*/
void MFTrafficButton::ReInit(RECT newSize)
{
    KillTimer(GRIDTIMER);
    KillTimer(NETTIMER);

    //wyw
    if (TrafficStats) {
        delete []TrafficStats;
        TrafficStats = NULL;
    }

    TGSize.cx = newSize.right - newSize.left;
    TGSize.cy = newSize.bottom - newSize.top;

    TrafficEntries = TGSize.cx / plotgranularity;
    TrafficStats = new TRAFFICENTRY[TrafficEntries+1];

    MaxTrafficAmount = 0.0;
    SelectedInterface = 0;

    for(DWORD x=0; x<TrafficEntries; x++) {
        TrafficStats[x].connected = TRUE;
        TrafficStats[x].value	= 0.0;
    }
    m_cTrafficClass.GetTraffic(SelectedInterface);
    m_cTrafficClass.GetTraffic(SelectedInterface);
    MaxTrafficAmount = 0.0;
    SetTimer(GRIDTIMER,	gridupdatespeed,	0);
    SetTimer(NETTIMER,	netupdatespeed,		0);

}

/*
	We want to monitor another interface
*/
void MFTrafficButton::ReInit(int newInterface)
{
    KillTimer(GRIDTIMER);
    KillTimer(NETTIMER);

    SelectedInterface = newInterface;

    for(DWORD x=0; x<TrafficEntries; x++) {
        TrafficStats[x].connected = TRUE;
        TrafficStats[x].value	= 0.0;
    }
// 	if(isVista())
// 	{
    m_cTrafficClass.m_pDlgConnect=m_pDlgConnect;
    m_cTrafficClass.GetTrafficVista(SelectedInterface);
// 	}
// 	else
// 	{
// 		m_cTrafficClass.GetTraffic(SelectedInterface);
// 		m_cTrafficClass.GetTraffic(SelectedInterface);
// 	}

    MaxTrafficAmount = 0.0;
    SetTimer(GRIDTIMER,	gridupdatespeed,	0);
    SetTimer(NETTIMER,	netupdatespeed,		0);

}

void MFTrafficButton::SetUpdateSpeed(UINT netspeed, UINT gridspeed)
{
    gridupdatespeed	= gridspeed;
    netupdatespeed	= netspeed;
    KillTimer(GRIDTIMER);
    KillTimer(NETTIMER);
    SetTimer(GRIDTIMER,	gridupdatespeed,	0);
    SetTimer(NETTIMER,	netupdatespeed,		0);
}



/*
	React calling the callback function
*/
void MFTrafficButton::InterfaceHasChanged()
{
    if(interfaceCallBack !=NULL) {
        interfaceCallBack(SelectedInterface);
    }
}

/*
	Is someone wants to be informed, he has to tell us so
*/
void MFTrafficButton::SetInterfaceNumberNotificationFunction(INTERFACECHANCEDPROC callfunct)
{
    interfaceCallBack = callfunct;
}

/*
	Which kind of traffic do you want to monitor?
*/
void MFTrafficButton::SelectTrafficType(int trafficType)
{
    switch(trafficType) {
    case Traffic_Incoming:
        m_cTrafficClass.SetTrafficType(MFNetTraffic::IncomingTraffic);
        break;
    case Traffic_Outgoing:
        m_cTrafficClass.SetTrafficType(MFNetTraffic::OutGoingTraffic);
        break;
    case Traffic_Total:
        m_cTrafficClass.SetTrafficType(MFNetTraffic::AllTraffic);
        break;
    default:
        m_cTrafficClass.SetTrafficType(MFNetTraffic::AllTraffic);
    }
}

/*
	Which interface do you want to monitor
*/
void MFTrafficButton::SetInterfaceNumber(int interfacenumber)
{
    SelectedInterface = interfacenumber;
    ReInit(SelectedInterface);
}

/*
	Sound important, isnt it?
	Decides, wether to scale the graph using the total maximum traffic amount or the current maximum
*/
void MFTrafficButton::SetAdaptiveScaling(BOOL adaptive)
{
    useAdaptiveScale = adaptive;
}

void MFTrafficButton::setDrawFlag(BOOL flag)
{
    drawFlag=flag;
}

BOOL MFTrafficButton::isVista()
{
    WORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;
    dwVersion = GetVersion();
    dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
    if(dwWindowsMajorVersion>5) { //Windows vista
        return TRUE;
    } else { //Windows XP and others
        return FALSE;
    }
}
