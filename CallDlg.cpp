// CallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPAdlg.h"
#include "CallDlg.h"
#include "SmsWriteDlg.h"
#include "SelContactDlg.h"
#include "SkinWin\SkinButton.h"
#include "TabDialog.h"
#include "VersionDlg.h"
#include "Dev.h"
#include "HSDPADlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int ONELINELENGTH = 16;

/////////////////////////////////////////////////////////////////////////////
// CCallDlg dialog

IMPLEMENT_DYNAMIC(CCallDlg, CTabedDialog)
CCallDlg::CCallDlg(CWnd* pParent /*=NULL*/)
	: CTabedDialog(CCallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCallDlg)
	m_strEditNum = _T("");
	m_strStatus = _T("");
	m_Numtemp = _T("");//add by liub
	//}}AFX_DATA_INIT
    
	m_pHistDlg = NULL;
	m_blIsInCalling = FALSE;
    ResetCallHistData();
	
	m_pPbData   = ((CHSDPAApp *)AfxGetApp())->GetPbData();
	m_pHistData = ((CHSDPAApp *)AfxGetApp())->GetHistData();

	OutCallorInCall = 0;
	IdleFont = FALSE;

	m_bWaitingCall = FALSE;

	m_cKeyDown = ' ';
}

void CCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabedDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallDlg)
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sliderVol);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_EditNum);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_strEditNum);
	DDX_Control(pDX, IDC_TAB_CALLHISTORY_CALL, m_tabHistCall);//add by liub 1030 for call history
	DDX_Control(pDX, IDC_LIST_CALLHISTORY_CALL, m_lstHistCall);//add by liub 1030 for call history

	////////////////////////////////////////////////////////////////////////// add by liub for key begin
	DDX_Control(pDX,IDC_BUTTON_NUM_1,m_key1);
    DDX_Control(pDX,IDC_BUTTON_NUM_2,m_key2);
	DDX_Control(pDX,IDC_BUTTON_NUM_3,m_key3);
	DDX_Control(pDX,IDC_BUTTON_NUM_4,m_key4);
	DDX_Control(pDX,IDC_BUTTON_NUM_5,m_key5);
	DDX_Control(pDX,IDC_BUTTON_NUM_6,m_key6);
	DDX_Control(pDX,IDC_BUTTON_NUM_7,m_key7);
	DDX_Control(pDX,IDC_BUTTON_NUM_8,m_key8);
	DDX_Control(pDX,IDC_BUTTON_NUM_9,m_key9);
	DDX_Control(pDX,IDC_BUTTON_NUM_0,m_key0);
	DDX_Control(pDX,IDC_BUTTON_NUM_STAR,m_keyStar);
	DDX_Control(pDX,IDC_BUTTON_NUM_HASH,m_keyHash);
	DDX_Control(pDX,IDC_BUTTON_GETPHBITEM,m_keyContacts);
	DDX_Control(pDX,IDC_BUTTON_CALL,m_keydial);
	DDX_Control(pDX,IDC_BUTTON_HUNGUP,m_keyend);
	DDX_Control(pDX,IDC_BUTTON_CLEAR,m_keyclear);

	////////////////////////////////////////////////////////////////////////// add by liub end
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
	//DDX_Control(pDX, IDC_BUTTON_NUM_HASH, m_btnHash);	
	//DDX_Control(pDX, IDC_BUTTON_HUNGUP, m_cHangup);
	//DDX_Control(pDX, IDC_BUTTON_CALL, m_cOriCall);
}

void CCallDlg::SetVolume()
{
	
//m_sliderVol.m_nVolValue;
//AT Command
	const TCHAR ATVTS[]=_T("AT^DDSETEX=2");
	const TCHAR DQuote[]=_T("\x0d\x00");
	TCHAR szAtBuf[512] = {0};
	char szAtAscBuf[512] = {0};
	CString strAtComm=ATVTS;
	strAtComm+= DQuote;
	wcscpy(szAtBuf, strAtComm);
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	WCharToChar(szAtBuf, szAtAscBuf);
	
	if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf))){		
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespVts, this);
	}
	
	
}

BEGIN_MESSAGE_MAP(CCallDlg, CTabedDialog)
	//{{AFX_MSG_MAP(CCallDlg)
	ON_BN_CLICKED(IDC_BUTTON_CALLHISTORY, OnButtonCallhistory)
	ON_BN_CLICKED(IDC_BUTTON_NUM_1, OnButtonNum1)
	ON_BN_CLICKED(IDC_BUTTON_NUM_2, OnButtonNum2)
	ON_BN_CLICKED(IDC_BUTTON_NUM_3, OnButtonNum3)
	ON_BN_CLICKED(IDC_BUTTON_NUM_4, OnButtonNum4)
	ON_BN_CLICKED(IDC_BUTTON_NUM_5, OnButtonNum5)
	ON_BN_CLICKED(IDC_BUTTON_NUM_6, OnButtonNum6)
	ON_BN_CLICKED(IDC_BUTTON_NUM_7, OnButtonNum7)
	ON_BN_CLICKED(IDC_BUTTON_NUM_8, OnButtonNum8)
	ON_BN_CLICKED(IDC_BUTTON_NUM_9, OnButtonNum9)
	ON_BN_CLICKED(IDC_BUTTON_NUM_STAR, OnButtonNumStar)
	ON_BN_CLICKED(IDC_BUTTON_NUM_0, OnButtonNum0)
	ON_BN_CLICKED(IDC_BUTTON_NUM_HASH, OnButtonNumHash)
	ON_BN_CLICKED(IDC_BUTTON_CALL, OnButtonCall)
	ON_BN_CLICKED(IDC_BUTTON_HUNGUP, OnButtonHungup)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SMS, OnButtonSms)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GETPHBITEM, OnButtonGetphbitem)
	ON_EN_CHANGE(IDC_EDIT_NUMBER, OnChangeEditNumber)
	ON_WM_CHAR()
	ON_EN_UPDATE(IDC_EDIT_NUMBER, OnUpdateEditNumber)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CALLHISTORY_CALL, OnSelchangeTabHistCall)//add by liub 1030 for tabstatuscall
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CALLHISTORY_CALL, OnColumnclickListHistCall)//add by liub 1030 for listscall
	ON_BN_CLICKED(IDC_BUTTON_CALL2, OnButtonCall2)//add by liub for Dial2Button
	ON_BN_CLICKED(IDC_BUTTON_PB_SAVE,OnButtonSave)//add by liub for SaveButton
	ON_BN_CLICKED(IDC_BUTTON_PB_DELETE,OnButtonDelete)//add by liub for deletebutton
	//}}AFX_MSG_MAP
	ON_MESSAGE(IDT_CALL_DIAL, OnDial)
	ON_MESSAGE(WM_ATCDVRESULT, SetCallState)
	ON_MESSAGE(WM_CALLING_FAILED, OnCallingFailed)
	ON_MESSAGE(WM_SEL_CONTACT, OnGetNumFromPhb)
	ON_MESSAGE(WM_HIST_MENUITEM_CLICK, OnMenuItemRightClick)//add by liub for Call Rightkey
	ON_MESSAGE(WM_HIST_SAVE_MSG_CALLDLG, OnSaveHist)//add by liub
	ON_MESSAGE(WM_HANDSET_ANSWER_INCOMINGCALL,AnswerIncomingCall)//add by liub for HeadSet answer incomingcall
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallDlg message handlers
/************************************************************************/
/*
Name:	CanSwitchMe
Desc:	是否可以切换目前的窗口
Param:	

Return:	 TRUE:允许
		 
Global:	n.a.
Note:	
*/
/*************************************************************************/
BOOL CCallDlg::CanSwitchMe()
{
	CHSDPADlg* dlg;
	dlg = (CHSDPADlg*)AfxGetMainWnd();

	CTabDialog* dlgTab;
	dlgTab = (CTabDialog*)AfxGetMainWnd();

#ifdef FEATURE_UI_TEST
    return TRUE;
#endif
/*
    if ((CPIN_SIM_NONE_REQUIRED != dlg->m_cHandlePin.m_nSimStat)
		&&(CPIN_SIM_PUK_REQUIRED != dlg->m_cHandlePin.m_nSimStat))
	{
		AfxMessageBox(IDS_CALL_SIMSTATEWRONG);
		return FALSE;
	}
*/
#ifdef FEATURE_GCF_CALLCANSWITCH
    if(0)
#else
	if(m_blIsInCalling)
#endif
    {
		//AfxMessageBox(IDS_INCALLING);
	}
	else
	{
		KillTimer(IDT_TICK_TIMER);
	}

#ifdef FEATURE_GCF_CALLCANSWITCH
    return TRUE;
#else
	//return !m_blIsInCalling;
	return TRUE;
#endif

}

void CCallDlg::OnButtonCallhistory()
{
	CCallHistoryDlg dlg(this);
    m_pHistDlg = &dlg;
	dlg.DoModal();
    m_pHistDlg = NULL;
}

int CCallDlg::DealwithResponseProc(LPARAM lParam,WPARAM wParam)
{
	return 1;
}

void CCallDlg::SendVTS(TCHAR* cvts)
{
	const TCHAR ATVTS[]=_T("AT+VTS=");
	const TCHAR DQuote[]=_T(";\x0d\x00");
	TCHAR szAtBuf[512] = {0};
	char szAtAscBuf[512] = {0};
	CString strAtComm=ATVTS;
	strAtComm+=_T("\"");
	strAtComm+= cvts;
	strAtComm+=_T("\"");
	strAtComm+= DQuote;
	wcscpy(szAtBuf, strAtComm);
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	WCharToChar(szAtBuf, szAtAscBuf);
	if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf))){		
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespVts, this);
	}
}

void CCallDlg::AtRespVts(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	
}

void CCallDlg::OnButtonNum1() 
{
    CString Num = _T("1");
    // TODO: Add your control notification handler code here    
    UpdateData(TRUE);
    
    if(m_blIsInCalling) 
    {
        SendVTS(_T("1\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
   if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T("") )
   {
	   m_Numtemp +="\r\n";
   }
    m_strEditNum +=_T("1");
	m_Numtemp+=_T("1");
	m_strStatus = m_Numtemp; 
	
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum2() 
{
	// TODO: Add your control notification handler code here	
    UpdateData(TRUE);
    
    if(m_blIsInCalling) 
    {
        SendVTS(_T("2\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("2");
	m_Numtemp+=_T("2");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum3() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    
    if(m_blIsInCalling) 
    {
        SendVTS(_T("3\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    
    m_strEditNum +=_T("3");
	m_Numtemp+=_T("3");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum4() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    
    if(m_blIsInCalling) 
    {
        SendVTS(_T("4\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("4");
	m_Numtemp+=_T("4");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum5() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    
    if(m_blIsInCalling) 
    {
        SendVTS(_T("5\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("5");
	m_Numtemp+=_T("5");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum6() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if(m_blIsInCalling) 
    {
        SendVTS(_T("6\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("6");
	
	m_Numtemp+=_T("6");
	m_strStatus = m_Numtemp;  
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum7() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    
    if(m_blIsInCalling) 
    {
        SendVTS(_T("7\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("7");
	m_Numtemp+=_T("7");
	m_strStatus = m_Numtemp;
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum8()
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if(m_blIsInCalling) 
    {
        SendVTS(_T("8\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("8");
	m_Numtemp+=_T("8");
	m_strStatus = m_Numtemp;  
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum9() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if(m_blIsInCalling) 
    {
        SendVTS(_T("9\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}

    m_strEditNum +=_T("9");
	
	m_Numtemp+=_T("9");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNumStar() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if(m_blIsInCalling) 
    {
        SendVTS(_T("*\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("*");
	m_Numtemp+=_T("*");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNum0() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if(m_blIsInCalling) 
    {
        SendVTS(_T("0\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }
    
    CSkinButton *pbtn = (CSkinButton*)GetDlgItem(IDC_BUTTON_NUM_0);
    if((pbtn->ed - pbtn->st>500)&&(!m_blIsInCalling))
    {
        if(m_strEditNum.GetLength()==0)
		{
		    m_strEditNum +=_T("+");
			m_Numtemp+=_T("+");
        	m_strStatus = m_Numtemp; 
		}
        else
		{
			if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
			{
				m_Numtemp +="\r\n";
			}
            m_strEditNum +=_T("0");
			
			m_Numtemp+=_T("0");
          	m_strStatus = m_Numtemp;  
		}
    }
    else
    {
			if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
			{
				m_Numtemp +="\r\n";
			}
            m_strEditNum +=_T("0");
			
			m_Numtemp+=_T("0");
          	m_strStatus = m_Numtemp; 
    }
    
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonNumHash() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    if(m_blIsInCalling) 
    {
        SendVTS(_T("#\0"));
    }
    else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }        
	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T(""))
	{
		m_Numtemp +="\r\n";
	}
    m_strEditNum +=_T("#");
	m_Numtemp+=_T("#");
	m_strStatus = m_Numtemp; 
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnButtonCall()
{
	SetVolume();
	Sleep(80);
	if(m_strEditNum.IsEmpty())
		return;
	
	if(g_SetData.Setup_nPowerManagement != 1)
		return;

	if (!g_bSimReady && wcslen(m_strEditNum) == 3 && !wcsstr(g_SetData.Call_szEmergencyNumbers, m_strEditNum))
	{
		AfxMessageBox(IDS_CALL_EMERGENCY);
		return;
	}

	if (wcsstr(m_strEditNum, _T("+62")))
	{
		int lenTemp = wcslen(m_strEditNum);
		CString strTemp = m_strEditNum.Right(lenTemp-3);
		m_strEditNum = _T("0");
		m_strEditNum += strTemp;
		UpdateData(FALSE);
	}
	
//	m_EditNum.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_GETPHBITEM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALL2)->EnableWindow(FALSE);
	OutCallorInCall = 1;//(主叫)
	
// 	const TCHAR ATD[]=_T("ATD");//9508
// 	const TCHAR DQuote[]=_T(";\x0d\x00");//9508
	const TCHAR ATD[]=_T("AT");//CDMA CM200
	const TCHAR CDV[]=_T("+CDV");//CDMA CM200
	//const TCHAR CDV1[]=_T("^DDSETEX=2");//CDMA CM200
	const TCHAR CDV2[]=_T("^CVOICE=0");//CDMA CM200
	const TCHAR DQuote[]=_T("\x0d\x00");//CDMA CM200
	TCHAR szAtBuf[512] = {0};
	//TCHAR szAtBuf1[512] = {0};
	TCHAR szAtBuf2[512] = {0};
	//TCHAR szAtBuf3[512] = {0};
	char szAtAscBuf[512] = {0};
	//char szAtAscBuf1[512] = {0};
	char szAtAscBuf2[512] = {0};
	CString strDialNum=ATD;
	//CString strDialNum1=ATD;
	CString strDialNum2=ATD;
	strDialNum+=CDV;
	strDialNum+= m_strEditNum;
	strDialNum+= DQuote;
	//strDialNum1+=CDV1;
	//strDialNum1+=DQuote;
	strDialNum2+=CDV2;
	strDialNum2+=DQuote;
	


#ifdef FEATURE_EMERGENCY_NUMBER
	if (wcsstr(g_SetData.Call_szEmergencyNumbers, m_strEditNum) && wcslen(m_strEditNum) == 3)
	{
		const TCHAR CDVE[] = _T("+CDV");//zhjj test
		const TCHAR CDVEQuote[] = _T(",9\x0d\x00");
		
		strDialNum = ATD;
		strDialNum += CDVE;
		strDialNum += m_strEditNum;
		strDialNum += CDVEQuote;
	}
#endif

	wcscpy(szAtBuf, strDialNum);
	//wcscpy(szAtBuf1, strDialNum1);
	wcscpy(szAtBuf2, strDialNum2);

	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	CSerialPort* pComm1 = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	WCharToChar(szAtBuf, szAtAscBuf);
	//WCharToChar(szAtBuf1, szAtAscBuf1);
	WCharToChar(szAtBuf2, szAtAscBuf2);
	//pComm1->WriteToPort(szAtAscBuf2, wcslen(szAtBuf2), FALSE);
	Sleep(80);
	
	if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf), FALSE))
	{


		m_bWaitingCall = TRUE;

		m_CallHistNumber = m_strEditNum;
		m_CallHistKind = HISTKIND_SND;
		m_CallHistTimeStamp = COleDateTime::GetCurrentTime();	
		::ResetEvent(g_BGPassEvt);
		SetTimer(IDT_ATD_TIMEOUT, 30000, NULL);
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespDial, this);
	}
	else
	{
		AfxMessageBox(IDS_CALL_ERR_CALLFAIL);
	}

	
}

void CCallDlg::OnButtonHungup() 
{
	if (!m_blIsInCalling)
	{
		return;
	}
	
/*	const char ATCHUP[]="AT+CHUP\x0d\x00";//9508*/
#ifdef FEATURE_HAIER_CM
	const char ATCHUP[]="AT+CHV\x0d\x00";//CDMA CM200
#else
	const char ATCHUP[]="AT+CHV0\x0d\x00";//CDMA CM200
#endif
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCHUP);

	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
	{		
		SetTimer(IDT_CHV0_TIMEOUT, 30000, NULL);
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespHangup, this);
	}
	m_strEditNum = _T("");
	m_Numtemp =  _T("");
	m_strStatus = _T(""); 
}

void CCallDlg::OnButtonClear() 
{		
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	UINT nlen = 0;
	UINT nlentemp = 0;//add by liub
	nlen = m_strEditNum.GetLength();
	nlentemp = m_Numtemp.GetLength();//add by liub
	if (m_blIsInCalling || nlen==0) 
	{
		return;
	}
	///////////
	CSkinButton *pbtn = (CSkinButton*)GetDlgItem(IDC_BUTTON_CLEAR);
	if((pbtn->ed - pbtn->st>500) && (!m_blIsInCalling))
	{
		m_strEditNum = "";
		m_Numtemp = _T("");//add by liub
	}
	else
	{
		m_strEditNum = m_strEditNum.Left(nlen-1);
		////////////////////////////////////////////////////////////////////////////add by liub
		nlentemp = m_Numtemp.GetLength();
		if ((nlentemp%(ONELINELENGTH+3) == 0))
		{
			m_Numtemp = m_Numtemp.Left(nlentemp-3);
		}
		else
			m_Numtemp = m_Numtemp.Left(nlentemp-1);
		
		m_strStatus = m_Numtemp;
	}
	//////////////////////add by liub
	
	//m_strEditNum = m_strEditNum.Left(nlen-1);
	//m_strEditNum = "";
	UpdateData(FALSE);
	m_EditNum.GetWindowText(m_strEditNum);
	m_EditNum.SetFocus();
	m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
	OnChangeEditNumber();
	//GetDlgItem(IDC_BUTTON_GETPHBITEM)->EnableWindow(TRUE);	
}

/************************************************************************/
/*
Name:	OnDial
Desc:	处理拨号的消息请求，
Param:
	wParam:	
	lParam: a point to a number string

Return:	 0:						
		 :	  errcode	
Global:	n.a.
Note:	
*/
/*************************************************************************/

LRESULT CCallDlg::OnDial(WPARAM wParam, LPARAM lParam)
{		
	if (m_blIsInCalling)		//wyw_0401 add
	{
		AfxMessageBox(IDS_INCALLING);
		return 0;
	}

	BOOL BParam = (BOOL)wParam;
	if (BParam)
	{
		
		TCHAR num[PB_NUM_MAX + 2];
		memset(num,0,PB_NUM_MAX + 2);
		swprintf(num, _T("%s"),lParam);
		m_strEditNum = num;
		m_Numtemp = m_strEditNum;
		m_strStatus = m_strEditNum;

		UpdateData(FALSE);
	//	BOOL BParam = (BOOL)wParam;
		if(BParam)//（直接拨号）
		{
			OnButtonCall();
		}
		return 0;
	}
	else
	{
		char num[PB_NUM_MAX + 2];
		memset(num,0,PB_NUM_MAX + 2);
		sprintf(num, "%s",lParam);
		m_strEditNum = num;
		m_Numtemp = m_strEditNum;
		m_strStatus = m_strEditNum;
		m_strEditNum = num;
		UpdateData(FALSE);
	//	BOOL BParam = (BOOL)wParam;
		if(!BParam)
		{
			OnButtonCall();
		}
    	return 0;
	}
}
//////////////////////////////////////////////////////////////////////////add by liub
bool CCallDlg::SelContact(int nItem, CString* strNumber)
{
	LVITEM item;
	item.mask = LVIF_IMAGE;
	item.iItem = nItem;
	item.iSubItem = 0;
	m_lstHistCall.GetItem(&item);
	return true;
}
//////////////////////////////////////////////////////////////////////////

void CCallDlg::OnButtonSms() 
{
	POSITION pos = m_lstHistCall.GetFirstSelectedItemPosition();
	int ilen = 0;
    if(!pos)
    {
        AfxMessageBox(IDS_PB_SELNULL);
        return;
    }

	int nItemNum = 0;
    nItemNum = m_lstHistCall.GetSelectedCount();
    if (nItemNum > SMS_GROUPSEND_MAX)
    {
        AfxMessageBox(IDS_PB_SMSSELMAX);
        return;
    }
	

	CSmsWriteDlg dlg(this);
	CString strTempNumber = _T(""); //用于存储用户选择的号码

    while (pos)
    {
		int nItem = m_lstHistCall.GetNextSelectedItem(pos);
		UpdateData();
		
		//返回false,则中断当前的操作
		bool bResponse = SelContact(nItem, &strTempNumber);
		if (!bResponse)
		{
			return;
		}
		
		/*strTempNumber为空表示选择了cancel，只有不为空，
		即选择了ok按纽，才将其值赋给SMS的dlg.m_strNumber
		*/
		if (0 != strTempNumber.GetLength())
		{
			ilen = dlg.m_strNumber.GetLength();
			if ((ilen > 0) && (dlg.m_strNumber[ilen-1] != ';'))
			{
				dlg.m_strNumber += _T(";");
			}
			
			dlg.m_strNumber += strTempNumber;
		}
	}
	UpdateData(FALSE);
	//////////////////////////////////////////////////////////////////////////
	int nCurSel;
	
    if(m_pHistData->IsEmpty(m_DispFilter))
        return;
    else if((nCurSel = m_lstHistCall.GetNextItem(-1, LVNI_SELECTED)) == -1)
    {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }
	
    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);
    
    WORD nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
    ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));
	
    LPARAM lParam = (LPARAM)(LPCTSTR)(m_pHistData->GetItem(m_DispFilter, nIndex)).szNumber;
    
    if(wcslen((LPCTSTR)lParam) > 0 && wcscmp((LPCTSTR)lParam, strUnknown))
    {
        CSmsWriteDlg dlg(this, (LPCTSTR)lParam, NULL);
        dlg.DoModal();
    }
    else
        AfxMessageBox(IDS_SELCONNECT_NULLNUM);
}

BOOL CCallDlg::OnInitDialog() 
{
	CTabedDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////liub add
	CString skinbtn;
	skinbtn.LoadString(IDS_APPSKINPASH);
    m_btnkeyres.LoadSkin(skinbtn,"Bitmapcall");
	
	m_key1.SetResource(&m_btnkeyres);
	m_key2.SetResource(&m_btnkeyres);
	m_key3.SetResource(&m_btnkeyres);
	m_key4.SetResource(&m_btnkeyres);
	m_key5.SetResource(&m_btnkeyres);
	m_key6.SetResource(&m_btnkeyres);
	m_key7.SetResource(&m_btnkeyres);
	m_key8.SetResource(&m_btnkeyres);
	m_key9.SetResource(&m_btnkeyres);
	m_key0.SetResource(&m_btnkeyres);
	m_keyStar.SetResource(&m_btnkeyres);
	m_keyHash.SetResource(&m_btnkeyres);
	m_keyContacts.SetResource(&m_btnkeyres);
	m_keyclear.SetResource(&m_btnkeyres);
	m_keydial.SetResource(&m_btnkeyres);
	m_keyend.SetResource(&m_btnkeyres);


	//////////////////////////////////////////////////////////////////////////
	InitTabCtrl();//add by liub 1030 for call history
	InitListCtrl();//add by liub 1030 for call history
	InitDspIndexArr();//add by liub 1111 for call history
	DispCallHistory();//add by liub 1030 for call history
	m_sliderVol.SetSkin(IDB_BACK_SLIDER,IDB_BACK_SLIDERTIC);
	m_sliderVol.SetRange(0,5);
	m_sliderVol.SetPos(2);
	if (!m_blIsInCalling) {
		GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(TRUE);
	}
	
	m_EditNum.m_bFlag  = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//设置通话状态
LRESULT CCallDlg::SetCallState(WPARAM wParam, LPARAM lParam)
{
	CHSDPADlg* dlg;
	dlg = (CHSDPADlg*)AfxGetMainWnd();

	if(wParam == 1)
    {
		m_EditNum.SetWindowText(_T(""));
		m_blIsInCalling = TRUE;
		m_sliderVol.m_bVolSet = TRUE;
		GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(TRUE);
		KillTimer(IDT_TICK_TIMER);
		SetTimer(IDT_DIAL_TIME,1000,0);
//		m_EditNum.EnableWindow(FALSE);
 		GetDlgItem(IDC_BUTTON_GETPHBITEM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALL2)->EnableWindow(FALSE);		
	}
    else
    {
		m_Numtemp = "";
		m_strEditNum = "";
		m_strStatus = "";
		m_strStatus += m_CurrentTime.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
		KillTimer(IDT_DIAL_TIME);
		SetTimer(IDT_TICK_TIMER,1000,0);
		UpdateData(FALSE);	
		m_blIsInCalling = FALSE;
		m_sliderVol.m_bVolSet = FALSE;
		GetDlgItem(IDC_SLIDER_VOLUME)->EnableWindow(FALSE);
//		m_EditNum.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALL2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_GETPHBITEM)->EnableWindow(TRUE);

		SaveRecordToCallHist();
	
		ResetCallHistData();

		if (m_bWaitingCall)
		{
			m_bWaitingCall = FALSE;
			AfxMessageBox(IDS_CALL_ERR_CALLFAIL);
		}
	}

	return TRUE;
}

void CCallDlg::DispStatus(CString& sDur)
{
    StPbRecord PbItem;
	if(m_blIsInCalling){/*通话中计时*/
		
		if (FALSE == IdleFont)
		{
			//////////////////////////////////////////////////////////////////////////
			//添加字体，用于模拟屏幕上使用
			LOGFONT lf;  
			memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
			lf.lfHeight = 28;                  // Request a 20-pixel-high font
			wcscpy(lf.lfFaceName, _T("Impact"));    //    with face name "Impact".
			m_fontIdle.CreateFontIndirect(&lf);    // Create the font.
			CStatic *m_Static=(CStatic *)GetDlgItem(IDC_STATIC_SCREENINFO);
			m_Static->SetFont(&m_fontIdle,FALSE);
			IdleFont = TRUE;
		}
		//m_strStatus.LoadString(IDS_CALL_STATUS_CALLING);
		
        m_strStatus = _T("");
		if(!(m_pPbData->SearchItemByNumber(m_CallHistNumber, &PbItem)))
		{
			m_strStatus = m_CallHistNumber;//(主叫时显示的号码存储变量)			
		}
		else
		{
			m_strStatus += PbItem.strName;
		}
		
		if (m_strStatus.GetLength()>16)			//wyw_0409 add
		{
			m_strStatus.Insert(16, _T("\r\n"));
		}
		
		m_strStatus += "\r\n";
		
		m_strStatus += sDur;
		
		m_strStatus += "\r\n";
		
		int lenDTMF = m_strEditNum.GetLength();
		int nTimes = lenDTMF/16;
		int nIndex = 0;
		CString strEditNumDTMF = m_strEditNum;
		while (nTimes > 0)
		{
			nIndex++;
			strEditNumDTMF.Insert(16*nIndex+2*(nIndex-1), _T("\r\n"));
			
			nTimes--;
		}
		
		m_strStatus += strEditNumDTMF;

	}else{/*待机计时*/
        if (FALSE == IdleFont)
        {
			//////////////////////////////////////////////////////////////////////////
			//添加字体，用于模拟屏幕上
			LOGFONT lf;  
			memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
			lf.lfHeight = 28;                  // Request a 20-pixel-high font
			wcscpy(lf.lfFaceName, _T("Impact"));    //    with face name "Impact".
			m_fontIdle.CreateFontIndirect(&lf);    // Create the font.
			CStatic *m_Static=(CStatic *)GetDlgItem(IDC_STATIC_SCREENINFO);
			m_Static->SetFont(&m_fontIdle,FALSE);
			
			IdleFont = TRUE;
        }		
		
        if (m_strEditNum ==_T(""))
        {
			m_Numtemp = _T("");
			m_strStatus = _T("");
			m_strStatus += m_CurrentTime.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
			m_strStatus +=  "\r\n";
			m_strStatus += m_strEditNum;			
        }
		else if (m_bWaitingCall)
		{
			m_strStatus.LoadString(IDS_CALL_STATUS_CALLING);
			m_strStatus +=  "\r\n";
			m_strStatus += m_strEditNum;
		}
		
	}
	GetDlgItem(IDC_STATIC_SCREENINFO)->SetWindowText(m_strStatus);
	// m_screenInfo.SetText(m_strStatus);//delete by liub
	
}

void CCallDlg::AtRespDial(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{	
	CCallDlg *pdlg = (CCallDlg*)pWnd;
	CString  RetStr;
    RetStr = strArr[0];

	pdlg->KillTimer(IDT_ATD_TIMEOUT);
	::SetEvent(g_BGPassEvt);

	pdlg->m_bWaitingCall = FALSE;

    //如果呼叫成功
	if(RetStr.Find(_T("OK"),0) != -1)
    {
	    pdlg->m_CallHistNumber = pdlg->m_strEditNum;
		pdlg->m_CallHistKind = HISTKIND_SND;
		pdlg->m_CallHistTimeStamp = COleDateTime::GetCurrentTime();
		pdlg->PostMessage(WM_ATCDVRESULT, 1, 0);
	}
    else
    {
//		pdlg->m_EditNum.EnableWindow(TRUE);
		pdlg->GetDlgItem(IDC_BUTTON_CALL2)->EnableWindow(TRUE);
		
		pdlg->GetDlgItem(IDC_BUTTON_GETPHBITEM)->EnableWindow(TRUE);
		pdlg->GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(TRUE);
		
		pdlg->GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);

		pdlg->PostMessage(WM_CALLING_FAILED,0,0);
      
    }
}

void CCallDlg::AtRespHangup(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CCallDlg *pDlg = (CCallDlg*)pWnd;
	pDlg->KillTimer(IDT_CHV0_TIMEOUT);
#ifdef FEATURE_HAIER_CM
	pDlg->PostMessage(WM_ATCDVRESULT,0,0);
#else
//	pDlg->PostMessage(WM_ATCDVRESULT,0,0);
#endif
}

void CCallDlg::OnTimer(UINT nIDEvent) 
{
	CString strDur;
	UpdateData();
	if(TIMER_ID_ATRESP_TIMEOUT == nIDEvent)/*普通AT超时*/
	{
		AfxMessageBox(IDS_USB_PORT_TIMEOUT);
		KillTimer(TIMER_ID_ATRESP_TIMEOUT);
		DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
		((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
	}
	else if(IDT_DIAL_TIME==nIDEvent)
	{
//		m_CallHistTimeSpan = (COleDateTime::GetCurrentTime()-m_CallHistTimeStamp)*24*60*60;	
		m_CallHistTimeSpan = m_CallHistTimeSpan+1;			//wyw_0401 remove up add this  为了改变通话计时跳跃

		strDur = m_CallHistTimeSpan.Format("%H:%M:%S");

		DispStatus(strDur);
		UpdateData(FALSE);
	}
	else if(IDT_CHV0_TIMEOUT==nIDEvent)/*挂断超时*/
	{
		KillTimer(IDT_CHV0_TIMEOUT);
		DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
		((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
	}
	else if(IDT_TICK_TIMER==nIDEvent)
	{
		if(m_blIsInCalling){
			KillTimer(IDT_TICK_TIMER);
		}else{
			m_CurrentTime = COleDateTime::GetCurrentTime();
			DispStatus(strDur);
			UpdateData(FALSE);
		//	SetTimer(IDT_TICK_TIMER,1000,0);
			SetTimer(IDT_TICK_TIMER,200,0);//mofify by liub 1030 for 通话中显示的刷新间隔
		}	
	}
	else if(IDT_ATD_TIMEOUT == nIDEvent) 
	{
		KillTimer(IDT_ATD_TIMEOUT);
		DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
		((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);

		::SetEvent(g_BGPassEvt);
	}
	
	CTabedDialog::OnTimer(nIDEvent);
}

//作为被叫，接听来电
void CCallDlg::AtRespAnswer(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pdlg = (CHSDPADlg*)pWnd;
	CString strWriteSmsDlg;
    ASSERT(pdlg);
    ASSERT(pdlg->m_pCallDlg);

	strWriteSmsDlg.LoadString(IDS_STR_WRITESMS_TITILE);
    pdlg->m_pCallDlg->m_CallHistKind = HISTKIND_RCV;
	pdlg->m_pCallDlg->m_CallHistTimeStamp = COleDateTime::GetCurrentTime();
    //pdlg->PostMessage(IDT_CALL_DIAL, 0, (LPARAM)(LPCTSTR)(pdlg->m_pCallDlg->m_CallHistNumber));
	pdlg->m_pCallDlg->m_strEditNum = pdlg->m_pCallDlg->m_CallHistNumber;
	pdlg->m_pCallDlg->OutCallorInCall = 0;//(被叫)

    HWND win=::FindWindow(NULL,strWriteSmsDlg);
	if (win)
	{
		CSmsWriteDlg* pDlgWriteSms =(CSmsWriteDlg*)FromHandle(win);
		pDlgWriteSms->SendMessage(WM_SMS_SAVE_MSG_ANSWER_INCALL,0,0);
	}

	pdlg->PostMessage(IDT_TABED_SWITCHTO, 0, 1);
	
	//
	// 来电关闭子窗口
	//
	CWnd *pWndChild;
    CString strPopText,strTipText,strMainText,strResult;
    
    //strTipText="Incoming Call";
	strTipText.LoadString(IDS_TIPINFO_CALL);
    strMainText.LoadString(IDS_APPNAME);
	AddPrefixName(strMainText);
    Sleep(100);
    do 
    {   
        pWndChild = pdlg->GetLastActivePopup();
        pWndChild->GetWindowText(strPopText);
        strResult.Format(_T("WindowText=%s\n"), strPopText);
        //AfxMessageBox(strResult);
        if (strPopText==strMainText || strPopText=="")
        {
            break;
        }
        else if (strPopText!=strTipText)
        {
            pWndChild->SendMessage(WM_CLOSE, 0, 0); 
            Sleep(100);
            strResult+="Close";
            //AfxMessageBox(strResult);
        }
    } while(1);
}

void CCallDlg::AtRespReject(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CHSDPADlg* pdlg = (CHSDPADlg*)pWnd;

    pdlg->m_pCallDlg->m_CallHistKind = HISTKIND_RCV;		//wyw_0203 modify
	pdlg->m_pCallDlg->m_CallHistTimeStamp = COleDateTime::GetCurrentTime();
	pdlg->PostMessage(IDT_TABED_SWITCHTO, 0,0);
}

void CCallDlg::ResetCallHistData()
{
    m_CallHistKind = HISTKIND_MIS;
    m_CallHistTimeStamp = COleDateTime::GetCurrentTime();
    m_CallHistTimeSpan = 0;
    m_CallHistNumber.Empty();
}

void CCallDlg::SaveRecordToCallHist()
{
    CString strUnknown;
	stHistItem Item;
    memset(&Item, 0x00, sizeof(stHistItem));
	Item.cTimeStamp = m_CallHistTimeStamp;
	Item.cDuration  = m_CallHistTimeSpan;

    strUnknown.LoadString(IDS_UNKNOWN);

    if(m_CallHistNumber.GetLength() > 0)
	    wcsncpy((TCHAR* )Item.szNumber, m_CallHistNumber, PB_NUM_MAX);
    else
        wcscpy((TCHAR* )Item.szNumber, strUnknown);
    
	m_pHistData->SaveItem(m_CallHistKind, Item);

    //if(m_pHistCallDlg)
    //    m_pHistCallDlg->PostMessage(WM_HIST_SAVE_MSG_CALLDLG, (WPARAM)m_CallHistKind, 0);
	this->PostMessage(WM_HIST_SAVE_MSG_CALLDLG, (WPARAM)m_CallHistKind, 0);//modify by liub
}

void CCallDlg::OnButtonGetphbitem() 
{
		CSelContactDlg dlg(this, SELCONTACT_CALL);
    	dlg.DoModal();
}

LRESULT CCallDlg::OnGetNumFromPhb(WPARAM wParam, LPARAM lParam)
{	
	if(wParam == 0)
		return 0;
	UpdateData(TRUE);
	m_strEditNum = (CString)(TCHAR*)wParam;
	///////////////////////add by liub (从电话本取号码在屏幕换行显示)
	int i;
	i = m_strEditNum.GetLength();
	if (i > ONELINELENGTH)
	{
		for (int j=0; j<i; j++)
		{
			if (0 == (j%ONELINELENGTH) && j!=0 )
			{
				m_Numtemp +="\r\n";
			}
			else
				m_Numtemp += m_strEditNum[j];
		}
		
	}
	else
		m_Numtemp = m_strEditNum;
	
	m_strStatus = m_Numtemp;
	///////////////////////
	UpdateData(FALSE);

	return 0;
}


void CCallDlg::OnChangeEditNumber() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CTabedDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    for(int i = 0; i < VERSION_MAX; i++)
    {
        if(m_strEditNum.Compare((TCHAR* )szVersionNum[i]) == 0)
        {
            CVersionDlg dlg(this, (EnVersionType)i);
            dlg.DoModal();
            m_strEditNum.Empty();
            UpdateData(FALSE);
            break;
        }
    }	
}

void CCallDlg::OnUpdateEditNumber() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CTabedDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
    BOOL bUndo = FALSE;
    USHORT length = 0;
    TCHAR szEditNum[PB_NUM_MAX + 2];

    memset(szEditNum, 0x0000, sizeof(szEditNum));
	
    UpdateData(TRUE);
    length = m_strEditNum.GetLength(); 

    if(!m_blIsInCalling && length > PB_NUM_MAX)
        bUndo = TRUE;
    else
    {
        for(USHORT i = 0; i < length; i++)
        {
            if(!IsValidCallNumChar(m_strEditNum.GetAt(i)))
            {
                bUndo = TRUE;
                break;
            }
        }
    }
    
    if(bUndo)
    {
		wcsncpy(szEditNum, m_strEditNum, PB_NUM_MAX);
        m_EditNum.SetWindowText(szEditNum);
        m_EditNum.SetSel(m_EditNum.GetWindowTextLength(), m_EditNum.GetWindowTextLength());
        m_EditNum.ReplaceSel(_T(""));
        Beep(600, 50);
    }
    UpdateData(FALSE);
}

//add by liub 1030 for call history begin
void CCallDlg::InitTabCtrl()
{
    if(m_ilTabs.Create(IDB_CALL_STATE, 16, 1, 255))
    {
        m_tabHistCall.SetImageList(&m_ilTabs);
    }
	
    m_tabHistCall.SetParent(this);    
	
    CString strTabCaption;
    strTabCaption.LoadString(IDS_CALLHIST_MISSED);
    m_tabHistCall.InsertItem(0, strTabCaption, 0);
    strTabCaption.LoadString(IDS_CALLHIST_RECEIVED);
    m_tabHistCall.InsertItem(1, strTabCaption, 1);
    strTabCaption.LoadString(IDS_CALLHIST_DIALED);
    m_tabHistCall.InsertItem(2, strTabCaption, 2);
	
    DWORD dwFlags = 0;
    dwFlags |= ETC_FLAT;        
    dwFlags |= ETC_SELECTION;
    dwFlags |= ETC_GRADIENT;
    
    m_tabHistCall.SetBkgndColor(RGB(238,241,243));
    m_tabHistCall.EnableDraw(BTC_ALL);
    
    CEnTabCtrl::EnableCustomLook(dwFlags, dwFlags);
    
    //列表控件类型初始化为未接电话
    m_DispFilter = HISTKIND_MIS;
}

void CCallDlg::OnSelchangeTabHistCall(NMHDR* pNMHDR, LRESULT* pResult)
{
	switch(m_tabHistCall.GetCurSel())
    {
    case 0:
        m_DispFilter = HISTKIND_MIS;
        break;
    case 1:
        m_DispFilter = HISTKIND_RCV;
        break;
    case 2:
        m_DispFilter = HISTKIND_SND;
        break;
    default:
        m_DispFilter = HISTKIND_MIS;
        break;
    }
    DispCallHistory();
    *pResult = 0;
}

void CCallDlg::DispCallHistory()
{
	WORD  cnt;
    WORD  nIndex;
    WORD  nHistNum;
    stHistItem HistItem;
    TCHAR  szSeq[10];
    TCHAR  szName[PB_NAME_MAX + 2];
    CString strUnknown;
	
    strUnknown.LoadString(IDS_UNKNOWN);
	
    ASSERT(m_DispFilter >= HISTKIND_MIS && m_DispFilter < HISTKIND_MAX);
	
    nHistNum = m_pHistData->GetCount(m_DispFilter);
    ASSERT(nHistNum <= HIST_ITEM_MAX);
	
    strUnknown.LoadString(IDS_UNKNOWN);
	
    m_lstHistCall.SetRedraw(FALSE);
    m_lstHistCall.DeleteAllItems();
	
    for(cnt = 0; cnt < nHistNum; cnt++)
    {
        nIndex = m_DspIndexArr[m_DispFilter][cnt];
        ASSERT(nIndex < nHistNum);
		
        memset(szSeq, 0x00, sizeof(szSeq));
        memset(szName, 0x00, sizeof(szName));
        memset(&HistItem, 0x00, sizeof(stHistItem));
		
        HistItem = m_pHistData->GetItem(m_DispFilter, nIndex);
		
		if(wcscmp(HistItem.szNumber, strUnknown) == 0
			|| !m_pPbData->SearchNameByNumber(HistItem.szNumber, szName))
			wcsncpy(szName, strUnknown, PB_NAME_MAX);
		
        m_lstHistCall.InsertItem(cnt, _T(""), (int)m_DispFilter);
        m_lstHistCall.SetItemText(cnt, HISTCOL_NAME, szName);
        m_lstHistCall.SetItemText(cnt, HISTCOL_NUMBER, HistItem.szNumber);
        m_lstHistCall.SetItemText(cnt, HISTCOL_TIMESTAMP, HistItem.cTimeStamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT));
        m_lstHistCall.SetItemText(cnt, HISTCOL_DURATION, HistItem.cDuration.Format(_T("%H:%M:%S")));
        m_lstHistCall.SetItemText(cnt, HISTCOL_FREQUENCY, _itot(HistItem.nSeqNum, szSeq, 10/*sizeof(szSeq)*/));
    }
	
    m_lstHistCall.m_headerCtrl.SetSortImage(m_SortColArr[m_DispFilter], m_bSortAscendArr[m_DispFilter]);
	
    m_lstHistCall.SetRedraw(TRUE);
    m_lstHistCall.Invalidate();
    m_lstHistCall.UpdateWindow();
    
}

void CCallDlg::InitListCtrl()
{
	if(m_ilLsts.Create(IDB_CALL_STATE, 16, 1, 255))
    {
//        m_lstHistCall.SetImageList(&m_ilLsts, LVSIL_SMALL);
    }
	
    m_lstHistCall.SetParent(&m_tabHistCall);
	
    CRect rect;
    m_tabHistCall.GetItemRect(0, &rect);
    m_lstHistCall.SetWindowPos(NULL, 4, 28, rect.right, rect.bottom, TRUE);
    m_lstHistCall.GetWindowRect(&rect);
	
    BYTE arr_cols[HISTCOL_MAX][2] = {
        IDS_NAME, 80,
			IDS_NUMBER, 130,
			IDS_DATETIME, 120,
			IDS_CALLHIST_DURATION, 100,
			IDS_CALLHIST_FREQUENCY, rect.Width() - 40*3,
    };
	
    CString strColTitle;
    for(int i = 0; i < HISTCOL_MAX; i++)
    {
        strColTitle.LoadString(arr_cols[i][0]);
        m_lstHistCall.InsertColumn(i, strColTitle, LVCFMT_LEFT, arr_cols[i][1]);
    }
	
    m_lstHistCall.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstHistCall.m_headerCtrl.DrawHeader();
    
}

void CCallDlg::ResetDspIndexArr(EnHistKind type)
{
    int i, j;
	
    if(type == HISTKIND_MAX)
    {
        for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
            for(j = 0; j < HIST_ITEM_MAX; j++)
                m_DspIndexArr[i][j] = 0;
            
			for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
				for(j = 0; j < m_pHistData->GetCount((EnHistKind)i); j++)
					m_DspIndexArr[i][j] = j;
    }
    else
    {
        for(j = 0; j < HIST_ITEM_MAX; j++)
            m_DspIndexArr[type][j] = 0;
        for(j = 0; j < m_pHistData->GetCount(type); j++)
            m_DspIndexArr[type][j] = j;
    }
}

void CCallDlg::UpdateHistSortList(EnHistKind type)
{
    ResetDspIndexArr(type);
	
    if(type == HISTKIND_MAX)
    {
        for(WORD i=0; i<HISTKIND_MAX; i++)
        {
            SortHistList((EnHistKind)i, m_SortColArr[i]);
            if(!m_bSortAscendArr[i])
                ReverseSortIndexArr((EnHistKind)i);
        }
    }
    else
    {
        SortHistList(type, m_SortColArr[type]);
        if(!m_bSortAscendArr[type])
            ReverseSortIndexArr(type);
    }
	
    if(type == HISTKIND_MAX || type == m_DispFilter)
        DispCallHistory();
}

//非递减冒泡排序
void CCallDlg::BubbleSort(EnHistKind type, EnHistCol col)
{
    ASSERT(type >= HISTKIND_MIS && type < HISTKIND_MAX);
    ASSERT(col >= HISTCOL_NAME && col <= HISTCOL_FREQUENCY);
    
    WORD nNum = m_pHistData->GetCount(type);
    
    int  nSubItem = (int)col;
    int  change = 1;
    WORD tmp;
    WORD i, j;
    int  res;
    TCHAR  szName1[PB_NAME_MAX + 2];
    TCHAR  szName2[PB_NAME_MAX + 2];    
    
    for(i = 0; i < nNum - 1 && change; i++)
    {
        change = 0;
        for(j = nNum - 1; j > i; j--)
        {
            res = 0;
            switch(col)
            {
            case HISTCOL_NAME:
                memset(szName1, 0x00, sizeof(szName1));
                memset(szName2, 0x00, sizeof(szName2));
                m_pPbData->SearchNameByNumber(CString(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).szNumber), szName1);
                m_pPbData->SearchNameByNumber(CString(m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).szNumber), szName2);
                res = wcscmp(szName1, szName2);                
                break;
            case HISTCOL_NUMBER:
                res = strcmp((const char *)(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).szNumber),
					(const char *)(m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).szNumber));
                break;
            case HISTCOL_TIMESTAMP:
                if(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).cTimeStamp
                    < m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).cTimeStamp)
                    res = -1;
                break;
            case HISTCOL_DURATION:
                if(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).cDuration
                    < m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).cDuration)
                    res = -1;
                break;
            case HISTCOL_FREQUENCY:
                if(m_pHistData->GetItem(type, m_DspIndexArr[type][j]).nSeqNum
                    < m_pHistData->GetItem(type, m_DspIndexArr[type][j-1]).nSeqNum)
                    res = -1;
                break;
            }
			
            if(res < 0)
            {
                tmp = m_DspIndexArr[type][j];
                m_DspIndexArr[type][j] = m_DspIndexArr[type][j-1];
                m_DspIndexArr[type][j-1] = tmp;
                change   = 1;
            }
        }
    }
}

//以当前选中的排序关键字逆序排序
void CCallDlg::ReverseSortIndexArr(EnHistKind type)
{    
    WORD tmp;
    WORD nNum = m_pHistData->GetCount(type);
	
    for(WORD i = 0; i < nNum/2; i++)
    {
        tmp = m_DspIndexArr[type][i];
        m_DspIndexArr[type][i] = m_DspIndexArr[type][nNum-1-i];
        m_DspIndexArr[type][nNum-1-i] = tmp;
    }
}

//各字段以从小到大次序递增排序
void CCallDlg::SortHistList(EnHistKind type, EnHistCol col)
{
    ASSERT(type >= HISTKIND_MIS && type < HISTKIND_MAX);
    ASSERT(col >= HISTCOL_NAME && col <= HISTCOL_FREQUENCY);
	
    WORD j;
    WORD nNum = m_pHistData->GetCount(type);
	
    if(col == HISTCOL_TIMESTAMP)
    {
        for(j = 0; j < nNum; j++)
            m_DspIndexArr[type][j] = nNum - 1 - j;
    }
    else
    {
        BubbleSort(type, col);
    }
}

//默认以时间递减顺序显示
void CCallDlg::InitDspIndexArr()
{
    int i;
	
    for(i = HISTKIND_MIS; i < HISTKIND_MAX; i++)
    {
        m_bSortAscendArr[i] = FALSE;
        m_SortColArr[i] = HISTCOL_TIMESTAMP;
    }
    
    ResetDspIndexArr(HISTKIND_MAX);
}

void CCallDlg::OnColumnclickListHistCall(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
    // TODO: Add your control notification handler code here
    int col = pNMListView->iSubItem;
	
    ASSERT(col >= (int)HISTCOL_NAME && col <= (int)HISTCOL_FREQUENCY);
	
    if(col == (int)m_SortColArr[m_DispFilter])
    {
        m_bSortAscendArr[m_DispFilter] = !m_bSortAscendArr[m_DispFilter];
        ReverseSortIndexArr(m_DispFilter);
    }
    else
    {
        m_bSortAscendArr[m_DispFilter] = TRUE;
        m_SortColArr[m_DispFilter] = (EnHistCol)col;
        SortHistList(m_DispFilter, m_SortColArr[m_DispFilter]);
    }
	
    m_lstHistCall.m_headerCtrl.SetSortImage(col, m_bSortAscendArr[m_DispFilter]);
    DispCallHistory();
    
    *pResult = 0;
}

//////////////////////////////////////////////////////////////////////////add by liub for call2 begin
void CCallDlg::OnButtonCall2()
{
	int nCurSel;
    
    if(m_pHistData->IsEmpty(m_DispFilter))
        return;
    else if((nCurSel = m_lstHistCall.GetNextItem(-1, LVNI_SELECTED)) == -1)
    {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }
	
    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);
	
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    
    if(pComm->CommIsReady())
    {
        WORD nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
        ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));
		
        WPARAM wParam = 1;
        LPARAM lParam = (LPARAM)(LPCTSTR)(m_pHistData->GetItem(m_DispFilter, nIndex)).szNumber;
		
        if(wcslen((LPCTSTR)lParam) > 0 && wcscmp((LPCTSTR)lParam, strUnknown))
        {
            ::SendMessage(GetOwner()->GetSafeHwnd(),IDT_CALL_DIAL, wParam, lParam);
            OnOK();
        }
        else
            AfxMessageBox(IDS_SELCONNECT_NULLNUM);
    }
}
////////////////////////////////////////////////////////////////////////// add by liub end

//////////////////////////////////////////////////////////////////////////addd by liub for SaveButton
void CCallDlg::OnButtonSave()
{
    int nCurSel;
    
    if(m_pHistData->IsEmpty(m_DispFilter))
        return;

    if((nCurSel = m_lstHistCall.GetNextItem(-1, LVNI_SELECTED)) == -1)
    {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }
	
    if(m_pPbData->CheckFull())
    {
        AfxMessageBox(IDS_PB_FULL);
        return;
    }
	
    CString strUnknown;
    strUnknown.LoadString(IDS_UNKNOWN);
    
    WORD nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
    ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));
	
    stHistItem HistItem;
    memset(&HistItem, 0x00, sizeof(stHistItem));
    HistItem = m_pHistData->GetItem(m_DispFilter, nIndex);
	
    if(wcslen((TCHAR*)HistItem.szNumber) > 0
        && wcscmp((TCHAR*)HistItem.szNumber, strUnknown))
    {
        if(CompareChar((TCHAR*)HistItem.szNumber))
        {
            CSelPosition dlg(this, (TCHAR *)HistItem.szNumber);
            dlg.DoModal();
            DispCallHistory();
        }
    }
    else
        AfxMessageBox(IDS_SELCONNECT_NULLNUM);

}
//////////////////////////////////////////////////////////////////////////add by liub end

//////////////////////////////////////////////////////////////////////////add by liub for delete button begin
void CCallDlg::OnButtonDelete()
{
	int nCurSel;
    
    if(m_pHistData->IsEmpty(m_DispFilter))
        return;
    else if((nCurSel = m_lstHistCall.GetNextItem(-1, LVNI_SELECTED)) == -1)
    {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return;
    }
	
    CString str;
    str.LoadString(IDS_DELETECONFIRM);
	
    if(MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
        return;
    else
    {
        WORD nIndex;
        UINT nDelCount = m_lstHistCall.GetSelectedCount();
        
        do {
            nIndex = m_DspIndexArr[m_DispFilter][nCurSel];
            ASSERT(nIndex < m_pHistData->GetCount(m_DispFilter));
            
            int i;
            for(i = nCurSel; i < m_pHistData->GetCount(m_DispFilter) - 1; i++)
            {
                m_DspIndexArr[m_DispFilter][i] = m_DspIndexArr[m_DispFilter][i+1];
            }
            m_DspIndexArr[m_DispFilter][i] = 0;
            
            m_lstHistCall.DeleteItem(nCurSel);   
            m_pHistData->RemoveItem(m_DispFilter, nIndex);
            
            for(i = 0; i < m_pHistData->GetCount(m_DispFilter); i++)
                if(m_DspIndexArr[m_DispFilter][i] > nIndex)
                    m_DspIndexArr[m_DispFilter][i]--;
                
				if(nDelCount == 1 && !m_pHistData->IsEmpty(m_DispFilter))
				{
					if(nCurSel == m_pHistData->GetCount(m_DispFilter))
						m_lstHistCall.SetItemState(nCurSel-1, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
					else
						m_lstHistCall.SetItemState(nCurSel, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
					break;
				}
        } while((nCurSel = m_lstHistCall.GetNextItem(nCurSel-1, LVNI_SELECTED)) != -1);
    }

}
//////////////////////////////////////////////////////////////////////////add by liub end

//////////////////////////////////////////////////////////////////////////add by liub for Call Rightkey
LRESULT CCallDlg::OnMenuItemRightClick(WPARAM wParam, LPARAM lParam)
{
    UINT nID = wParam;
    
    if(nID == IDC_BUTTON_REPLY)
        OnButtonCall2();
    else if(nID == IDC_BUTTON_ABSTRACT)
        OnButtonSave();
    else if(nID == IDC_BUTTON_SENDSMS)
        OnButtonSms();
    else if(nID == IDC_BUTTON_DELETE)
        OnButtonDelete();
    
    return 0;
}
//////////////////////////////////////////////////////////////////////////end

LRESULT CCallDlg::OnSaveHist(WPARAM wParam, LPARAM lParam)
{
    EnHistKind type = (EnHistKind)wParam;
    ASSERT(type >= HISTKIND_MIS && type <= HISTKIND_MAX);
    UpdateHistSortList(type);
    return 0;
}

//////////////////////////////////////////////////////////////////////////add by liub for HeadSet answer incomingcall begin
void CCallDlg::AnswerIncomingCall(WPARAM wParam, LPARAM lParam)
{
	//$QCCAV
    char szAtBuf[512] = {0};
	//	const char ATA[]="ATA\x0d\x00";//9508 Answer
	const char ATA[]="AT$QCCAV\x0d\x00";//CDMA CM200 Answer
    strcpy(szAtBuf, ATA);
	
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
        ASSERT(pMainDlg);
        pMainDlg->m_nRingTimes = 0;
        pMainDlg->m_bInComCall = FALSE; 
       
        pMainDlg->ForegroundWnd();
        RegisterAtRespFunc(ATRESP_GENERAL_AT, CCallDlg::AtRespAnswer, pMainDlg);
		//        Sleep(100);
		//        DestroyWindow();
    }
}

void CCallDlg::EndCallbyHANDSET()
{
	OnButtonHungup();
}
//////////////////////////////////////////////////////////////////////////add by liub end

BOOL CCallDlg::EnableButton(BOOL bEnable, BOOL bSimCard)
{
	if (bSimCard)
	{
		GetDlgItem(IDC_BUTTON_SMS)->EnableWindow(bEnable);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CALL2)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_CALL)->EnableWindow(bEnable);
	}
	
	return TRUE;
}

BOOL CCallDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) 
	{		
		CHAR cKey = pMsg->wParam;
		if (cKey == VK_BACK || cKey == VK_DELETE)
		{
			OnButtonClear();  
			return TRUE;
		}
		else if (cKey == VK_RETURN)
		{
			if (!m_strEditNum.IsEmpty() && g_bSimReady)
			{
				OnButtonCall();
				return TRUE;
			}
			else
			{
				return TRUE;
			}
		}
		else if(cKey == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	if (pMsg->message == WM_CHAR) 
	{
		CHAR cKey = pMsg->wParam;
		switch (cKey)
		{
		case '0':
			m_cKeyDown = '0';
			return TRUE;
		case '1':
			m_cKeyDown = '1';
			return TRUE;
		case '2':
			m_cKeyDown = '2';
			return TRUE;
		case '3':
			m_cKeyDown = '3';
			return TRUE;
		case '4':
			m_cKeyDown = '4';
			return TRUE;
		case '5':
			m_cKeyDown = '5';
			return TRUE;
		case '6':
			m_cKeyDown = '6';
			return TRUE;
		case '7':
			m_cKeyDown = '7';
			return TRUE;
		case '8':
			m_cKeyDown = '8';
			return TRUE;
		case '9':
			m_cKeyDown = '9';
			return TRUE;
		case '+':
			m_cKeyDown = '+';
			return TRUE;
		case '*':
			m_cKeyDown = '*';
			return TRUE;
		case '#':
			m_cKeyDown = '#';
			return TRUE;
		}
	}

	if (pMsg->message == WM_KEYUP) 
	{
		CHAR cKey = pMsg->wParam;
		if ((cKey>='0' && cKey<='9') || (cKey>='a' && cKey<='k') || cKey=='`' || cKey==-69)
		{
			switch (m_cKeyDown)
			{
			case '0':
				OnKey0();
				m_cKeyDown = ' ';
				return TRUE;
			case '1':
				OnButtonNum1();
				m_cKeyDown = ' ';
				return TRUE;
			case '2':
				OnButtonNum2();
				m_cKeyDown = ' ';
				return TRUE;
			case '3':
				OnButtonNum3();
				m_cKeyDown = ' ';
				return TRUE;
			case '4':
				OnButtonNum4();
				m_cKeyDown = ' ';
				return TRUE;
			case '5':
				OnButtonNum5();
				m_cKeyDown = ' ';
				return TRUE;
			case '6':
				OnButtonNum6();
				m_cKeyDown = ' ';
				return TRUE;
			case '7':
				OnButtonNum7();
				m_cKeyDown = ' ';
				return TRUE;
			case '8':
				OnButtonNum8();
				m_cKeyDown = ' ';
				return TRUE;
			case '9':
				OnButtonNum9();
				return TRUE;
			case '+':
				OnButtonAdd();
				m_cKeyDown = ' ';
				return TRUE;
			case '*':
				OnButtonNumStar();
				m_cKeyDown = ' ';
				return TRUE;
			case '#':
				OnButtonNumHash();
				m_cKeyDown = ' ';
				return TRUE;
			}
		}
	}
	
	return CTabedDialog::PreTranslateMessage(pMsg);
}

void CCallDlg::OnButtonAdd()
{
	UpdateData(TRUE);

	if(m_strEditNum.GetLength()>0)
	{
		return; 
	}
    m_strEditNum +=_T("+");
	m_Numtemp+=_T("+");
	m_strStatus = m_Numtemp;
    
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

void CCallDlg::OnKey0()
{
	UpdateData(TRUE);

	if(m_blIsInCalling) 
    {
        SendVTS(_T("0\0"));
    }
	else
    {
        if(m_strEditNum.GetLength() >= PB_NUM_MAX)
        {
            Beep( 600, 50 );
            return;
        }
    }

	if ((m_strEditNum.GetLength()%ONELINELENGTH == 0)&&m_strEditNum!=_T("") )
	{
		m_Numtemp +="\r\n";
	}
	
    m_strEditNum +=_T("0");
	m_Numtemp+=_T("0");
	m_strStatus = m_Numtemp;
    
    UpdateData(FALSE);
    m_EditNum.GetWindowText(m_strEditNum);
    m_EditNum.SetFocus();
    m_EditNum.SetSel(m_strEditNum.GetLength(), m_strEditNum.GetLength());//设置在最后
    OnChangeEditNumber();
}

LRESULT CCallDlg::OnCallingFailed(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(IDS_CALL_ERR_CALLFAIL);
	return 0;
}
