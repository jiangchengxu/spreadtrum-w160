// PopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "PopDlg.h"
#include "CallDlg.h"
#include "ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPopDlg, CBaseDialog)
/////////////////////////////////////////////////////////////////////////////
// CPopDlg dialog
CPopDlg::CPopDlg(UINT nWaveID) : CBaseDialog(CPopDlg::IDD, NULL), m_nWaveID(nWaveID)
{
	m_strTipInfo = _T("");
	m_nBkTag = 1;
	m_bPreMsgBox = TRUE;
    m_bIsPopUp = FALSE;
}

CPopDlg::~CPopDlg()
{

}

void CPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPopDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_STATIC_TIPINFO, m_strTipInfo);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPopDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CPopDlg)
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BUTTON_ANSWER, OnButtonAnswer)
    ON_BN_CLICKED(IDC_BUTTON_REJECT, OnButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_PVIEW, OnButtonPview)
	ON_BN_CLICKED(IDC_BUTTON_PCANCEL, OnButtonPcancel)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_POPDLG_DESTROY, OnDestroyWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopDlg message handlers

BOOL CPopDlg::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	m_bFirst = TRUE;
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & (~(WS_SYSMENU))); 

	//SetSkin(g_SetData.Setup_nSkinStyle);

    ////////////////////////////
    CRect dlgRect;
    GetWindowRect(dlgRect);
    CRect desktopRect; 
    GetDesktopWindow()->GetWindowRect(desktopRect); 
    
    //MoveWindow(x值，y值，窗口宽，窗口高)
    MoveWindow( (desktopRect.Width() - dlgRect.Width()), 
                (desktopRect.Height()-dlgRect.Height()), 
                dlgRect.Width(), dlgRect.Height()); 

    MoveWindow( (desktopRect.Width() - dlgRect.Width()), 
                (desktopRect.Height()-dlgRect.Height()), 
                0, 0 ); 
    
    //m_nWidth m_nHeight m_dx m_dy 是该类的保护成员
    //m_nWidth m_nHeight 是最终宽和高
    //m_dx表示窗口在x轴上的渐变值，m_dy 表示窗口在y轴上的渐变值
    m_nWidth=dlgRect.Width(); 
    m_nHeight=dlgRect.Height(); 
    m_dx=40; // you can modify this 
    m_dy=8 ; // you can modify this 
    
    ::SetTimer(this->m_hWnd, IDT_POPDLG_TIMER,50, NULL);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
void CPopDlg::OnTimer(UINT nIDEvent) 
{
    CRect dlgRect;
    GetWindowRect(dlgRect);
    
    CRect desktopRect;

#if 0
    GetDesktopWindow()->GetWindowRect(desktopRect);
	desktopRect.bottom-=16;
#else
    SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopRect, 0);
#endif

    if(nIDEvent == IDT_POPDLG_TIMER)
    {
		if(m_nPos==1)
		{
			MoveWindow((desktopRect.Width() - m_nWidth),
						(desktopRect.Height() - dlgRect.Height() - m_dy), 
						m_nWidth,
						dlgRect.Height() + m_dy);

        }
		else// if(m_nPos==2)
		{
			MoveWindow((desktopRect.Width() - m_nWidth),
				(desktopRect.Height() - dlgRect.Height() - m_dy-m_nHeight-10), 
				m_nWidth,
                    dlgRect.Height() + m_dy);
        }
        if(dlgRect.Height() >=m_nHeight)
            m_dy=0; // do not over grow
        
        if(dlgRect.Height() >=m_nHeight)
        {
            ::KillTimer(this->m_hWnd, IDT_POPDLG_TIMER); //Stop the timer

            SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
        }
    }
    else if(nIDEvent == IDT_DEPOPDLG_TIMER)
    {
        MoveWindow(    (desktopRect.Width() - m_nWidth),
            (desktopRect.Height() - dlgRect.Height() + m_dy), 
            m_nWidth,
            dlgRect.Height() - m_dy );
        
        MoveWindow( (desktopRect.Width() - m_nWidth), 
            (desktopRect.Height()-dlgRect.Height() + m_dy), 
            0, 0 );

        if(dlgRect.Height() <= 0)
        {
            m_dy=0; // do not over grow
            ::KillTimer(this->m_hWnd, IDT_DEPOPDLG_TIMER); //Stop the timer
        }
    }
    CDialog::OnTimer(nIDEvent);
}

void CPopDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
    TRACKMOUSEEVENT t_MouseEvent;
    t_MouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
    t_MouseEvent.dwFlags     = TME_LEAVE | TME_HOVER;
    t_MouseEvent.hwndTrack   = m_hWnd;
    t_MouseEvent.dwHoverTime = 1;
    ::_TrackMouseEvent(&t_MouseEvent);
}

void CPopDlg::OnButtonAnswer()
{
	//$QCCAV
    char szAtBuf[512] = {0};
#ifdef FEATURE_HAIER_CM
	const char ATA[]="ATA\x0d\x00";//9508 Answer
#else
	const char ATA[]="AT$QCCAV\x0d\x00";//CDMA CM200 Answer
#endif
    strcpy(szAtBuf, ATA);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
        ASSERT(pMainDlg);
        pMainDlg->m_nRingTimes = 0;
        pMainDlg->m_bInComCall = FALSE; 
        DestroyWindow();
        pMainDlg->ForegroundWnd();
        RegisterAtRespFunc(ATRESP_GENERAL_AT, CCallDlg::AtRespAnswer, pMainDlg);
//        Sleep(100);
//        DestroyWindow();
    }
}

/*拒接电话*/
void CPopDlg::OnButtonReject() 
{
    char szAtBuf[512] = {0};
//	const char ATCHUP[]="AT+CHUP\x0d\x00";//9508 Reject
#ifdef FEATURE_HAIER_CM
	const char ATCHUP[]="AT+CHV\x0d\x00";//CDMA CM200 Reject
#else
	const char ATCHUP[]="AT+CHV0\x0d\x00";//CDMA CM200 Reject
#endif
    strcpy(szAtBuf, ATCHUP);
	
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {   
        CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
        ASSERT(pMainDlg);
        pMainDlg->m_nRingTimes = 0;
        pMainDlg->m_bInComCall = FALSE; 
		pMainDlg->m_pCallDlg->m_CallHistKind = HISTKIND_RCV;		//wyw_0330 add
		RegisterAtRespFunc(ATRESP_GENERAL_AT, CCallDlg::AtRespReject, pMainDlg);
        Sleep(100);
        DestroyWindow();
    }
}

//View New Sms
void CPopDlg::OnButtonPview()
{
    ((CHSDPADlg*)AfxGetMainWnd())->ForegroundWnd();
    AfxGetMainWnd()->PostMessage(IDT_TABED_SWITCHTO, 1, 0);
    DestroyWindow();
}

void CPopDlg::OnButtonPcancel() 
{
	DestroyWindow();
}

//来电:  bInComCallOrSms = TRUE
//来短信:bInComCallOrSms = FALSE
void CPopDlg::ShowButton(BOOL bInComCallOrSms)
{
    if(bInComCallOrSms)
    {
		GetDlgItem(IDC_BUTTON_ANSWER)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_REJECT)->ShowWindow(SW_NORMAL);		
		GetDlgItem(IDC_BUTTON_PVIEW)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_PCANCEL)->ShowWindow(SW_HIDE);
    }
    else
    {
		GetDlgItem(IDC_BUTTON_ANSWER)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BUTTON_REJECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_PVIEW)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_PCANCEL)->ShowWindow(SW_NORMAL);
    }
}

BOOL CPopDlg::IsPopUp()
{
    return m_bIsPopUp;
}

LRESULT CPopDlg::OnDestroyWnd(WPARAM wParam, LPARAM lParam)
{
    DestroyWindow();
    return 0;
}

void CPopDlg::OnDestroy() 
{
	CBaseDialog::OnDestroy();
}


