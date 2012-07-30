/*
File: HSDPADlg.cpp 
Desc:
    窗口切换，管理其中的各窗口的显示效果
    消息转发给其各子窗口
----------    ----------    ----------
*/


#include "stdafx.h"
#include "HSDPA.h"
#include "TabDialog.h"
#include "TabedDialog.h"
#include "CallDlg.h"
#include "HSDPADlg.h"
#include "SetupDlg.h"
#include "NetStatistics.h"

#include "MenuBar.h"
#include "AboutDialog.h"

#include "Internetusagelimitdlg.h"
#include "PwdConfirmDlg.h"
#include "DiagnosticDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern CNetStatistics* m_pDlgUsage;

/////////////////////////////////////////////////////////////////////////////
// CTabDialog dialog

IMPLEMENT_DYNAMIC(CTabDialog, CBaseDialog)

CTabDialog::CTabDialog(UINT nID, CWnd* pParent /*=NULL*/)
    : CBaseDialog(nID, pParent)
{
    //{{AFX_DATA_INIT(CTabDialog)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_nTotalPage = 0;
    m_pCurWnd = NULL;
	m_pCurBtn = NULL;
    m_ispeed=12;
    m_bMoveWindow = TRUE;
    m_wndheight = 0;
    //m_nStyle = TABDIALOGSTYLE_VERICAL;
    m_nStyle = TABDIALOGSTYLE_HORIZONTAL;
    m_nMoveDirection = TABDIALOG_MOVEDIRECTION_LEFTTORIGHT;
    m_nBkTag = 2;
	m_Currentbutton = -1;
	InitMenuMap();
}

CTabDialog::~CTabDialog()
{
    for(int i = 0; i < m_nTotalPage; i ++)
    {
        CWnd* pButton = NULL;
        CTabedDialog* pDialog;
        
        if(m_DialogMap.Lookup(i, (CTabedDialog*)pDialog))
        {
            delete pDialog;
        }
    }
    
    m_ButtonMap.RemoveAll();
    m_DialogMap.RemoveAll();

}
void CTabDialog::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTabDialog)
        // NOTE: the ClassWizard will add DDX and DDV calls here
//	DDX_Control(pDX, IDC_STATIC_INSTRUCTION, m_TabStaticBKground);//add by liub 1030 for tabstatic background 
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_BUTTON_SMS, m_btn1);
	if(g_SetData.Main_nCall)
	{
		DDX_Control(pDX, IDC_BUTTON_CALL, m_btn2);
	}
    DDX_Control(pDX, IDC_BUTTON_PHONEBOOK, m_btn3);
    DDX_Control(pDX, IDC_BUTTON_INTERNET, m_btn4);
    DDX_Control(pDX, IDC_BUTTON_SETUP, m_btn5);
    DDX_Control(pDX, IDC_BUTTON_HELP, m_btn6);
    DDX_Control(pDX, IDC_BUTTON_BROWSE, m_btn7);
	DDX_Control(pDX, IDC_BUTTON_USSD, m_btn8);
	DDX_Control(pDX, IDC_BUTTON_USAGE, m_btn9);

}


BEGIN_MESSAGE_MAP(CTabDialog, CBaseDialog)
    //{{AFX_MSG_MAP(CTabDialog)
    ON_WM_TIMER()
    ON_MESSAGE(IDT_TABED_SWITCHME, OnSwitchMe)
    ON_MESSAGE(IDT_CALL_DIAL, OnDial)
    ON_MESSAGE(WM_CHANGE_SKIN, ChangeSkin)
	ON_MESSAGE(WM_WINDOWS_MAX,WindowMax )
	ON_MESSAGE(WM_WINDOWS_RESTORE,WindowRestore )
	ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabDialog message handlers

//pDialog子窗口的指针,nButtonID与子窗口对应的BUTTON ID
BOOL CTabDialog::AddPage(CTabedDialog* pDialog, UINT nButtonID)
{
    CWnd*pBtnWnd = NULL;

    pBtnWnd = GetDlgItem(nButtonID);
    if (pBtnWnd == NULL){
        return FALSE;
    }

    return AddPage(pDialog, pBtnWnd);
}
//pDialog子窗口的指针,pButton与子窗口对应的按钮指针
//将指针保存到各自的MAP中    
BOOL CTabDialog::AddPage(CTabedDialog* pDialog, CWnd* pButton)
{
    ASSERT (pDialog != NULL);
    ASSERT (pButton != NULL);

    pDialog->SetButton((CShadeButtonST*)pButton);
    //为子窗口指定父窗口[容器]
   if (m_bMoveWindow)
    {
       pDialog->SetParent(GetChildDlgContainer());
    }
	
    //指定父窗口类为CTabDialog
    pDialog->SetOwner(this);
    
    //Add dialog to dialog map
    m_DialogMap.SetAt(m_nTotalPage, pDialog);

    //add button according to button map
    m_ButtonMap.SetAt(m_nTotalPage, pButton);

    //increase the total page
    m_nTotalPage ++;

    return TRUE;
}


void CTabDialog::InitMenuMap()
{
	m_MenuMap.SetAt(IDM_CONNECT,IDC_BUTTON_INTERNET);
	m_MenuMap.SetAt(IDM_WEB,IDC_BUTTON_BROWSE);
	m_MenuMap.SetAt(IDM_USAGE,IDC_BUTTON_USAGE);
	m_MenuMap.SetAt(IDM_PHONEBOOK,IDC_BUTTON_PHONEBOOK);
	m_MenuMap.SetAt(IDM_VOICE,IDC_BUTTON_CALL);
	m_MenuMap.SetAt(IDM_TEXT,IDC_BUTTON_SMS);

}

///////////////////////////////////////////////////////////////////
//the default display of the pages
//all buttons displayed and only first dialog displayed
void CTabDialog::InitPagesShow()
{
    m_bFirst = TRUE;

	SetSkin(g_SetData.Setup_nSkinStyle);

     if (m_bMoveWindow) 
     {    
        CWnd *pContainer = GetDlgItem(IDC_STATIC_DLGCONTAINER);
        CRect rect;
        //GetClientRect(&rect);
		rect = g_TabDlgRect;

        if (m_nStyle == TABDIALOGSTYLE_VERICAL) 
        {
            rect.left += BUTTON_WIDTH + 2;
        }
        else if (m_nStyle == TABDIALOGSTYLE_HORIZONTAL) 
        {
            //rect.top += BUTTON_HEIGHT + 2;
            rect.top += BUTTON_HEIGHT + 8;
        }
        
       
		pContainer->SetWindowPos(NULL, rect.left, rect.top+23+MENU_HEIGHT, rect.Width(), rect.Height(), SWP_NOZORDER |SWP_SHOWWINDOW);//liub_modify +23
    }

    InitButtonsShow();
	InitTestBoxShow();
    InitDialogShow();
}

///////////////////////////////////////////////////////////////
//show all the buttons
void CTabDialog::InitButtonsShow()
{
    for(int i = 0; i < m_nTotalPage; i ++)
    {
        CWnd* pButton = NULL;

        if(m_ButtonMap.Lookup(i, pButton)==FALSE)
            return;

        if(pButton != NULL)
        {
            /*
            if (m_nStyle == TABDIALOGSTYLE_VERICAL) 
            {
                pButton->SetWindowPos(NULL, 11, 10 + BUTTON_HEIGHT*i, BUTTON_WIDTH, BUTTON_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);
            }        
            else if (m_nStyle == TABDIALOGSTYLE_HORIZONTAL) 
            {
                pButton->SetWindowPos(NULL, 10 + BUTTON_WIDTH*i, 11, BUTTON_WIDTH, BUTTON_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);
            }
            */
            if (m_nStyle == TABDIALOGSTYLE_VERICAL) 
            {
                pButton->SetWindowPos(NULL, 0, BUTTON_HEIGHT*i, BUTTON_WIDTH, BUTTON_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);
            }        
            else if (m_nStyle == TABDIALOGSTYLE_HORIZONTAL) 
            {
                pButton->SetWindowPos(NULL, BUTTON_WIDTH*i, MENU_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);		//wyw_0206
            }
        }
    }

}
//////////////////////////////////////////////////////////////////////////add by liub for 添加OnCtlColor响应函数，控制界面上中间的信息文本颜色 begin
HBRUSH CTabDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor); 
	HBRUSH   hNullBr   =   (HBRUSH)::GetStockObject(NULL_BRUSH); //add by liub 定义一个空画刷，使static文本背景透明

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_INSTRUCTION) 
	{ 
		pDC->SetTextColor(RGB(255, 255, 255)); 
		pDC->SetBkMode(TRANSPARENT); 
	    return hNullBr; 
		
	} 
	else
	{
	//	pDC->SetTextColor(RGB(255, 255, 255)); 
	//	pDC->SetBkMode(TRANSPARENT); 
	//	hbr=CreateSolidBrush(RGB(255,0,0)); 
	    return hbr; 
	}
    
	// TODO: Return a different brush if the default is not desired 
	
}
//////////////////////////////////////////////////////////////////////////add by liub end

//显示说明文字的文本框liub_modify显示提示信息
void CTabDialog::InitTestBoxShow()
{
	CWnd* pInsTestBox;
	CWnd* pTabTextBox;
	RECT winrect_temp;
	GetClientRect (&winrect_temp);
	
	pInsTestBox = GetDlgItem(IDC_STATIC_TABBKGROUD);
	pInsTestBox->SetWindowPos (NULL, 0, BUTTON_HEIGHT + MENU_HEIGHT, 
		/*winrect_temp.right - 2*/805, 31, SWP_NOZORDER | SWP_SHOWWINDOW);
	
	pTabTextBox = GetDlgItem (IDC_STATIC_INSTRUCTION);
    pTabTextBox->SetWindowPos(NULL, 0, BUTTON_HEIGHT+7+MENU_HEIGHT,winrect_temp.right - 2,18,SWP_NOZORDER | SWP_SHOWWINDOW);
	//m_TabStaticBKground.SetBitmap(IDB_BITMAP_STATUSICON_BKGROUND);//modify by liub 1030 for tabstatic background
    
}

///////////////////////////////////////////////////////////
//display the first dialog and hide all other dialog
//this is the default display of dialog tab
void CTabDialog::InitDialogShow()
{
    for(int i = 0; i < m_nTotalPage; i ++)
    {
        CTabedDialog* pDialog;
        CRect rect;
        
        m_DialogMap.Lookup(i, (CTabedDialog*)pDialog);

        if(pDialog == NULL){
            return;
        }

        if (m_bMoveWindow){
            CWnd *pContainer = pDialog->GetParent();
            //CWnd *pContainer = GetDlgItem(IDC_STATIC_DLGCONTAINER);
            pContainer->GetWindowRect(&rect);
        }

        if (pDialog->GetParent()->IsKindOf( RUNTIME_CLASS( CTabDialog ))) 
        {
            if (m_nStyle == TABDIALOGSTYLE_VERICAL) 
            {
                rect.left += BUTTON_WIDTH + 2;
                rect.right -= BUTTON_WIDTH + 2;                        
            }        
            else if (m_nStyle == TABDIALOGSTYLE_HORIZONTAL) 
            {
                rect.top += BUTTON_HEIGHT + 2;
                rect.bottom -= BUTTON_HEIGHT + 2;                        
            }            
        }

        pDialog->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER);
        ((CBaseDialog*)pDialog)->m_OriginRect.SetRect(0,0,rect.Width(), rect.Height());
  
        if(i==0)
        {
            CWnd* pButton = NULL;
            CRect ButtonRect;
            m_ButtonMap.Lookup(i, pButton);
            ::SendMessage(m_hWnd,WM_COMMAND,BN_CLICKED, (LPARAM)pButton->m_hWnd);
        }

    }
}

//////////////////////////////////////////////////////////////////
//Hide all the pages
void CTabDialog::HideAllPages()
{
    for(int i = 0; i < m_nTotalPage; i ++)
    {
        CTabedDialog* pDialog;
        m_DialogMap.Lookup(i, (CTabedDialog*)pDialog);

        pDialog->ShowWindow(SW_HIDE);
    }
}
//wParam[BN_CLICKED],lParam[按钮的指针],通过按钮指针确定子窗口指针
//按钮与子窗口的顺序是一一对应的.
//m_ButtonMap,m_DialogMap
BOOL CTabDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//wyw_0207 begin add
	CHSDPADlg* pMainWnd = NULL;
	pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	if (wParam == IDC_BUTTON_FILE)
	{
		CMenuBar::GetInstance().TrackPopup(0);
	}
	else if (wParam == IDC_BUTTON_OPERATION)
	{
		CMenuBar::GetInstance().TrackPopup(1);
	}
	else if (wParam == IDC_BUTTON_TOOL)
	{
		CMenuBar::GetInstance().TrackPopup(2);
	}
	else if (wParam == IDC_BUTTON_HELP_MENU)
	{
		CMenuBar::GetInstance().TrackPopup(3);
	}

	if (wParam == IDM_ABOUT)		//菜单中的消息
	{
		CAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	//wyw_0207 end 

	if(wParam==IDM_DIAGNOSTICS)
	{
		CDiagnosticDlg dlg;
		dlg.DoModal();
	}
	if(wParam==IDM_TEXT_SETTING)
	{
		pMainWnd->m_pSetupDlg->OnButtonText();
	}
	if(wParam==IDM_NETWORK_SETTING)
	{
		pMainWnd->m_pSetupDlg->OnButtonNetWork();
	}
	if(wParam==IDM_PROFILE_MANAGE)
	{
		pMainWnd->m_pSetupDlg->OnButtonProfiles();
	}
	if(wParam==IDM_PIN_ENABLE)
	{
		pMainWnd->m_pSetupDlg->OnButtonEnablePIN();
	}
	if(wParam==IDM_PIN_DISABLE)
	{
		pMainWnd->m_pSetupDlg->OnButtonDisablePIN();
	}
	if(wParam==IDM_PIN_MODIFY)
	{
		pMainWnd->m_pSetupDlg->OnButtonModifyPIN();
	}
	if(wParam==IDM_PIN_UNLOCK)
	{
		pMainWnd->m_pSetupDlg->OnButtonUnlockPIN();
	}
	if(wParam==IDM_STARTUP_OPTION)
	{
		pMainWnd->m_pSetupDlg->OnButtonPreference();
	}
	if(wParam==IDM_EXIT)
	{
		pMainWnd->SendMessage(WM_CLOSE,NULL,NULL);
	}

	if(wParam==IDM_LIMIT)
	{
		CPwdConfirmDlg PwdConfirmDlg(this);
		int nRes = PwdConfirmDlg.DoModal();
		if(nRes == IDOK)
		{
			CInternetUsageLimitDlg dlg;
			dlg.DoModal();
		}
	}



	////////////取得菜单项对应的按钮，并触发按钮的点击事件
	int ButtonID;
	if(m_MenuMap.Lookup(wParam,ButtonID))
	{
		CWnd *pButton = GetDlgItem(ButtonID);
		SendMessage(WM_COMMAND,(WPARAM)ButtonID,(LPARAM)pButton->m_hWnd);
		return FALSE;
	
	}




//     CHSDPADlg* pMainWnd = NULL;
//     pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
/*
	CCallDlg* dlgCall = NULL;
	dlgCall = (CCallDlg*)(pMainWnd->m_pCallDlg);
	InCalling = dlgCall->m_blIsInCalling;
*/
	//int nUSSDBtn = -1;
	int nHelpBtn = -1;
	if((g_SetData.Main_nCall))
	{
		//nUSSDBtn = 6;
		nHelpBtn = 7;
	}
	else 
	{
		//nUSSDBtn = 5;
		nHelpBtn = 6;
	}

    if(HIWORD(wParam)==BN_CLICKED)
    {
        for(int i = 0; i < m_nTotalPage; i++)
        {
			CWnd* pButton = NULL;
            CRect ButtonRect;
            m_ButtonMap.Lookup(i, pButton);

            pButton->GetWindowRect(&ButtonRect);

            if(pButton->m_hWnd==(HWND)lParam)
            {                  
                CTabedDialog* pDialog;
				CWnd* cWnd = this->GetParent();				

		/*		if(m_bWinSize && (i != 0) && (i != 1)&& (i != nUSSDBtn)&& (i != nHelpBtn))
				{
					cWnd->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);					
				}
				else if(pMainWnd->m_bInitMask && !m_bWinSize && 
					(i != 0) && (i != 1) && (i != nUSSDBtn) && (i != nHelpBtn)
					&&(m_Currentbutton == i))
				{
					cWnd->SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
				}

				if((i != 0) && (i != 1) && (i != nUSSDBtn) && (i != nHelpBtn))
				{
					m_Currentbutton = i;
				}
      *///liub_modify(判断按钮是第一个或USSD或Help就不调用Tabdialog窗口)
                   
				if(m_bWinSize  && (i != 1)&& (i != nHelpBtn))
				{
					cWnd->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);					
				}
				else if(pMainWnd->m_bInitMask && !m_bWinSize && 
					(i != 1) && (i != nHelpBtn)
					&&(m_Currentbutton == i))
				{
					cWnd->SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
				}
				
				if((i != 1) && (i != nHelpBtn))
				{
					m_Currentbutton = i;
				}


				CWnd *pUSSDBtn = GetDlgItem(IDC_BUTTON_USSD);
                CWnd *pHelpBtn = GetDlgItem(IDC_BUTTON_HELP);
                CWnd *pInternetBtn = GetDlgItem(IDC_BUTTON_INTERNET);
                CWnd *pBrowseBtn = GetDlgItem(IDC_BUTTON_BROWSE);
				CWnd *pCallBtn = GetDlgItem(IDC_BUTTON_CALL);
				CWnd *pSmsBtn = GetDlgItem(IDC_BUTTON_SMS);
				CWnd *pPbmBtn = GetDlgItem(IDC_BUTTON_PHONEBOOK);
				CWnd *pOptionBtn = GetDlgItem(IDC_BUTTON_SETUP);
				CWnd *pUsageBtn=GetDlgItem(IDC_BUTTON_USAGE);

                CWnd *pWndPrev, *pWndChild;
                CString str, strHelpTitle,strTip;

				//如果是USSD，直接弹出USSD对话框
#if 1
 				if(pUSSDBtn == pButton)
				{  


				//	if (FALSE ==InCalling)
					{
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  USSD"));//add by liub
						//////////////////////////////////////////////////////////////////////////add by liub for tabbutton begin
						m_btn8.m_buttonpressed = TRUE;
					    m_btn1.m_buttonpressed = FALSE;
						if(g_SetData.Main_nCall)
						{
							m_btn2.m_buttonpressed = FALSE;
						}
						else
						{
							GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
						}
						m_btn3.m_buttonpressed = FALSE;  
						m_btn4.m_buttonpressed = FALSE;
						m_btn5.m_buttonpressed = FALSE;
						if(g_SetData.Main_nHelp)
						{
							GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
							m_btn6.m_buttonpressed = FALSE;
						}
						
						m_btn7.m_buttonpressed = FALSE; 
						//m_btn9.m_buttonpressed = FALSE; 
                //////////////////////////////////////////////////////////////////////////add by liub end

					}
			    //	szText = _T("USSD");
                    //m_TabStaticBKground.SetBitmap(IDB_BITMAP_STATUSICON_BKGROUND);//add by liub 1030 for tabstatic background 
// 					m_TabStaticBKground.SetText(szText);
// 					//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  USSD"));
// 					if(!pMainWnd->m_bussdflag)
// 					{
// 						CUSSDServiceDlg dlg(AfxGetMainWnd());
// 						pMainWnd->m_bussdflag = true;
// 						dlg.DoModal();
// 					}
// 					return TRUE;

			}

				else if(pUsageBtn==pButton)
				{
				//	if (FALSE == InCalling)
					{
					    //////////////////////////////////////////////////////////////////////////add by liub for buttons begin
						m_btn9.m_buttonpressed = TRUE;
						m_btn1.m_buttonpressed = FALSE;
						if(g_SetData.Main_nCall)
						{
							m_btn2.m_buttonpressed= FALSE;
						}
						else
						{
							GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
						}
						m_btn3.m_buttonpressed= FALSE;  
						m_btn4.m_buttonpressed= FALSE;
						m_btn5.m_buttonpressed= FALSE;
						if(g_SetData.Main_nHelp)
						{
							GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
						}
						
						m_btn7.m_buttonpressed= FALSE; 
					//	}
						////////////////////////////////////////////////////////////////////////// add by liub end
						strTip.LoadString(IDS_USAGE);
						//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  Usage"));
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);
					}
							
				}
				
#endif

#if 0
				else if(pSmsBtn == pButton)
				{
					CSmsDlg* pSmsWnd = NULL;
					pSmsWnd = (CSmsDlg*)(pMainWnd->m_pSmsDlg);
					pSmsWnd->OnInitSMSRecord();
				}
#endif

#if 0
				else if(pPbmBtn == pButton)
				{
					CPhoneBookDlg* pPbWnd = NULL;
					pPbWnd = (CPhoneBookDlg*)(pMainWnd->m_pPhoneBookDlg);
					//pPbWnd->OnInitPBRecord();
				}	
#endif
				else if(pOptionBtn == pButton)
				{
					
			//		if (FALSE == InCalling)
					{
						//////////////////////////////////////////////////////////////////////////add by liub
						m_btn5.m_buttonpressed = TRUE;
						m_btn1.m_buttonpressed = FALSE;
						if(g_SetData.Main_nCall)
						{
							m_btn2.m_buttonpressed = FALSE;
						}
						else
						{
							GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
						}
						m_btn3.m_buttonpressed = FALSE;   
						m_btn4.m_buttonpressed = FALSE;
						if(g_SetData.Main_nHelp)
						{
							GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
							m_btn6.m_buttonpressed = FALSE;
						}
						
						m_btn7.m_buttonpressed = FALSE; 
						
						m_btn9.m_buttonpressed = FALSE;
						//////////////////////////////////////////////////////////////////////////add by liub end
						strTip.LoadString(IDS_OPTIONS);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);//add by liub
						//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  Option"));//add by liub
						CSetupDlg* pOptionWnd = NULL;
						pOptionWnd = (CSetupDlg*)(pMainWnd->m_pSetupDlg);
            	    	pOptionWnd->CheckPINUnlockSetup();
					}
					
				}				


                else if(pHelpBtn == pButton)
                {
			//		if (FALSE == InCalling)
					{
						strTip.LoadString(IDS_HELPDOC);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);//add by liub
						//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  HELP"));//add by liub
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
							CString CInfo;                        
							CString strHelpFile;
							strHelpFile = g_SetData.Main_szHelpFile;
							
							if((_waccess(strHelpFile, 0)) == -1)
							{
								CInfo.LoadString(IDS_HELPINFO);
								AfxMessageBox(CInfo);
							}
							else
								ShellExecute(NULL, _T("open"), strHelpFile, NULL, NULL, SW_SHOWNORMAL); 
						}
                    return TRUE;
					}
	            }
               else if(pInternetBtn == pButton)
                {
			//	   if (FALSE == InCalling)
				   {
					   //////////////////////////////////////////////////////////////////////////add by liub for buttons begin
					   m_btn4.m_buttonpressed = TRUE;
					   m_btn1.m_buttonpressed = FALSE;
					   if(g_SetData.Main_nCall)
					   {
						   m_btn2.m_buttonpressed = FALSE;
					   }
					   else
					   {
						   GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
					   }
					   m_btn3.m_buttonpressed = FALSE; 
					   m_btn5.m_buttonpressed = FALSE;
					   if(g_SetData.Main_nHelp)
					   {
						   GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
						   m_btn6.m_buttonpressed = FALSE;
					   }
					   
					   m_btn7.m_buttonpressed = FALSE; 
					   
					   m_btn9.m_buttonpressed = FALSE;
					   strTip.LoadString(IDS_CONNECTION);
					   ////////////////////////////////////////////////////////////////////////// add by liub end
						 GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);//add by liub	
					   // GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  Connection"));//add by liub	
				   }
				    
				
				//	pMainWnd->m_pConnectDlg->PostMessage(WM_CLICK_INTERNET_BUTTON,0,0);
				   
                }
                else if(pBrowseBtn == pButton)
                {
			//		if (FALSE == InCalling)
					{
					m_btn7.m_buttonpressed = FALSE;
					
					//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  Browser"));
                    ShellExecute(NULL, _T("open"), _T("IEXPLORE.EXE"), 
                                 g_SetData.Main_szURL, NULL,
                                 SW_SHOW);
                    /*
					ShellExecute(NULL, "open", "http://www.indosatm2.com", 
                                 NULL, NULL,
                                 SW_SHOW);//专用于indosatm2运营商
					*/
                    return TRUE;
					}
                }
				else if(pCallBtn == pButton)
				{
			//		if (FALSE == InCalling)
					{
						//////////////////////////////////////////////////////////////////////////add by liub for buttons begin
						if(g_SetData.Main_nCall)
						{
							m_btn2.m_buttonpressed = TRUE;
						}
						else
						{
							GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
						}
						m_btn1.m_buttonpressed = FALSE;
						m_btn3.m_buttonpressed = FALSE;   
						m_btn4.m_buttonpressed = FALSE;
						m_btn5.m_buttonpressed = FALSE;
						if(g_SetData.Main_nHelp)
						{
							GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
							m_btn6.m_buttonpressed = FALSE;//11-2
						}
						
						m_btn7.m_buttonpressed = FALSE; 
						m_btn9.m_buttonpressed = FALSE;
						////////////////////////////////////////////////////////////////////////// add by liub end
						strTip.LoadString(IDS_CALLTIP);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);//add by liub
						//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  CALL"));//add by liub
						CHSDPADlg* pMainWnd = NULL;
						CSmsDlg* pSmsWnd = NULL;
						CPhoneBookDlg *pPbmWnd = NULL;
						
						pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
						pSmsWnd = (CSmsDlg*)(pMainWnd->m_pSmsDlg);
						pPbmWnd = (CPhoneBookDlg*)(pMainWnd->m_pPhoneBookDlg);

						pMainWnd->m_pCallDlg->DispCallHistory();
						
						if(m_pCurBtn == pSmsBtn)
						{
//							pSmsWnd->SetupCall();//delete by liub(取消选取短信条目，然后直接切换到call面板就拨打电话功能)
						}
						else if (m_pCurBtn == pPbmBtn)
						{
//							pPbmWnd->SetupCall();//delete by liub(取消选取电话本条目，然后直接切换到call面板就拨打电话功能)
						}
					}
				
				
				}
				else if(pSmsBtn == pButton)
				{
			//		if (FALSE == InCalling)
					{
						//////////////////////////////////////////////////////////////////////////add by liub for buttons begin
						m_btn1.m_buttonpressed = TRUE;
    					if(g_SetData.Main_nCall)
						{
							m_btn2.m_buttonpressed = FALSE;
						}
						else
						{
							GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
						}
						m_btn3.m_buttonpressed = FALSE; 
						m_btn4.m_buttonpressed = FALSE;
						m_btn5.m_buttonpressed = FALSE;
						if(g_SetData.Main_nHelp)
						{
							GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
							m_btn6.m_buttonpressed = FALSE;
						}
						
						m_btn7.m_buttonpressed = FALSE; 
						
						m_btn9.m_buttonpressed = FALSE;
						////////////////////////////////////////////////////////////////////////// add by liub end
						strTip.LoadString(IDS_SMSTIP);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->ShowWindow(SW_HIDE);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->ShowWindow(SW_SHOW);
					//	GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  SMS"));
					}
				}
				else if(pPbmBtn == pButton)
				{
				   // if (FALSE == InCalling)
				    {
						//////////////////////////////////////////////////////////////////////////add by liub for buttons begin
						m_btn3.m_buttonpressed = TRUE;
     					m_btn1.m_buttonpressed = FALSE;
						if(g_SetData.Main_nCall)
						{
							m_btn2.m_buttonpressed = FALSE;
						}
						else
						{
							GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
						}
						m_btn4.m_buttonpressed = FALSE;
						m_btn5.m_buttonpressed = FALSE;
						if(g_SetData.Main_nHelp)
						{
							GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
							m_btn6.m_buttonpressed = FALSE;
						}
						
						m_btn7.m_buttonpressed = FALSE; 
						
						m_btn9.m_buttonpressed = FALSE;
				 		////////////////////////////////////////////////////////////////////////// add by liub end
			    		strTip.LoadString(IDS_PHONEBOOKTIP);
						GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(strTip);
						//GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  Phonebook"));

				    }
					
				}

                
				m_pCurBtn = pButton;

                //get the current dilaog and display it
                if(m_DialogMap.Lookup(i, (CTabedDialog*)pDialog))
                {
                   return SwitchDlg(pDialog);                                           
                }
            }
        }
        return FALSE;
    }

    return CBaseDialog::OnCommand(wParam, lParam);
}

BOOL CTabDialog::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    //SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER)));
    // TODO: Add extra initialization here
    m_btn1.SetIcon(IDI_ICON_SMS_HV,IDI_ICON_SMS,IDI_ICON_SMS_PRESS);
	if(g_SetData.Main_nCall)
    {
		m_btn2.SetIcon(IDI_ICON_CALL_HV,IDI_ICON_CALL,IDI_ICON_CALL_PRESS);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
	}
    m_btn3.SetIcon(IDI_ICON_PHBK_HV,IDI_ICON_PHBK,IDI_ICON_PHONEBOOK_PRESS);   //IDI_ICON_PHBK_HV为获得焦点时的图片
    m_btn4.SetIcon(IDI_ICON_NET_HV,IDI_ICON_NET,IDI_ICON_NET_PRESS);
    m_btn5.SetIcon(IDI_ICON_SET_HV,IDI_ICON_SET,IDI_ICON_SETUP_PRESS);
	if(g_SetData.Main_nHelp)
	{
		GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
		m_btn6.SetIcon(IDI_ICON_HELP_HV, IDI_ICON_HELP);
	}

    m_btn7.SetIcon(IDI_ICON_BROWSE_HV, IDI_ICON_BROWSE,IDI_ICON_BROWSE_PRESS); 

	m_btn9.SetIcon(IDI_ICON_USAGE_HV, IDI_ICON_USAGE,IDI_ICON_USAGE_PRESS); 

    InCalling = FALSE;
#ifdef FEATURE_BUTTON_NOCAPTION
	GetDlgItem(IDC_BUTTON_SMS)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_CALL)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_PHONEBOOK)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_SETUP)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_BROWSE)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_INTERNET)->SetWindowText(_T(""));
	GetDlgItem(IDC_BUTTON_USAGE)->SetWindowText(_T(""));	
#endif

#ifdef FEATURE_VERSION_NOMENU

	GetDlgItem(IDC_BUTTON_FILE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_OPERATION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_TOOL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_HELP_MENU)->ShowWindow(SW_HIDE);
		
	return TRUE;
#endif
	//wyw_0207 add
	CMenuBar::GetInstance().SetMenu(IDR_MENU_MAIN, m_hWnd);	
	UINT item[] = {IDC_BUTTON_FILE, IDC_BUTTON_OPERATION, IDC_BUTTON_TOOL, IDC_BUTTON_HELP_MENU};
	CMenuBar::GetInstance().SetButtonItem(item, 4);
	CMenuBar::GetInstance().SetFont(-13, 0, _T("System"));
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

CWnd* CTabDialog::GetChildDlgContainer()
{    
    return GetDlgItem(IDC_STATIC_DLGCONTAINER);
}

/*************************************************************************/
/*
Name:    MoveTabDlg
Desc:    移动TAB中对话框
Param:    

Return:     
*/
/*************************************************************************/
#define TABDIALOGMOVTIMES	10	//liub_modify
void CTabDialog::MoveTabDlg() 
{    
    m_pCurWnd->GetClientRect(&m_CurWndRect);

    m_wndwidth=m_CurWndRect.Width();
    m_wndheight = m_CurWndRect.Height();
    m_topleft=m_CurWndRect.TopLeft();
	m_topleft.y -= 30;//liub_modify
    m_bottomright=m_CurWndRect.BottomRight();
    m_bottomright.y -= 30;//liub_modify
	
    if (m_nMoveDirection == TABDIALOG_MOVEDIRECTION_LEFTTORIGHT)
    {
        m_topleft.x-=m_CurWndRect.Width();
        m_bottomright.x-=m_CurWndRect.Width();

        m_ispeed = m_wndwidth/TABDIALOGMOVTIMES;
    }
    else if (m_nMoveDirection == TABDIALOG_MOVEDIRECTION_RIGHTTOLEFT) 
    {
        m_topleft.x=m_CurWndRect.Width();
        m_bottomright.x=m_CurWndRect.Width();

        m_ispeed = m_wndwidth/TABDIALOGMOVTIMES;
    }
    else if (m_nMoveDirection == TABDIALOG_MOVEDIRECTION_UPTODOWN) 
    {
        m_topleft.y-=m_CurWndRect.Height();
        m_bottomright.y-=m_CurWndRect.Height();

        m_ispeed = m_wndheight/TABDIALOGMOVTIMES;
    }
    else if (m_nMoveDirection == TABDIALOG_MOVEDIRECTION_DOWNTOUP) 
    {
        m_topleft.y=m_CurWndRect.Height();
        m_bottomright.y=m_CurWndRect.Height();

        m_ispeed = m_wndheight/TABDIALOGMOVTIMES;
    }
    
    m_CurWndRect.TopLeft().x = m_topleft.x;    
    m_CurWndRect.right = 0;
	m_CurWndRect.TopLeft().y +=0;//liub_modify TAB框的高度定位
	m_CurWndRect.BottomRight().y += 0;//liub_modify
    
    m_pCurWnd->MoveWindow(&m_CurWndRect,true);//将当前子窗体移到左边
    m_pCurWnd->ShowWindow(SW_SHOW);


    m_moveEnding=FALSE;
    SetTimer(IDT_TIMER_MOVEDIALOG,10,NULL);//开始运动  

	
}

void CTabDialog::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
    
    if (IDT_TIMER_MOVEDIALOG == nIDEvent) 
    {
        if (m_nStyle == TABDIALOGSTYLE_VERICAL) 
        {
            if(m_CurWndRect.left<4-m_ispeed)
            {
                m_CurWndRect.TopLeft().x+=m_ispeed;
                m_CurWndRect.BottomRight().x+=m_ispeed;
                m_CurWndRect.BottomRight().y += m_ispeed;
                m_pCurWnd->MoveWindow(&m_CurWndRect,true);
            }
            else
            {
               	m_CurWndRect.TopLeft().x=0;
                m_CurWndRect.BottomRight().x=m_wndwidth;
                m_pCurWnd->MoveWindow(&m_CurWndRect,true);
				KillTimer(IDT_TIMER_MOVEDIALOG);
				if (m_pCurWnd->IsKindOf( RUNTIME_CLASS( CNetStatistics )))
				{
						CNetStatistics* pDlg=(CNetStatistics *)m_pCurWnd;	
						if(g_bDataService)
							pDlg->setDrawFlag(TRUE);
				}
            }			
        }        
        else if (m_nStyle == TABDIALOGSTYLE_HORIZONTAL) 
        {
        
            if(m_CurWndRect.left<4-m_ispeed){
                m_CurWndRect.TopLeft().x+=m_ispeed;
                m_CurWndRect.BottomRight().x+=m_ispeed;
                m_CurWndRect.BottomRight().y += m_ispeed;
                m_pCurWnd->MoveWindow(&m_CurWndRect,true);
            }
			else{
                m_CurWndRect.TopLeft().x=0;
                m_CurWndRect.BottomRight().x=m_wndwidth;
                m_pCurWnd->MoveWindow(&m_CurWndRect,true);	
				KillTimer(IDT_TIMER_MOVEDIALOG);
				
				 if (m_pCurWnd->IsKindOf( RUNTIME_CLASS( CNetStatistics )))
				{
					CNetStatistics* pDlg=(CNetStatistics *)m_pCurWnd;	
					if(g_bDataService) //if network is connected
						pDlg->setDrawFlag(TRUE);
				}
            }
        }            
	}
    
    CBaseDialog::OnTimer(nIDEvent);
}

LRESULT CTabDialog::OnSwitchMe(WPARAM wParam, LPARAM lParam)
{
    CTabedDialog *pDlg = (CTabedDialog *)wParam;
    SwitchDlg(pDlg);

    return 0;
}

/*************************************************************************/
/*
Name:    SwitchDlg
Desc:    切换窗口
Param:    

Return:     
         
*/
/*************************************************************************/

BOOL CTabDialog::SwitchDlg(CTabedDialog *pDlg)
{   
    if(m_pCurWnd != NULL && !m_pCurWnd->CanSwitchMe())
        return FALSE;
    
    m_pCurWnd = pDlg;
    
    CRect rect;
    CRect ButtonRect;
        
    CShadeButtonST* pButton = pDlg->GetButton();
    pButton->GetWindowRect(&ButtonRect);

    m_btn1.m_bBigDaddy=FALSE;
	if(g_SetData.Main_nCall)
	{
		m_btn2.m_bBigDaddy=FALSE;
	}
    m_btn3.m_bBigDaddy=FALSE;
    m_btn4.m_bBigDaddy=FALSE;
    m_btn5.m_bBigDaddy=FALSE;
    m_btn6.m_bBigDaddy=FALSE; 
	m_btn8.m_bBigDaddy=FALSE; 
	m_btn9.m_bBigDaddy=FALSE;

    //m_btn1.Invalidate();
    Invalidate();
    pButton->m_bBigDaddy=TRUE;
    
	m_pCurBtn = pButton;
    
    HideAllPages();
    this->GetClientRect(&rect);                    
        
    if (m_nStyle == TABDIALOGSTYLE_VERICAL) 
    {
        rect.left = ButtonRect.Width() + 2;                    
    }        
    else if (m_nStyle == TABDIALOGSTYLE_HORIZONTAL) 
    {
        rect.top = ButtonRect.Height() + 2;                    
    }            
    
    if (m_bMoveWindow) 
    {
        MoveTabDlg();
    }
    else
    {
        pDlg->SetWindowPos(NULL, rect.left, rect.top-20, rect.Width(), rect.Height(), SWP_NOZORDER);//rect.top - 20 ,liub_modify
        pDlg->ShowWindow(SW_SHOW); 
    }
    if (pDlg->IsKindOf( RUNTIME_CLASS( CCallDlg ))){
        pDlg->SetTimer(IDT_TICK_TIMER,1000,0);
    }
    return TRUE;
}

/************************************************************************/
/*
Name:    OnDial
Desc:    转发拨号消息
Param:    

Return:     
         
*/
/*************************************************************************/

LRESULT CTabDialog::OnDial(WPARAM wParam, LPARAM lParam)
{        
    for(int i = 0; i < m_nTotalPage; i++)
    {
        CTabedDialog* pDialog;
        
        //get the current dilaog and display it
        m_DialogMap.Lookup(i, (CTabedDialog*)pDialog);
        if (pDialog->IsKindOf( RUNTIME_CLASS( CCallDlg ))) 
        {
            if (SwitchDlg(pDialog))
            {
                CWnd* pButton = NULL;
                m_ButtonMap.Lookup(i, pButton);
                pButton->SetFocus();
				//////////////////////////////////////////////////////////////////////////add by liub for buttons begin
				if(g_SetData.Main_nCall)
				{
					m_btn2.m_buttonpressed = TRUE;
				}
				else
				{
					GetDlgItem(IDC_BUTTON_CALL)->ShowWindow(SW_HIDE);
				}
				m_btn1.m_buttonpressed = FALSE;
				m_btn3.m_buttonpressed = FALSE;   //IDI_ICON_PHBK_HV为获得焦点时的图片
				m_btn4.m_buttonpressed = FALSE;
				m_btn5.m_buttonpressed = FALSE;
				if(g_SetData.Main_nHelp)
				{
					GetDlgItem(IDC_BUTTON_USSD)->EnableWindow(TRUE);
					m_btn6.m_buttonpressed = FALSE;
				}
				
				m_btn7.m_buttonpressed = FALSE; 
				
				m_btn9.m_buttonpressed = FALSE;

				GetDlgItem(IDC_STATIC_INSTRUCTION)->SetWindowText(_T("  CALL"));//add by liub
				////////////////////////////////////////////////////////////////////////// add by liub end
              
                
                ::SendMessage(pDialog->GetSafeHwnd(), IDT_CALL_DIAL, wParam, lParam);
                break;
            }            
        }
    }
    
    return 0;
}

LRESULT CTabDialog::ChangeSkin(WPARAM wParam, LPARAM lParam)
{
    ::SendMessage(GetOwner()->GetSafeHwnd(),WM_CHANGE_SKIN, wParam,lParam);
  
    m_bFirst = TRUE;
    SetSkin(g_SetData.Setup_nSkinStyle);    
    return 0;
}

//窗口最大化显示
LRESULT CTabDialog::WindowMax(WPARAM wParam, LPARAM lParam)
{	
	if(m_bWinSize)
	{
		//调整主窗口的大小
		CWnd* cMainWnd;			
		CRect rect;
		CRect winrect;
		cMainWnd = this->GetParent();				

		cMainWnd->GetWindowRect(&winrect);
		cMainWnd->SetWindowPos(cMainWnd,winrect.left,winrect.top,g_MainDlgRect.Width(),
			g_MainDlgRect.Height(),SWP_NOZORDER | SWP_SHOWWINDOW);

		//调整Tab窗口大小
		this->m_bmpBackground.DeleteObject();
		this->m_bmpBackground.LoadBitmap(IDB_BACK_CON);
		SetWindowPos(this,													//liub_modify
						   g_TabDlgRect.left - m_skinWin.m_BorderLeftWidth, 
						   g_TabDlgRect.top - m_skinWin.m_TitleHeight,		//m_skinWin.m_TitleHeight + 27
						   g_TabDlgRect.Width() + m_skinWin.m_BorderLeftWidth,
						   g_TabDlgRect.Height() + m_skinWin.m_TitleHeight - 27, 
						   SWP_NOZORDER | SWP_SHOWWINDOW);	


		//画皮和LOGO
	#ifdef FEATURE_VERSION_BRAZIL
		CHSDPADlg* pMainWnd = NULL;
		pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
		ReLoadLogo(pMainWnd->m_szPLMN);
	#else
		this->m_bmpBackground.DeleteObject();
		this->m_bmpBackground.LoadBitmap(IDB_BACK_CON);
		CPaintDC dc(this);
		CDC dcMem; 

		dcMem.CreateCompatibleDC(&dc); 
		BITMAP bitMap;
		this->m_bmpBackground.GetBitmap(&bitMap);
		CBitmap *pbmpOld=dcMem.SelectObject(&this->m_bmpBackground);

		CDC dctemp;
		dctemp.CreateCompatibleDC(&dcMem);
		CBitmap *m_pbmpTemp;

		HBITMAP m_hBitmap;        // Handle of the bitmap
		// Load the image
		m_hBitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),
									g_SetData.Main_szLogoBmp, IMAGE_BITMAP, 0, 0,
									LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		m_pbmpTemp=CBitmap::FromHandle(m_hBitmap);

		CBitmap *pbmpOld1=dctemp.SelectObject(m_pbmpTemp);

		dcMem.BitBlt(g_TabDlgRect.Width() - 150,25,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);
		//dcMem.BitBlt(g_TabDlgRect.Width() - 150,1,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);

	#endif
		
		//调整下面的状态栏到合适的位置liub_modify					
		RECT rect1;
		RECT rectup;
		RECT rectupspeed;
		RECT rectdown;
		RECT rectdownspeed;
		CWnd* cWnd = NULL;
		
		int nX = 10; //横向位移
		cMainWnd->GetClientRect(&rect1);
		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_UPFLUX);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectup);
		}
		cWnd->MoveWindow(nX,rect1.bottom - 40,rectup.right,rectup.bottom,TRUE);
		
		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_UPSPEED);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectupspeed);
		}
		nX += rectup.right ;
		cWnd->MoveWindow(nX ,rect1.bottom - 40,rectupspeed.right,rectupspeed.bottom,TRUE);

		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_DWFLUX);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectdown);
		}
		nX += rectupspeed.right + 260;
		cWnd->MoveWindow(nX,rect1.bottom - 40,rectdown.right,rectdown.bottom,TRUE);

		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_DWSPEED);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectdownspeed);
		}
		nX +=  rectdown.right ;
		cWnd->MoveWindow(nX,rect1.bottom - 40,rectdownspeed.right,rectdownspeed.bottom,TRUE);

		//cMainWnd->UpdateWindow();
		m_bWinSize = false;
		m_skinWin.m_winstate = 1;
		this->InvalidateRect(NULL,FALSE);
	}
	return 0;
}

//还原窗口大小
LRESULT CTabDialog::WindowRestore(WPARAM wParam, LPARAM lParam)
{
	return true;//liub_modify
	if(!m_bWinSize)
	{
		CWnd* cMainWnd;			
		CRect rect;
		CRect winrect;
		cMainWnd = this->GetParent();				

		cMainWnd->GetWindowRect(&winrect);
		cMainWnd->SetWindowPos(cMainWnd,winrect.left,winrect.top,g_MainDlgMinRect.Width(),
			g_MainDlgMinRect.Height(),SWP_NOZORDER | SWP_SHOWWINDOW);

		
		//调整Tab窗口大小
		SetWindowPos(this,
						   g_TabDlgRect.left - m_skinWin.m_BorderLeftWidth, 
						   g_TabDlgRect.top - m_skinWin.m_TitleHeight + 27,
						   g_TabDlgRect.Width() + m_skinWin.m_BorderLeftWidth,
						   100,
						   SWP_NOZORDER | SWP_SHOWWINDOW);
		this->m_bmpBackground.DeleteObject();
		this->m_bmpBackground.LoadBitmap(IDB_BACK_CON_DEFAULT);

		//画皮和LOGO	
#ifdef FEATURE_VERSION_BRAZIL
		CHSDPADlg* pMainWnd = NULL;
		pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
		ReLoadLogo(pMainWnd->m_szPLMN);
#else		
		CPaintDC dc(this);
		CDC dcMem; 
		dcMem.CreateCompatibleDC(&dc); 
		BITMAP bitMap;
		this->m_bmpBackground.GetBitmap(&bitMap);
		CBitmap *pbmpOld=dcMem.SelectObject(&this->m_bmpBackground);

		CDC dctemp;
		dctemp.CreateCompatibleDC(&dcMem);
		CBitmap *m_pbmpTemp;
		HBITMAP m_hBitmap;        // Handle of the bitmap
		// Load the image

		HINSTANCE hInstance = AfxGetInstanceHandle();
		m_hBitmap=(HBITMAP)LoadImage(hInstance,
			g_SetData.Main_szLogoBmp, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		m_pbmpTemp=CBitmap::FromHandle(m_hBitmap);
//*/
//		m_bmpTemp.LoadBitmap(IDB_LOGO);
		
		CBitmap *pbmpOld1=dctemp.SelectObject(m_pbmpTemp);
		#ifdef FEATURE_UICOLOR_GREY	
			dcMem.BitBlt(g_TabDlgRect.Width() - 130,2,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);
		#else
			dcMem.BitBlt(g_TabDlgRect.Width() - 130,1,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);
			//dcMem.BitBlt(g_TabDlgRect.Width() - 150,1,bitMap.bmWidth,bitMap.bmHeight,&dctemp,0,0,SRCCOPY);
		#endif
#endif
		
		//调整下面的状态栏到合适的位置					
		RECT rect1;
		RECT rectup;
		RECT rectupspeed;
		RECT rectdown;
		RECT rectdownspeed;
		CWnd* cWnd = NULL;
		
		int nX = 30; //横向位移
		cMainWnd->GetClientRect(&rect1);
		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_UPFLUX);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectup);
		}
		cWnd->MoveWindow(nX,rect1.bottom - 20,rectup.right,rectup.bottom,TRUE);
		
		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_UPSPEED);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectupspeed);
		}
		nX += rectup.right + 20;
		cWnd->MoveWindow(nX ,rect1.bottom - 20,rectupspeed.right,rectupspeed.bottom,TRUE);

		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_DWFLUX);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectdown);
		}
		nX += rectupspeed.right + 40;
		cWnd->MoveWindow(nX,rect1.bottom - 20,rectdown.right,rectdown.bottom,TRUE);

		cWnd = cMainWnd->GetDlgItem(IDC_UI_ICON_DWSPEED);
		if(cWnd != NULL)
		{
			cWnd->GetClientRect(&rectdownspeed);
		}
		nX +=  rectdown.right + 20;
		cWnd->MoveWindow(nX,rect1.bottom - 20,rectdownspeed.right,rectdownspeed.bottom,TRUE);

		m_bWinSize = true;
		this->InvalidateRect(NULL,FALSE);		
	}
	return 0;
}

void CTabDialog::ReLoadLogo(CString strPLMN)
{	
	CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    CDC dcMem; 
    dcMem.CreateCompatibleDC(&dc); 
    BITMAP bitMap;
    m_bmpBackground.GetBitmap(&bitMap);
    CBitmap *pbmpOldBack = dcMem.SelectObject(&m_bmpBackground);
	 
	CDC dcMemLogo; 
	CBitmap bmpPlmnLogo;
	dcMemLogo.CreateCompatibleDC(&dcMem); 

	CBitmap *pbmpOldlogo;
	
	if(strPLMN.GetLength() == 0)
	{
		HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		g_SetData.Main_szLogoBmp, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		CBitmap *pBitmap = CBitmap::FromHandle(hBitmap);
		pbmpOldlogo = dcMemLogo.SelectObject(pBitmap);
	}
	else if(!strPLMN.Compare(_T("AMAZONIA")))
	{
		bmpPlmnLogo.LoadBitmap(IDB_BITMAP_AMAZONIA);
		pbmpOldlogo = dcMemLogo.SelectObject(&bmpPlmnLogo);
	}
	else if(!strPLMN.Compare(_T("TELEMIGC")))
	{
		bmpPlmnLogo.LoadBitmap(IDB_BITMAP_TELEMIG);
		pbmpOldlogo = dcMemLogo.SelectObject(&bmpPlmnLogo);
	}
	else if(!strPLMN.Compare(_T("Claro")))
	{
		bmpPlmnLogo.LoadBitmap(IDB_BITMAP_CLARO);
		pbmpOldlogo = dcMemLogo.SelectObject(&bmpPlmnLogo);
	}
	else if(!strPLMN.Compare(_T("VIVO")))
	{
		bmpPlmnLogo.LoadBitmap(IDB_BITMAP_VIVO);
		pbmpOldlogo = dcMemLogo.SelectObject(&bmpPlmnLogo);
	}
	else if(!strPLMN.Compare(_T("Oi")))
	{
		bmpPlmnLogo.LoadBitmap(IDB_BITMAP_OI);
		pbmpOldlogo = dcMemLogo.SelectObject(&bmpPlmnLogo);
	}
	else
	{
		HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		g_SetData.Main_szLogoBmp, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		CBitmap *pBitmap = CBitmap::FromHandle(hBitmap);
		pbmpOldlogo = dcMemLogo.SelectObject(pBitmap);
	}	
	 	
	BOOL res = dcMem.BitBlt(g_TabDlgRect.Width() - 130,1,bitMap.bmWidth,bitMap.bmHeight,
		          &dcMemLogo,0,0,SRCCOPY);

	this->InvalidateRect(NULL,FALSE);
	
	return;
}

BOOL CTabDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pCurBtn == GetDlgItem(IDC_BUTTON_CALL))
	{
		CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
		return pMainWnd->m_pCallDlg->PreTranslateMessage(pMsg);
	}
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}














