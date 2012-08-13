/*
File: HSDPADlg.cpp 
Desc:
    主界面

    本模块主要管理TABDIALOG，进行DIALOG之间的切换及消息转发
*/

// HSDPADlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "AboutDialog.h"
#include "MAPIUTIL.h"
#include "Ds.h"
#include "Dev.h"
#include "DlgSignIn.h"
#include "SetupData.h"
#include "PreMsg.h"
#include "resource.h"
#include "Dlgsubversion.h"


#include <sstream>
#include "MessageManager.h"
#include "WDSSetEventReportMsg.h"
#include "DMSSetEventReportMsg.h"
#include "NASSetEventReportMsg.h"
#include "DMSGetPowerStateMsg.h"
#include "DMSGetDeviceMfrMsg.h"
#include "DMSGetDeviceModelIdMsg.h"
#include "DMSGetDeviceRevIdMsg.h"
#include "DMSGetMsisdnMsg.h"
#include "DMSGetDeviceSerialNumbersMsg.h"
#include "DMSGetDeviceCapMsg.h"
#include "NASGetSignalStrengthMsg.h"
#include "NASGetServingSystemMsg.h"
#include "WDSAbortMsg.h"
#include "WDSStartNetworkInterfaceMsg.h"
#include "WDSStopNetworkInterfaceMsg.h"
#include "WDSGetPktSrvcStatusMsg.h"
#include "WDSGetCurrentChannelRateMsg.h"
#include "WDSCreateProfileMsg.h"
#include "WDSDeleteProfileMsg.h"
#include "WDSModifyProfileSettingsMsg.h"
#include "WDSGetProfileListMsg.h"
#include "WDSGetProfileSettingsMsg.h"
#include "WDSGetDefaultSettingsMsg.h"


#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_TRAYICON WM_USER + 300

#define WM_AT_RING		(WM_USER+301)
#define WM_AT_SMS		(WM_USER+302)

//查询PIN状态
#define WM_PIN_CHECK	(WM_USER+303)
#define WM_PIN_VERIFY	(WM_USER+304)

#define VK_D 68
#define VK_S 83
#define VK_V 86

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// user defined windows message for new notice received
const static uint16 WM_NEW_NOTICE = 0x0401;
	static BOOL b_start;
// constants used to determine whether or not to display an AfxMsgBox
enum
{
    NO_MSG_BOX = 0,
    MSG_BOX = 1
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


/////////////////////////////////////////////////////////////////////////////
// CHSDPADlg dialog
CReadFalshSMS *CHSDPADlg::m_pReadFalshSMSDlg = NULL;//add by liub for CFT

CHSDPADlg::CHSDPADlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CHSDPADlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CHSDPADlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    //一个指针句柄，不知道有什么作用
    m_hAniCursor = NULL;

    //不清楚m_nid是什么意思,shellapi
    memset(&m_nid, 0 , sizeof(m_nid));
    m_nid.cbSize = sizeof(m_nid);
    m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    m_nid.uID = IDI_ICON_INFO;
    m_nid.hIcon = ::LoadIcon ( AfxGetInstanceHandle() ,MAKEINTRESOURCE (IDI_ICON_INFO) );
    
    m_pTabDialog = NULL;
   // m_pInternetDlg = NULL;
    m_pSmsDlg = NULL;
    m_pSetupDlg = NULL;
    m_pPhoneBookDlg = NULL;
    m_pCallDlg = NULL;

	// @@@@@@
    m_pBrowseDlg = NULL;
    m_pHelpDlg = NULL;  
	
	m_blUseFlashEffect = FALSE;
    m_nBkTag = 0;

	VoiceMail = 0;
	bInitstatus = TRUE;
	VoicemailIndex = -1;
	

	m_pSmsPopDlg = NULL;
    m_pSmsPopDlg = new CPopDlg(IDR_WAVE_SMS);
    ASSERT(m_pSmsPopDlg);

    m_pCallPopDlg = NULL;
	m_pCallPopDlg = new CPopDlg(IDR_WAVE_CALL);
    ASSERT(m_pCallPopDlg);
    
    //m_cRssiInfo.SetBitmap(IDB_ICON_NULL);
	m_bPinProtected = FALSE;
    
    m_SmsFullState = SMS_FULL_UNKNOWN;
    m_nRingTimes = 0;
    m_bEnableClip = FALSE;
	b_start = 0;//初始化未完成

	SMS_type_CPTCMTCFTCDS_VOICEMAIL = 1;
	tempvoicemailASCUS = _T("");
	SMS_Priority = 0;
	VoiceMailCount = _T("0");//add by liub for voicemail

	UE_InitSmsBuf(); 


    ME_SmsNum = ME_SmsMax = SM_SmsNum = SM_SmsMax = 0;
    ME_SmsCnt = SM_SmsCnt = 0;
    m_nReadIndex = 0;
    m_nStep = m_nUpper = m_nLower = 0;
	m_bInComSms     = FALSE;
	m_bInComCall    = FALSE;
    m_bInitMask     = FALSE;
    m_bSyncInitMask = FALSE;
    m_hSyncInitEvt    = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSyncPconlineInitEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSyncSmsInitEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSyncPbmInitEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSyncSimInitEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    InitializeCriticalSection(&m_csSyncInitMask);
	m_bPortStatus = false;
    m_CMModePref = CM_MODE_PREF_MAX;    
    m_GWAcqOrderPref = CM_GW_ACQ_ORDER_PREF_MAX;
	m_bussdflag = false;

	m_NetworkSrv = NW_SRV_NONE;
	m_ProcessDeviceNotice = TRUE; //预先设定网卡安装成功

	m_hSyncCVMIEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hMonitorThread = NULL;	
	m_hSimLockEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CHSDPADlg::~CHSDPADlg()
{
    m_nid.hIcon = NULL;
    Shell_NotifyIcon (NIM_DELETE, &m_nid);

	UE_ReleaseSmsBuf(); 

	DELETEIF(m_pTabDialog);
	DELETEIF(m_pSmsPopDlg);
	DELETEIF(m_pCallPopDlg);

    CloseHandle(m_hSyncInitEvt);

	CloseHandle(m_hSimLockEvt);
    CloseHandle(m_hSyncPconlineInitEvt);

    CloseHandle(m_hSyncSmsInitEvt);
    CloseHandle(m_hSyncPbmInitEvt);
    CloseHandle(m_hSyncSimInitEvt);
    DeleteCriticalSection(&m_csSyncInitMask);
	CloseHandle(m_hSyncCVMIEvt);
}

void CHSDPADlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHSDPADlg)
	DDX_Control(pDX, IDC_UI_ICON_HANDSET, m_cHandSet);
	DDX_Control(pDX, IDC_UI_ICON_ROAM, m_cRoam);
	DDX_Control(pDX, IDC_UI_ICON_NETWK_TYPE, m_cUEType);
    DDX_Control(pDX, IDC_UI_ICON_RSSI, m_cRssiInfo);
	DDX_Control(pDX, IDC_UI_ICON_SMS_UNRD, m_cSmsUnrdInfo);
	DDX_Control(pDX, IDC_UI_ICON_SMS_FULL, m_cSmsFullInfo);
	DDX_Control(pDX, IDC_UI_ICON_PRIVACY_CALL, m_cCallPrivacyInfo);
    DDX_Control(pDX, IDC_UI_ICON_PLMN, m_cPlmnInfo);
    DDX_Control(pDX, IDC_UI_ICON_PLMN2, m_cPlmnInfo2);
	DDX_Control(pDX, IDC_UI_ICON_NWSRV, m_cNWSrvType);
    DDX_Control(pDX, IDC_UI_ICON_UPSPEED, m_cUpSpeed);
    DDX_Control(pDX, IDC_UI_ICON_DWSPEED, m_cDwSpeed);
    DDX_Control(pDX, IDC_UI_ICON_UPFLUX, m_cUpFlux);
    DDX_Control(pDX, IDC_UI_ICON_DWFLUX, m_cDwFlux);
	DDX_Control(pDX, IDC_UI_ICON_VOICESMS,m_cVoiceSms);//add by liub for voicemail
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHSDPADlg, CBaseDialog)
    //{{AFX_MSG_MAP(CHSDPADlg)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SETCURSOR()
    ON_WM_TIMER()
    ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_TRAY_MAXI, OnAppTrayMaxi)
    ON_MESSAGE(WM_PB_ReDrawListCtrl, PbReDraw)
	ON_WM_CANCELMODE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
    ON_MESSAGE(IDT_CALL_DIAL, OnDial)
    ON_MESSAGE(WM_CHANGE_SKIN, ChangeSkin)
    ON_MESSAGE(WM_SEL_CONTACT_INDEX, SelContactIndex)
    ON_MESSAGE(WM_ADD_CONTACT, OnAddContact)
    ON_MESSAGE(WM_AT_RING, OnPopDlg)
	ON_MESSAGE(WM_AT_SMS,OnPopDlg)
    ON_MESSAGE(IDT_TABED_SWITCHTO, SwitchTo)
	ON_MESSAGE(WM_PIN_CHECK,CheckPinStat)
	ON_MESSAGE(WM_PIN_VERIFY,PinVerify)
	ON_MESSAGE(WM_ICON_UPDATE,UpdateIcon)
    ON_MESSAGE(WM_SMS_FULL_NOTIFY, OnSmsFullNotify)
    ON_MESSAGE(WM_SMS_INIT_NOTIFY, OnSmsInitNotify)
    ON_MESSAGE(WM_CREATE_PROGRESS, OnCreateProgress)
	ON_MESSAGE(WM_WINDOWS_MAX, WindowMax)
	ON_MESSAGE(WM_WINDOWS_RESTORE, WindowRestore)	
	ON_MESSAGE(WM_SEARCH_CONTACT, SearchContactInList)
	ON_MESSAGE(WM_SEARCH_GETTOTALCOUNT, GetListTotalCount)
    ON_MESSAGE(WM_NEW_NOTICE,OnNewNotice)
	ON_MESSAGE(WM_INIT_SMSANDPB, OnInitSMSAndPB)
	ON_MESSAGE(WM_CREATE_INTERNET, OnInternetSetup)
		ON_MESSAGE(WM_HOTKEY,OnHotKey)   
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHSDPADlg message handlers

BOOL CHSDPADlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();    
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon



	
	m_nHotKeyID=0;
     BOOL m_isKeyRegistered;
 	m_isKeyRegistered = RegisterHotKey(GetSafeHwnd(),0,MOD_CONTROL|MOD_ALT|MOD_SHIFT,VK_S);

	m_bmpBkStatus.LoadBitmap(IDB_BITMAP_STATUSICONBK);
    
// @@@@@@
#ifndef OPEN_PAGE_UI
    m_connectionPage.SetHSDPADlg(this);
    m_profilesPage.SetHSDPADlg(this);
    m_statisticsPage.SetHSDPADlg(this);
    m_statusPage.SetHSDPADlg(this);

    // subscribe to MessageManager publications
    SubscribeTo(MessageManager::GetInstance());

    // start the MessageManager
    MessageManager::GetInstance().StartMessageManager();
#endif

    // TODO: Add extra initialization here
    if(m_hAniCursor == NULL)
    {
        m_hAniCursor = LoadCursorFromFile(_T("skin\\Cursor.cur"));
    }

    CString strTitle;
    strTitle.LoadString(IDS_APPNAME);
	AddPrefixName(strTitle);
    SetWindowText(strTitle);
    
    //得到一个窗口句柄，如空则返回NULL
    m_nid.hWnd = GetSafeHwnd();
    m_nid.uCallbackMessage = UM_TRAYNOTIFY;
    
    // Set tray icon and tooltip:
    m_nid.hIcon = m_hIcon;
    
    CString strToolTip;
    strToolTip.LoadString(IDS_APPNAME);
	AddPrefixName(strToolTip);
    wcsncpy(m_nid.szTip, strToolTip, strToolTip.GetLength());
#if WINVER>=0x500
    Shell_NotifyIcon(NIM_ADD,&m_nid);
#endif
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_pSmsData = ((CHSDPAApp*)AfxGetApp())->GetSmsData();
    m_pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    
    ASSERT(m_pPbData && m_pSmsData && m_pComm);
	///////////
	//m_cHandlePin.ReSetPinData();
	m_cHandlePin.m_pComm = m_pComm;
	m_cHandlePin.m_pRetWnd = (CHSDPADlg*)this;
	////////////////
    
    GetVersionInfo();
    ResetSyncData();
    RegSyncInitFunc();
    RegisterDsAutoMsgRsp();
    
    UpdateUnreadSmsIcon(); 
    UpdateFullSmsIcon();

    PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPFLUX, 0);
    PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWFLUX, 0);
    PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPSPEED, 0);
    PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWSPEED, 0);
    
    m_bInitMask = TRUE;

	m_bWinSize = true;

	Sleep(80);

	//liub_modify初始化界面最大化
	CWnd*  hWnd = AfxGetMainWnd();
	hWnd->SendMessage(WM_WINDOWS_MAX,0,0);

	SetBottomIconPos();
	
	m_hMonitorThread = ::CreateThread(NULL, 0, MonitorThread, this, 0, NULL);

	if(g_SetData.Setup_nStartMini)
    {
        PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
    }

    return TRUE;
}

void CHSDPADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDialog  dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CBaseDialog::OnSysCommand(nID, lParam);
    }
}

void CHSDPADlg::OnDestroy()
{
	m_bmpBkStatus.DeleteObject();

    WinHelp(0L, HELP_QUIT);
    CBaseDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHSDPADlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
		//add by liub 1110 for bottom background begin
	/*	CRect rect;
		CPaintDC dc(this);
		GetClientRect(&rect);
		CPen PenBlue(PS_SOLID,1,RGB(6,114,160));
		CBrush BrushAqua(RGB(6,114,160));
		dc.SelectObject(PenBlue);
		dc.SelectObject(BrushAqua);
		dc.Rectangle(rect.left,rect.bottom-20,rect.right,rect.bottom);*/

		CRect rect;
		CPaintDC dc(this);
		GetClientRect(&rect);
// 		CBrush m_brBk;
// 		CBitmap bmp;
// 		CPen PenBlue(PS_SOLID,1,RGB(6,114,160));
// 		bmp.LoadBitmap(IDB_BITMAP_STATUSICONBK);
// 		m_brBk.CreatePatternBrush(&bmp);
// 		dc.SelectObject(PenBlue);
// 		dc.SelectObject(m_brBk);
// 		dc.Rectangle(rect.left-2, rect.bottom-45, rect.right+2, rect.bottom);
// 		bmp.DeleteObject();
		//wyw_0207 add down ,remove up
		rect.top = rect.bottom -42;
		m_bmpBkStatus.Draw(&dc, &rect);


	/*	CPen PenH(PS_SOLID,1,RGB(204,204,204));
		CBrush BrushH(RGB(204,204,204));
		dc.SelectObject(PenH);
		dc.SelectObject(BrushH);
		dc.Rectangle(rect.left,rect.bottom-40,rect.right,rect.bottom-20);*/
		//add by liub end

		//wyw_0103 remove: 由于 IDB_BITMAP_STATUSICONBK 显示刷新的问题
//        CBaseDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHSDPADlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


BOOL CHSDPADlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    // TODO: Add your message handler code here and/or call default
    if(m_hAniCursor!=NULL)
    {
        SetCursor(m_hAniCursor);
        return TRUE;
    }
    else
    {
        return CBaseDialog::OnSetCursor(pWnd, nHitTest, message);
    }
}


BOOL CHSDPADlg::InitTabDialog()
{
    //create the TabDialog
    m_pTabDialog = new CTabDialog(IDD_TABDLG_MAIN, this);
    if (m_pTabDialog->Create(IDD_TABDLG_MAIN, this) == FALSE)
        return FALSE;
    
    if(!AddPagesToTabDialog())
        return FALSE;
    
    //get TabDialog's position from the static control
    CRect rect;
    GetClientRect(&rect);
    //rect.bottom -= rect.top;
    //set the TabDialog's positon
    //ClientToScreen(&rect);
    
    m_bFirst = TRUE;

	SetSkin(g_SetData.Setup_nSkinStyle);  //配置文件中的数据，为0调用2K风格，为1则调用XP风格
	if(m_bWinSize)
	{
    m_pTabDialog->SetWindowPos(this,
                               rect.left-m_skinWin.m_BorderLeftWidth, 
                               rect.top-m_skinWin.m_TitleHeight+27,
                               rect.Width()+m_skinWin.m_BorderLeftWidth,
                               rect.Height()+m_skinWin.m_TitleHeight-47, 
                               SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		
		m_pTabDialog->SetWindowPos(this,
                               rect.left-m_skinWin.m_BorderLeftWidth, 
                               rect.top-m_skinWin.m_TitleHeight+27,
                               rect.Width()+m_skinWin.m_BorderLeftWidth,
                               //rect.Height()+m_skinWin.m_TitleHeight-27, 
							   100,
                               SWP_NOZORDER | SWP_SHOWWINDOW);
	////////////////////////////////////////////////////////////////////////// add by liub (获取显示器分辨率)
    xSize = ::GetSystemMetrics(SM_CXSCREEN);
	ySize = ::GetSystemMetrics(SM_CYSCREEN);
	//////////////////////////////////////////////////////////////////////////
	MoveWindow(g_MainDlgMinRect.left + (xSize/2-409) ,g_MainDlgMinRect.top + (ySize/2-264) ,g_MainDlgMinRect.Width(),
				g_MainDlgMinRect.Height(),TRUE);
	RECT rect1;
	RECT rectup;
	RECT rectupspeed;
	RECT rectdown;
	RECT rectdownspeed;
	CWnd* cWnd = NULL;
	int nX; //横向位移

	GetClientRect(&rect1);

	cWnd = GetDlgItem(IDC_UI_ICON_UPFLUX);
	cWnd->GetClientRect(&rectup);
	nX = 10;
	cWnd->MoveWindow(nX,rect1.bottom - 20,rectup.right,rectup.bottom,TRUE);
	
	cWnd = GetDlgItem(IDC_UI_ICON_UPSPEED);
	cWnd->GetClientRect(&rectupspeed);	
	nX += rectup.right;
	cWnd->MoveWindow(nX ,rect1.bottom - 20,rectupspeed.right,rectupspeed.bottom,TRUE);

	cWnd = GetDlgItem(IDC_UI_ICON_DWFLUX);
	cWnd->GetClientRect(&rectdown);	
	nX += rectupspeed.right ;
	cWnd->MoveWindow(nX,rect1.bottom - 20,rectdown.right,rectdown.bottom,TRUE);

	cWnd = GetDlgItem(IDC_UI_ICON_DWSPEED);
	cWnd->GetClientRect(&rectdownspeed);	
	nX +=  rectdown.right ;
	cWnd->MoveWindow(nX,rect1.bottom - 20,rectdownspeed.right,rectdownspeed.bottom,TRUE);	
	}
    
    //m_pTabDialog->SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),SWP_NOZORDER | SWP_SHOWWINDOW);
    
    //initialize the showing of TabDialog
    m_pTabDialog->InitPagesShow();

    
    return TRUE;
}

BOOL CHSDPADlg::AddPageToTabDialog(CTabedDialog* pDlg, UINT nDLGID, RECT ButtonRect, UINT nButtonTextID, UINT nIButtonDD)
{

    //create first dialog    
    if(pDlg->Create(nDLGID, m_pTabDialog) == FALSE)
        return FALSE;
    
    //add first page
    m_pTabDialog->AddPage(pDlg, nIButtonDD);
    
    return TRUE;
}
////////////////////////////////////////////////////////////
//Add pages (include button and dialog) to TabDialog
BOOL CHSDPADlg::AddPagesToTabDialog()
{
    RECT rect;

    memset(&rect, 0, sizeof(RECT));
	
	//Connection 
  	m_pConnectDlg = new CDlgConnect(m_pTabDialog);
       AddPageToTabDialog(m_pConnectDlg, IDD_DIALOG_CONNECT, rect, IDS_INTERNET, IDC_BUTTON_INTERNET);

    //Browse
	        m_pBrowseDlg = new CDlgConnect(m_pTabDialog);
     AddPageToTabDialog(m_pBrowseDlg, IDD_DIALOG_CONNECT, rect, IDS_INTERNET, IDC_BUTTON_BROWSE);

	//Usage
 	m_pUsageDlg=new CNetStatistics(m_pTabDialog);
 	m_pUsageDlg->pDlgConnect=m_pConnectDlg;
 	AddPageToTabDialog(m_pUsageDlg, IDD_DIALOG_NET_STATISTICS, rect, IDS_INTERNET, IDC_BUTTON_USAGE);

	//Call
    if(g_SetData.Main_nCall)
	{
		m_pCallDlg = new CCallDlg(m_pTabDialog);
		AddPageToTabDialog(m_pCallDlg, IDD_DIALOG_CALL, rect, IDS_CALL, IDC_BUTTON_CALL);
	}

    //SMS
    m_pSmsDlg = new CSmsDlg(m_pTabDialog);
    AddPageToTabDialog(m_pSmsDlg, IDD_DIALOG_SMS, rect, IDS_SMS, IDC_BUTTON_SMS);



    //PhoneBook
    m_pPhoneBookDlg = new CPhoneBookDlg(m_pTabDialog);
    AddPageToTabDialog(m_pPhoneBookDlg, IDD_DIALOG_PHONEBOOK, rect, IDS_PHONEBOOK, IDC_BUTTON_PHONEBOOK);

    //Options
    m_pSetupDlg = new CSetupDlg(m_pTabDialog);
    AddPageToTabDialog(m_pSetupDlg, IDD_DIALOG_SETUP, rect, IDS_SETUP, IDC_BUTTON_SETUP);

    //Help
    if(g_SetData.Main_nHelp)
    {
        m_pHelpDlg = new CSetupDlg(m_pTabDialog);
        AddPageToTabDialog(m_pHelpDlg, IDD_DIALOG_SETUP, rect, IDS_SETUP, IDC_BUTTON_HELP);
    }
    return TRUE;
}

BOOL CHSDPADlg::SubClassButton( HWND hwnd )
{
    return CBaseDialog::SubClassButton(hwnd);
}

int CHSDPADlg::ActionAfterInitDlgWithFlashEffect()
{
    InitTabDialog();    
    return 1;
}

int CHSDPADlg::DealwithResponseProc(LPARAM lParam,   // application-defined value
                                   WPARAM wParam   // application-defined value
                                   )
{
    
    return 1;
}

/************************************************************************/
/*
Name:    OnDial
Desc:    处理拨号的消息请求，并转发给其中的TabDialog
Param:    

Return:     TRUE:成功
         其它:      失败        
*/
/*************************************************************************/


LRESULT CHSDPADlg::OnDial(WPARAM wParam, LPARAM lParam)
{        
    ::SendMessage(m_pTabDialog->GetSafeHwnd(), IDT_CALL_DIAL, wParam, lParam);
    
    return 0;
}

LRESULT CHSDPADlg::ChangeSkin(WPARAM wParam, LPARAM lParam)
{
    m_bFirst = TRUE;

	//From skin set page, enable WS_SYSMENU to draw the SysMenu
	HWND m_wnd=GetSafeHwnd();
	DWORD style = GetWindowLong( m_wnd, GWL_STYLE );
    style = style | WS_MINIMIZEBOX;
    style = style | WS_SYSMENU;
	SetWindowLong( m_wnd, GWL_STYLE, style );
	
	SetSkin(lParam);

    OnPaint();
    return 0;
}

LRESULT CHSDPADlg::SelContactIndex(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pPhoneBookDlg->GetSafeHwnd(), WM_SEL_CONTACT_INDEX, wParam, lParam);
    return 0;
}

LRESULT CHSDPADlg::GetListTotalCount(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_pPhoneBookDlg->GetSafeHwnd(), WM_SEARCH_GETTOTALCOUNT, wParam, lParam);
    return 0;
}

LRESULT CHSDPADlg::SearchContactInList(WPARAM wParam, LPARAM lParam)
{
    ::SendMessage(m_pPhoneBookDlg->GetSafeHwnd(), WM_SEARCH_CONTACT, wParam, lParam);
    return 0;
}

LRESULT CHSDPADlg::OnAddContact(WPARAM wParam, LPARAM lParam)
{
    ::SendMessage(m_pPhoneBookDlg->GetSafeHwnd(), WM_ADD_CONTACT, wParam, lParam);
    return 0;
}

void CHSDPADlg::AtRespRing(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg=(CHSDPADlg*)pWnd;

    if(!pDlg->m_bEnableClip && !pDlg->m_pCallDlg->m_blIsInCalling )
    {
        pDlg->m_bInComCall = TRUE;
        pDlg->SendMessage(WM_AT_RING,0,0);
    }
}

void CHSDPADlg::AtRespHandSetCHG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CString RetStr;
	CHSDPADlg *pdlg=(CHSDPADlg *)pWnd;
	RetStr = strArr[0];
	int nRect =0 ;

	if( RetStr.Find(_T("5"),0)!=-1 )
	{
		if (pdlg->m_pCallDlg->m_bWaitingCall)
		{
			pdlg->m_pCallDlg->KillTimer(IDT_ATD_TIMEOUT);
			DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
			((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);	
			::SetEvent(g_BGPassEvt);
		}

		::PostMessage(pdlg->m_pCallPopDlg->GetSafeHwnd(),WM_POPDLG_DESTROY,0,0); 
		::PostMessage(pdlg->m_pCallDlg->GetSafeHwnd(),WM_ATCDVRESULT,0,0);

#ifdef FEATURE_CALL_PRIVACY
		pdlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_CALLPRIVACY, 0);
#endif

		pdlg->m_bInComCall = FALSE;
	}
	else if (RetStr.Find(_T("0"),0) != -1)
	{
		pdlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_HANDSET, 0);
	}
	else if (RetStr.Find(_T("1"),0) != -1)
	{
		pdlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_HANDSET, 1);
	}
	else if (RetStr.Find(_T("2"),0) != -1)
	{
		if (pdlg->m_pCallDlg->m_blIsInCalling == FALSE)
		{
			if(pdlg->m_pCallPopDlg->IsPopUp() && pdlg->m_pCallPopDlg->GetSafeHwnd())
			{
				pdlg->m_nRingTimes = 0;
				::PostMessage(pdlg->m_pCallPopDlg->GetSafeHwnd(),WM_POPDLG_DESTROY,0,0);  
				::PostMessage(pdlg->m_pCallDlg->GetSafeHwnd(),WM_HANDSET_ANSWER_INCOMINGCALL,0,0);
			}
		}
		else
		{
			pdlg->m_pCallDlg->EndCallbyHANDSET();
		}
	}
}

void CHSDPADlg::AtRespSimReady(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(((CHSDPADlg*)pWnd)->m_hSyncSimInitEvt)
        SetEvent(((CHSDPADlg*)pWnd)->m_hSyncSimInitEvt);
}

//断开数据网络，不处理该消息
void CHSDPADlg::AtRespNoCarrier(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	//////////////////////////////////////////////////////////////////////////add by liub 无载波时自动挂断电话，回到待机界面
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    ASSERT(pDlg->m_pCallPopDlg);
	
    pDlg->m_bInComCall = FALSE; 
	
    if(pDlg->m_pCallPopDlg->IsPopUp() && pDlg->m_pCallPopDlg->GetSafeHwnd())
    {
        pDlg->m_nRingTimes = 0;
        ::PostMessage(pDlg->m_pCallPopDlg->GetSafeHwnd(),WM_POPDLG_DESTROY,0,0);  
    }
//	::PostMessage(pDlg->m_pCallDlg->GetSafeHwnd(),WM_ATCDVRESULT,0,0);		//wyw_0402 remove
	//////////////////////////////////////////////////////////////////////////add by liub end
}

//网络侧挂断语音呼叫或通话
void CHSDPADlg::AtRespDialTone(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    ASSERT(pDlg->m_pCallPopDlg);
 
    pDlg->m_bInComCall = FALSE; 

    if(pDlg->m_pCallPopDlg->IsPopUp() && pDlg->m_pCallPopDlg->GetSafeHwnd())
    {
        pDlg->m_nRingTimes = 0;
        ::PostMessage(pDlg->m_pCallPopDlg->GetSafeHwnd(),WM_POPDLG_DESTROY,0,0);  
    }
#ifdef FEATURE_HAIER_CM
	char endRes[3] = {0};
	char *ptr = (char *)strArr[0];
	char *pEndCode = strchr(ptr, ':') + 1;
	int codeLen = strlen(pEndCode);
	strncpy(endRes, pEndCode, codeLen);
	int codeType = atoi(pEndCode);

	if(codeType == 6)
	{//网络侧主动挂断，返回+CEND:6
		::PostMessage(pDlg->m_pCallDlg->GetSafeHwnd(),WM_ATCDVRESULT,0,0);
	}
#else
	//::PostMessage(pDlg->m_pCallDlg->GetSafeHwnd(),WM_ATCDVRESULT,0,0);
#endif
}

void CHSDPADlg::AtRespCMTI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    
    ASSERT(wStrNum == 1);

    BOOL bSnd = FALSE; BYTE cnt = 0;
    char *p = (char*)(strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_CMTI][gc_dsatmode]));
    char *p1, *p2;
    p1 = p; p2 = 0;
    int nItemIndex = -1;
    StSmsCardRecord CardRecord;
    
    memset(&CardRecord, 0x00, sizeof(StSmsCardRecord));

    char szAtBuf[80] = {0};
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
 
    ::WaitForSingleObject(g_BGReadNewSmsEvt, INFINITE);
    
    if(memcmp(strArr[0], gc_dsatResCodeTbl[DSAT_CMTI][gc_dsatmode],
        strlen(gc_dsatResCodeTbl[DSAT_CMTI][gc_dsatmode])))
        goto INCOMSMS;
    
    while(*p)
    {
        if(*p == ',')
        {
            *p++ = 0;
            p2 = p;
            break;
        }
        else p++;
    }
    
    memset(pDlg->m_szNewSmsLoc, 0x00, sizeof(pDlg->m_szNewSmsLoc));
    strncpy(pDlg->m_szNewSmsLoc, p1, 4);
    pDlg->m_NewSmsLoc = LOC_MAX;
    
    if(!memcmp(pDlg->m_szNewSmsLoc+1, gcstrLoc[LOC_ME], strlen(gcstrLoc[LOC_ME])))
        pDlg->m_NewSmsLoc = LOC_ME;
    else if(!memcmp(pDlg->m_szNewSmsLoc+1, gcstrLoc[LOC_UIM], strlen(gcstrLoc[LOC_UIM])))
        pDlg->m_NewSmsLoc = LOC_UIM;
    else if(!memcmp(pDlg->m_szNewSmsLoc+1, "ALL", 3))
        pDlg->m_NewSmsLoc = LOC_MAX;
    else
        goto INCOMSMS;
    
    if(p2 == 0)
        goto INCOMSMS;
    else
        pDlg->m_nNewSmsIndex = atoi(p2);

    if(pDlg->m_NewSmsLoc == LOC_MAX && pDlg->m_nNewSmsIndex != -1)
        goto INCOMSMS;
    
  
    if(pDlg->m_nNewSmsIndex == -1) //current storage is full
    {
        if(pDlg->m_NewSmsLoc == LOC_ME)
        {
            g_ME_SmsNum = g_ME_SmsMax;
            AfxMessageBox(IDS_SMS_PCCARD_FULL, MB_OK | MB_ICONINFORMATION);
        }
        else if(pDlg->m_NewSmsLoc == LOC_UIM)
        {
            g_SM_SmsNum = g_SM_SmsMax;
            pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_SMSFULL, 1);
        }
        else if(pDlg->m_NewSmsLoc == LOC_MAX)
        {
            g_ME_SmsNum = g_ME_SmsMax;
            g_SM_SmsNum = g_SM_SmsMax;
            pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_SMSFULL, 1);
            AfxMessageBox(IDS_SMS_PCCARD_FULL, MB_OK | MB_ICONINFORMATION);
        }
        goto INCOMSMS;
    }
    else
        sprintf(szAtBuf, "%s%s\r", gcstrAtSms[AT_SMS_QCPMS], pDlg->m_szNewSmsLoc);


    //如果新短信已读取到内存中,不再读取,直接弹出来新短信提示框
    if((nItemIndex = UE_SmsFindCardRecord(pDlg->m_NewSmsLoc, pDlg->m_nNewSmsIndex)) != -1)
    {
        pDlg->m_pSmsData->ClearSmsFlag(SMS_TYPE_INBOX, SMS_RECORD_FLAG_NEW);
        UE_ClearSmsFlag(LOC_MAX, SMS_RECORD_FLAG_NEW);
        CardRecord = UE_ReadSmsCardRecord(pDlg->m_NewSmsLoc, nItemIndex);
        CardRecord.record.state = SMS_STATE_MT_NOT_READ;
        CardRecord.record.flag |= SMS_RECORD_FLAG_NEW;
        UE_EditSmsCardRecord(pDlg->m_NewSmsLoc, nItemIndex, CardRecord.record);
        pDlg->UpdateInboxWhenRcvNewSms(pDlg->m_NewSmsLoc, CardRecord.record, CardRecord.index, FALSE);
        goto INCOMSMS; 
    }

    while(cnt < 5)
    {
        bSnd = pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE);
        if(bSnd)
        {
            pDlg->SetTimer(IDT_QCMTI_TIMEOUT, 30000, NULL);
            RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadNewSms, (LPVOID)pWnd);
            break;
        }
        else
        {
            Sleep(200);
            cnt++;
        }
    }

    if(!bSnd)
    {
INCOMSMS:
        ASSERT(0);
        ::SetEvent(g_BGReadNewSmsEvt);
        pDlg->m_bInComSms = FALSE;
    }
}

void CHSDPADlg::AtRespRVMFB(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
 	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
//          //   pDlg->KillTimer(IDT_QCSCA_TIMEOUT);
//             
//             if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
//                // pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCSCA, (LPARAM)TRUE);
//             else
//                 pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCSCA, (LPARAM)TRUE);

	pDlg->AtSendCVMI();




}

void CHSDPADlg::AtRespRVMFBUPDATA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
	if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[/*DSAT_CVMI*/DSAT_OK][gc_dsatmode]) == 0)
	{
		//	 pDlg->VoiceMailCount = (char*)p;
		CString strTemp = (char *)strArr[0];
		
		int nIndexFrom = strTemp.Find(':');
		int nIndexTo = strTemp.GetLength();
		pDlg->VoiceMailCount = strTemp.Mid(nIndexFrom + 2);
		
		if (pDlg->VoiceMailCount == _T("-1") || pDlg->VoiceMailCount == _T("0"))
		{
			pDlg->m_pSmsDlg->voicesmscount = _T("0");
			
		}
		else
			pDlg->m_pSmsDlg->voicesmscount = pDlg->VoiceMailCount;
		
		if (pDlg->VoiceMailCount == _T("0") || pDlg->VoiceMailCount == _T("-1"))
		{
			pDlg->PostMessage(WM_ICON_UPDATE,ICON_TYPE_VOICEMAIL,0);
		}
		else
		{
			pDlg->SMS_type_CPTCMTCFTCDS_VOICEMAIL = 5;//说明是voicemail
			if (b_start == 1)
			{
				pDlg->PostMessage(WM_AT_SMS,1,0);
			}
			
			pDlg->PostMessage(WM_ICON_UPDATE,ICON_TYPE_VOICEMAIL,1);	

		}
		
		HTREEITEM hRoot = pDlg->m_pSmsDlg->m_treeSms.GetRootItem();
		hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
		hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#ifndef FEATURE_VERSION_NOSIM
		hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#endif
		 pDlg->m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);


	}
}

BOOL CHSDPADlg::AtSendCVMI()
{
	//	UpdateUnreadSmsIcon();
    const TCHAR ATCVMI[] = _T("AT+CVMI?\x0d\x00");
    char szAtBuf[255] = {0};
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	WCharToChar(ATCVMI, szAtBuf);
	if(pComm->WriteToPort(szAtBuf, wcslen(ATCVMI), FALSE))
	{
		SetTimer(IDT_TIMER_VOICEMAIL, 30000, NULL);
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCVMI, this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncCVMIEvt, 1000))
		{
			return TRUE;
		}
    }
    return FALSE;
	

}
void CHSDPADlg::AtRespCVMI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    pDlg->KillTimer(IDT_TIMER_VOICEMAIL);
	


	 if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0)
     {
		pDlg->PostMessage(WM_ICON_UPDATE,ICON_TYPE_VOICEMAIL,0);
	 }
	 else if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[/*DSAT_CVMI*/DSAT_OK][gc_dsatmode]) == 0)
	 {
	//	 pDlg->VoiceMailCount = (char*)p;
		 CString strTemp = (char *)strArr[0];
		 
		 int nIndexFrom = strTemp.Find(':');
		 int Voicemailform = strTemp.Find(',');
		 int nIndexTo = strTemp.GetLength();
         pDlg->VoiceMailCount = strTemp.Mid(nIndexFrom + 2,Voicemailform - nIndexFrom -2);
		 
		 pDlg->VoicemailIndex = _wtoi(strTemp.Mid(Voicemailform + 1));

		 pDlg->m_pSmsDlg->VoiceMailCountSMSDlg = pDlg->VoiceMailCount;

		 if (pDlg->VoiceMailCount == _T("-1") || pDlg->VoiceMailCount == _T("0"))
		 {
			 pDlg->m_pSmsDlg->voicesmscount = _T("0");

		 }
		 else
		 {
			 pDlg->m_pSmsDlg->voicesmscount = pDlg->VoiceMailCount;
		 }

		 if (pDlg->VoiceMailCount == _T("0") || pDlg->VoiceMailCount == _T("-1"))
		 {
			 pDlg->PostMessage(WM_ICON_UPDATE,ICON_TYPE_VOICEMAIL,0);
		 }
		 else
		 {
			 pDlg->SMS_type_CPTCMTCFTCDS_VOICEMAIL = 5;//说明是voicemail
			 if (b_start == 1)
			 {
				 pDlg->PostMessage(WM_AT_SMS,1,0);
			 }
			 
	  		 pDlg->PostMessage(WM_ICON_UPDATE,ICON_TYPE_VOICEMAIL,1);	
		 }

		 HTREEITEM hRoot = pDlg->m_pSmsDlg->m_treeSms.GetRootItem();
	     hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
         hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#ifndef FEATURE_VERSION_NOSIM
		 hRoot = pDlg->m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
#endif
		 pDlg->m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
		 //取消切换功能
// 		 pDlg->m_pSmsDlg->m_treeSms.Expand(hRoot,TVE_EXPAND); //展开
// 
// //		 if (pDlg->m_pCallDlg)
// 		 {
// 			 if (pDlg->m_pCallDlg->m_blIsInCalling == FALSE)
// 			 {
// 				 pDlg->PostMessage(IDT_TABED_SWITCHTOSMSDLG,0,1);
// 				// pDlg->m_pSmsDlg->PostMessage(WM_SMS_UPSMSDLGTREE,0,0);
// 			 }
// 			 else
// 			 {
// 				 pDlg->PostMessage(IDT_TABED_SWITCHTOSMSDLG,1,1);//通话时不切换界面
// 				 //pDlg->m_pSmsDlg->PostMessage(WM_SMS_UPSMSDLGTREE,0,0);
// 			 }
// 
// 		 }
		 

	 }
	 SetEvent(((CHSDPADlg*)pWnd)->m_hSyncCVMIEvt);

}

#ifdef FEATURE_HAIER_SMS
void CHSDPADlg::AtRespCMT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;

	pDlg->SMS_type_CPTCMTCFTCDS_VOICEMAIL = 1;
    int cnt = 0;
	int priorityFlag = 0;
	CString temp;
    char *p, *ptmp;
	char ptr[SMS_SC_NUM_MAX] = {0};
    p = (char*)(strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_CMT][gc_dsatmode]));

	int curstate = STATE_OA_NUMBER;
	int length = 0;
	int mYear, mMonth, mDay, mHour, mMinute, mSecond;
	mYear = mMonth = mDay = mHour = mMinute = mSecond = 0;
	int format = 0;


    StSmsRecord record;
    memset(&record, 0x00, sizeof(StSmsRecord));
	record.m_NoATRspCDS = TRUE;
//	record.m_FlashMessage = FALSE;
	record.voicemail = 0;
    record.state = SMS_STATE_MT_NOT_READ;

    while(*p != '\0')
    {
		memset(ptr, 0, SMS_SC_NUM_MAX+2);
		ptmp = strchr((const char *)p, ',');
		if(ptmp == NULL){
			//最后一个参数的长度
			length = strlen(p);
		}else{
			length = ptmp - p;
		}

		strncpy(ptr, p, length > SMS_SC_NUM_MAX ? SMS_SC_NUM_MAX : length);

		switch(curstate){
		case STATE_OA_NUMBER:
			strcpy(record.szNumber, ptr);
			break;
		case STATE_DAYTIME_YEAR:
			mYear = atoi(ptr);
			break;
		case STATE_DAYTIME_MONTH:
			mMonth = atoi(ptr);
			break;
		case STATE_DAYTIME_DAY:
			mDay = atoi(ptr);
			break;
		case STATE_DAYTIME_HOUR:
			mHour = atoi(ptr);
			break;
		case STATE_DAYTIME_MINUTE:
			mMinute = atoi(ptr);
			break;
		case STATE_DAYTIME_SECOND:
			mSecond = atoi(ptr);
			break;
		case STATE_LANGUAGE:
			break;
		case STATE_FORMAT:
			format = atoi(ptr);
			break;
		case STATE_LENGTH:
			break;
		case STATE_PRIORITY:
			record.SMSpriority = atoi(ptr);
			break;
		case STATE_SECURITY:
			break;
		case STATE_TYPE:
			break;
		case STATE_STATUS:
			break;
		case STATE_SMS_END:
			break;
		}

		if(curstate == STATE_SMS_END){
			break;
		}

		curstate++;
		p+=length + 1;
    }

	pDlg->SMS_Priority = record.SMSpriority;


    if((mMonth >= 1 && mMonth <= 12) && (mDay >= 1 && mDay <= 31) 
        && (mHour >= 0 && mHour <= 23) && (mMinute >=0 && mMinute <= 59) && (mSecond >=0 && mSecond <= 59))
    {
        record.timestamp = COleDateTime(mYear, mMonth, mDay, mHour, mMinute, mSecond);
    }
    else{
		record.timestamp = COleDateTime::GetCurrentTime();
	}

	int nRefCnt=0, nSeqCnt=0, nTotalCnt=0;
	p = (char *)strArr[1];
	if(ExtractConcatenateSmsPara_ChinaTel(p, &nRefCnt, &nSeqCnt, &nTotalCnt)){
		//判断是否为长短信，如果是，跳过udh
		p += *p + 1;
		record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
		record.nRefCnt = nRefCnt;
		record.nSeqCnt = nSeqCnt;
		record.nTotalCnt = nTotalCnt;
	}

    if(wStrNum == 2)
    {
		if(format == 0){
			//GSM 7bit
		}else if(format == 1){
			//ASCII
			strcat(record.szContent, (char*)p);
		}else if(format == 6){
			//UNICODE
			//去掉短信内容结束符0X001A/0X001B
			int wlength = wcslen((wchar_t*)p);
			wchar_t *content = new wchar_t[wlength];
			memset(content, 0, wlength*sizeof(wchar_t));
			wcsncpy(content, (wchar_t*)p, wlength-1);
			char * strGb = WCharToGB(content);
			strcat(record.szContent, (strGb));
		}else{
			//OTHER FORMAT
		}
    }
    else
        memset(record.szContent, 0, SMS_CHAR_MAX*2);
    
	pDlg->RcvNewSmsProc(LOC_PC, record);
    Sleep(100);
}

BOOL ExtractConcatenateSmsPara_ChinaTel(char *Para, int *nRefCnt, int *nSeqCnt, int *nTotalCnt){
	if(Para == NULL || strlen(Para) == 0){
		return FALSE;
	}

    char *p = Para;
	int hrLength = 0;
	int idenLength = 0;

	hrLength = *p++;

	if(p[0] == 0x00){
		idenLength = 1;
	}else if(p[0] == 0x08){
		idenLength = 2;
	}

	if((hrLength != 0x05 || idenLength != 1)
		&& (hrLength != 0x06 || idenLength != 2))
		return FALSE;

	p+= idenLength;

	if(p[0] != hrLength - 2){
		return FALSE;
	}

	p++;

	*nRefCnt = p[0];
	p++;
	*nTotalCnt = p[0];
	p++;
	*nSeqCnt = p[0];
	return TRUE;
}
#else
void CHSDPADlg::AtRespCMT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;

	pDlg->SMS_type_CPTCMTCFTCDS_VOICEMAIL = 1;
    int cnt = 0;
	int priorityFlag = 0;
	CString temp;
    char *ptr[4], *p;
    p = (char*)(strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_CMT][gc_dsatmode]));
    ptr[0] = ptr[1] = ptr[2] = ptr[3] = 0;
    
    BOOL bOutQuot = TRUE;
    
    while(*p != '\0')
    {
        if(*p == '\"')
        {   
            if(bOutQuot && cnt < 4)
                ptr[cnt++] = p;
            
            bOutQuot = !bOutQuot;
            p++;
        }
        else if(*p == ',' && bOutQuot)
        {
            *p++ = '\0';
			priorityFlag++;
        }
        else
        {
			if (3 == priorityFlag)
			{
				ptr[cnt++] = p;
			}
            p++;
        }
    }
    
    for(int i = 0; i < 4; i++)
    {
        if(ptr[i] != 0)
        {
            if(*(ptr[i] + strlen(ptr[i]) - 1) == '\"')
                *(ptr[i] + strlen(ptr[i]) - 1) = '\0';
            if(*ptr[i] == '\"')
                ptr[i]++;
        }
    }
    	USES_CONVERSION;
    StSmsRecord record;
    memset(&record, 0x00, sizeof(StSmsRecord));
	record.m_NoATRspCDS = TRUE;
//	record.m_FlashMessage = FALSE;
	record.voicemail = 0;
	temp = *ptr[2];
    record.SMSpriority = _wtoi(temp);
	pDlg->SMS_Priority = record.SMSpriority;

    
    record.state = SMS_STATE_MT_NOT_READ;
	//Modified by Zhou Bin 2008.12.30
	//CString ptr0=(char*)ptr[0];
    
    //wcscpy(record.szNumber, ptr0);
	strcpy(record.szNumber, ptr[0]);
    
    int time, scnum, concatenate;
    time = 1; scnum = 0, concatenate = 3;
    
    if(!(ptr[time] && *ptr[time]))
        record.timestamp = COleDateTime::GetCurrentTime();
    else
    {
        if(!GetTimeFromStr((LPCSTR)ptr[time], record.timestamp))
            record.timestamp = COleDateTime::GetCurrentTime();
    }
    //Modified by Zhou Bin 2008.12.30
	//	CString ptrsc=(char*)(ptr[scnum]);
    if(!(ptr[scnum] && *ptr[scnum]))
        memset(record.szSCNumber, 0x00, SMS_SC_NUM_MAX);
    else
        strncpy((char*)record.szSCNumber, ptr[scnum], SMS_SC_NUM_MAX);
		// wcsncpy(record.szSCNumber, ptrsc, SMS_SC_NUM_MAX);

    if((ptr[concatenate] && *ptr[concatenate]))//flexi LMS:此处需修改为：根据下位机修改过的参数来判断是否是长短信
    {


      //  if(ExtractConcatenateSmsPara(ptr[concatenate], &record.nRefCnt, &record.nSeqCnt, &record.nTotalCnt))
	  //	            record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;//默认

		//flexi start
		//flexi LMS:此处需修改为：定义record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
		//重新修改ExtractConcatenateSmsPara函数，各个参数修改为从短信内容前7个字符取得，并删除短信内容中的前7个字符；
		record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
		CString strGb = UCS2ToGB(A2W((char*)strArr[1]));//add by liub for UCS2转换为GB
        CString str_concatenate = strGb.Left(7);
		ExtractConcatenateSmsPara_Flexi(str_concatenate, &record.nRefCnt, &record.nSeqCnt, &record.nTotalCnt);//此函数还待修改
		//flexi end
    }
    

    if(wStrNum == 2)
    {
       //CString strGb = GBToUCS2((char*)strArr[1]);//delete by liub 不知为什么要把GB转换为UCS2格式。
       //CString strGb = UCS2ToGB((CString)((char*)strArr[1]));//add by liub for UCS2转换为GB
		CString strGb = UCS2ToGB(A2W((char*)strArr[1]));//add by liub for UCS2转换为GB //flexi LMS:此处需修改为：如果是长短信，去掉前7个字符（待修改）
	   //wcsncpy(record.szContent, strGb, SMS_CHAR_MAX);
		//strncpy(record.szContent, W2A(strGb), SMS_CHAR_MAX);
		if ((ptr[concatenate] && *ptr[concatenate]))
		{
			strGb = strGb.Mid(7);
		}
		strcat(record.szContent, W2A(strGb));
    }
    else
        memset(record.szContent, 0, SMS_CHAR_MAX*2);
    
	pDlg->RcvNewSmsProc(LOC_PC, record);
    Sleep(100);
}
#endif

void CHSDPADlg::AtRespRSSI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	int len,valRssi;
	BYTE strRssi[5];
	BYTE *p=strArr[0];
#ifdef FEATURE_HAIER_CM
	int respLen = strlen((char *)p);
	p = (unsigned char *)strchr((char *)p,':') + 1;
	memset(strRssi,0,5);
	len = respLen-strlen((char *)p);
	strncpy((char *)strRssi,(char *)p,len);
#else
	p = (unsigned char *)strchr((char *)p,' ');
	memset(strRssi,0,5);
	len = strchr((char *)p,',')-(char *)p;
	strncpy((char *)strRssi,(char *)p,len);
#endif
#if 0
	valRssi = atoi((char *)strRssi)/6;
    if (valRssi>6) {
        valRssi=6;
    }
#else
    valRssi = atoi((char*)strRssi);
#endif

	((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_RSSI, valRssi);

    if(valRssi == 6)
        ((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 0);
    else
    {
        if(wcslen(((CHSDPADlg*)pWnd)->m_szPLMN) == 0)
        {
            //Sleep(3000);
            //((CHSDPADlg*)pWnd)->AtSndCOPSEx();
        }
        else
            ((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 1);
    }
}

void CHSDPADlg::AtRespReadNewSms(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    pDlg->KillTimer(IDT_QCMTI_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                    strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->m_nNewSmsIndex = 0;
        memset(pDlg->m_szNewSmsLoc, 0x00, sizeof(pDlg->m_szNewSmsLoc));  
        pDlg->m_NewSmsLoc = LOC_MAX;
        pDlg->m_bInComSms = FALSE;
        ::SetEvent(g_BGReadNewSmsEvt);
        return;
    }

    if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
        && memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCPMS], strlen(gcstrResSms[AT_SMS_QCPMS])) == 0)
    {

        if(pDlg->m_nNewSmsIndex == -1) //current storage is full
        {
            ASSERT(0);
        }

        BOOL bSnd = FALSE; BYTE cnt = 0;
        char szAtBuf[50] = {0};
        sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGR], pDlg->m_nNewSmsIndex);
        CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
        ASSERT(pComm);
        while(cnt < 5)
        {
            bSnd = pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE);
            if(bSnd)
            {
                pDlg->SetTimer(IDT_QCMTI_TIMEOUT, 30000, NULL);
                RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespReadNewSms, (LPVOID)pWnd);
                break;
            }
            else
            {
                Sleep(200);
                cnt++;
            }
        }

        if(!bSnd)
        {
            ASSERT(0);
            pDlg->m_bInComSms = FALSE; 
            ::SetEvent(g_BGReadNewSmsEvt);
        }
    }
    else if(strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
            && memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCMGR],
                        strlen(gcstrResSms[AT_SMS_QCMGR])) == 0)
    { 
        StSmsRecord record;
        memset(&record, 0x00, sizeof(StSmsRecord));
		record.m_NoATRspCDS = TRUE;
        
        if(SmsAtCMGRRspProc(strArr, wStrNum, record, SMS_KIND_MT))
		{
            if(g_bSmsSyncComm)
                pDlg->RcvNewSmsProc(pDlg->m_NewSmsLoc, record, pDlg->m_nNewSmsIndex);
            else
            {
                pDlg->UpdateInboxWhenRcvNewSms(LOC_PC, record, 0, TRUE, TRUE);
            }
		}

        pDlg->m_bInComSms = FALSE; 
        ::SetEvent(g_BGReadNewSmsEvt);
    }
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCPMS()
{
    char szAtBuf[50] = {0};

    EnLocType PreStore = LOC_UIM;

    if(g_SetData.Messages_nPreStor == SMS_PRESTOR_ME)
        PreStore = LOC_ME;
    
    //第一个参数读，删必须指定为USIM卡
    sprintf(szAtBuf, "%s\"%s\",\"%s\",\"%s\"\r", gcstrAtSms[AT_SMS_QCPMS],
            gcstrLoc[LOC_UIM], gcstrLoc[LOC_ME], gcstrLoc[PreStore]);

    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPMS, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg *pDlg = (CHSDPADlg*)pWnd;
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0
        || strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
        || memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCPMS], strlen(gcstrResSms[AT_SMS_QCPMS])))
    {
        pDlg->m_SmsFullState = SMS_FULL_UNKNOWN;
        SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
        return;
    }

    int cnt = 0;
    char *ptr[4], *p;
    ptr[0] = p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCPMS])];
    
    while(*p != '\0' && cnt < 4)
    {
        if(*p == ',')
        {
            *p++ = '\0';
            ptr[++cnt] = p;
        }
        else
        {
            p++;
        }
    }

    
    pDlg->SM_SmsNum = atoi((const char*)ptr[0]);
    pDlg->SM_SmsMax = atoi((const char*)ptr[1]);
    pDlg->ME_SmsNum = atoi((const char*)ptr[2]);
    pDlg->ME_SmsMax = atoi((const char*)ptr[3]);

    g_ME_SmsNum = pDlg->ME_SmsNum;
    g_ME_SmsMax = pDlg->ME_SmsMax;
    g_SM_SmsNum = pDlg->SM_SmsNum;
    g_SM_SmsMax = pDlg->SM_SmsMax;
	
    if(pDlg->ME_SmsNum == pDlg->ME_SmsMax && pDlg->SM_SmsNum == pDlg->SM_SmsMax)
        pDlg->m_SmsFullState = SMS_FULL_BOTH;
    else if(pDlg->ME_SmsNum == pDlg->ME_SmsMax)
        pDlg->m_SmsFullState = SMS_FULL_ME;
    else if(pDlg->SM_SmsNum == pDlg->SM_SmsMax)
        pDlg->m_SmsFullState = SMS_FULL_SM;

    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

LRESULT CHSDPADlg::OnPopDlg(WPARAM wParam, LPARAM lParam)
{
	int		nPos = 0;
    CString strNum;
    CString strName;
    CString strUnknown;
    CString strT1, strT2;
    TCHAR    szName[PB_NAME_MAX + 2];
	UCHAR chr;

    memset(szName, 0x00, sizeof(szName));

    strT1.LoadString(IDS_POP_SMS_T);
    strT2.LoadString(IDS_POP_CALL_T);
	
    strUnknown.LoadString(IDS_UNKNOWN);

    if(wParam == 1)
    {
        if(g_SetData.Messages_nAlertTone)
        {
            if(wcslen(g_SetData.Messages_szSoundFile) == 0
                || !PlaySound(g_SetData.Messages_szSoundFile, NULL, SND_ASYNC | SND_NOSTOP))
                PlaySound(_T("wav\\sms.wav"), NULL, SND_ASYNC | SND_NOSTOP);
        }

        if(g_SetData.Messages_nAlertWindow)
        {   
            KillTimer(IDT_SMS_TIMEOUT);
            SetTimer(IDT_SMS_TIMEOUT, 8000, NULL);
            
            ASSERT(m_pSmsPopDlg);    
            m_pSmsPopDlg->DestroyWindow();
            m_pSmsPopDlg->Detach();
            nPos = (m_pCallPopDlg->m_nPos!=1)?1:2;
            m_pSmsPopDlg->Create(IDD_DIALOG_POP_TIP, NULL, nPos );
			m_pSmsPopDlg->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);	//wyw_0402 add
            m_pSmsPopDlg->SetWindowText(strT1);
            m_pSmsPopDlg->m_strTipInfo.LoadString(IDS_TIPINFO_SMS);
			if (SMS_type_CPTCMTCFTCDS_VOICEMAIL == 5)//voicemail
			{
				m_pSmsPopDlg->m_strTipInfo = VoiceMailCount;
				m_pSmsPopDlg->m_strTipInfo += _T(" Voice mail Messages");
			}

          
			if (SMS_type_CPTCMTCFTCDS_VOICEMAIL != 5)
			{
           // strNum.Format(_T("%s"), (LPCTSTR)lParam);
			///add by liub for Unicode显示，字节转换（短——》宽）
			char* tempNum = (char*)lParam;
			chr = *(tempNum + 1);

           // strNum.Format(_T("%s"), (LPCTSTR)lParam);
			///add by liub for Unicode显示，字节转换（短——》宽）
			if (chr)
			{
			DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, tempNum, -1, NULL, 0);
			wchar_t *pwText;
			pwText = new wchar_t[dwNum];
			if(!pwText)
			{
				delete []pwText;
			}
			MultiByteToWideChar (CP_ACP, 0, tempNum, -1, pwText, dwNum);
	        strNum = pwText;
			delete []pwText;
	
			}
			else
				strNum.Format(_T("%s"), (LPCTSTR)lParam);


	
    ////////////////
            
            if(strNum.GetLength() > 0)
            {
                if(m_pPbData->SearchNameByNumber(strNum, szName))
                    strName.Format(_T("%s\r\n%s"), szName, strNum);
                else
                    strName.Format(_T("%s"), strNum);
            }
            else
                strName.Format(_T("%s"), strUnknown);
            
            m_pSmsPopDlg->m_strTipInfo += strName;
			}
            m_pSmsPopDlg->SetDlgItemText(IDC_STATIC_TIPINFO, m_pSmsPopDlg->m_strTipInfo);
            m_pSmsPopDlg->ShowButton(FALSE);
            m_pSmsPopDlg->ShowWindow(SW_SHOW);
        }
	}
    else
    {
#if 0
        KillTimer(IDT_RING_TIMEOUT);
        SetTimer(IDT_RING_TIMEOUT, 8000, NULL);
#endif
        ASSERT(m_pCallPopDlg);
		if(m_pCallPopDlg->IsPopUp())
			return FALSE;

#if 1
        //连续收到(RING_TIMES-1)次RING表示一次来电事件
        if(m_nRingTimes < RING_TIMES)
        {
            m_nRingTimes++;
            return FALSE;
        }
        else
            m_nRingTimes = 0;
#endif        
        m_pCallPopDlg->DestroyWindow();
        m_pCallPopDlg->Detach();
		nPos = (m_pSmsPopDlg->m_nPos!=1)?1:2;
		m_pCallPopDlg->Create(IDD_DIALOG_POP_TIP, NULL,nPos);
		m_pCallPopDlg->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);	//wyw_0402 add
		m_pCallPopDlg->SetWindowText(strT2);
		m_pCallPopDlg->m_strTipInfo.LoadString(IDS_TIPINFO_CALL);

        strNum = m_pCallDlg->m_CallHistNumber;

		if(strNum.GetLength() > 0)
		{
			if(m_pPbData->SearchNameByNumber(strNum, szName))
				strName.Format(_T("%s\r\n%s"), szName, strNum);
            else
                strName.Format(_T("%s"), strNum);
		}
        else
            strName.Format(_T("%s"), strUnknown);
/*
        CWnd *pWndChild;
        
        // If so, does it have any popups?
        pWndChild = this->GetLastActivePopup();
        
        // If iconic, restore the main window
        if (this->IsIconic())
            this->ShowWindow(SW_RESTORE);
        
        this->ShowWindow(SW_SHOW);
        // Bring the main window or its popup to the foreground
        pWndChild->SetForegroundWindow();
//*/
/*
        //CWnd *pwnd = FindWindow(NULL,""); 
        this->ShowWindow(SW_SHOW);
        this->SetForegroundWindow();
//*/
/*
        CString strPopText,strTipText,strMainText,strResult;

        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
        CWnd *pWndChild;

        // If so, does it have any popups?
        pWndChild = this->GetLastActivePopup();
        pWndChild->GetWindowText(strPopText);
        strResult.Format("WindowText=%s\n", strPopText);
        //AfxMessageBox(strResult);
        
        // If iconic, restore the main window
        if (this->IsIconic())
            this->ShowWindow(SW_RESTORE);
        
        // Bring the main window or its popup to the foreground
        pWndChild->SetForegroundWindow();

        //this->ShowWindow(SW_RESTORE);
        //this->SetForegroundWindow();
*/
        m_pCallPopDlg->m_strTipInfo += strName;
		m_pCallDlg->m_CallHistKind = HISTKIND_MIS;
		m_pCallDlg->m_CallHistTimeStamp = COleDateTime::GetCurrentTime();
		m_pCallPopDlg->SetDlgItemText(IDC_STATIC_TIPINFO, m_pCallPopDlg->m_strTipInfo);
        m_pCallPopDlg->ShowButton(TRUE);        
		m_pCallPopDlg->ShowWindow(SW_SHOW);
	}
    return TRUE;
}

LRESULT CHSDPADlg::SwitchTo(WPARAM wParam, LPARAM lParam)
{
    if(wParam == 0) //Call
    {
        if(lParam == 1) //Answer
        {
            m_pTabDialog->SwitchDlg(m_pCallDlg);
            m_pCallDlg->UpdateData(FALSE);

			::SendMessage(m_pCallDlg->GetSafeHwnd(), WM_ATCDVRESULT, lParam, 0);		//wyw_0402 modify
        }
#ifdef FEATURE_HAIER_CM
		::SendMessage(m_pCallDlg->GetSafeHwnd(), WM_ATCDVRESULT, lParam, 0);
#else
//        ::SendMessage(m_pCallDlg->GetSafeHwnd(), WM_ATCDVRESULT, lParam, 0);
#endif
    }
    else //Sms
    {
        //查看新短信时不切换主界面
        //m_pTabDialog->SwitchDlg(m_pSmsDlg);
        ::SendMessage(m_pSmsDlg->GetSafeHwnd(), WM_SMS_VIEW_MSG, 0, 0);
    }

    return 0;
}

void CHSDPADlg::AtRespCSMSS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CSMSS][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_CSMSS][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_CSMSS][gc_dsatmode]);
        ((CHSDPADlg*)pWnd)->m_bSMSS = (BOOL)atoi(ptr);
    }

    if(((CHSDPADlg*)pWnd)->m_hSyncSmsInitEvt)
        SetEvent(((CHSDPADlg*)pWnd)->m_hSyncSmsInitEvt);
}

void CHSDPADlg::AtRespSIDLOCK(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_SIDLOCK][gc_dsatmode],
		strlen(gc_dsatResCodeTbl[DSAT_SIDLOCK][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_SIDLOCK][gc_dsatmode]);
        ((CHSDPADlg*)pWnd)->m_nSimLock = atoi(ptr);
    }
	
    if(((CHSDPADlg*)pWnd)->m_hSimLockEvt)
        SetEvent(((CHSDPADlg*)pWnd)->m_hSimLockEvt);
}

void CHSDPADlg::AtRespCPBSS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CPBSS][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_CPBSS][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_CPBSS][gc_dsatmode]);
        ((CHSDPADlg*)pWnd)->m_bPBSS = (BOOL)atoi(ptr);
    }

    if(((CHSDPADlg*)pWnd)->m_hSyncPbmInitEvt)
        SetEvent(((CHSDPADlg*)pWnd)->m_hSyncPbmInitEvt);
}

/*监听到来电号码*/
/*返回串格式：
+CLIP:	Number type,Subaddress,Subaddress type,
		DSAT_TA_UNKNOWN       129	0x80|CM_TON_UNKNOWN      |CM_NPI_ISDN
		DSAT_TA_INTERNATIONAL 145	0x80|CM_TON_INTERNATIONAL|CM_NPI_ISDN
		
	"",128,,,,0
*/
void CHSDPADlg::AtRespCLIP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pdlg=(CHSDPADlg*)pWnd;

	if (pdlg->m_pCallDlg->m_blIsInCalling)
	{
		return;
	}
#ifdef FEATURE_HAIER_CM
	CString sRet = strArr[0];
	TCHAR strv[128]={0};
    TCHAR number[50] = {0};
	TCHAR typestr[5]={0};
	TCHAR *pnum,*ptype;
	int	 type;
	int  numlen;
	int  typelen;
	
    ASSERT(sRet.GetLength()<=128);
	
    wcscpy(strv,sRet);
	pnum = wcschr(strv,':')+1;
	numlen = wcschr(pnum,',')-pnum;
	wcsncpy(number,pnum,numlen);
	ptype = pnum+numlen+1;
	typelen = wcschr(ptype,',')-ptype;
	wcsncpy(typestr,ptype,typelen);
	type = _wtoi(typestr);

#else
	CString sRet = strArr[0];
	TCHAR strv[128]={0};
    TCHAR number[50] = {0};
	TCHAR typestr[5]={0};
	TCHAR *pnum,*ptype;
	int	 type;
	int  numlen;
	int  typelen;
	
    ASSERT(sRet.GetLength()<=128);
	
    wcscpy(strv,sRet);
	pnum = wcschr(strv,'\"')+1;
	numlen = wcschr(pnum,'\"')-pnum;
	wcsncpy(number,pnum,numlen);
	ptype = pnum+numlen+3;
	typelen = wcschr(ptype,',')-ptype;
	wcsncpy(typestr,ptype,typelen);
	type = _wtoi(typestr);
#endif

	if(type==145 || type==129)
    {
		pdlg->m_pCallDlg->m_CallHistNumber.Format(_T("+%s"), number);
    }
    else
    {
        pdlg->m_pCallDlg->m_CallHistNumber.Format(_T("%s"), number);
    }

    pdlg->m_bInComCall = TRUE; 
    pdlg->SendMessage(WM_AT_RING,0,0);
}

void CHSDPADlg::AtRespQCSMSS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
#ifdef FEAUTURE_HAIER_SMS
       && !memcmp((const char*)strArr[0], "+VSPST:", strlen("+VSPST:")))
#else
       && !memcmp((const char*)strArr[0], "$CSMSS:", strlen("$CSMSS:")))
#endif
    {
#ifdef FEATURE_HAIER_SMS
		char *ptr = (char*)strArr[0] + strlen("+VSPST:");
#else
        char *ptr = (char*)strArr[0] + strlen("$CSMSS:");
#endif
        ((CHSDPADlg*)pWnd)->m_bSMSS = (BOOL)atoi(ptr);
    }
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncSmsInitEvt);
}

void CHSDPADlg::AtRespQSIDLOCK(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
		&& !memcmp((const char*)strArr[0], "$SIDLOCK: ", strlen("$SIDLOCK: ")))
    {
        char *ptr = (char*)strArr[0] + strlen("$SIDLOCK: ");
        ((CHSDPADlg*)pWnd)->m_nSimLock = atoi(ptr);
    }
	else if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]))
	{
		((CHSDPADlg*)pWnd)->m_nSimLock = 1;
	}
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncSmsInitEvt);
}

//add by GL AtRespQPCONLINE
void CHSDPADlg::AtRespQPCONLINE(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
       && !memcmp((const char*)strArr[0], _T("OK"), wcslen(_T("OK"))))
    {
      //  char *ptr = (char*)strArr[0] + strlen("$PCONLINE");
      // ((CHSDPADlg*)pWnd)->m_bPCONLINE = (BOOL)atoi(ptr);
    }
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncPconlineInitEvt);
}

void CHSDPADlg::AtRespQCPBSS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
#ifdef FEATURE_HAIER_SMS
       && !memcmp((const char*)strArr[0], "+VSPST:", strlen("+VSPST:")))
#else
       && !memcmp((const char*)strArr[0], "$CPBSS:", strlen("$CPBSS:")))
#endif
    {
#ifdef FEATURE_HAIER_SMS
		char *ptr = (char*)strArr[0];
		ptr += strlen(ptr) - 1;
#else
        char *ptr = (char*)strArr[0] + strlen("$CPBSS:");
#endif
        ((CHSDPADlg*)pWnd)->m_bPBSS = (BOOL)atoi(ptr);
    }
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncPbmInitEvt);
}

void CHSDPADlg::AtRespNWCHG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
   if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_NWCHG][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_NWCHG][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_NWCHG][gc_dsatmode]);
        char *p = ptr;
        while(*p)
        {
            if(*p == ',')
            {
                *p = 0;
                break;
            }
            else
                p++;
        }
        ((CHSDPADlg*)pWnd)->m_NetworkType = (EnNetWorkType)atoi(ptr);
        ASSERT(((CHSDPADlg*)pWnd)->m_NetworkType >= NW_TYPE_NOSRV
            && ((CHSDPADlg*)pWnd)->m_NetworkType < NW_TYPE_MAX);
		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, ((CHSDPADlg*)pWnd)->m_NetworkType);
    }
}

void CHSDPADlg::AtRespNWSRVCHG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;

    if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_NWSRVCHG][gc_dsatmode],
        strlen(gc_dsatResCodeTbl[DSAT_NWSRVCHG][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_NWSRVCHG][gc_dsatmode]);
        char *p = ptr;
        pDlg->m_NetworkSrv = (EnNetWorkSrv)atoi(ptr);
        ASSERT(pDlg->m_NetworkSrv >= NW_SRV_NONE && pDlg->m_NetworkSrv < NW_SRV_MAX);
        
        BYTE nwsrv = 0;
		pDlg->m_NetCallLog = NW_CALLLOG_NOSRV;
        switch(pDlg->m_NetworkSrv)
        {
        case NW_SRV_GPRS:
            nwsrv = 1;
			pDlg->m_NetCallLog = NW_CALLLOG_GSM;
            break;
        case NW_SRV_EDGE:
            nwsrv = 2;
			pDlg->m_NetCallLog = NW_CALLLOG_GSM;
            break;
        case NW_SRV_HSDPA:
            nwsrv = 3;
			pDlg->m_NetCallLog = NW_CALLLOG_WCDMA;
            break;
        case NW_SRV_GSM:
            nwsrv = 4;
			pDlg->m_NetCallLog = NW_CALLLOG_GSM;
            break;
        case NW_SRV_UMTS:
            nwsrv = 5;
			pDlg->m_NetCallLog = NW_CALLLOG_WCDMA;
            break;
		case NW_SRV_HSUPA:
            nwsrv = 6;
			pDlg->m_NetCallLog = NW_CALLLOG_WCDMA;
            break;
		default:
			break;
        }

        if(nwsrv>=1 && nwsrv<=3)
        {
            g_nDataConnType = nwsrv;
        }
		pDlg->m_nNwSrv = nwsrv;
        ((CHSDPADlg*)pDlg)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NWSRV, (LPARAM)nwsrv);
//		((CHSDPADlg*)pDlg)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NSRVLN, pDlg->m_SrvStatus);
#if 0
        //拨号上网中来电或通话网络侧挂断不送提示，但数据业务恢复
        if(pDlg->m_pInternetDlg->m_blDataServicing
            && (pDlg->m_bInComCall || pDlg->m_pCallDlg->m_blIsInCalling)
            && (nwsrv>=1 && nwsrv<=3))
        {
            pDlg->m_bInComCall = FALSE; 

            if(pDlg->m_pCallPopDlg->IsPopUp() && pDlg->m_pCallPopDlg->GetSafeHwnd())
            {
                pDlg->m_nRingTimes = 0;
                ::PostMessage(pDlg->m_pCallPopDlg->GetSafeHwnd(),WM_POPDLG_DESTROY,0,0);  
            }
	        ::PostMessage(pDlg->m_pCallDlg->GetSafeHwnd(),WM_ATCDVRESULT,0,0);
        }
#endif
    }
}

void CHSDPADlg::AtRespROAMCHG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!memcmp((const TCHAR*)strArr[0], gc_dsatResCodeTbl[DSAT_ROAMCHG][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_ROAMCHG][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_ROAMCHG][gc_dsatmode]);
        ((CHSDPADlg*)pWnd)->m_RoamStatus = (EnRoamStatus)atoi(ptr);
        ASSERT(((CHSDPADlg*)pWnd)->m_RoamStatus >= ROAM_OFF 
            && ((CHSDPADlg*)pWnd)->m_RoamStatus <= ROAM_MAX);
		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_ROAM, ((CHSDPADlg*)pWnd)->m_RoamStatus);
    }
}

//Current PLMN: "China Mobile Com"
void CHSDPADlg::AtRespPLMNCHG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_PLMNCHG][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_PLMNCHG][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_PLMNCHG][gc_dsatmode]);
        ptr[strlen(ptr)-1] = 0;
        ptr++;

        CString strPLMN = UCS2ToGB(ptr);
		
        if(strlen(ptr) == 0)
            memset(((CHSDPADlg*)pWnd)->m_szPLMN, 0x00, sizeof(((CHSDPADlg*)pWnd)->m_szPLMN));
        else
            wcsncpy(((CHSDPADlg*)pWnd)->m_szPLMN, strPLMN, sizeof(((CHSDPADlg*)pWnd)->m_szPLMN)/2);
		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 1);
    }
}

void CHSDPADlg::AtRespSRVSTATUSCHG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_SRVSTATUSCHG][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_SRVSTATUSCHG][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_SRVSTATUSCHG][gc_dsatmode]);
        pDlg->m_SrvStatus = (EnSrvStatus)atoi(ptr);
        ASSERT(pDlg->m_SrvStatus >= SRVSTATUS_NONE && pDlg->m_SrvStatus < SRVSTATUS_MAX);
		pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN2, pDlg->m_SrvStatus);
//		pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN3, pDlg->m_SrvStatus);
    	pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, pDlg->m_NetworkType);//NETWORK SERVICE
		pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NWSRV, pDlg->m_nNwSrv);//NETWORK SERVICE
		pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 1);  //PLMN
		pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PS, pDlg->m_bPS);  //PS
		pDlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_ROAM, pDlg->m_RoamStatus);//ROAM

    }
}

//接受到状态报告的消息
//+CDS: 2,10,"+358501234567",145,"95/07/04/13:12:14+04","95/07/04/13:12:20+04",0 (status report of successful message delivery received)
/* +CDS: "15335151066",,"09/01/06,16:14:58+00",4,8
53D190016210529F */
void CHSDPADlg::AtRespCDS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
	
    int cnt = 0;
    char *ptr[4], *p;
    p = (char*)(strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_CDS][gc_dsatmode]));
    ptr[0] = ptr[1] = ptr[2] = ptr[3] = 0;
    
    BOOL bOutQuot = TRUE;
    
    while(*p != '\0')
    {
        if(*p == '\"')
        {   
            if(bOutQuot && cnt < 4)
                ptr[cnt++] = p;
            
            bOutQuot = !bOutQuot;
            p++;
        }
        else if(*p == ',' && bOutQuot)
        {
            *p++ = '\0';
        }
        else
        {
            p++;
        }
    }
    
    for(int i = 0; i < 4; i++)
    {
        if(ptr[i] != 0)
        {
            if(*(ptr[i] + strlen(ptr[i]) - 1) == '\"')
                *(ptr[i] + strlen(ptr[i]) - 1) = '\0';
            if(*ptr[i] == '\"')
                ptr[i]++;
        }
    }
    
    StSmsRecord record;
    memset(&record, 0x00, sizeof(StSmsRecord));
	
  // 	record.m_NoATRspCDS = FALSE;//normal，此标志位置FALSE，表示是发送报告，看过就删，不保存。
	record.m_NoATRspCDS = TRUE;//伊拉克需求，发送报告按正常信息保存在收件箱，所以此项置TRUE。

    record.state = SMS_STATE_MT_NOT_READ;
	//CString ptr0=(char*)ptr[0];
    
    //wcscpy(record.szNumber, ptr0);
	strcpy(record.szNumber, ptr[0]);
    
    int time, scnum, concatenate;
    time = 1; scnum = 0, concatenate = 2;
    
    if(!(ptr[time] && *ptr[time]))
        record.timestamp = COleDateTime::GetCurrentTime();
    else
    {
        if(!GetTimeFromStr((LPCSTR)ptr[time], record.timestamp))
            record.timestamp = COleDateTime::GetCurrentTime();
    }
    //Modified by Zhou Bin 2008.12.30
	//	CString ptrsc=(char*)(ptr[scnum]);
    if(!(ptr[scnum] && *ptr[scnum]))
        memset(record.szSCNumber, 0x00, SMS_SC_NUM_MAX);
    else
        strncpy((char*)record.szSCNumber, ptr[scnum], SMS_SC_NUM_MAX);
	// wcsncpy(record.szSCNumber, ptrsc, SMS_SC_NUM_MAX);
	
    if(ptr[concatenate] && *ptr[concatenate])
    {
        if(ExtractConcatenateSmsPara(ptr[concatenate], &record.nRefCnt, &record.nSeqCnt, &record.nTotalCnt))
            record.flag |= SMS_RECORD_FLAG_CONCATENATE_SEGE;
    }
    
	USES_CONVERSION;
    if(wStrNum == 2)
    {
		//CString strGb = GBToUCS2((char*)strArr[1]);//delete by liub 不知为什么要把GB转换为UCS2格式。
		//CString strGb = UCS2ToGB((CString)((char*)strArr[1]));//add by liub for UCS2转换为GB
		CString strGb = UCS2ToGB(A2W((char*)strArr[1]));//add by liub for UCS2转换为GB
		//wcsncpy(record.szContent, strGb, SMS_CHAR_MAX);
		strncpy(record.szContent, W2A(strGb), SMS_CHAR_MAX);
    }
    else
        memset(record.szContent, 0, SMS_CHAR_MAX*2);
    
	pDlg->RcvNewSmsProc(LOC_PC, record);
    Sleep(100);

}

void CHSDPADlg::AtRespCOPSFormat(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}
void CHSDPADlg::AtRespCOPS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{ 
    AtRespCOPSEx(pWnd, strArr, wStrNum);
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCOPSEx(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{ 
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
	#ifdef FEATURE_HAIER_DSI
       && !memcmp((const char*)strArr[0], "+CSPN:", strlen("+CSPN:")))
	#else
       && !memcmp((const char*)strArr[0], "+COPS: ", strlen("+COPS: ")))
    #endif
    
    {
        int cnt = 0;
		#ifdef FEATURE_HAIER_DSI
        char *ptr = (char*)strArr[0] + strlen("+CSPN:");
		#else
        char *ptr = (char*)strArr[0] + strlen("+COPS: ");
		#endif
        char *p = ptr;
        while(*p)
        {
            if(*p == '\"')
            {
                if(cnt == 0)
                {
                    ptr = ++p;
                    cnt++;
                }
                else if(cnt == 1)
                {
                    *p = 0;
                    break;
                }
            }
            else
                p++;
        }

        CString strPLMN;
		if(cnt != 0)
		{
			strPLMN = UCS2ToGB(ptr);
			if (_T("Unknown") == strPLMN)
			{
				strPLMN = _T(" ");
			}
			#ifndef FEATURE_HAIER_DSI
			else
			{
				TCHAR szWchar[1024] = { 0 };
				ASCHEXToWchar(ptr, szWchar);
				strPLMN = UCS2ToGB(szWchar);
			}
			#endif
		}
		
        if(cnt == 0)
            memset(((CHSDPADlg*)pWnd)->m_szPLMN, 0x00, sizeof(((CHSDPADlg*)pWnd)->m_szPLMN));
        else
            wcsncpy(((CHSDPADlg*)pWnd)->m_szPLMN, strPLMN, sizeof(((CHSDPADlg*)pWnd)->m_szPLMN)/2);
		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 1);
    }
}

void CHSDPADlg::AtRespNWRAT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
       && !memcmp((const char*)strArr[0], "$NWRAT: ", strlen("$NWRAT: ")))
    {
        char *ptr = (char*)strArr[0] + strlen("$NWRAT: ");
        char *p = ptr;
        while(*p)
        {
            if(*p == ',')
            {
                *p = 0;
                break;
            }
            else
                p++;
        }
        ((CHSDPADlg*)pWnd)->m_NetworkType = (EnNetWorkType)atoi(ptr);
        ASSERT(((CHSDPADlg*)pWnd)->m_NetworkType >= NW_TYPE_NOSRV && 
            ((CHSDPADlg*)pWnd)->m_NetworkType < NW_TYPE_MAX);
		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, ((CHSDPADlg*)pWnd)->m_NetworkType);
        ((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NETWORK, ((CHSDPADlg*)pWnd)->m_SrvStatus);
        BYTE nwsrv = 0;
		((CHSDPADlg*)pWnd)->m_NetCallLog = NW_CALLLOG_NOSRV;
        switch(((CHSDPADlg*)pWnd)->m_NetworkType)
        {
		case NW_TYPE_NOSRV:
	    	((CHSDPADlg*)pWnd)->m_SrvStatus = SRVSTATUS_LIMIT;			
        case NW_TYPE_GSM:
            nwsrv = 4;
	    	((CHSDPADlg*)pWnd)->m_NetCallLog = NW_CALLLOG_GSM;
            break;
        case NW_TYPE_WCDMA:
            nwsrv = 5;
			((CHSDPADlg*)pWnd)->m_NetCallLog = NW_CALLLOG_WCDMA;
            break;
		case NW_TYPE_EDGE:
            nwsrv = 2;
			((CHSDPADlg*)pWnd)->m_NetCallLog = NW_CALLLOG_GSM;
            break;			
        }
		((CHSDPADlg*)pWnd)->m_nNwSrv = nwsrv;
        ((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_NWSRV, (LPARAM)nwsrv);
    }
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespROAM(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
       && !memcmp((const char*)strArr[0], "$ROAM: ", strlen("$ROAM: ")))
    {
        char *ptr = (char*)strArr[0] + strlen("$ROAM: ");
        ((CHSDPADlg*)pWnd)->m_RoamStatus = (EnRoamStatus)atoi(ptr);
        ASSERT(((CHSDPADlg*)pWnd)->m_RoamStatus >= ROAM_OFF
            && ((CHSDPADlg*)pWnd)->m_RoamStatus <= ROAM_MAX);
		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_ROAM, ((CHSDPADlg*)pWnd)->m_RoamStatus);
    }
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCSQ(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	int len,valRssi;
	BYTE strRssi[5];
	BYTE *p=strArr[0];

	memset(strRssi,0,5);
#ifdef FEATURE_HAIER_CM
	p = (unsigned char *)strchr((char *)p,':') + 1;
#else
	p = (unsigned char *)strchr((char *)p,' ');
#endif
	len = strchr((char *)p,',')-(char *)p;
	strncpy((char *)strRssi,(char *)p,len);
	valRssi = atoi((char *)strRssi)/6;
    if (valRssi>6) {
        valRssi=6;
    }

	((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_RSSI, valRssi);
    
    if(valRssi == 6)
        ((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 0);
    else
        ((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PLMN, 1);

    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespHandSet(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CString RetStr;
	CHSDPADlg *pdlg=(CHSDPADlg *)pWnd;
	RetStr = strArr[0];
	int nRect;
	if(RetStr.Find(_T("1"),0)!=-1)
	{
		nRect = 1;
	}
	else/* if(RetStr.Find("0",0)!=-1)*/
	{
		nRect = 0;
	}
	pdlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_HANDSET, nRect);
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespATE0(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCLVL(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCSDH(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

/*AT$CPINS?的回调函数*/
#ifdef FEATURE_HAIER_PINMANAGE
void CHSDPADlg::AtRespCPINQS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	//+CPINC:3,10,3,10
	CHSDPADlg* pdlg= (CHSDPADlg*)pWnd;

	CString strRet = strArr[0];
	char* pbuf = (char *)strArr[0];
	int nfrom = 0;
	int nto = 0;
	int originalLen = strRet.GetLength();
	char temp[4];
	
	nfrom = strRet.Find(':') + 1;
	nto = strRet.Find(',');
	memcpy(temp,pbuf + nfrom,(nto - nfrom));

    pdlg->m_cHandlePin.m_nRemainTimes = atoi(temp);

	strRet.Delete(0, nto + 1);
	//AfxMessageBox(strRet);  //debug	
	nfrom = originalLen - strRet.GetLength();
	nto = strRet.Find(',') + nfrom;
	memcpy(temp,pbuf + nfrom,(nto - nfrom));
    pdlg->m_cHandlePin.m_nRemainTimes_puk = atoi(temp);

	if (0 >= pdlg->m_cHandlePin.m_nRemainTimes_puk)
	{
		pdlg->m_cHandlePin.m_nSimStat = CPIN_SIM_DESTROYED;
	}
	SetEvent(pdlg->m_hSyncInitEvt);
}
#else
void CHSDPADlg::AtRespCPINQS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	//$CPINS:1,0,10
	CHSDPADlg* pdlg= (CHSDPADlg*)pWnd;

	CString strRet = strArr[0];
	char* pbuf = (char *)strArr[0];
	int nfrom = 0;
	int nto = 0;
	char temp[4];

	nfrom = strRet.ReverseFind(',') + 1;
	nto = strRet.GetLength();
	memcpy(temp,pbuf + nfrom,(nto - nfrom + 1));

    pdlg->m_cHandlePin.m_nRemainTimes_puk = atoi(temp);

	if (0 >= pdlg->m_cHandlePin.m_nRemainTimes_puk)
	{
		pdlg->m_cHandlePin.m_nSimStat = CPIN_SIM_DESTROYED;
	}

	strRet = strRet.Mid(0,(nfrom - 1));
	//AfxMessageBox(strRet);  //debug	
	nfrom = strRet.ReverseFind(',') + 1;
	nto = strRet.GetLength();
	memcpy(temp,pbuf + nfrom,(nto - nfrom + 1));
    pdlg->m_cHandlePin.m_nRemainTimes = atoi(temp);

	SetEvent(pdlg->m_hSyncInitEvt);
}
#endif

void CHSDPADlg::AtRespCPINQ(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	/*
	struct _tPara {
		CHSDPADlg *pWnd;
		CPinEx *pHandle;
	};*/
	
	CHSDPADlg* pdlg= (CHSDPADlg*)pWnd;
//	CPinEx * pHandle = &(pdlg->m_cHandlePin);
	CString strRet=strArr[0];

    //需PIN码验证的提示有：
    const TCHAR *szPinLock[] = {
            _T("SIM PIN"), 
            _T("PH-SIM PIN"),  
            _T("PH-NET PIN"),  
            _T("PH-NETSUB PIN"),  
            _T("PH-SP PIN"),  
            _T("PH-CORP PIN"),  
            _T("PH-FSIM PIN")
    };
    
    //需PUK解锁的提示有：
    const TCHAR* szPukLock[] = {
            _T("SIM PUK"), 
            _T("PH-SIM PUK"), 
            _T("PH-NET PUK"), 
            _T("PH-NETSUB PUK"), 
            _T("PH-SP PUK"), 
            _T("PH-CORP PUK"), 
            _T("PH-FSIM PUK")
    };
    
    //查询SIM状态返回
	if (strRet.Find(_T("READY"),0) != -1) 
	{
		pdlg->m_cHandlePin.m_nSimStat = CPIN_SIM_NONE_REQUIRED;
		pdlg->m_cHandlePin.m_nPinStat = 0;
    }

	else if(strRet.Find(_T("PH-NET PIN"),0)!=-1)
	{
		pdlg->m_cHandlePin.m_nSimStat = CME_SIM_NONE_INVALID;
	}

	else if (strRet.Find(_T("SIM busy"),0)!=-1) 
	{
		pdlg->m_cHandlePin.m_nSimStat = CME_SIM_BUSY;
		//pHandle->m_nPinStat = 0;
    }
	else if (strRet.Find(_T("SIM failure"),0)!=-1) 
	{
		pdlg->m_cHandlePin.m_nSimStat = CME_SIM_FAILURE;
		//pHandle->m_nPinStat = 0;
    }
		else if (strRet.Find(_T("PH-NET PIN"),0) != -1 )
	{
		pdlg->m_cHandlePin.m_nSimStat = CME_SIM_NONE_INVALID;
	}
    else
    {
        int i;
        for(i=0; i<sizeof(szPinLock)/sizeof(TCHAR *);i++)
        {
            if(strRet.Find(szPinLock[i],0) != -1)
            {
		        pdlg->m_cHandlePin.m_nSimStat = CPIN_SIM_PIN_REQUIRED;
		        pdlg->m_cHandlePin.m_nPinStat = 1;
                i = -1;
                break;
            }   
        }
        
        if(i!=-1)
        {
            for(i=0; i<sizeof(szPukLock)/sizeof(TCHAR *);i++)
            {
                if(strRet.Find(szPukLock[i],0) != -1)
                {
		            pdlg->m_cHandlePin.m_nSimStat = CPIN_SIM_PUK_REQUIRED;
		            //pdlg->m_cHandlePin.m_nPinStat = 1;
                    break;
                } 
            }
        }
    }

	::SetEvent(pdlg->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCLCKsc2(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pdlg = (CHSDPADlg*)pWnd;
	CString RetStr;
	RetStr = strArr[0];
	
	if(RetStr.Find(_T("1"),0)!=-1)
	{
		pdlg->m_bPinProtected = TRUE;
	}
	else if (RetStr.Find(_T("0"),0)!=-1) {
		pdlg->m_bPinProtected = FALSE;
	}
	pdlg->PostMessage(WM_PIN_VERIFY,0,0);
}

void CHSDPADlg::OnTimer(UINT nIDEvent) 
{
    if(IDT_USB_CONN_DETECT == nIDEvent)
    {
        CHSDPAApp *pApp = (CHSDPAApp*)AfxGetApp();
        memset(pApp->m_szDevName, 0, sizeof(pApp->m_szDevName));
        
        if(GetPCUIPortName(pApp->m_szDevName)
            && m_pComm->StartPort(pApp->m_szDevName))
        {
            KillTimer(IDT_USB_CONN_DETECT);
            ShowWindow(SW_SHOWMINIMIZED);
            SyncInitFunc();
        }
    }
    else if(IDT_QCMTI_TIMEOUT == nIDEvent)
    {
        m_bInComSms = FALSE; 
        ::SetEvent(g_BGReadNewSmsEvt);
    }
    else if(IDT_RING_TIMEOUT == nIDEvent)
    {
        KillTimer(IDT_RING_TIMEOUT);
        m_nRingTimes = 0;
        m_pCallPopDlg->DestroyWindow();
    }
    else if(IDT_SMS_TIMEOUT == nIDEvent)
    {
        KillTimer(IDT_SMS_TIMEOUT);
        m_pSmsPopDlg->DestroyWindow();
    }
    else if(nIDEvent == IDT_QCMGR_TIMEOUT || nIDEvent == IDT_QCMGD_TIMEOUT)
    {
        KillTimer(nIDEvent);
        ProgressClose();
        UpdateSmsDlg();
    }
	else if(nIDEvent >= IDT_RCVCONCSMS_TIMEOUT_BEGIN && nIDEvent < IDT_RCVCONCSMS_TIMEOUT_END)
	{
		KillTimer(nIDEvent);
	//	ProcSmsRcvConcTimeout(nIDEvent);
		ProcSmsRcvConcTimeout_Flexi(nIDEvent);
	}
	else if(nIDEvent = IDT_TIMER_COPSINIT)
	{
		AtSndCOPS();
		KillTimer(nIDEvent);
		if (wcslen(m_szPLMN) == 0)
		{
			SetTimer(IDT_TIMER_COPSINIT,5000,NULL);
		}
		else if (m_pConnectDlg->m_pConnTipWait)
		{
			m_pConnectDlg->m_pConnTipWait->SendMessage(WM_CLOSE, 0, 0);
		//	PostMessage(WM_ICON_UPDATE, ICON_TYPE_RSSI, m_nRSSI);
			Sleep(2000);
			m_pConnectDlg->OnClickConnectButton();
		}
	}

    CBaseDialog::OnTimer(nIDEvent);
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCSMSS()
{
    char szAtBuf[20] = {0};
#ifdef FEATURE_HAIER_SMS
    strcpy(szAtBuf, "AT+VSPST?\r");
#else
    strcpy(szAtBuf, "AT$CSMSS?\r");
#endif
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespQCSMSS, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncSmsInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndSICLOCK()
{
#ifdef FEATURE_HAIER_ADAPTER
	m_nSimLock = 1;
	return SYNCINITFUNCRET_DONE;
#endif
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$SIDLOCK?\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespQSIDLOCK, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncSmsInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

//AtSndPCONLINE
EnSyncInitFuncRetType CHSDPADlg::AtSndPCONLINE()
{
#ifdef FEATURE_HAIER_ADAPTER
	return SYNCINITFUNCRET_DONE;
#endif
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$PCONLINE=1\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespQPCONLINE, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncPconlineInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

//AtSndOPCONLINE
EnSyncInitFuncRetType CHSDPADlg::AtSndOPCONLINE()
{
#ifdef FEATURE_HAIER_ADAPTER
	return SYNCINITFUNCRET_DONE;
#endif
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$PCONLINE=0\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespQPCONLINE, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncPconlineInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCPBSS()
{
    char szAtBuf[20] = {0};
#ifdef FEATURE_HAIER_SMS
    strcpy(szAtBuf, "AT+VSPST?\r");
#else
    strcpy(szAtBuf, "AT$CPBSS?\r");
#endif
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespQCPBSS, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncPbmInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCOPSFormat()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+COPS=3,1\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCOPSFormat, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}
EnSyncInitFuncRetType CHSDPADlg::AtSndCOPS()
{
    char szAtBuf[20] = {0};
	#ifdef FEATURE_HAIER_DSI
    strcpy(szAtBuf, "AT+CSPN?\r");
	#else
    strcpy(szAtBuf, "AT+COPS?\r");
	#endif
	BOOL bResponse = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->CommIsReady();
	if (!bResponse)
	{
		return SYNCINITFUNCRET_SND_ERR;
	}
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCOPS, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

BOOL CHSDPADlg::AtSndCOPSEx()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+COPS?\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCOPSEx, (LPVOID)this);
        return TRUE;
    }
    else
        return FALSE;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndNWRAT()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$NWRAT?\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespNWRAT, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndROAM()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$ROAM?\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespROAM, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCSQ()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+CSQ\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCSQ, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndHANDSET()
{
#ifdef FEATURE_HAIER_ADAPTER
	return SYNCINITFUNCRET_DONE;
#endif
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$HANDSET?\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespHandSet, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

LRESULT CHSDPADlg::PinVerify(WPARAM wParam, LPARAM lParam)
{
	if (m_bPinProtected) {
		CDlgSignIn dlgSignIn(this);
		dlgSignIn.SetFocus();
		int nRect = dlgSignIn.DoModal();
		/*
		if(IDOK == nRect)
		{
			
			
		}
		else if(nRect == IDCANCEL){
			AfxMessageBox("Quit");
			OnCancel();
		}
		*/

	}
	else{

	}
	return 0;
	
}

LRESULT CHSDPADlg::OnInitSMSAndPB(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		SyncInitFunc();
	}
	else						//未插入数据卡
	{
		m_pSetupDlg->EnableButton(FALSE);
		m_pCallDlg->EnableButton(FALSE);
		m_pCallDlg->EnableButton(FALSE, FALSE);
	}
	m_pSmsDlg->PostMessage(WM_NO_MODEM_FOR_SMS,0,0);
	m_pConnectDlg->PostMessage(WM_MODEM_IN_OUT, 0, 0);
	m_pPhoneBookDlg->PostMessage(WM_MODEM_IN_OUT, 0, 0);

	return NULL;
}

LRESULT CHSDPADlg::OnTrayNotify(WPARAM wParam,LPARAM lParam)
{
    //wParam接收的是图标的ID，而lParam接收的是鼠标的行为
    
    if(wParam!=IDI_ICON_INFO)
        return 1;
    
	CMenu menu;
    menu.LoadMenu(IDR_MENU_TRAY);
	CString str;
	CString temp;

    switch(lParam)
    {
    case WM_RBUTTONUP://右键起来时弹出快捷菜单
        {
            LPPOINT lpoint=new tagPOINT;
            ::GetCursorPos(lpoint);//得到鼠标位置
			switch(m_skinWin.m_winstate)
			{
			case 0:
			case 1:
				str.LoadString(IDS_TRAY_TOTRAY);
				break;
			case 2:
			default:
				temp.LoadString(IDS_TRAY_OPENDLG);
				str.Format(temp, g_SetData.Main_szPrefixName);
				break;
			}
			menu.GetSubMenu(0)->ModifyMenu(0, MF_BYPOSITION,
				ID_APP_TRAY_MAXI, str);
            menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,
				lpoint->x, lpoint->y, this);
            //资源回收
            HMENU hmenu=menu.Detach();
            menu.DestroyMenu();
            delete lpoint;
        }
        break;
        
    case WM_LBUTTONDBLCLK://双击左键的处理
        {
            this->ShowWindow(SW_SHOW);//显示主窗口
            if(m_bWinSize)
			{
				m_skinWin.m_winstate = 0;
			}
			else
			{
				m_skinWin.m_winstate = 1;
			}
			m_skinWin.OnNcPaint(0);
            switch(m_skinWin.m_winstate)
			{
			case 0:
			case 1:
				str.LoadString(IDS_TRAY_TOTRAY);
				break;
			case 2:
			default:
				temp.LoadString(IDS_TRAY_OPENDLG);
				str.Format(temp, g_SetData.Main_szPrefixName);
				break;
			}
			menu.GetSubMenu(0)->ModifyMenu(0, MF_BYPOSITION,
				ID_APP_TRAY_MAXI, str);
        }
        break;
    }
    return 0;
}

LRESULT CHSDPADlg::PbReDraw(WPARAM wParam, LPARAM lParam)
{
    ::SendMessage(m_pPhoneBookDlg->GetSafeHwnd(), WM_PB_ReDrawListCtrl, wParam, lParam);
        
    return 0;
}

LRESULT CHSDPADlg::CheckPinStat(WPARAM wParam, LPARAM lParam)
{
	//关回显后，查询SIM的状态
	const char ATCLCKsc2[]="AT+CLCK=\"SC\",2\x0d\x00";
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCLCKsc2);
	
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf))){		
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCLCKsc2, (LPVOID)this);			
	}
	return 0;
}

void CHSDPADlg::RegisterDsAutoMsgRsp()
{
    RegisterAtRespFunc(ATRESP_RING, AtRespRing, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_NO_CARRIER, AtRespNoCarrier, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_DIALTONE, AtRespDialTone, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_CMTI, AtRespCMTI, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_CMT, AtRespCMT, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_RSSI, AtRespRSSI, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_CSMSS, AtRespCSMSS, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_CPBSS, AtRespCPBSS, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_CLIP, AtRespCLIP, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_NWCHG, AtRespNWCHG, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_NWSRVCHG, AtRespNWSRVCHG, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_ROAMCHG, AtRespROAMCHG, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_HANDSETCHG, AtRespHandSetCHG, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_SIMREADY, AtRespSimReady, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_PLMNCHG, AtRespPLMNCHG, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_SRVSTATUSCHG, AtRespSRVSTATUSCHG, (LPVOID)this);
    RegisterAtRespFunc(ATRESP_CDS, AtRespCDS, (LPVOID)this);
	RegisterAtRespFunc(ATRESP_RVMFB, AtRespRVMFB, (LPVOID)this);//TATA 需求 add by liub//add by liub for voicemail
	RegisterAtRespFunc(ATRESP_RVMFBUPDATA, AtRespRVMFBUPDATA, (LPVOID)this);//TATA 需求 add by liub//add by liub for voicemail
	RegisterAtRespFunc(ATRESP_PS, AtRespPS, (LPVOID)this);
	RegisterAtRespFunc(ATRESP_HVPRIV, AtRespHVPRIV, (LPVOID)this);
	RegisterAtRespFunc(ATRESP_SIDLOCK, AtRespSIDLOCK, (LPVOID)this);
    ::SetEvent(g_AppRegEvt);
}

void CHSDPADlg::OnAppAbout() 
{
	// TODO: Add your command handler code here
    CAboutDialog  dlgAbout;

	dlgAbout.m_strFWVersion=m_strFWVersion;
	dlgAbout.m_strIMEI=m_strIMEI;

    dlgAbout.DoModal();
}

void CHSDPADlg::OnAppTrayMaxi() 
{
	// TODO: Add your command handler code here
	switch(m_skinWin.m_winstate)
	{
	case 0:
	case 1:
		this->ShowWindow(SW_HIDE);
		m_skinWin.m_winstate=2;
		break;
	case 2:
		this->ShowWindow(SW_SHOW);
		if(m_bWinSize)
		{
			m_skinWin.m_winstate = 0;
		}
		else
		{
			m_skinWin.m_winstate = 1;
		}
		m_skinWin.OnNcPaint(0);
		break;
	default:
		break;
	}
}

EnSyncInitFuncRetType CHSDPADlg::AtSndATE0()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "ATE0\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespATE0, (LPVOID)this);

        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 8000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCLVL()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+CLVL=2\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCLVL, (LPVOID)this);

        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCSDH()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+CSDH=1\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCSDH, (LPVOID)this);

        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCPINQ()
{
	const char ATCPINA[]="AT+CPIN?\x0d\x00";
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCPINA);
	
	ASSERT(m_pComm);
	if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
	{
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPINQ, this);

		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
		{
			return SYNCINITFUNCRET_DONE;
		}
		else
		{
			return SYNCINITFUNCRET_RSP_TO;
		}
	}
	else
	{
		 return SYNCINITFUNCRET_SND_ERR;
	}
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCPINQS()
{
#ifdef FEATURE_HAIER_PINMANAGE
	const char ATCPINA[]="AT+CPINC?\x0d\x00";
#else
	const char ATCPINA[]="AT$CPINS?\x0d\x00";
#endif
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCPINA);
	ASSERT(m_pComm);

    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPINQS, this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
	}
    else
	    return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCSCA()
{
    char szAtBuf[50] = {0};

    sprintf(szAtBuf, "AT+CSCA?\r");

    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCSCA, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCMGF(EnSmsMode mode)
{
    ASSERT(mode == SMS_MODE_PDU || mode == SMS_MODE_TEXT);

    char szAtBuf[20] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGF], mode);
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCMGF, this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCNMI(int para1, int para2)
{
    char szAtBuf[20] = {0};

    sprintf(szAtBuf, "%s%d,%d,,1\r", gcstrAtSms[AT_SMS_QCNMI], para1, para2);

    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCNMI, this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	memset(gSmsCentreNum, 0x00, sizeof(gSmsCentreNum));

	if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
	{
        char *q;
		char *ptr = (char*)strArr[0] + strlen("+CSCA: \"");
		q = ptr;
        while(*q)
        {
            if(*q == '\"')
            {
                *q = 0;
                break;
            }
            else
                q++;
        }
		CString szptr=(char*)ptr;
		wcsncpy(gSmsCentreNum, szptr, SMS_SC_NUM_MAX);
	}

    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCMGF(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespCNMI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
//    AfxMessageBox("AtRespCNMI");
}

BOOL CHSDPADlg::SyncInitFunc(int nStatus)
{
    BYTE cnt = 0;
    BOOL res = TRUE;
    CString str;
    EnSyncInitFuncRetType InitType = SYNCINITFUNCRET_DONE;

    EnterCriticalSection(&m_csSyncInitMask);
    m_bSyncInitMask = TRUE;
    LeaveCriticalSection(&m_csSyncInitMask);

    //初始化时不接收来电来短信
    ::ResetEvent(g_BGPassEvt); 
	
	PreMsg *PreMsgDlg = new PreMsg;
	ASSERT(PreMsgDlg);
	PreMsgDlg->Create(PreMsgDlg->IDD);
	
	str.LoadString(IDS_INIT_SEARCH_DEVICE);
    PreMsgDlg->SetText(str);

	EnOpenPortRet OpenRet = theApp.OpenPort(FALSE, 1, 1000);
	if(OpenRet != OPENPORT_RET_SUCC)
	{
		if (OpenRet == OPENPORT_RET_FAIL && nStatus == 1)
		{
		}
		else
		{
			str.LoadString(IDS_USB_START_FAIL);
			PreMsgDlg->SetText(str);
			Sleep(500);
			PreMsgDlg->DestroyWindow();
			delete PreMsgDlg;
			PreMsgDlg = NULL;
			return FALSE;
		}
	}
	else
	{
		str.LoadString(IDS_USB_START_SUCC);
		PreMsgDlg->SetText(str);
		Sleep(200);
    }

#ifdef FEATURE_VERSION_NOSIM
#else
    str.LoadString(IDS_CHECK_SIM_STATUS);
    PreMsgDlg->SetText(str);
#endif

#ifdef FEATURE_ATTEST_SUPPORT
	CStdioFile file;
	file.Open("AtDebug.log", CFile::modeCreate);
	file.Close();
#endif

#ifdef FEATURE_ATTEST_SUPPORT
	file.Open("Sms.log", CFile::modeCreate);
	file.Close();
#endif

    //发送关闭回显的AT命令
    cnt = 0;
    do {
        cnt++;
    	InitType = AtSndATE0();
        if(InitType == SYNCINITFUNCRET_RSP_TO)
        {
            DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
            m_pComm->SetSerialState(SERIAL_STATE_CMD);
        }
    } while(cnt < 10 && InitType != SYNCINITFUNCRET_DONE);

    if(InitType != SYNCINITFUNCRET_DONE)
        res = FALSE;

	 if(res)
	 {
		 if(SYNCINITFUNCRET_DONE != (InitType = SndAtPowerCFUN(POWER_ON)))
		 {
			 if(InitType == SYNCINITFUNCRET_RSP_TO)
			 {
				 DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
				 m_pComm->SetSerialState(SERIAL_STATE_CMD);
			 }
			 res = FALSE;
		 }
    }

    //发送命令将通话音量设置为2
    if(res && g_SetData.Main_nCall)
    {
        if(SYNCINITFUNCRET_DONE != (InitType = AtSndCLVL()))
        {
            if(InitType == SYNCINITFUNCRET_RSP_TO)
            {
                DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
                m_pComm->SetSerialState(SERIAL_STATE_CMD);
            }
            res = FALSE;
        }
    }

#ifdef FEATURE_VERSION_NOSIM
#else
    //发送PIN校验的AT命令
	int bSimlock = 1;
//m_bSimReady = TRUE;
	while(bSimlock)
	{
		bSimlock--;
		if(res && SYNCINITFUNCRET_DONE == (InitType = AtSndCPINQ()))
		{
			if((m_cHandlePin.m_nSimStat == CME_SIM_FAILURE) || (m_cHandlePin.m_nSimStat == CME_SIM_NONE_INVALID))
			{
				res = FALSE;
			}
			else if(m_cHandlePin.m_nSimStat == CME_SIM_BUSY)
			{
				for(cnt=0;cnt<30;cnt++)
				{
					Sleep(1000);
					if(SYNCINITFUNCRET_DONE == (InitType = AtSndCPINQ()))
					{
						if(m_cHandlePin.m_nSimStat != CME_SIM_BUSY)
							break;
					}
					else
					{
						if(InitType == SYNCINITFUNCRET_RSP_TO)
						{
							DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
							m_pComm->SetSerialState(SERIAL_STATE_CMD);
						}
						res = FALSE;
						break;
					}
				}
				
				if(cnt == 30)
					res = FALSE;
			}

			if(res)
			{
				if(SYNCINITFUNCRET_DONE != (InitType =AtSndCPINQS()))
				{
					if(InitType == SYNCINITFUNCRET_RSP_TO)
					{
						DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
						m_pComm->SetSerialState(SERIAL_STATE_CMD);
					}
					res = FALSE;
				}
			}

			//对SIM卡和PIN状态进行校验，
			bool bSimStatus = false;  //用于SIM卡在PIN码锁的情况下直接进入PUK解锁状态。
			if(res)
			{
				while(!bSimStatus)
				{
					switch (m_cHandlePin.m_nSimStat)
					{
					case CPIN_SIM_PIN_REQUIRED:  //需要进行PIN码验证
						{
							if (m_cHandlePin.m_nPinStat == 1)
							{
								CDlgSignIn* dlgSI;
								dlgSI = new CDlgSignIn(PreMsgDlg);
								int nResp = dlgSI->DoModal();
								
								if(nResp == IDOK)
								{
									bSimlock++; 
									if (CPIN_SIM_PUK_REQUIRED != m_cHandlePin.m_nSimStat)
									{
										res = TRUE;
										bSimStatus = true;
									}									
								}
								else if (nResp == IDCANCEL)
								{
									res = FALSE;
									bSimStatus = true;
								}
								
								MSG msg;
								while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
								{
									if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
										break;
									DispatchMessage(&msg);
								}

								Sleep(1000);
								
								if (NULL != dlgSI)
								{
									delete dlgSI;
								}
							}
							
							break;
						}
						
					case CPIN_SIM_PUK_REQUIRED:    //需要使用PUK码来解锁
						{
							CModifyPinDlg* ModifyPindlg;
							ModifyPindlg = new CModifyPinDlg(PreMsgDlg,1);
							int nResp = ModifyPindlg->DoModal();
							
							if(nResp == IDOK)
							{
								res = TRUE;
								bSimStatus = true;
							}
							else if (nResp == IDCANCEL)
							{
								res = FALSE;
								bSimStatus = true;
							}
							if (NULL != ModifyPindlg)
							{
								delete ModifyPindlg;
							}
							
							break;
						}
						
					case CPIN_SIM_NONE_REQUIRED:   //PIN码验证未打开，暂保留
						{
							bSimStatus = true;
							break;
						}
						
					case CPIN_SIM_DESTROYED:    
						{
							bSimStatus = true;
							res = FALSE;
							
							break;
						}

					default:
						{
							bSimStatus = true;
							res = FALSE;
						}
					} // switch
				}  
			}
		}
		else
		{
			if(InitType == SYNCINITFUNCRET_RSP_TO)
			{
				DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
				m_pComm->SetSerialState(SERIAL_STATE_CMD);
			}
			res = FALSE;
		}
	
		if(!res)
		{
			m_bSimReady = FALSE;
        
			if (m_cHandlePin.m_nSimStat == CME_SIM_NONE_INVALID)
			{
				str.LoadString(IDS_SIMLOCK);
				PreMsgDlg->SetText(str);
				Sleep(2000);
			}
			else if(!g_SetData.Main_nCall)
			{
				//AfxMessageBox(IDS_SIM_NOT_INSERT);
			}
			else
			{
				//str.LoadString(IDS_INT_CONN_SIMNOTREADY);	
			}
		}
		else
		{
			m_bSimReady = TRUE;
		}	
	}

	if(res && g_SetData.Main_nSimLock)
    {		
        AtSndSICLOCK();
		if(m_nSimLock != 1)
		{
			int i=0;
			
			while (WAIT_TIMEOUT == WaitForSingleObject(m_hSimLockEvt, 200))
			{
				if (i%3 == 0)
				{
					str.LoadString(IDS_CHECK_SIM_STATUS);
					str += _T(".");
					PreMsgDlg->SetText(str);
				}
				else if (i%3 == 1)
				{
					str.LoadString(IDS_CHECK_SIM_STATUS);
					str += _T("..");
					PreMsgDlg->SetText(str);
				}
				else
				{
					str.LoadString(IDS_CHECK_SIM_STATUS);
					str += _T("...");
					PreMsgDlg->SetText(str);
				}
				
				MSG msg;
				while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
				{
					if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
						break;
					DispatchMessage(&msg);
				}
				i++;
				
				if (i == SYNCINIT_TIMEOUT_LONG/200)
				{
					str.LoadString(IDS_TIMEOUT_SIM_INIT);
					PreMsgDlg->SetText(str);
					Sleep(500);
					break;
				}
			}
			if (m_nSimLock != 1)
			{
				res = FALSE;
				m_bSimReady = FALSE;
				str.LoadString(IDS_SIM_LOCK_FLEXI);
				PreMsgDlg->SetText(str);
				Sleep(2000);
			}
		}
	}

#endif //NOSIM

    //等待SMS初始化完成
    if(res)
    {
        str.LoadString(IDS_CHECK_SMS_STATUS);
        PreMsgDlg->SetText(str);

        AtSndCSMSS();
		m_bSMSS=1;//wj test
		if(!m_bSMSS)				//wyw_0121 modify
		{
			int i=0;
			
			while (WAIT_TIMEOUT == WaitForSingleObject(m_hSyncSmsInitEvt, 200))
			{
				if (i%3 == 0)
				{
					str.LoadString(IDS_CHECK_SMS_STATUS);
					str += _T(".");
					PreMsgDlg->SetText(str);
				}
				else if (i%3 == 1)
				{
					str.LoadString(IDS_CHECK_SMS_STATUS);
					str += _T("..");
					PreMsgDlg->SetText(str);
				}
				else
				{
					str.LoadString(IDS_CHECK_SMS_STATUS);
					str += _T("...");
					PreMsgDlg->SetText(str);
				}
				
				MSG msg;
				while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
				{
					if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
						break;
					DispatchMessage(&msg);
				}
				i++;
				
				if (i == SYNCINIT_TIMEOUT_LONG/200)
				{
					res = FALSE;
					str.LoadString(IDS_TIMEOUT_SMS_INIT);
					PreMsgDlg->SetText(str);
					Sleep(500);
					break;
				}
			}
		}
	}

    //等待PBM初始化完成
    if(res)
    {
        str.LoadString(IDS_CHECK_PBM_STATUS);
        PreMsgDlg->SetText(str);
		
        AtSndCPBSS();
		m_bPBSS=1;
        if(!m_bPBSS)
            if(WAIT_TIMEOUT == WaitForSingleObject(m_hSyncPbmInitEvt, SYNCINIT_TIMEOUT_LONG))
            {
                res = FALSE;
                str.LoadString(IDS_TIMEOUT_PBM_INIT);
                PreMsgDlg->SetText(str);
                Sleep(500);
            }
    }

#ifndef _DEBUG
    str.LoadString(IDS_CHECK_DATACARD_STATUS);
    PreMsgDlg->SetText(str);
#endif  
    //发送网络,信号查询和短消息,字符集配置的AT命令
    #if 1
    cnt = SYNCINITFUNCID_HANDSET;
    while(InitType == SYNCINITFUNCRET_DONE 
            && m_pSyncFuncTbl[cnt] != NULL && cnt < SYNCINITFUNCID_MAX)
    {
#ifdef _DEBUG
        PreMsgDlg->SetText(m_szSyncInitInfo[cnt][0]);
        Sleep(200);
#else
		//wyw_0120
		MSG msg;
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

        Sleep(50);
#endif
        
        InitType = (this->*m_pSyncFuncTbl[cnt])();
        switch(InitType)
        {
        case SYNCINITFUNCRET_DONE:
		     break;
        case SYNCINITFUNCRET_RSP_TO:
            DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
            m_pComm->SetSerialState(SERIAL_STATE_CMD);
        case SYNCINITFUNCRET_SND_ERR:
            res = FALSE;
            PreMsgDlg->SetText(m_szSyncInitInfo[cnt][1]);
            Sleep(500);
            break;
        }
        cnt++;
    }
	#endif
    PreMsgDlg->DestroyWindow();
    delete PreMsgDlg;
	PreMsgDlg = NULL;
	

   AtSendCVMI();

#ifdef FEATURE_VERSION_NOSIM
   m_cHandlePin.m_nSimStat = CPIN_SIM_NONE_REQUIRED;
#endif

	if (m_bSimReady)
	{
		m_pPhoneBookDlg->ReadDataFromSimOrUSB(0); //从USB Modem读取电话本记录
		m_pPhoneBookDlg->UpdateContactNumForTreeCtrl(m_pPhoneBookDlg->m_ht_PCCard,1);
	}
	
#ifndef FEATURE_VERSION_NOSIM
	if (m_bSimReady)
	{
		m_pPhoneBookDlg->ReadDataFromSimOrUSB(1);//从SIM卡读取电话本记录
		m_pPhoneBookDlg->UpdateContactNumForTreeCtrl(m_pPhoneBookDlg->m_ht_USIM,2);
	}
#endif
	
	Sleep(200);
	
	if(g_SetData.Setup_nAutoConnect)
	{  
#ifndef FEATURE_VERSION_NOSIM
//		if (m_bSimReady)
#endif
	//		m_pConnectDlg->OnClickConnectButton();
	}

#ifndef FEATURE_VERSION_NOSIM
	if (!m_bSimReady)			//未插入SIM卡
	{
		m_pSetupDlg->EnableButton(FALSE);
		m_pCallDlg->EnableButton(FALSE);
		m_pCallDlg->EnableButton(FALSE, FALSE);
		g_bSimReady=FALSE;

		AfxMessageBox(IDS_SIM_NOT_INSERT);

		SndAtPowerCFUN(POWER_OFF);
		Sleep(200);
	}
	else
#endif
	{
		m_pSmsDlg->OnInitSMSRecord();
		m_pSetupDlg->EnableButton(TRUE);
		m_pCallDlg->EnableButton(TRUE);
		m_pCallDlg->EnableButton(TRUE, FALSE);
		g_bSimReady=TRUE;
		if (wcslen(m_szPLMN) == 0)
		{
			SetTimer(IDT_TIMER_COPSINIT, 5000, NULL);
 		}
	}

    //可以接收来电来短信
    ::SetEvent(g_BGPassEvt);
    
    EnterCriticalSection(&m_csSyncInitMask);
    m_bSyncInitMask = FALSE; 
    LeaveCriticalSection(&m_csSyncInitMask);
	b_start = 1;//初始化完成
    return res;
}


/*
EnSyncInitFuncRetType CHSDPADlg::AtSndCSCA()
{
    return (AtSndCSCA(NULL));
}
*/


EnSyncInitFuncRetType CHSDPADlg::AtSndCMGF()
{
    return (AtSndCMGF(gSmsMode));
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCNMI()
{
    if(g_SetData.Messages_nPreStor == SMS_PRESTOR_PC)
        return (AtSndCNMI(1,2));
    else
        return (AtSndCNMI(2,1));
}

void CHSDPADlg::ResetSyncData()
{
    m_bSimReady = FALSE;
    m_NetworkType = NW_TYPE_NOSRV;
    m_RoamStatus = ROAM_OFF;
    m_SrvStatus = SRVSTATUS_FULL;
	m_NetCallLog = NW_CALLLOG_NOSRV; 
    m_bSMSS = FALSE; 
    m_bPBSS = FALSE;
	m_bPS = FALSE; 
	m_nNwSrv = -1;
    memset(m_szPLMN, 0x00, sizeof(m_szPLMN));
	m_nSimLock = 0;
}

LRESULT CHSDPADlg::OnSmsFullNotify(WPARAM wParam, LPARAM lParam)
{

/*
    ::ResetEvent(g_BGPassEvt);

    if((SM_SmsNum > 0)
        && (IDOK == AfxMessageBox(IDS_SMS_MOVE_TIP, MB_OKCANCEL | MB_ICONQUESTION)))
    {
        if(SYNCINITFUNCRET_DONE == AtSndCPMS())
        {
            ProgressOpen();
            PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCPMS, (LPARAM)TRUE);            
        }
        return 0;
    }
    
    ::SetEvent(g_BGPassEvt);
    switch(m_SmsFullState)
    {
    case SMS_FULL_UNKNOWN:
        break;
    case SMS_FULL_ME:
        AfxMessageBox(IDS_SMS_ME_FULL, MB_OK | MB_ICONINFORMATION);
        break;
    case SMS_FULL_SM:
        AfxMessageBox(IDS_SMS_USIM_FULL, MB_OK | MB_ICONINFORMATION);
        break;
    case SMS_FULL_BOTH:
        AfxMessageBox(IDS_SMS_MEUSIM_FULL, MB_OK | MB_ICONINFORMATION);
        break;
    }
*/


    return 0;
}

LRESULT CHSDPADlg::OnSmsInitNotify(WPARAM wParam, LPARAM lParam)
{
/*
    EnAtSmsType AtType = (EnAtSmsType)wParam;
    BOOL bProcRst = (BOOL)lParam;
    BOOL bSndRes  = TRUE;
        
    if(bProcRst)
    {
        if(AtType == AT_SMS_QCPMS)
        {
            SM_SmsCnt = 0;
            m_nReadIndex = 0;
            ProgressSet(0, SM_SmsNum, 1);
            if(SM_SmsCnt < SM_SmsNum && m_nReadIndex < SM_SmsMax)
            {
                bSndRes = SndAtSmsQCMGR(m_nReadIndex);
            }
        }
        else if(AtType == AT_SMS_QCMGR)
        {
            bSndRes = SndAtSmsQCMGD(m_nReadIndex);
        }
        else if(AtType == AT_SMS_QCMGD)
        {
            ProgressStep();
            m_pSmsDlg->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_INBOX, 0);
            m_nReadIndex++;
            if(SM_SmsCnt < SM_SmsNum && m_nReadIndex < SM_SmsMax)
            {
                bSndRes = SndAtSmsQCMGR(m_nReadIndex);
            }
            else
            {
                ProgressClose();
                UpdateSmsDlg();
                if(strlen(m_szPLMN) == 0)
                    AtSndCOPSEx();
            }
        }
    }

    if(!bProcRst || !bSndRes)
    {
        ProgressClose();
        UpdateSmsDlg();
        if(strlen(m_szPLMN) == 0)
            AtSndCOPSEx();
    }
*/
    return 0;
}

LRESULT CHSDPADlg::OnCreateProgress(WPARAM wParam, LPARAM lParam)
{
    int nUpper = (int)wParam;
    int nStep = (int)lParam;

    if(nUpper == 0 || nStep == 0)
    {
        CProgressDlg dlg(this);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;
    }
    else
    {
        CProgressDlg dlg(this, nUpper, nStep);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;        
    }

    return 0;
}

BOOL CHSDPADlg::ProgressSet(int nLower, int nUpper, int nStep)
{
    if(pDlg)
    {
        m_nLower = nLower;
        m_nUpper = nUpper;
        m_nStep  = nStep;
        pDlg->m_Progress.SetRange(m_nLower, m_nUpper);
        pDlg->m_Progress.SetStep(m_nStep);
        return TRUE;
    }
    return FALSE;
}

BOOL CHSDPADlg::ProgressStep()
{
    if(pDlg && pDlg->m_Progress.GetPos() < m_nUpper)
    {
        pDlg->m_Progress.StepIt();
        return TRUE;
    }
    return FALSE;
}

BOOL CHSDPADlg::ProgressClose()
{
    if(pDlg)
    {
        pDlg->PostMessage(WM_CLOSE);
        return TRUE;
    }
    return FALSE;
}

BOOL CHSDPADlg::ProgressOpen(int nUpper, int nStep)
{
    PostMessage(WM_CREATE_PROGRESS, (WPARAM)nUpper,(LPARAM)nStep);
    return TRUE;
}

BOOL CHSDPADlg::SndAtSmsQCMGR(USHORT index)
{   
    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGR], index);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {        
        SetTimer(IDT_QCMGR_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGR, this);
        return TRUE;
    }
    return FALSE;
}

void CHSDPADlg::RspAtSmsQCMGR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGR_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);        
        return;
    }

    //empty sms entry
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGR, (LPARAM)TRUE);            
        return;
    }

    ASSERT(wStrNum == 3 || wStrNum == 2);

    if(strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
        || memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCMGR], strlen(gcstrResSms[AT_SMS_QCMGR])))
    {
        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);                
        return;
    }

    StSmsRecord record;
    memset(&record, 0x00, sizeof(StSmsRecord));
	record.m_NoATRspCDS = TRUE;

    if(SmsAtCMGRRspProc(strArr, wStrNum, record))
    {
        pDlg->SM_SmsCnt++;
        if(record.state == SMS_STATE_MT_NOT_READ
            || record.state == SMS_STATE_MT_READ)
            pDlg->m_pSmsData->AddSmsRecord(SMS_TYPE_INBOX, record);
        else
            pDlg->m_pSmsData->AddSmsRecord(SMS_TYPE_OUTBOX, record);

        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGR, (LPARAM)TRUE);
    }
    else
        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);
}

BOOL CHSDPADlg::SndAtSmsQCMGD(USHORT index)
{   
    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGD], index);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        SetTimer(IDT_QCMGD_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGD, this);
        return TRUE;
    }
    return FALSE;
}

void CHSDPADlg::RspAtSmsQCMGD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd; 
    pDlg->KillTimer(IDT_QCMGD_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGD, (LPARAM)TRUE);
    else
        pDlg->PostMessage(WM_SMS_INIT_NOTIFY, (WPARAM)AT_SMS_QCMGD, (LPARAM)FALSE);
}

LRESULT CHSDPADlg::UpdateIcon(WPARAM wParam, LPARAM lParam)
{
    CString strInfo;

#ifndef FEATURE_VERSION_NOSIM
	if (!m_bSimReady)
	{
		lParam = 0;
	}
#endif

#ifndef FEATURE_UI_TEST
	EnIconType kind = (EnIconType)wParam;
	DWORD value = lParam;
    
	int rssi_v[]={                   ///信号强度
		IDB_ICON_RSSI0,
		IDB_ICON_RSSI1,
		IDB_ICON_RSSI2,
		IDB_ICON_RSSI3,
		IDB_ICON_RSSI4,
		IDB_ICON_RSSI5,
		IDB_ICON_NULL,
	};
	int call_privacy[] = {
		IDB_ICON_NULL,
		IDB_CALL_PRIVACY,
	};
	int sms_unrd[]={ //.................................新未读短信
		IDB_ICON_NULL,                
		IDB_ICON_SMS_UNRD,
	};
	int voicesms[]={ //add by liub for voicemail
		IDB_ICON_NULL,
		IDB_ICON_VOICEMAIL,
	};
    int sms_full[]={ //.................................短信满
		IDB_ICON_NULL,
		IDB_ICON_SMS_FULL,
    };
	int nw_type[]={  //.................................网络类型
		IDB_ICON_NULL,
		IDB_ICON_NWT_GSM,
		IDB_ICON_NWT_WCDMA
	};

	int roam_v[]={  //..................................漫游
		IDB_ICON_NULL,
		IDB_ICON_ROAM_ON,
		IDB_ICON_ROAM_ON, 
        IDB_ICON_ROAM_ON,
	};

    int handset_v[]={ //................................耳机
        IDB_ICON_NULL/*IDB_ICON_HANDSET_OFF*/, 
        IDB_ICON_HANDSET_ON,    
    };

#ifdef FEATURE_VERSION_ITELCO
    int nw_srv[]={   
        IDB_ICON_NULL, 
        IDB_ICON_NWT_GSM,
        IDB_ICON_NWT_EDGE, 
        IDB_ICON_NWT_HSDPA, 
		IDB_ICON_NWT_GSM,
		IDB_ICON_NWT_WCDMA, 
		IDB_ICON_NWT_HSUPA, 
    };
#else
	int nw_srv[]={   //..................................网络类型 
        IDB_ICON_NULL, 
        IDB_ICON_NWT_GPRS,
        IDB_ICON_NWT_EDGE, 
        IDB_ICON_NWT_HSDPA, 
		IDB_ICON_NWT_GSM,
		IDB_ICON_NWT_WCDMA, 
		IDB_ICON_NWT_HSUPA, 
#ifdef FEATURE_HAIER_CM
		IDB_ICON_NWT_CDMA,	//7
		IDB_ICON_NWT_HDR,
		IDB_ICON_NWT_CDMA_HDR,
#endif
    };
#endif

	int ue_type[]={  //...................................双向箭头图标
		IDB_ICON_NULL, 
		//IDB_ICON_UE_PS, //去掉双向剪头图标 delete by liub
	};

	switch(kind) {
	case ICON_TYPE_RSSI:
		m_cRssiInfo.SetBitmap(rssi_v[value]);
		m_cRssiInfo.Invalidate();
		break;
	case ICON_TYPE_NETWORK:
/*
		m_cNetWkType.SetBitmap(nw_type[value]);
		m_cNetWkType.Invalidate();
*/
		if(m_SrvStatus != 2)//支持数据业务
		{
			m_cNWSrvType.SetBitmap(nw_srv[0]);
			m_cRoam.SetBitmap(roam_v[0]);
		}
		else
		{
			switch(m_NetworkType)
			{
			case 0:
				m_cNWSrvType.SetBitmap(nw_srv[0]);
				m_cRoam.SetBitmap(roam_v[0]);
				break;
			#ifdef FEATURE_HAIER_CM
			case 2:
				m_cNWSrvType.SetBitmap(nw_srv[7]);
				break;
			case 4:
				m_cNWSrvType.SetBitmap(nw_srv[8]);
				break;
			case 8:
				m_cNWSrvType.SetBitmap(nw_srv[9]);
			#else
			case 1:
				m_cNWSrvType.SetBitmap(nw_srv[4]);
				break;
			case 2:
				m_cNWSrvType.SetBitmap(nw_srv[5]);
				break;
			case 3:
				m_cNWSrvType.SetBitmap(nw_srv[2]);
				break;
			#endif
			default:
				m_cNWSrvType.SetBitmap(nw_srv[0]);
				m_cRoam.SetBitmap(roam_v[0]);
				break;
			}
		}
		m_cNWSrvType.Invalidate();
		m_cRoam.Invalidate();

		break;

	case ICON_TYPE_PS:
	#ifdef FEATURE_VERSION_ITELCO
		if(m_SrvStatus != 2)//支持数据业务
		{
			m_cUEType.SetBitmap(ue_type[0]);
		}
		else
		{
			m_cUEType.SetBitmap(ue_type[value]);
		}
		m_cUEType.Invalidate();
	#endif
		break;
	case ICON_TYPE_ROAM:
		if(m_SrvStatus != 2)
		{
			m_cRoam.SetBitmap(roam_v[0]);
		}
		else
		{
			m_cRoam.SetBitmap(roam_v[value]);
		}
		m_cRoam.Invalidate();
		break;
	case ICON_TYPE_SMSUNREAD:
	    m_cSmsUnrdInfo.SetBitmap(sms_unrd[value]);
		m_cSmsUnrdInfo.Invalidate();
        break;
    case ICON_TYPE_SMSFULL:
		//if((m_pSmsData->m_SmsNum[SMS_TYPE_INBOX]== SMS_RECORD_MAX)&&(g_ME_SmsNum==g_ME_SmsMax)&&(g_SM_SmsNum==g_SM_SmsMax))
        // if(m_pSmsData->m_SmsNum[SMS_TYPE_INBOX]== SMS_RECORD_MAX)//added by lly 2007.12.28
         //{
		m_cSmsFullInfo.SetBitmap(sms_full[value]);
		m_cSmsFullInfo.Invalidate();
         //}
        break;
    case ICON_TYPE_PLMN:  
	{
		m_cPlmnInfo.SetBitmap(IDB_ICON_LONGNULL);

		if((value == 1) &&(m_SrvStatus == 2))  //value为1代表找到网络
		{
			if(wcslen(m_szPLMN))
			{
				m_cPlmnInfo.SetText(m_szPLMN);
			}

		}
		else
		{
			m_cPlmnInfo.SetText(_T(""));
		}
		#ifdef FEATURE_VERSION_BRAZIL
			m_pTabDialog->ReLoadLogo(m_szPLMN);  
		#endif
		m_cPlmnInfo.Invalidate();
		break;
	}
    case ICON_TYPE_PLMN2:
        m_cPlmnInfo2.SetBitmap(IDB_ICON_LONGNULL);
        switch(value)
        {
        case 0:
            strInfo.LoadString(IDS_NW_SRV_NO);
            break;
        case 1:
            strInfo.LoadString(IDS_NW_SRV_LIMIT);
            break;
        case 2:
        default:
            strInfo = _T("");
            break;
        }
		m_cPlmnInfo2.SetText(strInfo);
		m_cPlmnInfo2.Invalidate();

        break;
	case ICON_TYPE_CALLPRIVACY:
			m_cCallPrivacyInfo.SetBitmap(call_privacy[value]);
			m_cCallPrivacyInfo.Invalidate();
		break;
    case ICON_TYPE_HANDSET:
        if(g_SetData.Main_nCall != 0)
        {
            m_cHandSet.SetBitmap(handset_v[value]);
		    m_cHandSet.Invalidate();
        }
        break;
    case ICON_TYPE_NWSRV:
		if(m_SrvStatus != 2)
		{
			m_cNWSrvType.SetBitmap(nw_srv[0]);
		}
		else
		{
			m_cNWSrvType.SetBitmap(nw_srv[value]);
		}
		m_cNWSrvType.Invalidate();
        break;

    case ICON_TYPE_UPFLUX:
//        //m_cUpFlux.SetBitmap(IDB_ICON_LONGNULL);
		//m_cUpFlux.SetBitmap(IDB_ICON_LONGNULL_SPEED);//delete by liub
		m_cUpFlux.SetBitmap(IDB_BITMAP_UPSPEEDICON);//modify by liub
//      strInfo.Format(_T("UP: %.3f Mbytes"), value / (1024.0 * 1024.0));//delete by liub
// 		m_cUpFlux.SetText(strInfo);
		m_cUpFlux.Invalidate();

         break;
    case ICON_TYPE_UPSPEED:
        //m_cUpSpeed.SetBitmap(IDB_ICON_LONGNULL);	
		
		m_cUpSpeed.SetBitmap(IDB_ICON_LONGNULL_SPEED);
	    strInfo.Format(_T("%d kbps"), value);		
		m_cUpSpeed.SetText(strInfo);
		m_cUpSpeed.Invalidate();
        break;
    case ICON_TYPE_DWFLUX:
//       //m_cDwFlux.SetBitmap(IDB_ICON_LONGNULL);
		//m_cDwFlux.SetBitmap(IDB_ICON_LONGNULL_SPEED);//delete by liub for downspeed
		m_cDwFlux.SetBitmap(IDB_BITMAP_DOWNSPEEDICON);//modify by liub for downspeed
        //strInfo.Format(_T("DW: %.3f Mbytes"), value / (1024.0 * 1024.0));//delete by liub
	//	m_cDwFlux.SetText(strInfo);
		m_cDwFlux.Invalidate();
        break;
    case ICON_TYPE_DWSPEED:
        //m_cDwSpeed.SetBitmap(IDB_ICON_LONGNULL);
		m_cDwSpeed.SetBitmap(IDB_ICON_LONGNULL_SPEED);//modify by liub
        strInfo.Format(_T("%d kbps"), value);		
		m_cDwSpeed.SetText(strInfo);
		m_cDwSpeed.Invalidate();
        break;
	case ICON_TYPE_VOICEMAIL://add by liub for voicemail
		m_cVoiceSms.SetBitmap(voicesms[value]);
		m_cVoiceSms.Invalidate();

	}

#else   //Icon Test
    m_cRssiInfo.SetBitmap(IDB_ICON_RSSI5);
    m_cRssiInfo.Invalidate();
/*
    m_cNetWkType.SetBitmap(IDB_ICON_NWT_GSM);
    m_cNetWkType.Invalidate();
*/
    m_cRoam.SetBitmap(IDB_ICON_ROAM_ON);
    m_cRoam.Invalidate();
    m_cSmsUnrdInfo.SetBitmap(IDB_ICON_SMS_UNRD);
    m_cSmsUnrdInfo.Invalidate();
    m_cSmsFullInfo.SetBitmap(IDB_ICON_SMS_FULL);
    m_cSmsFullInfo.Invalidate();
	m_cCallPrivacyInfo.SetBitmap(IDB_CALL_PRIVACY);
	m_cCallPrivacyInfo.Invalidate();
    m_cPlmnInfo.SetText(_T("CHN MOBILE"));
    m_cPlmnInfo.Invalidate();
    strInfo.LoadString(IDS_NW_SRV_LIMIT);
    m_cPlmnInfo2.SetText(strInfo);
	m_cPlmnInfo2.Invalidate();

    if(g_SetData.Main_nCall != 0)
    {
        m_cHandSet.SetBitmap(IDB_ICON_HANDSET_ON);
        m_cHandSet.Invalidate();
    }
    m_cNWSrvType.SetBitmap(IDB_ICON_NWT_WCDMA);
	m_cNWSrvType.Invalidate();
	m_cUpFlux.SetText(_T("UP: 0 Mbytes"));
	m_cUpFlux.Invalidate();
    m_cUpSpeed.SetText(_T("0 kbps"));
    m_cUpSpeed.Invalidate();
	m_cDwFlux.SetText(_T("DW: 0 Mbytes"));
	m_cDwFlux.Invalidate();
    m_cDwSpeed.SetText(_T("0 kbps"));
    m_cDwSpeed.Invalidate();
#endif
	return 0;
}





void CHSDPADlg::UpdateUnreadSmsIcon()
{
#ifdef FEATURE_VERSION_NOSIM
	if((m_pSmsData->GetUnreadSmsNum() + UE_GetUnreadSmsNum(LOC_ME)) > 0)
#else
	if((m_pSmsData->GetUnreadSmsNum() + UE_GetUnreadSmsNum(LOC_MAX)) > 0)

#endif
        PostMessage(WM_ICON_UPDATE, ICON_TYPE_SMSUNREAD, 1);
    else
        PostMessage(WM_ICON_UPDATE, ICON_TYPE_SMSUNREAD, 0);
}

void CHSDPADlg::UpdateFullSmsIcon()
{
//    if(m_pSmsData->IsFull(SMS_TYPE_INBOX))

    //if((m_pSmsData->m_SmsNum[SMS_TYPE_INBOX]== SMS_RECORD_MAX)&&(UE_SmsIsFull(LOC_MAX)))
    if(m_pSmsData->m_SmsNum[SMS_TYPE_INBOX]== SMS_RECORD_MAX)

        PostMessage(WM_ICON_UPDATE, ICON_TYPE_SMSFULL, 1);
    else
        PostMessage(WM_ICON_UPDATE, ICON_TYPE_SMSFULL, 0);
}

//RF 
EnSyncInitFuncRetType CHSDPADlg::AtSndCFUN()
{
    char szAtBuf[20] = {0};

#ifdef FEATURE_AUTOSETRF
    strcpy(szAtBuf, "AT+CFUN=1\r");
#else
    strcpy(szAtBuf, "AT+CFUN?\r");
#endif
    
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCFUN, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespCFUN(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    LoadIniFile();
#ifdef FEATURE_AUTOSETRF
    g_SetData.Setup_nPowerManagement=1;
#else
	//int len;
	char cPower;
	BYTE *p=strArr[0];
	p = (unsigned char *)strchr((char *)p,' ');
	//len = strchr((char *)p,',')-(char *)p;
    cPower=*(p+1);

    switch(cPower-48)
    {
    case 1:
        //RF ON
        g_SetData.Setup_nPowerManagement=1;
        break;
    case 4:
    default:
        //RF OFF
        g_SetData.Setup_nPowerManagement=0;
        break;
    }
#endif
    SaveIniFile();
	//((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_RSSI, valRssi);	
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCLIP()
{
    char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT+CLIP=1\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespClip, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespHVPRIV(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
#ifndef FEATURE_CALL_PRIVACY
	return;
#endif

	CHSDPADlg *pdlg = (CHSDPADlg*)pWnd;
	
	if (!memcmp((const char*)strArr[0], "+HVPRIV: ", strlen("+HVPRIV: ")))
	{
		char *ptr = (char*)strArr[0] + strlen("+HVPRIV: ");
		int nPrivacy = 0;
		nPrivacy = atoi(ptr);
		if (nPrivacy == 0)
		{
			pdlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_CALLPRIVACY, 0);
		}
		else if (nPrivacy == 1)
		{
			pdlg->PostMessage(WM_ICON_UPDATE, ICON_TYPE_CALLPRIVACY, 1);
		}
	}
}

void CHSDPADlg::AtRespClip(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg *pDlg = (CHSDPADlg*)pWnd;
    pDlg->m_bEnableClip = TRUE;
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

EnSyncInitFuncRetType CHSDPADlg::AtSndCSCS()
{
    char szAtBuf[30] = {0};
    char szChSet[10] = {0};

#ifdef FEATURE_HAIER_ADAPTER
	return SYNCINITFUNCRET_DONE;
#endif
    strncpy(szChSet, "UCS2", sizeof(szChSet));

    sprintf(szAtBuf, "AT+CSCS=\"%s\"\r", szChSet);
 
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCSCS, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespCSCS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::RegSyncInitFunc()				//wyw_0120 remove all Sleep
{
    //注册启动函数
    m_pSyncFuncTbl[SYNCINITFUNCID_HANDSET] = &CHSDPADlg::AtSndHANDSET;
//    Sleep(150);
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
	//	m_pSyncFuncTbl[SYNCINITFUNCID_CFUN] = &CHSDPADlg::AtSndCFUN;	
	//	m_pSyncFuncTbl[SYNCINITFUNCID_COPSFORMAT] = &CHSDPADlg::AtSndCOPSFormat;
	}
//	m_pSyncFuncTbl[SYNCINITFUNCID_ROAM] = &CHSDPADlg::AtSndROAM;
	m_pSyncFuncTbl[SYNCINITFUNCID_COPS] = &CHSDPADlg::AtSndCOPS;
//	Sleep(150);
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{			
	//	m_pSyncFuncTbl[SYNCINITFUNCID_NWRAT] = &CHSDPADlg::AtSndNWRAT;
	}
    m_pSyncFuncTbl[SYNCINITFUNCID_CSQ] = &CHSDPADlg::AtSndCSQ;
//	Sleep(150);
    m_pSyncFuncTbl[SYNCINITFUNCID_CMGF] = &CHSDPADlg::AtSndCMGF;
//	Sleep(150);
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
	  //  m_pSyncFuncTbl[SYNCINITFUNCID_CSCA] = &CHSDPADlg::AtSndCSCA;
	}

    m_pSyncFuncTbl[SYNCINITFUNCID_CSDH] = &CHSDPADlg::AtSndCSDH;
//	Sleep(150);
    //m_pSyncFuncTbl[SYNCINITFUNCID_CPMS] = &CHSDPADlg::AtSndCPMS; 
//	Sleep(150);
    m_pSyncFuncTbl[SYNCINITFUNCID_CNMI] = &CHSDPADlg::AtSndCNMI;
//	Sleep(150);
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
		//if(g_SetData.Main_nCall)
	  //  m_pSyncFuncTbl[SYNCINITFUNCID_CLIP] = &CHSDPADlg::AtSndCLIP;
	}
	
	m_pSyncFuncTbl[SYNCINITFUNCID_CSCS] = &CHSDPADlg::AtSndCSCS;
//	Sleep(150);
	m_pSyncFuncTbl[SYNCINITFUNCID_CGMR] = &CHSDPADlg::AtSndCGMR;
//	Sleep(150);

	//add by liub for CDMA2000 SMSSettings
    m_pSyncFuncTbl[SYNCINITFUNCID_HMSGP] = &CHSDPADlg::SndAtSmsQHMSGP;
//	Sleep(150);
	m_pSyncFuncTbl[SYNCINITFUNCID_CSMP] = &CHSDPADlg::SndAtSmsQCSMP;
//	Sleep(150);
	//add by liub end

	m_pSyncFuncTbl[SYNCINITFUNCID_CGSN] = &CHSDPADlg::AtSndCGSN;
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
	//	m_pSyncFuncTbl[SYNCINITFUNCID_SYSCONFIG] = &CHSDPADlg::AtSndSYSCONFIG;
	//	m_pSyncFuncTbl[SYNCINITFUNCID_PS] = &CHSDPADlg::AtSndQCPDSI;
	}
    m_pSyncFuncTbl[SYNCINITFUNCID_NULL] = NULL;

    //注册用户提示信息
    CString str;
    str.LoadString(IDS_CHECK_HANDSET_STATUS);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_HANDSET][0], str);
    str.LoadString(IDS_TIMEOUT_HANDSET_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_HANDSET][1], str);

    //str.LoadString(IDS_CHECK_RFPOWER_STATUS);
   // wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CFUN][0], str);
   // str.LoadString(IDS_TIMEOUT_RF_POWER_INIT);
  //  wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CFUN][1], str);
 
 	//str.LoadString(IDS_CHECK_ROAM_STATUS);
   // wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_ROAM][0], str);
    //str.LoadString(IDS_TIMEOUT_ROAM_INIT);
   // wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_ROAM][1], str);
	
   // str.LoadString(IDS_READ_PLMN_DATA);    
  //  wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_COPSFORMAT][0], str);
   // wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_COPS][0], str);
   // str.LoadString(IDS_TIMEOUT_PLMN_INIT);
  //  wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_COPSFORMAT][1], str);
  //  wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_COPS][1], str);

   // str.LoadString(IDS_READ_NWTYPE_DATA);
 //   wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_NWRAT][0], str);
  //  str.LoadString(IDS_TIMEOUT_NWTYPE_INIT);
  //  wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_NWRAT][1], str);



    str.LoadString(IDS_READ_RSSI_DATA);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CSQ][0], str);
    str.LoadString(IDS_TIMEOUT_RSSI_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CSQ][1], str);

    str.LoadString(IDS_CONFIG_SMS_MODE);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CMGF][0], str);
    str.LoadString(IDS_TIMEOUT_SMSMODE_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CMGF][1], str);

    str.LoadString(IDS_CONFIG_SMS_ROUTE);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CNMI][0], str);
    str.LoadString(IDS_TIMEOUT_SMSROUTE_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CNMI][1], str);

    str.LoadString(IDS_CONFIG_SMS_CSDH);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CSDH][0], str);
    str.LoadString(IDS_TIMEOUT_SMSCSDH_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CSDH][1], str);

//     str.LoadString(IDS_CONFIG_SMS_ROUTE);
//     wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CPMS][0], str);
//     str.LoadString(IDS_TIMEOUT_SMSSTORAGE_INIT);
//     wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CPMS][1], str);

	//if(g_SetData.Main_nCall)
	{
	//	str.LoadString(IDS_CONFIG_INCOMING_CALL);
	//	wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CLIP][0], str);
	//	str.LoadString(IDS_TIMEOUT_INCOMCALL_INIT);
	//	wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CLIP][1], str);
	}

    str.LoadString(IDS_CONFIG_CHSET);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CSCS][0], str);
    str.LoadString(IDS_TIMEOUT_CHSET_INIT);
   wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CSCS][1], str);

	str.LoadString(IDS_CONFIG_CGMR);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CGMR][0], str);
    str.LoadString(IDS_TIMEOUT_CGMR_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CGMR][1], str);

	str.LoadString(IDS_CONFIG_CGSN);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CGSN][0], str);
    str.LoadString(IDS_TIMEOUT_CGSN_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_CGSN][1], str);

    str.LoadString(IDS_CONFIG);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_NULL][0], str);
    str.LoadString(IDS_TIMEOUT_CONFIG_INIT);
    wcscpy(m_szSyncInitInfo[SYNCINITFUNCID_NULL][1], str);
}

void CHSDPADlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

    if(g_bDataService)
    {
        AfxMessageBox(IDS_QUIT_INTERNET, MB_OK | MB_ICONINFORMATION);
        return;
    }
	CString StrQuit,strTitle;
	StrQuit.LoadString(IDS_QUIT_TIP);
	strTitle.LoadString(IDS_APPNAME);
    AddPrefixName(strTitle);
   //if(AfxMessageBox(IDS_QUIT_TIP, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDCANCEL)
	//Zhou Bin modified 2009.4.21
	if(MessageBox(StrQuit,strTitle,MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDCANCEL)
        return;

	if (g_bIsExist)
	{
		int i = 0;
		m_pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
		while(m_pComm->GetSerialState() != SERIAL_STATE_CMD)
		{
			if (i == 30)
			{
				DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
				((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);			
				
				break;
			}
			ShowWindow(SW_HIDE);
			i++;
			Sleep(1000);
		}
	}

// @@@@@@
#ifndef OPEN_PAGE_UI
    // make sure the device is terminated before closing windows
    MessageManager::GetInstance().TerminateDevice();
#endif

#ifndef FEATURE_UI_TEST
	if (g_bIsExist && g_SetData.Main_nCall)
	{
		AtSndBeforeClose();
		Sleep(200);
	}
#endif

	if (g_bIsExist)
	{
		//结束发送AT命令0
		AtSndOPCONLINE();
		Sleep(200);

#ifndef FEATURE_UI_TEST
		SndAtPowerCFUN(POWER_OFF); //关机RF关  TATA需求（add by liub）
		Sleep(200);
#endif
	}

	if (m_hMonitorThread)
	{
		SetEvent(g_hExitEvent);
		WaitForSingleObject(m_hMonitorThread, INFINITE);
		CloseHandle(m_hMonitorThread);
	}
	if (g_hExitEvent)
	{
		CloseHandle(g_hExitEvent);
	}

	CBaseDialog::OnClose();
}

void CHSDPADlg::ForegroundWnd()
{
    CWnd *pWndChild = NULL;

    pWndChild = GetLastActivePopup();    
    ShowWindow(SW_RESTORE);
    ShowWindow(SW_SHOWNA);
    if(pWndChild)
    {
        pWndChild->SetForegroundWindow();
        pWndChild->SetActiveWindow();
        pWndChild->SetFocus();
    }
}

BOOL CHSDPADlg::AtSndBeforeClose()
{   
    BYTE index = 0;
    char szAtBuf[100];
    memset(szAtBuf, 0x00, sizeof(szAtBuf));

    char szAtCmdArr[][20] = {
        /*"+CHUP",*/
#ifdef FEATURE_HAIER_CM
		"+CHV",
#else
		"+CHV0",
#endif
#ifdef FEATURE_AUTOSETRF
//        "+PHMOD=4",
#endif
        ""
    };

    strcpy(szAtBuf, "AT");
    for(index = 0; strlen(szAtCmdArr[index]) > 0; index++)
    {
        strcat(szAtBuf, szAtCmdArr[index]);
        strcat(szAtBuf, ";");
    }
    strcat(szAtBuf, "\r");

    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtResSndBeforeClose, this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
		{
			return TRUE;
		}
    }
    return FALSE;
}

void CHSDPADlg::AtResSndBeforeClose(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

//get the firmware version
EnSyncInitFuncRetType CHSDPADlg::AtSndCGMR()
{
    const char szATCGMR[]="AT+GMR\r";

    if(m_pComm->WriteToPort(szATCGMR, strlen(szATCGMR)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCGMR, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespCGMR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;    

	char *p;
    p = (char*)(strArr[0]);
	int nErrorLen = strlen(gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode]);
    if(_strnicmp((const char*)strArr[0], 
		(const char*)gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode],
		nErrorLen) == 0)
    {		
		//pDlg->m_strFWVersion="";
       CString strTemp = (char *)(strArr[0]+nErrorLen);
		pDlg->m_strFWVersion.Format(_T("%s"),strTemp);
    }
    else
    {
#ifdef FEATURE_HAIER_DSI
	char *ptr = p;
	int cnt = 0;
	while(*p){
		if(*p == '\"'){
			if(cnt == 0){
				ptr = ++p;
				cnt++;
			}else{
				*p = 0;
				break;
			}
			
		}else{
			p++;
		}
	}
	CString strTemp = (char *)ptr;
#else
		//success
		while (*p != ' ')
		{
			p++;
		}
		*p = '\0';

		      CString strTemp = (char *)strArr;
#endif
	
          pDlg->m_strFWVersion.Format(_T("%s"),strTemp);
	} 
	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

//get the IMEI
EnSyncInitFuncRetType CHSDPADlg::AtSndCGSN()
{
    const char szATCGSN[]="AT+GSN\r";

    if(m_pComm->WriteToPort(szATCGSN, strlen(szATCGSN)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCGSN, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespCGSN(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;

	int nErrorLen = strlen(gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode]);
    if(_strnicmp((const char*)strArr[0], 
		(const char*)gc_dsatResCodeTbl[DSAT_CME_ERROR][gc_dsatmode],
		nErrorLen) == 0)
    {
		pDlg->m_strIMEI=_T("");
		//pDlg->m_strIMEI.Format("%s", strArr[0]+nErrorLen);
    }
    else
    {
#ifdef FEATURE_HAIER_DSI
		char *p = (char*)strArr[0] + strlen("+GSN: ");
		char *ptr = p;
		int cnt = 0;
		while(*p){
			if(*p == '\"'){
				if(cnt == 0){
					ptr = ++p;
					cnt++;
				}else{
					*p = 0;
					break;
				}
			}else{
				p++;
			}
		}
		CString strTemp = ptr;
#else
		//success
			      CString strTemp = (char *)strArr[0];
#endif
		pDlg->m_strIMEI.Format(_T("%s"), strTemp);
		//convert IMEI
		if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
			(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
		{
		pDlg->ConvertIMEI();
	} 
		
		
	} 
	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

EnSyncInitFuncRetType CHSDPADlg::AtSndSYSCONFIG()
{
    const char szAtSysconfig[] = "AT$SYSCONFIG?\r";

    if(m_pComm->WriteToPort(szAtSysconfig, strlen(szAtSysconfig)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespSYSCONFIG, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespSYSCONFIG(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    char *p, *ptr[2];
    int nAcq, nMode, cnt;
    EnCMModePref Mod;
    EnGWAcqOrderPref Acq;
    
    nAcq = nMode = cnt = 0;
    p = ptr[0] = ptr[1] = NULL;
    
    pDlg->m_CMModePref = CM_MODE_PREF_MAX;    
    pDlg->m_GWAcqOrderPref = CM_GW_ACQ_ORDER_PREF_MAX;
    
    if(wStrNum == 2 && 0 == strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]))
    {
        p = ptr[0] = (char*)strArr[0] + strlen("^SYSCONFIG: ");
        
        while(*p)
        {
            if(*p == ',')
            {
                *p = 0;
                if(cnt >= 1)
                    break;
                ptr[++cnt] = ++p;
            }
            else
                p++;
        }
        
        nMode = atoi(ptr[0]);
        nAcq= atoi(ptr[1]);
        
        if(nMode == CM_MODE_PREF_AUTOMATIC
            || nMode == CM_MODE_PREF_GSM_ONLY
            || nMode == CM_MODE_PREF_WCDMA_ONLY)
        {
            pDlg->m_CMModePref = (EnCMModePref)nMode;
        }
        
        if(nAcq == CM_GW_ACQ_ORDER_PREF_AUTOMATIC
            || nAcq == CM_GW_ACQ_ORDER_PREF_GSM_WCDMA
            || nAcq == CM_GW_ACQ_ORDER_PREF_WCDMA_GSM)
        {
            pDlg->m_GWAcqOrderPref = (EnGWAcqOrderPref)nAcq;
        }
    }
    
    gCurAutoPref = 3;

    Mod = pDlg->m_CMModePref;
    Acq = pDlg->m_GWAcqOrderPref;
    
    if(Mod == CM_MODE_PREF_GSM_ONLY)
        gCurAutoPref = 0;
    else if(Mod == CM_MODE_PREF_WCDMA_ONLY)
        gCurAutoPref = 1;
    else if(Acq == CM_GW_ACQ_ORDER_PREF_GSM_WCDMA)
        gCurAutoPref = 2;
    else if(Acq == CM_GW_ACQ_ORDER_PREF_WCDMA_GSM)
        gCurAutoPref = 3;

	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}
/*++

Routine Description:

	IMEI转换函数，加入最后一位CheckDigit
	
Arguments:

	None.

Return Value:

    None.

--*/
void CHSDPADlg::ConvertIMEI()
{
	int nBits[14];
	int i;
	int nSum;
	int nCD;

	if (m_strIMEI.GetLength()>14)
	{
		m_strIMEI=m_strIMEI.Left(14);
	}
	for (i=0; i<14; i++)
	{
		nBits[i]=0;
	}
	for (i=0; i<14; i++)
	{
		nBits[i]=m_strIMEI[i]-48;
	}
	for (i=1; i<14; i=i+2)
	{
		nBits[i]=nBits[i]*2;
	}
	nSum=0;
	for (i=0; i<14; i++)
	{
		if (i%2==0)
		{
			nSum+=nBits[i];
		} 
		else
		{
			nSum+=nBits[i]/10;
			nSum+=nBits[i]%10;
		}
	}
	if (nSum%10==0)
	{
		nCD=0;
	} 
	else
	{
		nCD=10-nSum%10;
	}
	CString temp;
	temp.Format(_T("%s%1d"), m_strIMEI, nCD);
	m_strIMEI=temp;
}

void CHSDPADlg::UpdateInboxWhenRcvNewSms(EnLocType loctype, StSmsRecord &record, WORD nIndex, BOOL bAdd, BOOL bCopyUE)//liub search
{
    if(bAdd)
    {
        m_pSmsData->ClearSmsFlag(SMS_TYPE_INBOX, SMS_RECORD_FLAG_NEW);
        UE_ClearSmsFlag(LOC_MAX, SMS_RECORD_FLAG_NEW);
        record.state = SMS_STATE_MT_NOT_READ;
        record.flag |= SMS_RECORD_FLAG_NEW;
        if(loctype == LOC_PC)
        {
            if(bCopyUE)
                record.flag |= SMS_RECORD_FLAG_COPY_UE;
            m_pSmsData->AddSmsRecord(SMS_TYPE_INBOX, record);
        }
        else
            UE_AddSmsCardRecord(loctype, record, nIndex);
    }

    m_pSmsDlg->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_INBOX, loctype);
    UpdateUnreadSmsIcon();
    UpdateFullSmsIcon();

	HTREEITEM hRoot = m_pSmsDlg->m_treeSms.GetRootItem();
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);

    LPARAM lParam = (LPARAM)(LPCTSTR)record.szNumber;
    PostMessage(WM_AT_SMS,1,lParam);
}

void CHSDPADlg::UpdateSmsDlg()
{
/*
    ::SetEvent(g_BGPassEvt);
    m_pSmsDlg->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_ALL, 0);
    UpdateUnreadSmsIcon();
    UpdateFullSmsIcon();
*/
}

//功能：获得SMS条数
//输入：type:类型，locFilter:位置
//输出：条数
WORD CHSDPADlg::GetTotalSmsNum(EnSmsType type, EnLocFilter locFilter)
{
	ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
	ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);
	
	WORD nPCNum = 0;
	WORD nMENum = 0;
	WORD nSMNum = 0;
	
	if(locFilter == LOCFILTER_PC || locFilter == LOCFILTER_ALL)
		nPCNum = m_pSmsData->GetSmsNum(type);
	
	if(g_bSmsSyncComm && type != SMS_TYPE_DRAFT && locFilter != LOCFILTER_PC)
	{
		WORD cnt;
		EnSmsState state;
		EnSmsState state1;
		EnSmsState state2;
		StSmsCardRecord CardRecord;
		
		if(type == SMS_TYPE_INBOX)
		{
			state1 = SMS_STATE_MT_NOT_READ;
			state2 = SMS_STATE_MT_READ;
		}
		else if(type == SMS_TYPE_OUTBOX)
		{
			state1 = SMS_STATE_MO_NOT_SENT;
			state2 = SMS_STATE_MO_SENT;
		}
		
		if(locFilter != LOCFILTER_UIM)
		{
			for(cnt=0; cnt<g_ME_SmsNum; cnt++)
			{
				CardRecord = UE_ReadSmsCardRecord(LOC_ME, cnt);
				state = CardRecord.record.state;
				if(state == state1 || state == state2)
					nMENum++;
			}
		}
		
		if(locFilter != LOCFILTER_ME)
		{
			for(cnt=0; cnt<g_SM_SmsNum; cnt++)
			{
				CardRecord = UE_ReadSmsCardRecord(LOC_UIM, cnt);
				state = CardRecord.record.state;
				if(state == state1 || state == state2)
					nSMNum++;
			}
		}
	}
	
	if(locFilter == LOCFILTER_PC)
		return nPCNum;
	else if(locFilter == LOCFILTER_ME)
		return nMENum;
	else if(locFilter == LOCFILTER_UIM)
		return nSMNum;
	else
		return (nPCNum+nMENum+nSMNum);
}

/*Flexi需求，查询新来的长短信片段是否属于已经保存在短信收件箱中的未收全长短信的一部分*/
/*如果是，则将此信息与之合并 liub add*/
BOOL CHSDPADlg::SearchAndInsertSmsSege(StSmsRecord &record)
{
	USES_CONVERSION;
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	StSmsRecord ConcatenateSMSSegeInlist;
	memset(&ConcatenateSMSSegeInlist, 0x00, sizeof(StSmsRecord));

	BOOL cnt;
	cnt = FALSE;
	int SmsNum = 0;
	int i;
	int nDelete_index;
	CString Inlist_szContenttemp;
	CString sege_szContenttemp;

	CString inlist_szleft;
	CString inlist_szReplacetext;
	CString inlist_szRight;


	sege_szContenttemp= /*= UCS2ToGB(A2W((char*)*/record.szContent/*))*/;

	SmsNum = GetTotalSmsNum(SMS_TYPE_INBOX,LOCFILTER_PC);
    for (i=0; i<SmsNum; i++)
    {
		ConcatenateSMSSegeInlist = m_pSmsDlg->GetTotalSmsRecordFromDspIndex(SMS_TYPE_INBOX,LOCFILTER_PC,i);
		
		Inlist_szContenttemp = /*UCS2ToGB(A2W((char*)*/ConcatenateSMSSegeInlist.szContent/*))*/;//add by liub for UCS2转换为GB

		if(ConcatenateSMSSegeInlist.nRefCnt == record.nRefCnt && ConcatenateSMSSegeInlist.nTotalCnt == record.nTotalCnt)
		{
            if(wcscmp((const TCHAR*)ConcatenateSMSSegeInlist.szNumber, (const TCHAR*)record.szNumber) == 0)
            {
                nDelete_index = i;
				Inlist_szContenttemp.Delete(ConcatenateSMSSegeInlist.segeindex[record.nSeqCnt -1][0],5);
				Inlist_szContenttemp.Insert(ConcatenateSMSSegeInlist.segeindex[record.nSeqCnt -1][0],sege_szContenttemp);

				for (int j=record.nSeqCnt;j<SMS_CONCAT_SEGMENT_MAX;j++)
				{
					if (ConcatenateSMSSegeInlist.segeindex[j][0] > 0)
					{
						ConcatenateSMSSegeInlist.segeindex[j][0] = ConcatenateSMSSegeInlist.segeindex[j][0] + sege_szContenttemp.GetLength();
					}
				}
				
				
				LPCTSTR pContent = Inlist_szContenttemp;
				char *WCharContent=W2A(Inlist_szContenttemp);
				memset(ConcatenateSMSSegeInlist.szContent,0x00,sizeof(SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 2));
                strncpy(ConcatenateSMSSegeInlist.szContent, WCharContent, sizeof(ConcatenateSMSSegeInlist.szContent));


//				m_pSmsData->DeleteSmsRecord(m_DispFilter, nIndex);

				cnt =  TRUE;

			}
			else
				cnt =  FALSE;
               
		}
		else
			cnt =  FALSE;
	}

	if (cnt)
	{
		m_pSmsDlg->SmsDelete_flexi(nDelete_index);

		UpdateInboxWhenRcvNewSms(LOC_PC, ConcatenateSMSSegeInlist);
	}
	return cnt;
}
  

	

	

void CHSDPADlg::RcvNewSmsProc(EnLocType loctype, StSmsRecord &record, WORD nIndex)
{
    if(loctype == LOC_PC)
    {
	    int cnt = -1;

	    if(record.flag & SMS_RECORD_FLAG_CONCATENATE_SEGE)
	    {
			if (!SearchAndInsertSmsSege(record))///*Flexi需求，查询新来的长短信片段是否属于已经保存在短信收件箱中的未收全长短信的一部分*/
			{		
				if((cnt = SaveSmsSegeToRcvConcBuf(record)) != -1)
				{
					
					SetTimer(gSmsRcvConcBUf[cnt].nTimerID, SMS_RCVCONC_TIMEOUT, NULL);//长短信等待时间

					cnt = -1;			
					if((cnt = MergeConcSmsSegeTogether(record)) != -1)
					{
						KillTimer(gSmsRcvConcBUf[cnt].nTimerID);
						UpdateInboxWhenRcvNewSms(LOC_PC, record);
					}
				}
				else
					UpdateInboxWhenRcvNewSms(LOC_PC, record);
			}
	    }
	    else
		    UpdateInboxWhenRcvNewSms(LOC_PC, record);
    }
    else
    {
        UpdateInboxWhenRcvNewSms(loctype, record, nIndex);
    }
}

/************************************************************************/
/* closing ports in sleep mode                                         */
/************************************************************************/
LRESULT CHSDPADlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_POWERBROADCAST && wParam == PBT_APMQUERYSUSPEND) 
	{
        if(g_SetData.Setup_nAllowSleep)
        {
            ASSERT(m_pComm);
            m_pComm->StopPort();
        }
		else
		{
			AfxMessageBox(IDS_SETUP_PRE_SH);   
			return BROADCAST_QUERY_DENY;   
		}
	}
	else if(message == WM_POWERBROADCAST && wParam == PBT_APMSUSPEND) 
	{
        if(g_SetData.Setup_nAllowSleep)
        {
            ASSERT(m_pComm);
            m_pComm->StopPort();
        }
		else
		{
			AfxMessageBox(IDS_SETUP_PRE_SH);   
			return BROADCAST_QUERY_DENY;   
		}
	}
    else if(message == WM_POWERBROADCAST && wParam == PBT_APMRESUMESUSPEND)
	{
		ShowWindow(SW_SHOWMINIMIZED);

        SyncInitFunc(1);

		ShowWindow(SW_RESTORE);
    }

	return CBaseDialog::WindowProc(message, wParam, lParam);
}

void CHSDPADlg::CtlAllSmsRcvConcTimer(BOOL bOpen)
{
#if 0
    BYTE cnt;
    for(cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++)
    {
        if(gSmsRcvConcBUf[cnt].bInuse)
        {
            if(bOpen)
                SetTimer(gSmsRcvConcBUf[cnt].nTimerID, SMS_RCVCONC_TIMEOUT, NULL);
            else
                KillTimer(gSmsRcvConcBUf[cnt].nTimerID);
        }
    }
#endif
}

BOOL CHSDPADlg::ProcSmsRcvConcTimeout(UINT nTimerID)
{
    if(nTimerID < IDT_RCVCONCSMS_TIMEOUT_BEGIN
        || nTimerID >= IDT_RCVCONCSMS_TIMEOUT_END)
        return FALSE;

    BYTE seg = 0;
    BYTE cnt = nTimerID - IDT_RCVCONCSMS_TIMEOUT_BEGIN;
    StSmsRecord record;

    if(gSmsRcvConcBUf[cnt].bInuse)
    {
        for(seg = 0; seg < SMS_CONCAT_SEGMENT_MAX; seg++)
        {
            if(gSmsRcvConcBUf[cnt].nSeqFlag[seg])
            {
                memset(&record, 0x00, sizeof(StSmsRecord));
				record.m_NoATRspCDS = TRUE;

				record.flag = SMS_RECORD_FLAG_NEW | SMS_RECORD_FLAG_CONCATENATE_SEGE;
				record.state = SMS_STATE_MT_NOT_READ;
				record.nRefCnt = gSmsRcvConcBUf[cnt].nRefCnt;
				record.nSeqCnt = seg + 1;
				record.nTotalCnt = gSmsRcvConcBUf[cnt].nTotalCnt;
				record.timestamp = gSmsRcvConcBUf[cnt].timestamp;
				strncpy((char*)record.szNumber, (const char*)gSmsRcvConcBUf[cnt].szNumber, PB_NUM_MAX);
				strncpy((char*)record.szSCNumber, (const char*)gSmsRcvConcBUf[cnt].szSCNumber, SMS_SC_NUM_MAX);
			    strncpy((char*)record.szContent, (const char*)gSmsRcvConcBUf[cnt].szContent[seg], SMS_CONCAT_ASCII_MAX*2);
                
                memset(&g_RcvSmsRecord, 0x00, sizeof(StSmsRecord));
                g_RcvSmsRecord = record;
                UpdateInboxWhenRcvNewSms(LOC_PC, g_RcvSmsRecord);
            }
        }
        
        ResetSmsRcvConcBuf(cnt);
        return TRUE;
    }
    return FALSE;
}

//flexi 版本的当为收全所有长短信时所做的处理

BOOL CHSDPADlg::ProcSmsRcvConcTimeout_Flexi(UINT nTimerID)
{
	USES_CONVERSION;
	BYTE i;
	BYTE cnt;

	StSmsRecord record;
	int badsegeindex[SMS_CONCAT_SEGMENT_MAX];
	for (int temp =0; temp<SMS_CONCAT_SEGMENT_MAX;temp++)
	{
		badsegeindex[temp] = -1;
	}
	CString smscontent_temp;
	
	memset(&record, 0x00, sizeof(StSmsRecord));
	
	for(cnt = 0; cnt < SMS_RCVCONCBUF_MAX; cnt++)
	{
		if(gSmsRcvConcBUf[cnt].bInuse && gSmsRcvConcBUf[cnt].nTotalCnt > 0)
		{
			for(i = 0; i < gSmsRcvConcBUf[cnt].nTotalCnt; i++)
			{
				if(!gSmsRcvConcBUf[cnt].nSeqFlag[i])
				{
					gSmsRcvConcBUf[cnt].nSeqFlag[i] = TRUE;
				    strncat((char*)gSmsRcvConcBUf[cnt].szContent[i], (const char*)LMS_Flexi_RcvConcNoAll, SMS_CONCAT_ASCII_MAX*2);
					badsegeindex[i] = i;
					//break;
				}
			}
			
			//Ready to merge
			if(i == gSmsRcvConcBUf[cnt].nTotalCnt)
			{
				record.flag = SMS_RECORD_FLAG_NEW | SMS_RECORD_FLAG_CONCATENATE_FULL;
				record.state = SMS_STATE_MT_NOT_READ;
				record.nRefCnt = gSmsRcvConcBUf[cnt].nRefCnt;
				record.nTotalCnt = gSmsRcvConcBUf[cnt].nTotalCnt;
				record.timestamp = gSmsRcvConcBUf[cnt].timestamp;
				strncpy((char*)record.szNumber, (const char*)gSmsRcvConcBUf[cnt].szNumber, PB_NUM_MAX);
				record.m_NoATRspCDS = TRUE;
				//	wcsncpy((TCHAR*)record.szSCNumber, (const TCHAR*)gSmsRcvConcBUf[cnt].szSCNumber, SMS_SC_NUM_MAX);//(WCDMA使用)
				
				for(i = 0; i < gSmsRcvConcBUf[cnt].nTotalCnt; i++)
				{
					smscontent_temp =/*UCS2ToGB(A2W((char*)*/record.szContent/*))*/;//add by liub for UCS2转换为GB

					if (i==badsegeindex[i])
					{
						record.segeindex[i][0] = smscontent_temp.GetLength();

					}
					strncat((char*)record.szContent, (const char*)gSmsRcvConcBUf[cnt].szContent[i], SMS_CONCAT_ASCII_MAX*2);

				}
				
				memset(&g_RcvSmsRecord, 0x00, sizeof(StSmsRecord));
                g_RcvSmsRecord = record;
				UpdateInboxWhenRcvNewSms(LOC_PC, g_RcvSmsRecord);
				ResetSmsRcvConcBuf(cnt);
				return TRUE;
			}

		}
	}
    return FALSE;


}

LRESULT CHSDPADlg::WindowRestore(WPARAM wParam, LPARAM lParam)
{
	m_pTabDialog->SendMessage(WM_WINDOWS_RESTORE,0,0);
	return 0;
}

LRESULT CHSDPADlg::WindowMax(WPARAM wParam, LPARAM lParam)
{
	m_pTabDialog->SendMessage(WM_WINDOWS_MAX,0,0);
	return 0;
}

void CHSDPADlg::GetVersionInfo()
{
    DWORD dwHandle            = 0;
    DWORD dwVersionInfoSize    = 0;
    CString strVerion;
    CString strExeFileName;
	strExeFileName.LoadString(IDS_APPPRONAME);
 
#ifdef FEATURE_VERSION_ITELCO
	if(g_SetData.Main_nCompany == 3)
	{
		strExeFileName.Insert(0,_T("MX20 "));
	}
	else if (g_SetData.Main_nCompany == 5)
	{
		strExeFileName.Insert(0,_T("MX45 "));
	}	
#endif

    dwVersionInfoSize       = GetFileVersionInfoSize(strExeFileName.GetBuffer(MAX_PATH), &dwHandle);
    BYTE *pucVersionInfo    = new BYTE[dwVersionInfoSize];
    if (NULL != pucVersionInfo)
    {
        memset(pucVersionInfo, 0, dwVersionInfoSize);

        if (GetFileVersionInfo(strExeFileName.GetBuffer(MAX_PATH), NULL, dwVersionInfoSize, pucVersionInfo))
        {
            VS_FIXEDFILEINFO *pvsFixedFileInfo    = NULL;
            UINT uiFileInfoSize    = sizeof(VS_FIXEDFILEINFO);

            if (VerQueryValue(pucVersionInfo, _T("\\"), (void**)(&pvsFixedFileInfo), &uiFileInfoSize))
            {
                WORD wVerHiHigh    = (WORD)(pvsFixedFileInfo->dwFileVersionMS >> 16);
                WORD wVerHiLow    = (WORD)(pvsFixedFileInfo->dwFileVersionMS & 0XFFFF);
                WORD wVerLoHigh    = (WORD)(pvsFixedFileInfo->dwFileVersionLS >> 16);
                WORD wVerLoLow    = (WORD)(pvsFixedFileInfo->dwFileVersionLS & 0XFFFF);
                m_strSWVersion.Format(_T("%u, %u, %u, %04u"), wVerHiHigh, wVerHiLow, wVerLoHigh, wVerLoLow);
            }
        }
        delete []pucVersionInfo;
    }    
}

EnSyncInitFuncRetType CHSDPADlg::AtSndQCPDSI()
{
	char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$QCPDSI?\r");
    if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespQCPDSI, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, SYNCINIT_TIMEOUT_SHORT))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::AtRespQCPDSI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
       && !memcmp((const char*)strArr[0], "$QCPDSI:", strlen("$QCPDSI:")))
    {
        char *ptr = (char*)strArr[0] + strlen("$QCPDSI:");
        ((CHSDPADlg*)pWnd)->m_bPS = (BOOL)atoi(ptr);
    }
	((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PS, ((CHSDPADlg*)pWnd)->m_bPS);
    SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::AtRespPS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	if(!memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_PS][gc_dsatmode],
       strlen(gc_dsatResCodeTbl[DSAT_PS][gc_dsatmode])))
    {
        char *ptr = (char*)strArr[0] + strlen(gc_dsatResCodeTbl[DSAT_PS][gc_dsatmode]);
        ((CHSDPADlg*)pWnd)->m_bPS = (BOOL)atoi(ptr);

		((CHSDPADlg*)pWnd)->PostMessage(WM_ICON_UPDATE, ICON_TYPE_PS, ((CHSDPADlg*)pWnd)->m_bPS);
    }
}


//////////////////////////////////////////////////////////////////////////add by liub for SMSSetings(有效期、优先级等)
EnSyncInitFuncRetType CHSDPADlg::SndAtSmsQHMSGP()
{
	char szAtBuf[50] = {0};
#ifdef FEATURE_HAIER_SMS
	sprintf(szAtBuf, "%s%d,%d,,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,g_SetData.Messages_nPriority);
#else
    sprintf(szAtBuf, "%s%d,,,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,g_SetData.Messages_nPriority);
#endif
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
	
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQHMSGP, (LPVOID)this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
//        SetTimer(IDT_QHMSGP_TIMEOUT, 60000, NULL);
//        return SYNCINITFUNCRET_DONE;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;

}

//CDMA2000的+CSMP，与WCDMA完全不同
EnSyncInitFuncRetType CHSDPADlg::SndAtSmsQCSMP()
{
#ifdef FEATURE_HAIER_SMS
	return SYNCINITFUNCRET_DONE;
#endif
	char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s,1,%d,1,%d\r", gcstrAtSms[AT_SMS_QCSMP],g_SetData.Messages_nValPeriod,g_SetData.Messages_nDefDelivery);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
	
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCSMP, (LPVOID)this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
//        SetTimer(IDT_QHCSMP_TIMEOUT, 60000, NULL);
//        return SYNCINITFUNCRET_DONE;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
//	pDlg->KillTimer(IDT_QHMSGP_TIMEOUT);
	
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) != 0)
		::MessageBox(NULL, _T("Message Priority Error!"), _T(""), MB_OK);

	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

void CHSDPADlg::RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
//	pDlg->KillTimer(IDT_QHCSMP_TIMEOUT);
	
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) != 0)
		::MessageBox(NULL, _T("Message Defer Delivery Error!"), _T(""), MB_OK);

	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// --------------------------------------------------------------------------
// OpenUserGuide
//
/// Open the QMICM User Guide pdf
///
/// @param relativePath - string indicating the relative path to the pdf.
///
/// @returns bool - true if pdf successfully opened.
// --------------------------------------------------------------------------
bool CHSDPADlg::OpenUserGuide(const char* relativePath)
{
    std::string modulePath;
    std::string path;
    std::string temp;

  

    // get the path to the exe
    char filePath[MAX_PATH];
    ::GetModuleFileName(NULL, (TCHAR *)filePath, MAX_PATH);

    // get the long path to the exe
    char longPath[MAX_PATH];
    if(0 < ::GetLongPathName((TCHAR *)filePath, (TCHAR *)longPath, MAX_PATH))
    {
        temp = longPath;
    }
    else
    {
        temp =  filePath;
    }

    // trim off the exe
    path = modulePath =  temp.substr(0, temp.find_last_of("\\/"));

    // make sure we have a separator
    if(relativePath[0] != '\\')
    {
        path += '\\';
    }

    // add relative path to path
    path += relativePath;

    // open the qmicm user guide
    int result = (int)ShellExecute(AfxGetMainWnd()->m_hWnd,
                                  _T("open"),
                                  (TCHAR *)path.c_str(),
                                  NULL,
                                  (TCHAR *)modulePath.c_str(),
                                  SW_SHOWNORMAL);

    // greater than 32 if successful
    return (result > 32);
}

// --------------------------------------------------------------------------
// SelectDevice
//
/// SelectDevice establishes the device name with the message manager and
/// collects information about that device to be displayed.
///
/// @param deviceName - string name of the device to connect to.
// --------------------------------------------------------------------------
void CHSDPADlg::SelectDevice(std::string& deviceName)
{
    BeginWaitCursor();
    MessageManager::GetInstance().EstablishDevice(deviceName);
}

// --------------------------------------------------------------------------
// DeselectDevice
//
/// DeselectDevice coordinates with MessageManager to disable an active
/// device and removes information about that device from the gui.
// --------------------------------------------------------------------------
void CHSDPADlg::DeselectDevice()
{
    BeginWaitCursor();
    MessageManager::GetInstance().TerminateDevice();
}

// --------------------------------------------------------------------------
// SendRequest
//
/// SendRequest takes a string representation of a Message and coordinates 
/// with the MessageManager to send the request to the device.
///
/// @param msgStr - string representation of a Message.
// --------------------------------------------------------------------------
void CHSDPADlg::SendRequest(std::string& msgStr)
{
    // send request to MessageManager
    if (!msgStr.empty())
    {
        MessageManager::GetInstance().SendMessage(msgStr);
    }
}

// --------------------------------------------------------------------------
// OnPublisherNotify
//
/// Actions to take when a NoticeRCP is received from a NoticePublisher. Put
/// the NoticeRCP on the notice queue and post a message that a new notice
/// has been received. Notices can be sent from multiple threads and the
/// queue/post paradigm ensures that only the main thread processes notices. 
// --------------------------------------------------------------------------
void CHSDPADlg::OnPublisherNotify
(
    const Publisher<NoticeRCP>& publisher,
    const NoticeRCP& noticeRCP
)
{
    // add the notice reference counting poiniter to the notice queue
    m_noticeRCPs.push(noticeRCP);

    // post a message indicating that a new notice has been received
    ::PostMessage(m_hWnd,WM_NEW_NOTICE,NULL,NULL);
}

// --------------------------------------------------------------------------
// OnNewNotice
//
/// Actions to take when a new notice has been placed on the Notice queue.
// --------------------------------------------------------------------------
LRESULT CHSDPADlg::OnNewNotice(WPARAM wParam,LPARAM lParam)
{
    NoticeRCP noticeRCP = m_noticeRCPs.front();
    m_noticeRCPs.pop();

    // get the notice type
    uint8 noticeType = noticeRCP->GetNoticeType();

    switch(noticeType)
    {
        case NT_MESSAGE:
            {
            ProcessMsgNotice(noticeRCP);
            break;
            }
        case NT_STATUS:
            ProcessStatusNotice(noticeRCP);
            break;
        case NT_DEVICE:
            ProcessDeviceNotice(noticeRCP);
            break;
        default:
            std::stringstream stream;
            stream << _T("Error: Unknown notice type received:") << std::endl
                   << _T("  Notice Type: ") << (int)noticeType << std::endl;
	        /*modify by GL on 2008-03-24 begin*/

			CStdioFile file;
			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString((TCHAR *)stream.str().c_str());
				file.Close();		
			}
    }

    return NULL;
}

// --------------------------------------------------------------------------
// ProcessMsgNotice
//
/// Actions to take when a MsgNotice is received from the MessageManager.
///
/// @params noticeRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessMsgNotice(NoticeRCP noticeRCP)
{
    MsgNoticeRCP msgNoticeRCP = rcp_cast<MsgNotice*>(noticeRCP);
    MessageRCP msgRCP = msgNoticeRCP->GetMessageRCP();
    uint8 svcType = msgRCP->GetSvcType();
    uint8 ctlType = msgRCP->GetCtlType();
    uint16 msgType = msgRCP->GetMsgType();

    // display msg on status page
    LogMessage(msgRCP);

    // compose UID
    uint32 msgUID = (svcType << 24 | ctlType << 16 | msgType);

    // switch on control type
    switch (ctlType)
    {
        case QMI_CTL_FLAG_TYPE_CMD:
            ProcessReqMsgNotice(msgRCP,msgUID);
            break;
        case QMI_CTL_FLAG_TYPE_RSP:
            ProcessRspMsgNotice(msgRCP,msgUID);
            break;
        case QMI_CTL_FLAG_TYPE_IND:
            ProcessIndMsgNotice(msgRCP,msgUID);
            break;
        default:
            // control type not recognized
            std::stringstream stream;
            stream.str("");
            stream << _T("Error: Unknown message control type received:") << std::endl
                   << _T("  Control Type: ") << (int)ctlType << std::endl
                   << std::endl;  
			
				
			CStdioFile file;
			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString((TCHAR *)stream.str().c_str());
				file.Close();		
			}
				
            m_statusPage.AddEventMessage(stream.str(),ST_ERROR);
            break;
    }
}

// --------------------------------------------------------------------------
// ProcessReqMsgNotice
//
/// Actions to take when a request message is received from the MessageManager.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessReqMsgNotice(MessageRCP msgRCP,uint32 msgUID)
{
    // switch on unique message id
    switch (msgUID)
    {
        case WDSStartNetworkInterfaceReqUID:
            ProcessWDSStartReq(msgRCP);
            break;
        case WDSDeleteProfileReqUID:
            ProcessWDSDeleteProfileReq(msgRCP);
            break;
        case WDSGetProfileSettingsReqUID:
            ProcessWDSGetProfileSettingsReq(msgRCP);
            break;
        case WDSGetDefaultSettingsReqUID:
            ProcessWDSGetDefaultSettingsReq(msgRCP);
            break;
        default:
            // most requests do not need processing
            break;
    }
}

// --------------------------------------------------------------------------
// ProcessRspMsgNotice
//
/// Actions to take when a response message is received from the 
/// MessageManager.
///
/// @params msgRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessRspMsgNotice(MessageRCP msgRCP,uint32 msgUID)
{
    // switch on unique message id
    switch (msgUID)
    {
        case WDSSetEventReportRspUID:
            ProcessWDSSetEventReportRsp(msgRCP);
            break;
        case DMSSetEventReportRspUID:
            ProcessDMSSetEventReportRsp(msgRCP);
            break;
        case NASSetEventReportRspUID:
            ProcessNASSetEventReportRsp(msgRCP);
            break;
        case DMSGetPowerStateRspUID:
            ProcessDMSGetPowerStateRsp(msgRCP);
            break;
        case DMSGetDeviceMfrRspUID:
            ProcessDMSGetDeviceMfrRsp(msgRCP);
            break;
        case DMSGetDeviceModelIdRspUID:
            ProcessDMSGetDeviceModelIdRsp(msgRCP);
            break;
        case DMSGetDeviceRevIdRspUID:
            ProcessDMSGetDeviceRevIdRsp(msgRCP);
            break;
        case DMSGetMsisdnRspUID:
            ProcessDMSGetMsisdnRsp(msgRCP);
            break;
        case DMSGetDeviceSerialNumbersRspUID:
            ProcessDMSGetDeviceSerialNumbersRsp(msgRCP);
            break;
        case DMSGetDeviceCapRspUID:
            ProcessDMSGetDeviceCapRsp(msgRCP);
            break;
        case NASGetSignalStrengthRspUID:
            ProcessNASGetSignalStrengthRsp(msgRCP);
            break;
        case NASGetServingSystemRspUID:
            ProcessNASGetServingSystemRsp(msgRCP);
            break;
        case WDSAbortRspUID:
            ProcessWDSAbortRsp(msgRCP);
            break;
        case WDSStartNetworkInterfaceRspUID:
            ProcessWDSStartRsp(msgRCP);
            break;
        case WDSStopNetworkInterfaceRspUID:
            ProcessWDSStopRsp(msgRCP);
            break;
        case WDSGetCurrentChannelRateRspUID:
            ProcessWDSGetCurrentChannelRateRsp(msgRCP);
            break;
        case WDSCreateProfileRspUID:
            ProcessWDSCreateProfileRsp(msgRCP);
            break;
        case WDSModifyProfileSettingsRspUID:
            ProcessWDSModifyProfileSettingsRsp(msgRCP);
            break;
        case WDSDeleteProfileRspUID:
            ProcessWDSDeleteProfileRsp(msgRCP);
            break;
        case WDSGetProfileListRspUID:
            ProcessWDSGetProfileListRsp(msgRCP);
            break;
        case WDSGetProfileSettingsRspUID:
            ProcessWDSGetProfileSettingsRsp(msgRCP);
            break;
        case WDSGetDefaultSettingsRspUID:
            ProcessWDSGetDefaultSettingsRsp(msgRCP);
            break;
        default:
            // msgUID not recognized, details in status
            std::stringstream stream;
            stream << _T("Error: Unknown message type received:") << std::endl
                   << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[msgRCP->GetSvcType()] << std::endl
                   << _T("  Control Type: ") << (int)msgRCP->GetCtlType() << std::endl
                   << _T("  Message Type: ") << (int)msgRCP->GetMsgType() << std::endl
                   << std::endl;  
		   
			CStdioFile file;
			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString((TCHAR *)stream.str().c_str());
				file.Close();		
			}

            m_statusPage.AddEventMessage(stream.str(),ST_ERROR);
            break;
    }
}

// --------------------------------------------------------------------------
// ProcessIndMsgNotice
//
/// Actions to take when an indicator message is received from the 
/// MessageManager.
///
/// @params msgRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessIndMsgNotice(MessageRCP msgRCP,uint32 msgUID)
{
    // switch on unique message id
    switch (msgUID)
    {
        case WDSEventReportIndUID:
            ProcessWDSEventReportInd(msgRCP);
            break;
        case DMSEventReportIndUID:
            ProcessDMSEventReportInd(msgRCP);
            break;
        case NASEventReportIndUID:
            ProcessNASEventReportInd(msgRCP);
            break;
        case NASServingSystemIndUID:
            ProcessNASServingSystemInd(msgRCP);
            break;
        case WDSPktSrvcStatusIndUID:
            ProcessWDSPktSrvcStatusInd(msgRCP);
            break; 
        default:
            // msgUID not recognized, details in status
            std::stringstream stream;
            stream << _T("Error: Unknown message type received:") << std::endl
                   << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[msgRCP->GetSvcType()] << std::endl
                   << _T("  Control Type: ") << (int)msgRCP->GetCtlType() << std::endl
                   << _T("  Message Type: ") << (int)msgRCP->GetMsgType() << std::endl
                   << std::endl;    
				  
			CStdioFile file;
			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString((TCHAR *)stream.str().c_str());
				file.Close();		
			}

            m_statusPage.AddEventMessage(stream.str(),ST_ERROR);
            break;
    }
}

// --------------------------------------------------------------------------
// ProcessWDSStartReq
//
/// Actions to take when a WDSStartNetworkInterfaceReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSStartReq(MessageRCP msgRCP)
{
    WDSStartNetworkInterfaceReqRCP reqRCP = 
        rcp_cast<WDSStartNetworkInterfaceReq*>(msgRCP);

    // set the start message transaction id in connection page (for abort)
    m_connectionPage.SetTxId(reqRCP->GetTxId());
}

// --------------------------------------------------------------------------
// ProcessWDSDeleteProfileReq
//
/// Actions to take when a WDSDeleteProfileReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSDeleteProfileReq(MessageRCP msgRCP)
{
    WDSDeleteProfileReqRCP reqRCP = rcp_cast<WDSDeleteProfileReq*>(msgRCP);

    // call ProfilesPage process method
    m_profilesPage.ProcessWDSDeleteProfileReq(reqRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileSettingsReq
//
/// Actions to take when a WDSGetProfileSettingsReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSGetProfileSettingsReq(MessageRCP msgRCP)
{
    WDSGetProfileSettingsReqRCP reqRCP = 
        rcp_cast<WDSGetProfileSettingsReq*>(msgRCP);

    // call ProfilesPage process method
    m_profilesPage.ProcessWDSGetProfileSettingsReq(reqRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetDefaultSettingsReq
//
/// Actions to take when a WDSGetDefaultSettingsReq is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSGetDefaultSettingsReq(MessageRCP msgRCP)
{
    WDSGetDefaultSettingsReqRCP reqRCP = 
        rcp_cast<WDSGetDefaultSettingsReq*>(msgRCP);

    // call ProfilesPage process method
    m_profilesPage.ProcessWDSGetDefaultSettingsReq(reqRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSSetEventReportRsp
//
/// Actions to take when a WDSSetEventReportRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSSetEventReportRsp(MessageRCP msgRCP)
{
    WDSSetEventReportRspRCP rspRCP = rcp_cast<WDSSetEventReportRsp*>(msgRCP);

    // verify msg successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSSetEventReportReq",rspRCP->GetError(),NO_MSG_BOX);
    }
}

// --------------------------------------------------------------------------
// ProcessDMSSetEventReportRsp
//
/// Actions to take when a DMSSetEventReportRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSSetEventReportRsp(MessageRCP msgRCP)
{
    DMSSetEventReportRspRCP rspRCP = rcp_cast<DMSSetEventReportRsp*>(msgRCP);

    // verify msg successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSSetEventReportReq",rspRCP->GetError(),NO_MSG_BOX);
    }
}

// --------------------------------------------------------------------------
// ProcessNASSetEventReportRsp
//
/// Actions to take when a NASSetEventReportRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessNASSetEventReportRsp(MessageRCP msgRCP)
{
    NASSetEventReportRspRCP rspRCP = rcp_cast<NASSetEventReportRsp*>(msgRCP);

    // verify msg successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("NASSetEventReportReq",rspRCP->GetError(),NO_MSG_BOX);
    }
}

// --------------------------------------------------------------------------
// ProcessWDSEventReportInd
//
/// Actions to take when a WDSEventReportInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSEventReportInd(MessageRCP msgRCP)
{
    WDSEventReportIndRCP indRCP = rcp_cast<WDSEventReportInd*>(msgRCP);

    // call StatisticsPage process method
    m_statisticsPage.ProcessWDSEventReportInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSEventReportInd
//
/// Actions to take when a DMSEventReportInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSEventReportInd(MessageRCP msgRCP)
{
    DMSEventReportIndRCP indRCP = rcp_cast<DMSEventReportInd*>(msgRCP);

    // call ConnectionPage process method
    m_connectionPage.ProcessDMSEventReportInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessNASEventReportInd
//
/// Actions to take when a NASEventReportInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessNASEventReportInd(MessageRCP msgRCP)
{
    NASEventReportIndRCP indRCP = rcp_cast<NASEventReportInd*>(msgRCP);

    // call ConnectionPage process method
    m_connectionPage.ProcessNASEventReportInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetPowerStateRsp
//
/// Actions to take when a DMSGetPowerStateRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetPowerStateRsp(MessageRCP msgRCP)
{
    DMSGetPowerStateRspRCP rspRCP = rcp_cast<DMSGetPowerStateRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetPowerStateReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessDMSGetPowerStateRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceMfrRsp
//
/// Actions to take when a DMSGetDeviceMfrRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetDeviceMfrRsp(MessageRCP msgRCP)
{
    DMSGetDeviceMfrRspRCP rspRCP = rcp_cast<DMSGetDeviceMfrRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceMfrReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceMfrRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceModelIdRsp
//
/// Actions to take when a DMSGetDeviceModelIdRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetDeviceModelIdRsp(MessageRCP msgRCP)
{
    DMSGetDeviceModelIdRspRCP rspRCP = rcp_cast<DMSGetDeviceModelIdRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceModelIdReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceModelIdRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceRevIdRsp
//
/// Actions to take when a DMSGetDeviceRevIdRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetDeviceRevIdRsp(MessageRCP msgRCP)
{
    DMSGetDeviceRevIdRspRCP rspRCP = rcp_cast<DMSGetDeviceRevIdRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceRevIdReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceRevIdRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetMsisdnRsp
//
/// Actions to take when a DMSGetMsisdnRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetMsisdnRsp(MessageRCP msgRCP)
{
    DMSGetMsisdnRspRCP rspRCP = rcp_cast<DMSGetMsisdnRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetMsisdnReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetMsisdnRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceSerialNumbersRsp
//
/// Actions to take when a DMSGetDeviceSerialNumbersRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetDeviceSerialNumbersRsp(MessageRCP msgRCP)
{
    DMSGetDeviceSerialNumbersRspRCP rspRCP = rcp_cast<DMSGetDeviceSerialNumbersRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceSerialNumbersReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceSerialNumbersRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessDMSGetDeviceCapRsp
//
/// Actions to take when a DMSGetDeviceCapRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDMSGetDeviceCapRsp(MessageRCP msgRCP)
{
    DMSGetDeviceCapRspRCP rspRCP = rcp_cast<DMSGetDeviceCapRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("DMSGetDeviceCapReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statusPage.ProcessDMSGetDeviceCapRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessNASGetSignalStrengthRsp
//
/// Actions to take when a NASGetSignalStrengthRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessNASGetSignalStrengthRsp(MessageRCP msgRCP)
{
    NASGetSignalStrengthRspRCP rspRCP = rcp_cast<NASGetSignalStrengthRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("NASGetSignalStrengthReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessNASGetSignalStrengthRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessNASGetServingSystemRsp
//
/// Actions to take when a NASGetServingSystemRsp message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessNASGetServingSystemRsp(MessageRCP msgRCP)
{
    NASGetServingSystemRspRCP rspRCP = rcp_cast<NASGetServingSystemRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("NASGetServingSystemReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessNASGetServingSystemRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessNASServingSystemInd
//
/// Actions to take when a NASServingSystemInd message is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessNASServingSystemInd(MessageRCP msgRCP)
{
    NASServingSystemIndRCP indRCP = rcp_cast<NASServingSystemInd*>(msgRCP);

    // call Page process methods
    m_connectionPage.ProcessNASServingSystemInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSAbortRsp
//
/// Actions to take when a WDSAbortRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSAbortRsp(MessageRCP msgRCP)
{
    WDSAbortRspRCP rspRCP = rcp_cast<WDSAbortRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSAbortReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page enable methods
    m_connectionPage.DisableConnectedState();
}

// --------------------------------------------------------------------------
// ProcessWDSStartRsp
//
/// Actions to take when a WDSStartNetworkInterfaceRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSStartRsp(MessageRCP msgRCP)
{
    WDSStartNetworkInterfaceRspRCP rspRCP = 
        rcp_cast<WDSStartNetworkInterfaceRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSStartNetworkInterfaceReq",rspRCP->GetError(),MSG_BOX);

     
		// call Page disable methods
        m_connectionPage.DisableConnectedState();
        return;
    }
  
    // call Page enable methods
    m_connectionPage.EnableConnectedState(rspRCP->GetPktDataHandle());
    m_statisticsPage.EnableConnectedState();
}

// --------------------------------------------------------------------------
// ProcessWDSStopRsp
//
/// Actions to take when a WDSStopNetworkInterfaceRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSStopRsp(MessageRCP msgRCP)
{
    WDSStopNetworkInterfaceRspRCP rspRCP = 
        rcp_cast<WDSStopNetworkInterfaceRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSStopNetworkInterfaceReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page disable methods
    m_connectionPage.DisableConnectedState();
    m_statisticsPage.DisableConnectedState();
    
    // !!! call msgs to end receiving stats, have stats page handle?
}

// --------------------------------------------------------------------------
// ProcessWDSPktSrvcStatusInd
//
/// Actions to take when a WDSPktServiceStatusInd is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSPktSrvcStatusInd(MessageRCP msgRCP)
{
    WDSPktSrvcStatusIndRCP indRCP = rcp_cast<WDSPktSrvcStatusInd*>(msgRCP);

    // call Page process methods
    m_connectionPage.ProcessWDSPktSrvcStatusInd(indRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetCurrentChannelRateRsp
//
/// Actions to take when a WDSGetCurrentChannelRateRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSGetCurrentChannelRateRsp(MessageRCP msgRCP)
{
    WDSGetCurrentChannelRateRspRCP rspRCP = 
        rcp_cast<WDSGetCurrentChannelRateRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetCurrentChannelRateReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_statisticsPage.ProcessWDSGetCurrentChannelRateRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSCreateProfileRsp
//
/// Actions to take when a WDSCreateProfileRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSCreateProfileRsp(MessageRCP msgRCP)
{
    WDSCreateProfileRspRCP rspRCP = rcp_cast<WDSCreateProfileRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSCreateProfileReq",rspRCP->GetError(),MSG_BOX);
    }

    // call Page process methods
    m_profilesPage.ProcessWDSCreateProfileRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSModifyProfileSettingsRsp
//
/// Actions to take when a WDSModifyProfileSettingsRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSModifyProfileSettingsRsp(MessageRCP msgRCP)
{
    WDSModifyProfileSettingsRspRCP rspRCP = 
        rcp_cast<WDSModifyProfileSettingsRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSModifyProfileSettingsReq",rspRCP->GetError(),MSG_BOX);
    }

    // call Page process methods
    m_profilesPage.ProcessWDSModifyProfileSettingsRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSDeleteProfileRsp
//
/// Actions to take when a WDSDeleteProfileRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSDeleteProfileRsp(MessageRCP msgRCP)
{
    WDSDeleteProfileRspRCP rspRCP = rcp_cast<WDSDeleteProfileRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSDeleteProfileReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page process methods
    m_profilesPage.ProcessWDSDeleteProfileRsp();
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileListRsp
//
/// Actions to take when a WDSGetProfileListRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSGetProfileListRsp(MessageRCP msgRCP)
{
    WDSGetProfileListRspRCP rspRCP = rcp_cast<WDSGetProfileListRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetProfileListReq",rspRCP->GetError(),NO_MSG_BOX);
        return;
    }

    // call Page process methods
    m_connectionPage.ProcessWDSGetProfileListRsp(rspRCP);
    m_profilesPage.ProcessWDSGetProfileListRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileSettingsRsp
//
/// Actions to take when a WDSGetProfileSettingsRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSGetProfileSettingsRsp(MessageRCP msgRCP)
{
    WDSGetProfileSettingsRspRCP rspRCP = 
        rcp_cast<WDSGetProfileSettingsRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetProfileSettingsReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page process methods
    m_profilesPage.ProcessWDSGetProfileSettingsRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessWDSGetDefaultSettingsRsp
//
/// Actions to take when a WDSGetDefaultSettingsRsp is received.
///
/// @params msgRCP - Message reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessWDSGetDefaultSettingsRsp(MessageRCP msgRCP)
{
    WDSGetDefaultSettingsRspRCP rspRCP = 
        rcp_cast<WDSGetDefaultSettingsRsp*>(msgRCP);

    // verify message was successful
    if (rspRCP->GetResult() != QMI_RESULT_SUCCESS)
    {
        ReportMsgFailed("WDSGetDefaultSettingsReq",rspRCP->GetError(),MSG_BOX);
        return;
    }

    // call Page process methods
    m_profilesPage.ProcessWDSGetDefaultSettingsRsp(rspRCP);
}

// --------------------------------------------------------------------------
// ProcessStatusNotice
//
/// Actions to take when a StatusNotice is received from the MessageManager.
///
/// @params noticeRCP - Notice reference counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessStatusNotice(NoticeRCP noticeRCP)
{
    StatusNoticeRCP statusNoticeRCP = rcp_cast<StatusNotice*>(noticeRCP);
    uint8 severity = statusNoticeRCP->GetSeverity();
	USES_CONVERSION;
    if (severity == ST_ERROR)
    {
		/*
		char *pszText = NULL; 
		char szText[MAX_PATH] = { 0 }; 

		pszText = statusNoticeRCP->GetEvent().c_str(); 
		memcpy(szText, pszText, strlen(pszText)); 
		*/

		//TRACE(_T("%s\n"), statusNoticeRCP->GetEvent().c_str()); 
	
        AfxMessageBox(A2W(statusNoticeRCP->GetEvent().c_str()));
    }

    // pass to status page for processing
    m_statusPage.AddEventMessage
                 (
                     statusNoticeRCP->GetEvent(),
                     statusNoticeRCP->GetSeverity()
                 );
}

// --------------------------------------------------------------------------
// ProcessDeviceNotice
//
/// Actions to take when a DeviceNotice is received from the MessageManager.
///
/// @params noticeRCP - Notice refetence counted pointer to process.
// --------------------------------------------------------------------------
void CHSDPADlg::ProcessDeviceNotice(NoticeRCP noticeRCP)
{
    DeviceNoticeRCP deviceNoticeRCP = rcp_cast<DeviceNotice*>(noticeRCP);
    uint8 type = deviceNoticeRCP->GetType();
    std::stringstream stream;
    std::vector<std::string> networkAdapters = deviceNoticeRCP->GetNetworkAdapters();
    int size = (int)networkAdapters.size();
	CStdioFile file;
	// @@
	int i = 0;
	//CString CStrReason;
	USES_CONVERSION;
    switch (type)
    {
        case DT_AVAILABLE:
            stream.str("");
            stream << _T("Attached Network Adapters: ") << std::endl;

            if (size == 0)
            {
                stream << _T("None Detected") << std::endl;

				if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
				{
					
					DWORD dwActual = file.SeekToEnd();
					file.WriteString((TCHAR *)deviceNoticeRCP->GetReason().c_str());
					file.Close();		
				}
			
				//AfxMessageBox(deviceNoticeRCP->GetReason().c_str());
				m_ProcessDeviceNotice = FALSE;
            }
            for (/* @@ int */ i = 0; i < size; i++)
            {
                stream << networkAdapters[i] << std::endl;
            }

            stream << std::endl;
			if((strcmpi(networkAdapters[0].c_str(),"Qualcomm Wireless Ethernet Adapter 7002")<0)
            && (strcmpi(networkAdapters[0].c_str(),"HSDPA Wireless Ethernet Adapter 7002" )<0)
			&&(strcmpi(networkAdapters[0].c_str(),"HSPA Wireless Ethernet Adapter 7002")<0))
			{
				if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
				{
					
					DWORD dwActual = file.SeekToEnd();
					file.WriteString((TCHAR *)networkAdapters[0].c_str());
					file.Close();		
				}
				//AfxMessageBox(deviceNoticeRCP->GetReason().c_str());
				m_ProcessDeviceNotice = FALSE;

			}
            m_statusPage.AddEventMessage(stream.str(),ST_INFORMATION);

            // update the available devices
            m_connectionPage.PopulateDeviceNames(networkAdapters);
            break;

        case DT_CONNECT:
            // add event to status page
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_INFORMATION);
            
            // direct pages to enable appropriate fields
            m_connectionPage.EnableSelectedState();
            m_profilesPage.EnableSelectedState();
            m_statisticsPage.EnableSelectedState();
            m_statusPage.EnableSelectedState();
	        m_ProcessDeviceNotice = TRUE;
			
            EndWaitCursor();
            break;

        case DT_CONNECT_FAIL:
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_ERROR);

            // direct pages to disable appropriate fields
            m_connectionPage.DisableSelectedState();
            m_profilesPage.DisableSelectedState();
            m_statisticsPage.DisableSelectedState();
            m_statusPage.DisableSelectedState();

            EndWaitCursor();

            // do all work before displaying message box

			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString((TCHAR *)deviceNoticeRCP->GetReason().c_str());
				file.Close();		
			}
            //AfxMessageBox(deviceNoticeRCP->GetReason().c_str());
	        m_ProcessDeviceNotice = FALSE;
            break;

        case DT_DISCONNECT:
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_INFORMATION);

            // direct pages to disable appropriate fields
            m_connectionPage.DisableSelectedState();
            m_profilesPage.DisableSelectedState();
            m_statisticsPage.DisableSelectedState();
            m_statusPage.DisableSelectedState();
		    //m_ProcessDeviceNotice = FALSE;
            EndWaitCursor();
            break;

        case DT_DETACH:
            m_statusPage.AddEventMessage(deviceNoticeRCP->GetReason(),ST_WARNING);

            // update the available devices
            m_connectionPage.PopulateDeviceNames(networkAdapters);
            
            // direct pages to disable appropriate fields
            m_connectionPage.DisableSelectedState();
            m_profilesPage.DisableSelectedState();
            m_statisticsPage.DisableSelectedState();
            m_statusPage.DisableSelectedState();

			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString(A2W(deviceNoticeRCP->GetReason().c_str()));
				file.Close();		
			}
            //AfxMessageBox(deviceNoticeRCP->GetReason().c_str());
		    m_ProcessDeviceNotice = FALSE;
            break;

        default:
			if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
			{
				
				DWORD dwActual = file.SeekToEnd();
				file.WriteString(_T("Error: Unknown device notice received."));
				file.Close();		
			}
			break;
            //AfxMessageBox(_T("Error: Unknown device notice received."));
    }
}

// --------------------------------------------------------------------------
// LogMessage
//
/// Write a message to the status page.
///
/// @params msgRCP - Message reference counted pointer.
// --------------------------------------------------------------------------
void CHSDPADlg::LogMessage(MessageRCP msgRCP)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    std::stringstream stream;
    stream << _T("Time: ") << (uint32)time.wHour << _T(":")
           << (uint32)time.wMinute << _T(":")
           << (uint32)time.wSecond << _T(".")
           << (uint32)time.wMilliseconds << std::endl
           << _T("Transaction Id: ") << (uint32)msgRCP->GetTxId() << std::endl
           << std::endl;
    msgRCP->Print(stream);
    msgRCP->PrintMsgBuf(stream);
    stream << std::endl;
    m_statusPage.AddEventMessage(stream.str(),ST_INFORMATION);
}

// --------------------------------------------------------------------------
// ReportMsgFailed
//
/// Report a message failure to the status page, popup dialog when requested.
///
/// @params msgRCP - Message reference counted pointer.
// --------------------------------------------------------------------------
void CHSDPADlg::ReportMsgFailed(std::string name,uint16 error,bool isMsgBox)
{
    std::stringstream stream;
    stream << _T("Error: ") << name << _T(" failed with error '")
           << ERROR_STRINGS[error] << _T("'.");
    
    if (isMsgBox) 
	{ 		

		CStdioFile file;
		if(file.Open(_T("NDISErrDebug.log"), CFile::modeReadWrite))
		{
			
			DWORD dwActual = file.SeekToEnd();
			file.WriteString((TCHAR *)stream.str().c_str());
			file.Close();		
		}

	}
    
    stream << std::endl << std::endl;
    m_statusPage.AddEventMessage(stream.str(),ST_ERROR);
}

void CHSDPADlg::SetBottomIconPos()
{
	CRect rc, itemRect;
	GetClientRect(&rc);
	int x = 10;

	GetDlgItem(IDC_UI_ICON_RSSI)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_RSSI)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_PRIVACY_CALL)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_PRIVACY_CALL)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_SMS_UNRD)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_SMS_UNRD)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_SMS_FULL)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_SMS_FULL)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_NETWK_TYPE)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_NETWK_TYPE)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_NWSRV)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_NWSRV)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_ROAM)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_ROAM)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_HANDSET)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_HANDSET)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_VOICESMS)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_VOICESMS)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_PLMN)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_PLMN)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());

	x += itemRect.Width();
	GetDlgItem(IDC_UI_ICON_PLMN2)->GetWindowRect(itemRect);
	GetDlgItem(IDC_UI_ICON_PLMN2)->MoveWindow(x, rc.bottom-16, itemRect.Width(), itemRect.Height());
}

EnSyncInitFuncRetType CHSDPADlg::SndAtPowerCFUN(EnPowerType nPowerType)
{
    ASSERT(nPowerType == POWER_ON || nPowerType == POWER_OFF);
    const char szATSetPower[]="AT";
    char szAtBuf[50] = {0};
    
    sprintf(szAtBuf, "%s\r", szATSetPower);
	
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
  //      SetTimer(IDT_CFUN_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtPowerCFUN, this);
		
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSyncInitEvt, 30000))
            return SYNCINITFUNCRET_DONE;
        else
            return SYNCINITFUNCRET_RSP_TO;
    }
    else
        return SYNCINITFUNCRET_SND_ERR;
}

void CHSDPADlg::RspAtPowerCFUN(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CHSDPADlg* pDlg = (CHSDPADlg*)pWnd;
    pDlg->KillTimer(IDT_CFUN_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
        //pDlg->PostMessage(WN_SETUP_UPDATE_POWERON_OFF, (WPARAM)TRUE, (LPARAM)TRUE);
    }
    else
    {
        //设置失败
        AfxMessageBox(IDS_RF_SETERROR);
    }    
    //((CHSDPADlg*)theApp.GetMainWnd())->AtSndCOPS();
	SetEvent(((CHSDPADlg*)pWnd)->m_hSyncInitEvt);
}

LRESULT CHSDPADlg::OnInternetSetup(WPARAM wParam, LPARAM lParam)
{
//	CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	//added
	if(g_SetData.Setup_nAutoConnect)		//wyw_0409 add
	{   
		m_pConnectDlg->OnClickConnectButton();
	}
	return NULL;
}


LRESULT CHSDPADlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	CDlgSubversion dlgSubversion;
	dlgSubversion.DoModal();
	return 0;
}