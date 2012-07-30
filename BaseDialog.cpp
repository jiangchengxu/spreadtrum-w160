/*
Desc:
    为本工程所有对话框类的基类，
    功能包括：
    1、窗口及窗口中控件的重画
    2、回调、消息处理
    3、窗口启动效果，可以通过设置m_blUseFlashEffect，来决定是否启用渐变的启动效果
    4、窗口背景
    5、网络消息的注册、分发
*/

// BaseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "BaseDialog.h"
#include "AboutDialog.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CBaseDialog::m_bWinSize = false; //窗口是否最大化显示

//extern BOOL LoadIniFile();
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#define     GetWindowStyle(hwnd)    ((DWORD)GetWindowLong(hwnd, GWL_STYLE))

BOOL CALLBACK EnumChildProc(  HWND hwnd,      // handle to child window
                            LPARAM lParam   // application-defined value
                            )
{
    TCHAR classname[200];
	TCHAR ctrltext[20]={0};
    CBaseDialog *dlg = (CBaseDialog *)lParam;
    DWORD style;
    
    GetClassName( hwnd, classname, 200 );
    style = GetWindowStyle( hwnd );
	///////
	/*
	GetDlgItemText(hwnd,IDC_BUTTON_NUM_HASH,ctrltext,19);
	if (strcmp(ctrltext, "#") == 0)
	{
		dlg->SubClassButton( hwnd );
	}
	*/
	///////
    if (wcscmp(classname, _T("Button")) == 0)
    {
        style = (UINT)GetWindowLong(hwnd, GWL_STYLE) & 0xff;
//BS_PUSHBUTTON:发送消息按钮
//Creates a pushbutton that posts a WM_COMMAND message to the owner window 
//when the user selects the button.

//BS_DEFPUSHBUTTON:默认按钮
//Creates a button that has a heavy black border. 
//The user can select this button by pressing the ENTER key. 
//This style enables the user to quickly select the most likely option 
//(the default option).
        if ( style == BS_PUSHBUTTON || style == BS_DEFPUSHBUTTON )
        {
            dlg->SubClassButton( hwnd );            
        }
		
    }
    
    return TRUE;
}

/************************************************************************/
/*
Name:    DispatchResponseProc
Desc:    處理从底层返回的数据,根据传入的类对象指针来做分发
Param:    pCaller:  Caller's pointer
        lParam,   
        wParam

Return:     TRUE:成功
         其它:      失败        
*/
/*************************************************************************/
BOOL CALLBACK DispatchResponseProc(void* pCaller,      // Caller
                           LPARAM lParam,   // application-defined value
                           WPARAM wParam   // application-defined value
                           )
{    
    CBaseDialog *dlg = (CBaseDialog *)pCaller;
    
    dlg->DealwithResponseProc(lParam, wParam);            
    
    return TRUE;
}


IMPLEMENT_DYNAMIC(CBaseDialog, CDialog)

CBaseDialog::CBaseDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
    : CDialog(nIDTemplate, pParent)
{
    //{{AFX_DATA_INIT(CBaseDialog)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
 
    m_brBkgnd.CreateSolidBrush(RGB(238,241,243)); 
    
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_blUseFlashEffect = FALSE;
    m_blUseFlashEffectFinished = FALSE;
    m_OriginRect.SetRect(0,0,0,0);
    m_bstatus = FALSE;
    m_nBkTag = 0;
	m_bPreMsgBox = FALSE;
	g_MainDlgRect = CRect(0,0,817,528+MENU_HEIGHT);//liub_modify
	g_MainDlgMinRect   = CRect(0,0,817,148);
	g_TabDlgRect       = CRect(0,0,817,484+MENU_HEIGHT);//liub_modify

	m_skinWin.m_rectRestoreWin = g_MainDlgRect;

}

CBaseDialog::~CBaseDialog()
{
    POSITION pos= m_wndList.GetHeadPosition();
    CButton* pBtn = NULL;
    
    while (pos) 
    {
        pBtn = (CButton*)m_wndList.GetNext(pos);
        if (pBtn->GetSafeHwnd() != NULL) 
        {
            pBtn->UnsubclassWindow();
        }
        
        delete pBtn;
    }
    m_wndList.RemoveAll();
}

bool CBaseDialog::CompareChar(const TCHAR* chSource)
{
	while(*chSource)
	{ 
		UINT nChar = *chSource;
		if((nChar >= '0' && nChar <= '9' || nChar == 8) || nChar == '*' 
        || nChar == '#' || nChar == '+')
		{			
		}
		else
		{
			CString strTemp;
			strTemp.LoadString(IDS_PB_IMPORTTIP);
			AfxMessageBox(strTemp);
			return false;
		}

		chSource++;
	}
	return true;

}

bool CBaseDialog::CompareCharAnsi(const char* chSource)
{
	while(*chSource)
	{ 
		UINT nChar = *chSource;
		if((nChar >= '0' && nChar <= '9' || nChar == 8) || nChar == '*' 
        || nChar == '#' || nChar == '+')
		{			
		}
		else
		{
			CString strTemp;
			strTemp.LoadString(IDS_PB_IMPORTTIP);
			AfxMessageBox(strTemp);
			return false;
		}

		chSource++;
	}
	return true;

}
void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBaseDialog)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBaseDialog, CDialog)
    //{{AFX_MSG_MAP(CBaseDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_QUERYDRAGICON()    
    ON_WM_TIMER()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP    
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseDialog message handlers

BOOL CBaseDialog::OnInitDialog()
{
    //SetTextBkColor(RGB(238,241,243));
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
	//del MAX
	if (pSysMenu != NULL)
	{
		pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);//add by liub for 标题栏右键菜单删除最大化
	}

    if (m_bstatus)
    {
        if (pSysMenu != NULL)
        {
            CString strAboutMenu;
			CString temp;
            temp.LoadString(IDS_ABOUTBOX);
			strAboutMenu.Format(temp, g_SetData.Main_szPrefixName);
            if (!strAboutMenu.IsEmpty())
            {
                pSysMenu->AppendMenu(MF_SEPARATOR);
                pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
            }
        }
		//help  
        CString strHelpMenu;
        strHelpMenu.LoadString(IDS_HELPDOC);
        if (!strHelpMenu.IsEmpty())
        {
			 if(g_SetData.Main_nHelp)	
				pSysMenu->AppendMenu(MF_STRING, IDS_HELPDOC, strHelpMenu);
        }

    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // TODO: Add extra initialization here
    LoadIniFile();
    m_hIconInfo = (HICON)::LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_INFO), RT_GROUP_ICON), MAKEINTRESOURCE(IDI_ICON_INFO), IMAGE_ICON, 0, 0, 0);

	if(m_nBkTag == 0) 
	{
		if(m_bWinSize)
		{
			m_bmpBackground.LoadBitmap(/*IDB_BACK_MAIN*/IDB_BACK_CON);
		}
		else
		{
			m_bmpBackground.LoadBitmap(IDB_BACK_MAIN_DEFAULT);
		}
	}
	else if(m_nBkTag == 1)
	{
		m_bmpBackground.LoadBitmap(IDB_BACK_PAGE1);
	}
	else if(m_nBkTag == 2)
	{
		if(m_bWinSize)
		{
			m_bmpBackground.LoadBitmap(IDB_BACK_CON);
		}
		else
		{
			m_bmpBackground.LoadBitmap(IDB_BACK_CON_DEFAULT);
		}
		
		CPaintDC dc(this);
        CDC dcMem; 
        dcMem.CreateCompatibleDC(&dc); 
        BITMAP bitMap;
        m_bmpBackground.GetBitmap(&bitMap);
        CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);

		CDC dctemp;
		dctemp.CreateCompatibleDC(&dcMem);
		CBitmap *m_pbmpTemp;
//*		
		HBITMAP m_hBitmap;        // Handle of the bitmap
		// Load the image
		m_hBitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),
			g_SetData.Main_szLogoBmp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		m_pbmpTemp=CBitmap::FromHandle(m_hBitmap);
//*/
//		m_bmpTemp.LoadBitmap(IDB_LOGO);
		CBitmap *pbmpOld1=dctemp.SelectObject(m_pbmpTemp);

		#ifdef FEATURE_UICOLOR_GREY		
			dcMem.BitBlt(g_TabDlgRect.Width() - 130,2,bitMap.bmWidth,bitMap.bmHeight,
					&dctemp,0,0,SRCCOPY);
		#else
			dcMem.BitBlt(g_TabDlgRect.Width() - 130,1,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);
			//dcMem.BitBlt(g_TabDlgRect.Width() - 130,1,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);
		#endif
	}
    //m_bmpBackground.LoadBitmap(IDB_BACK_MAIN+m_nBkTag);
    //m_bmpBackground.LoadBitmap(IDB_BACK_MAIN);
        
 //   m_Tip.Create(this);
    
    if (m_blUseFlashEffect){
        InitDlgWithFlashEffect(); //渐变效果
    }else{
        ActionAfterInitDlgWithFlashEffect();
    }
    
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}


BOOL CBaseDialog::PreTranslateMessage(MSG* pMsg) 
{
    //m_Tip.RelayEvent(pMsg);
    
    return CDialog::PreTranslateMessage(pMsg);
}


void CBaseDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
    switch ( nID )
    {
        case IDM_ABOUTBOX:
        {
            CAboutDialog  dlgAbout;
            dlgAbout.DoModal();

            break;
        }
        case IDS_HELPDOC:
        {
			//App ICON -> Menu -> Help
			CWnd *pWndPrev, *pWndChild;
			CString str, strHelpTitle;
			str.LoadString(IDS_APPNAME);
			AddPrefixName(str);
			strHelpTitle.Format(_T("%s %s"), str, _T("Help"));
			
			if(pWndPrev = CWnd::FindWindow(NULL, strHelpTitle))
			{
				pWndChild = pWndPrev->GetLastActivePopup();                     
				pWndPrev->ShowWindow(SW_RESTORE);
				pWndChild->SetForegroundWindow();    
			}
			else
			{
				//Press Help button
				
				//CString str = AfxGetApp()->m_pszHelpFilePath;
				//int iPos = str.ReverseFind('\\');    
				CString    strHelpFile;// = str.Left(iPos);
				//strHelpFile += "\\DataCardHelp.chm";
				//strHelpFile = "DataCardHelp.chm";
				strHelpFile=g_SetData.Main_szHelpFile;
				
				CString CInfo;
				if( (_waccess( strHelpFile, 0 )) == -1 )
				{
					CInfo.LoadString(IDS_HELPINFO);
					AfxMessageBox(CInfo);
				}
				
				ShellExecute(NULL, _T("open"),strHelpFile,    NULL, NULL, SW_SHOWNORMAL); 
			}  
            break;
        }
        default:
        {
            CDialog::OnSysCommand(nID, lParam);

            break;
        }
    }
    
}

void CBaseDialog::OnDestroy()
{
    m_bmpBackground.DeleteObject();
    ::DestroyIcon(m_hIconInfo);
    WinHelp(0L, HELP_QUIT);
    CDialog::OnDestroy();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBaseDialog::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void CBaseDialog::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
    CRect dlgRect;
    GetWindowRect(dlgRect);
    
    CRect desktopRect;
    GetDesktopWindow()->GetWindowRect(desktopRect);
        
    if(nIDEvent == IDT_TIMER_FLASHDIALOG)
    {                
        MoveWindow(
            (-m_dx+desktopRect.Width() - dlgRect.Width()) / 2,
            (-m_dy+desktopRect.Height() - dlgRect.Height()) / 2, 
            +m_dx+dlgRect.Width(),
            +m_dy+dlgRect.Height() );
        
        if(dlgRect.Width() >=m_nWidth) 
            m_dx=0; // do not over grow
        if(dlgRect.Height() >=m_nHeight)
            m_dy=0; // do not over grow

        if((dlgRect.Width() >=m_nWidth) && (dlgRect.Height() >=m_nHeight))
        {
            ::KillTimer(this->m_hWnd, IDT_TIMER_FLASHDIALOG); //Stop the timer
            m_blUseFlashEffectFinished = TRUE;
            ActionAfterInitDlgWithFlashEffect();
        }
    }
    CDialog::OnTimer(nIDEvent);
}

/************************************************************************/
/*
Name:    InitDlgWithFlashEffect
Desc:    开始启动时的渐变效果
Param:    

Return:     

*/
/*************************************************************************/

void CBaseDialog::InitDlgWithFlashEffect()
{
    CRect dlgRect; 
    GetWindowRect(dlgRect); 
    CRect desktopRect; 
    GetDesktopWindow()->GetWindowRect(desktopRect); 

    //MoveWindow(x值，y值，窗口宽，窗口高)
    //先画一下
    MoveWindow( 
        (desktopRect.Width() - dlgRect.Width()) / 2, 
        (desktopRect.Height() - dlgRect.Height()) / 2, 
        dlgRect.Width(), dlgRect.Height() ); 

    //再擦掉，函数本身默认repaint==true
    MoveWindow( 
        (desktopRect.Width() - dlgRect.Width()) / 2, 
        (desktopRect.Height() - dlgRect.Height()) / 2, 
        0, 
        0 ); 
    
    //m_nWidth m_nHeight m_dx m_dy 是该类的保护成员
    //m_nWidth m_nHeight 是最终宽和高，调整成这样可能是为了好看
    //m_dx表示窗口在x轴上的渐变值，m_dy 表示窗口在y轴上的渐变值
    m_nWidth=dlgRect.Width() - 60; 
    m_nHeight=dlgRect.Height() - 80; 
    m_dx=40; 
    m_dy=25 ; 

    ::SetTimer(this->m_hWnd, IDT_TIMER_FLASHDIALOG,1, NULL); 
    
}

CString CBaseDialog::CmGetAppPath()
{
	CString strPath;
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer();
	int nPos;
	nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);
	return strPath;
}

/************************************************************************/
/*
Name:    SetSkin
Desc:    为窗口设置显示皮肤
Param:    

Return:     TRUE:成功
         其它:      失败        

*/
/*************************************************************************/
BOOL CBaseDialog::SetSkin(CString file)
{

    m_skinWin.LoadSkin( file );
    m_btnres.LoadSkin( file ,"Bitmap");// 按钮的图标  //liub modify
    
    //m_bFirst 保护成员
    if ( m_bFirst )
    {
        m_skinWin.InstallSkin( this );
        //父窗口，回调函数，回调函数接收的消息:与左边的按钮有关
        EnumChildWindows( m_hWnd, EnumChildProc, (LPARAM)this );
        //只能设置一次
        m_bFirst = FALSE;
    }
	else if(m_bPreMsgBox)
	{
		//DWORD style = GetWindowLong( m_hWnd, GWL_STYLE )
		//SetWindowLong(m_hWnd,GWL_STYLE, style&(~WS_SYSMENU));
		m_skinWin.InstallSkin(this);
		m_bPreMsgBox=FALSE;
    }
//相当于擦除
    SetWindowPos( 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE |SWP_FRAMECHANGED );
    return TRUE;
    
}

/************************************************************************/
/*
Name:    SubClassButton
Desc:    为每一个窗口上的BUTTON设置属性
Param:    

Return:     TRUE:成功
         其它:      失败        

*/
/*************************************************************************/

BOOL CBaseDialog::SubClassButton( HWND hwnd )
{
    //添加按钮
    CSkinButton * btn = new CSkinButton();
    m_wndList.AddTail(btn);
    
    CWnd* pWnd = CWnd::FromHandlePermanent(hwnd);
    if ( pWnd == NULL)
    {
        CString strCaption;

        btn->SubclassWindow( hwnd );
        btn->SetResource( &m_btnres );

        pWnd = CWnd::FromHandlePermanent(hwnd);
        pWnd->GetWindowText(strCaption);
        //添加提示
		// m_Tip.AddTool(pWnd, strCaption, m_hIconInfo);
        return TRUE;
    }
    return FALSE;
}

BOOL CBaseDialog::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

void CBaseDialog::OnPaint() 
{
    if (IsIconic())  // minimized
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
        CPaintDC dc(this);
        CRect rect;
        GetClientRect(&rect);
        CDC dcMem; 
        dcMem.CreateCompatibleDC(&dc); 
        BITMAP bitMap;
        m_bmpBackground.GetBitmap(&bitMap);
        CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);
		
        if (m_OriginRect == CRect(0,0,0,0)) 
        {
            dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);
        }
        else
        {
            dc.BitBlt  (0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
        }

		//wyw_0103
		dcMem.DeleteDC();        
    }
	
}

int CBaseDialog::ActionAfterInitDlgWithFlashEffect()
{    
    
    return 1;
}

/************************************************************************/
/*
Name:    DealwithResponseProc
Desc:    处理回调的例程，派生类必须覆盖本方法以支持特定的处理
Param:    strCmd    命令字符串，AT指令集

Return:     TRUE:成功
         其它:      失败        
*/
/*************************************************************************/
    
int CBaseDialog::DealwithResponseProc(LPARAM lParam,   // application-defined value
                                           WPARAM wParam   // application-defined value
                                           )
{    
    return 1;
}

/************************************************************************/
/*
Name:    PostCmd
Desc:    发送操作命令
Param:    strCmd    命令字符串，AT指令集

Return:     TRUE:成功
         其它:      失败        
*/
/*************************************************************************/

void CBaseDialog::PostCmd(CString strCmd)
{

    /*
     *    注意：SendCmd为与底层通讯的模型，其中的参数this是必须的，
     *    用于底层向上层返回时的进行分发,回调函数都通过DispatchResponseProc进行实际分发
     *            strCmd暂定必须，根据以后约定而定
    */            
    //SendCmd(this, strCmd,0);
}

/************************************************************************/
/*
Name:    RegisterForNetworkNotify
Desc:    注册网络消息，如：网络信号、短消息提示等，
Param:    nEventID    事件ID

Return:     TRUE:成功
         其它:      失败        
*/
/*************************************************************************/


void CBaseDialog::RegisterForNetworkNotify(UINT nEventID)
{
    /*
     *    注意：RegisterForNetworkNotify为与底层通讯的模型，注册网络消息
     *    其中的参数this是必须的，
     *    用于底层向上层返回时的进行分发,回调函数都通过DispatchResponseProc进行实际分发
     *            nEventID暂定必须，根据以后约定而定
    */            
    //RegisterForNetworkNotify(this, nEventID,0);

}

HBRUSH CBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    
    if(((nCtlColor==CTLCOLOR_STATIC)||(pWnd->GetDlgCtrlID()==IDC_STATIC))
        && (pWnd->GetDlgCtrlID()!= IDC_SLIDER_VOLUME) && pWnd->GetDlgCtrlID() != IDC_EDIT_SMSCONTENT/*&&(pWnd->GetDlgCtrlID()!=IDC_UI_ICON_RSSI)*/)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)(m_brBkgnd.GetSafeHandle());
    }
    
    return hbr;
}

//use g_SetData.Setup_nSkinStyle to load skin file name
BOOL CBaseDialog::SetSkin(int nSkinType)
{
	CString strSkin;
    switch(nSkinType)
	{
	case 0:
		strSkin.LoadString(IDS_APPSKINPASH);
		break;
	case 1:
		strSkin.LoadString(IDS_APPXPSKINPATH);
		break;
	}
	SetSkin(strSkin);
	return TRUE;
}
