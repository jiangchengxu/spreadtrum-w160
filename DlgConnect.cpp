// DlgConnect.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"

#include "HSDPA.h"
#include "HSDPADlg.h"
#include "dlgconnect.h"
#include "PwdConfirmDlg.h"
#include "Dev.h"
#include "ShadeButtonST.h"

#include "NetStatistics.h"


#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma comment(lib, "rasapi32.lib")
//
//	注册系统消息,接收拨号连接中的事件
//
const UINT WM_EVENTFROMRAS = ::RegisterWindowMessageA(RASDIALEVENT);

/////////////////////////////////////////////////////////////////////////////
// CDlgConnect dialog


CNetStatistics* m_pDlgUsage;

CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/)
	: CTabedDialog(CDlgConnect::IDD, pParent)
{
	m_pConnTip = NULL;

	m_hRasConn = NULL; // 不设置为NULL，是拨不了号的
	m_Oldnetwork = NW_CALLLOG_NOSRV;
	memset(&rsBeforeChangestatistics, 0x00, sizeof(RAS_STATS));
	m_BeforeChageTime.SetStatus(COleDateTime::null);
	
    m_nUpdateSec = 0;
    
	

	m_pLogData = ((CHSDPAApp *)AfxGetApp())->GetLogData();
	m_pConnProf = ((CHSDPAApp *)AfxGetApp())->GetConnProfile();
    m_pNdisConnProf = ((CHSDPAApp *)AfxGetApp())->GetNdisConnProfile();

    memset(&rsOldStatistics, 0x00, sizeof(RAS_STATS));
    memset(&rsNewStatistics, 0x00, sizeof(RAS_STATS));
    
	m_pConnTip = NULL;
	m_bPassPwdConfirm = false;

//#ifdef FEATURE_NDIS_SUPPORT
//	m_connect_type = CONN_TYPE_NDIS;
//#endif
	m_connect_type = CONN_TYPE_RAS;

	m_uTime			 = 0;
	m_TempTime			 = 0;
	m_TempLastTxOkCount			 = 0;
	m_TempLastRxOkCount			 = 0;
	TempdwByteIn = 0;
	TempdwByteOut = 0;
	
	m_nLastTxOkCount = 0;
	m_nLastRxOkCount = 0;
	m_dNDISData_Send=0.0;
	m_dNDISData_Rec=0.0;
	m_dNDISRate_Rec=0.0;
	m_dNDISRate_Send=0.0;
    m_NdisState = NDIS_STATE_DISCONNECT;

	m_pConnTipWait = NULL;

	::InitializeCriticalSection(&m_LogFileCs);
}


void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	 DDX_Control(pDX, IDC_COMBO_CONNECT_PROFILE, m_cmbConnectionName);
	// DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
	 	 
#ifdef FEATURE_NDIS_SUPPORT
	 DDX_Radio(pDX, IDC_RADIO_NDIS, m_connect_type);
#endif 
}



BEGIN_MESSAGE_MAP(CDlgConnect, CTabedDialog)

	ON_BN_CLICKED(IDC_BUTTON_CONNECT,OnClickConnectButton) 
	ON_CBN_SELCHANGE(IDC_COMBO_CONNECT_PROFILE, OnSelchangeComboConnectName)
	ON_REGISTERED_MESSAGE(WM_EVENTFROMRAS, OnRasDialEvent)
#ifdef FEATURE_NDIS_SUPPORT
	ON_BN_CLICKED(IDC_RADIO_NDIS, OnRadioNdis)
	ON_BN_CLICKED(IDC_RADIO_RAS, OnRadioRas)
	ON_WM_TIMER()
 #endif

ON_MESSAGE(WM_UPDATE_CONNECT_STATE, OnUpdateConnectStatus)
ON_MESSAGE(WM_CLICK_INTERNET_BUTTON, OnClickConnectButton)
ON_MESSAGE(WM_MODEM_IN_OUT, OnModemInOut)

    ON_WM_DESTROY()

END_MESSAGE_MAP()


BOOL CDlgConnect::OnInitDialog()
{
	CTabedDialog::OnInitDialog();
		
    
    //TODO: Add extra initialization here
	OnRadioRas();
	if (0 == m_connect_type) { 
		OnRadioNdis();
	}
	if (1 == m_connect_type) {
		OnRadioRas();
	}
	
	if(g_SetData.Setup_nNDIS==1)
	{
		GetDlgItem(IDC_RADIO_NDIS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_RAS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	}
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE	
}

//wyw_0104
void CDlgConnect::OnDestroy() 
{
	CTabedDialog::OnDestroy();
	
	::DeleteCriticalSection(&m_LogFileCs);
}



void CDlgConnect::OnModemInOut(WPARAM wParam, LPARAM lParam)
{
	if(g_bIsExist && g_bSimReady)
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	
}

void CDlgConnect::OnRadioRas()
{
	UpdateData(TRUE);
// 	CString StrDefault;
// 	StrDefault.LoadString(IDS_DEFAULT);

	WORD nConnNum = m_pConnProf->GetConnNum();
	if (nConnNum == 0)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
		//add by GL
		CString strTempDefault;
        //strTempDefault.Format(_T("%s %s"), g_SetData.Internet_szDefaultConnName,StrDefault);
		strTempDefault.Format(_T("%s"), g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szEntryName, strTempDefault);
		wcscpy(profile.szEntryName, g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szPhoneNumber, g_SetData.Internet_szLocalPhoneNumber);
			wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
			wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
		//	wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pConnProf->AddConnProfile(profile);
	} 
	nConnNum = m_pConnProf->GetConnNum();
#ifndef FEATURE_HAIER_INTERNET
/*modified by lijl begin 2008.12.19*/
//#if 0
	/*add by GL on 2008-03-24 begin*/
	if (nConnNum == 1)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		//		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
// 		CString strTempEdit;
// 		strTempEdit .Format(_T("%s (Edit.)"), g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szEntryName, _T("Flexinet[Time based]"));
		wcscpy(profile.szPhoneNumber, _T("#777"));
		wcscpy(profile.szUserName, _T("telkomnet@flexi-time"));
		wcscpy(profile.szPassword, _T("telkom"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pConnProf->AddConnProfile(profile);
	} 
	nConnNum = m_pConnProf->GetConnNum();
	if (nConnNum == 2)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		//		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
		// 		CString strTempEdit;
		// 		strTempEdit .Format(_T("%s (Edit.)"), g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szEntryName, _T("Flexinet[Volume based]"));
		wcscpy(profile.szPhoneNumber, _T("#777"));
		wcscpy(profile.szUserName, _T("telkomnet@flexi"));
		wcscpy(profile.szPassword, _T("telkom"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pConnProf->AddConnProfile(profile);
	} 
//#endif
/*modified by lijl end 2008.12.19*/
#endif
	m_cmbConnectionName.ResetContent();

	WORD cnt = 0;
	nConnNum = m_pConnProf->GetConnNum();
	for(cnt = 0; cnt < nConnNum; cnt++)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		profile = m_pConnProf->ReadConnProfile(cnt);
		//	 add data to combo box
		if(profile.bDefault)
		{
			CString StrProfileName;
			//StrProfileName.Format(_T("%s %s"),profile.szEntryName,StrDefault);
			StrProfileName.Format(_T("%s"),profile.szEntryName);
			m_cmbConnectionName.AddString(StrProfileName);
			//m_cmbConnectionName.AddString(profile.szEntryName);
		}
		else
			m_cmbConnectionName.AddString(profile.szEntryName);
	}

	WORD nIndex = m_pConnProf->GetDefaultConnProfileIndex();
    if(nIndex < nConnNum)
        m_cmbConnectionName.SetCurSel(nIndex);
    else
        m_cmbConnectionName.SetCurSel(0);
	
	
}

void CDlgConnect::OnRadioNdis()
{
	UpdateData(TRUE);
	CString StrDefault;
	StrDefault.LoadString(IDS_DEFAULT);
	// has default?
	WORD nConnNum = m_pNdisConnProf->GetConnNum();
	if (nConnNum == 0)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
		//add by GL
		CString strTempDefault;
        strTempDefault .Format(_T("%s %s"), g_SetData.Internet_szDefaultConnName,StrDefault);
		
		wcscpy(profile.szEntryName, strTempDefault);
		wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
		wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
		wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pNdisConnProf->AddConnProfile(profile);
	}
/*modified by lijl begin 2008.12.19*/
#if 0
	/*add by GL on 2008-03-24 begin*/
	if (nConnNum == 1)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		//		profile.bDefault = TRUE;
		profile.AuthProtocol = (EnAuthProtocol)g_SetData.Internet_nAuthType;
		CString strTempEdit;
		strTempEdit .Format(_T("%s (Edit.)"), g_SetData.Internet_szDefaultConnName);
		wcscpy(profile.szEntryName, strTempEdit);
		wcscpy(profile.szUserName, g_SetData.Internet_szUserName);
		wcscpy(profile.szPassword, g_SetData.Internet_szPassword);
		wcscpy(profile.szAPN, _T("ibox.tim.it"));
		profile.NetPreConfig = (EnInternetPreConf)m_connect_type; 
		m_pNdisConnProf->AddConnProfile(profile);
	} 
#endif
/*modified by lijl end 2008.12.19*/	

	m_cmbConnectionName.ResetContent();
	
	// load config file.
	WORD cnt = 0;
	nConnNum = m_pNdisConnProf->GetConnNum();
	for(cnt = 0; cnt < nConnNum; cnt++)
	{
		StConnProfile profile;
		memset(&profile, 0x00, sizeof(StConnProfile));
		
		profile = m_pNdisConnProf->ReadConnProfile(cnt);
		
		// add data to combo box
		m_cmbConnectionName.AddString(profile.szEntryName);
	}

	WORD nIndex = m_pNdisConnProf->GetDefaultConnProfileIndex();
    if(nIndex < nConnNum)
        m_cmbConnectionName.SetCurSel(nIndex);
    else
        m_cmbConnectionName.SetCurSel(0);
}

void CDlgConnect::OnSelchangeComboConnectName()
{
    // TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
    StConnProfile profile;
    int nIndex = m_cmbConnectionName.GetCurSel();
	
	BOOL bClear = FALSE;
	if(nIndex == CB_ERR)
	{
		bClear = TRUE;
	}
	if (0 == m_connect_type && (nIndex >= m_pNdisConnProf->GetConnNum())) 
	{
		bClear = TRUE;
	} 
	if (1 == m_connect_type && (nIndex >= m_pConnProf->GetConnNum())) 
	{
		bClear = TRUE;
	} 
	
	if (bClear) {
		m_bSetDefault = FALSE;
		m_strPassword = _T("");
		m_strUserName = _T("");
		m_strPhoneNumber = _T("");
		m_strAPN = "";

	}
	else
	{
		memset(&profile, 0x00, sizeof(StConnProfile));
		if (0 == m_connect_type) {
			profile = m_pNdisConnProf->ReadConnProfile(nIndex);
		} 
		else if (1 == m_connect_type) {
			profile = m_pConnProf->ReadConnProfile(nIndex);
		}
		else {
			ASSERT(TRUE);
		}
		m_bSetDefault = profile.bDefault;
        m_strPhoneNumber.Format(_T("%s"), profile.szPhoneNumber);
        m_strUserName.Format(_T("%s"), profile.szUserName);
        m_strPassword.Format(_T("%s"), profile.szPassword);
        m_strAPN.Format(_T("%s"), profile.szAPN);
	}
	
    UpdateData(FALSE);	
}

UINT WaitThread(LPVOID lpParam)
{
	CDlgConnect *pDlg = (CDlgConnect *)lpParam;

	//CInternetConnTip connTip(_T("Searching network, please wait..."));
	CCopsInitTip connTip(_T("Searching network, please wait..."));
	//pDlg->m_pConnTipWait = &connTip;
	pDlg->m_pConnTipWait = &connTip;

	if (connTip.DoModal() == IDCANCEL)
	{
		pDlg->m_pConnTipWait = NULL;
		return 0;
	}

	pDlg->m_pConnTipWait = NULL;
    return 1;
}

//LRESULT CDlgConnect::OnClickConnectButton(WPARAM wParam, LPARAM lParam)
void CDlgConnect::OnClickConnectButton()
{
	CHSDPADlg *pDlg = (CHSDPADlg *)AfxGetMainWnd();
	m_pDlgUsage=pDlg->m_pUsageDlg;


	if (wcslen(pDlg->m_szPLMN) == 0)
	{
		AfxBeginThread(WaitThread, this);

		return;
	}

	if (pDlg->m_pCallDlg->m_blIsInCalling)		//wyw_0408 add
	{
		AfxMessageBox(IDS_INCALLING);
		return;
	}

	UpdateData(TRUE);
    if(g_bDataService) {
			if (CONN_TYPE_NDIS == m_connect_type) {
			
			
			if(!m_BAutoDisconnect)
			{
				if(AfxMessageBox(IDS_INT_DISC_TIP, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
				{
					return;
				}
			}

			pDlg->m_connectionPage.OnConnectEx(m_NdisState, NULL); // Ndis disconnect cmd

			    m_BAutoDisconnect = FALSE;
                g_bConnLimitTip = TRUE;


		}
		else {
			OnButtonDisconnect();
		}
	
	}
    else
    {
		
		if (CONN_TYPE_NDIS == m_connect_type) {

			CStdioFile file;
			file.Open(_T("NDISErrDebug.log"), CFile::modeCreate);
			file.Close();
		   			

			CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

			if(!(pMainWnd-> m_ProcessDeviceNotice))
            {
				CString strProcessDevice;
				strProcessDevice.LoadString(IDS_NETCARD_NOT_FOUND);
				
				AfxMessageBox(strProcessDevice);
				return ;
            }

			PromptCheckBeforeConnect();
			if((g_SetData.Internet_nReachUsageLimit)
			&&(g_SetData.Internet_nPromptDisconnEnable)
			&&(!g_SetData.Internet_nPromptPwdConfirm))
			{
					CString strAppName;
				strAppName.LoadString(IDS_APPNAME);
				AddPrefixName(strAppName);

				CString strReachUsageLimit;
				strReachUsageLimit.LoadString(IDS_INTERNET_REACHLIMIT);
				MessageBox(strReachUsageLimit,strAppName,MB_OK);
				return ;
			}

			if((g_SetData.Internet_nReachUsageLimit)
			&&(g_SetData.Internet_nPromptPwdConfirm))
			{
				CPwdConfirmDlg PwdConDlg(this);
				int nRes = PwdConDlg.DoModal();
				if(nRes != IDOK)
				{
					return;
				}

				m_bPassPwdConfirm = true;
			}

			// TODO: Add your control notification handler code here

		
			int nIndex = m_cmbConnectionName.GetCurSel();
            if (nIndex == CB_ERR || nIndex >= m_pNdisConnProf->GetConnNum())
			{
				return ;
			}

			///SIM card state check 
			/*if(CPIN_SIM_NONE_REQUIRED != pMainWnd->m_cHandlePin.m_nSimStat)
			{
			AfxMessageBox(IDS_INT_CONN_SIMNOTREADY);
				return;
			}*/
	
            StConnProfile NdisProfile;
			memset((void *)&NdisProfile, 0, sizeof(StConnProfile));
			memset(&NdisProfile, 0x00, sizeof(StConnProfile));
            NdisProfile = m_pNdisConnProf->ReadConnProfile(nIndex);
		
			CHSDPADlg *pDlg = (CHSDPADlg *)AfxGetMainWnd();
			pDlg->m_connectionPage.OnConnectEx(m_NdisState, &NdisProfile); // Ndis connect cmd

		} 
		else if (CONN_TYPE_RAS == m_connect_type) {
			if (m_pConnProf->GetConnNum() == 0) {
				if(IDOK == AfxMessageBox(IDS_INT_NOPROFILE, MB_ICONINFORMATION | MB_OKCANCEL))
				{
					//OnButtonNewconn();
				}
				else
					return ;
			}
			

			OnButtonDial();
		}
		else {
			ASSERT(TRUE);
		}

		//Set connection type for CNetStatisitcs
		m_pDlgUsage->setConnection(m_connect_type);	
	
    }
   // return (LRESULT)0;
}

void CDlgConnect::OnButtonDial()
{
#if 0
    char szTip[100] = {0};
    COleDateTime datetime;
    DWORD dwMByte;
    DWORD dwMin;

    datetime.SetDateTime(g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
        , g_SetData.Internet_nPromptDay, 0,0,0);  
    
    dwMin = m_pLogData->CalculateTotalDuration(datetime);
    dwMByte = m_pLogData->CalculateTotalFlux(datetime);

    sprintf(szTip, "dwMByte: %d, dwMin: %d", dwMByte, dwMin);
    AfxMessageBox(szTip);
#else

	WriteLog(_T("ONButtonDial Begin!"));
	PromptCheckBeforeConnect();
	if((g_SetData.Internet_nReachUsageLimit)
		&&(g_SetData.Internet_nPromptDisconnEnable)
		&&(!g_SetData.Internet_nPromptPwdConfirm))
	{
		WriteLog(_T("enter messagebox!"));
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strReachUsageLimit;
		strReachUsageLimit.LoadString(IDS_INTERNET_REACHLIMIT);
		MessageBox(strReachUsageLimit,strAppName,MB_OK);
		return;
	}

	if((g_SetData.Internet_nReachUsageLimit)
		&&(g_SetData.Internet_nPromptEnable))
	{
		WriteLog(_T("enter messagebox!"));
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strReachUsageLimit;
		strReachUsageLimit.LoadString(IDS_INTERNET_REACHLIMIT);
		MessageBox(strReachUsageLimit,strAppName,MB_OK);
		return;
	}

	if((g_SetData.Internet_nReachUsageLimit)
		&&(g_SetData.Internet_nPromptPwdConfirm))
	{
		WriteLog(_T("Pwd confirm enter!"));
		CPwdConfirmDlg PwdConDlg(this);
		int nRes = PwdConDlg.DoModal();
		if(nRes != IDOK)
		{
			return;
		}
		m_bPassPwdConfirm = true;
		WriteLog("Pwd confirm succ");
	}

    // TODO: Add your control notification handler code here
    RASCONNSTATUS rsStatus;
    RASDIALPARAMS RasDialParams;
    StConnProfile profile;
    BOOL bPassword = TRUE;
    wchar_t m_szLastError[500];

	memset(&profile, 0x00, sizeof(StConnProfile));

    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

    int nIndex = m_cmbConnectionName.GetCurSel();
    
    if(nIndex == CB_ERR || nIndex >= m_pConnProf->GetConnNum())
    {
        return;
    }
    
	//SIM card state check 
//     if(CPIN_SIM_NONE_REQUIRED != pMainWnd->m_cHandlePin.m_nSimStat)
//     {
//         AfxMessageBox(IDS_INT_CONN_SIMNOTREADY);
//         return;
//     }

    if(!FindUSBModem())
    {
        AfxMessageBox(IDS_USB_PORT_INI);
        return;
    }

   // GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(FALSE);
   // PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)2, (LPARAM)TRUE);

    memset(&rsNewStatistics, 0x00, sizeof(RAS_STATS));
    rsNewStatistics.dwSize = sizeof(RAS_STATS);

    memset(&rsStatus, 0x00, sizeof(RASCONNSTATUS));
    rsStatus.dwSize = sizeof(RASCONNSTATUS);
	

    RasGetConnectStatus(m_hRasConn, &rsStatus);
	rsStatus.dwError=0;

// 	CString strStatus;
// 	strStatus.Format(_T("%d"),rsStatus.rasconnstate);
// 	AfxMessageBox(strStatus);
	//是否已建立连接
    if(rsStatus.rasconnstate != RASCS_Connected)
    {
		WriteLog(_T("RASCS_Disconnected"));

        UpdateData(TRUE);

		m_pConnProf->LoadConnProfile();
        profile = m_pConnProf->ReadConnProfile(nIndex);
        
		m_StrUserName=profile.szUserName;

		if(m_StrUserName.Compare(_T(""))==0)
		{
			AfxMessageBox(_T("Please input user name!"));
			return;
		}
		m_strAPN=profile.szAPN;
		m_strLastAPN=m_strAPN;

		CString strName =profile.szEntryName;
		strName.Insert(0,_T("profile.szEntryName is "));
		WriteLog(strName);

		////if 3G type is WCDMA, the APN checking is required. 
		if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
			(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
		{//检查APN
			if(m_strAPN.IsEmpty())
			{
				GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(TRUE);
				PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)1, (LPARAM)TRUE);
				AfxMessageBox(IDS_INT_APN_EMPTY, MB_ICONINFORMATION);
				return;
			}
		}
		///////If the 3Gtype is WCDMA, the APN is required. 
		if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
			(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
		{
			 if(wcscmp(m_strLastAPN, m_strAPN) != 0)
			{	
				CString strAPN = _T("Diff APN,the last APN is ");
				if(0 == m_strLastAPN.GetLength())
				{
					strAPN.Insert(888,_T("NULL "));
				}
				else
				{
					strAPN.Insert(888,m_strLastAPN);
				}			
				strAPN.Insert(888,_T(" Current APN is "));
				strAPN.Insert(888,m_strAPN);
				WriteLog(strAPN);
				USES_CONVERSION;
				char *ch_APN=W2A(m_strAPN);
				if(SndAtCGDCONT(ch_APN))
				{
                //GetDlgItem(IDC_EDIT_APN)->EnableWindow(FALSE);
                //GetDlgItem(IDC_BUTTON_SAVECONN)->EnableWindow(FALSE);
					nCGDCONTType = 0;

					WriteLog(_T("Set APN Succ"));
				}
				else
				{
					WriteLog(_T("Set APN faild!"));
				}
			}
	
			CString str = m_strAPN;
			str.Insert(0,_T("APN is "));
			WriteLog(str);
		}
		///////////////////////////////
        
     //   profile = m_pConnProf->ReadConnProfile(nIndex);

        m_strActiveConnection.Format(_T("%s"), profile.szEntryName);

        BOOL BRes = SyncSystemConnEntry(profile);
		if (!BRes)
		{
			WriteLog(_T("SyncSystemConnEntry faild!"));
		}

        memset(&RasDialParams, 0x00, sizeof(RASDIALPARAMS));
        RasDialParams.dwSize = sizeof(RASDIALPARAMS);
        wcscpy((PTCHAR)RasDialParams.szEntryName, profile.szEntryName);
    
        DWORD  dRes = RasGetEntryDialParams(NULL, &RasDialParams, &bPassword);
		if(dRes != 0)
		{
			WriteLog(_T("RasGetEntryDialParams faild!"));

			CString str;
			str.Format(_T("%d"),dRes);
			MessageBox(str);
		}
		memset(&rsOldStatistics, 0, sizeof(RAS_STATS));

		/*
		CString strUserAndPass = _T("user:\rPass:");
		strUserAndPass.Insert(5,RasDialParams.szUserName);
		strUserAndPass.Insert(PWLEN + 1,RasDialParams.szPassword);
		MessageBox(strUserAndPass);
		*/
		//if(profile.szUserName
		
// 		if(wcscmp(g_SetData.Internet_szUserName,profile.szUserName)!=0||
// 		wcscmp(g_SetData.Internet_szUserName,profile.szUserName)!=0)
// 		{
// 			AfxMessageBox(_T("Invalid user name or password!"));
// 			return;
// 		}

		wcscpy(RasDialParams.szPassword, profile.szPassword);//for vista os
		

		//added by lijiali 2007.8.10
		memset(&dTempItem,0x00,sizeof(stLogItem));		
		memset(&rsBeforeChangestatistics, 0x00, sizeof(RAS_STATS));
		m_BeforeChageTime.SetStatus(COleDateTime::null);
		
		m_StrUserName=profile.szUserName;
        if(m_hRasConn != NULL)
			m_hRasConn=NULL;
		DWORD dwRet = 0;
        dwRet = RasDial(NULL, NULL, &RasDialParams,
                        0xFFFFFFFF, this->m_hWnd, &m_hRasConn);

        SetTimer(IDT_TIMER_DATASERVICECONNECT, 1000*120, NULL);

        // If dwRet is non-zero, we had an error. Retrieve
        // the error into the m_szLastError char buffer.
		if(dwRet)
        {

            if(RasGetErrorString((UINT)dwRet, (LPWSTR)m_szLastError,
                sizeof(m_szLastError)) != 0)
            {
                CString strRASErrFormat;
                strRASErrFormat.LoadString(IDS_INT_ERR_RASERRORFORMAT);
                swprintf(m_szLastError, strRASErrFormat, dwRet);
            }
            //MessageBox(m_szLastError);
			WriteLog("RasDial Faild!");
        }
        else
        {
			m_nUpdateSec = 0;
			m_BAutoDisconnect = FALSE;
            g_bConnLimitTip = TRUE;
            OpenConnTip();
        }
    }
    else //已经建立连接
    {
		WriteLog(_T("RASCS_Connected"));

        CString strInfo;
        CString strConnected;

        g_bDataService = TRUE;
        SetTimer(IDT_TIMER_DATASERVICEDURATION, 1000, NULL);
        
        strConnected.LoadString(IDS_INT_CONNECTED);
        strInfo = m_strActiveConnection + strConnected;
        AfxMessageBox(strInfo);
    }
	WriteLog(_T("ONButtonDial end!"));
#endif
}

void CDlgConnect::WriteLog(CString str)
{
	::EnterCriticalSection(&m_LogFileCs);
	COleDateTime  dtCurrTime;
	dtCurrTime = COleDateTime::GetCurrentTime();
	dtCurrTime.Format();
	
	
	str.Insert(str.GetLength() + 5,_T("\n\n"));
	if(!(m_file.Open(_T("NetDebug.log"),   CFile::modeReadWrite)))
	{
		m_file.Open(_T("NetDebug.log"),  CFile::modeCreate | CFile::modeReadWrite);
	}	
	if (NULL != m_file) {
		DWORD dwActual = m_file.SeekToEnd();
		m_file.WriteString(str);
		m_file.Close();
	}
    ::LeaveCriticalSection(&m_LogFileCs);
	return;
	
}


void CDlgConnect::PromptCheckBeforeConnect()
{
	WriteLog(_T("PromptCheckBeforeConnect enter"));
	if(m_bPassPwdConfirm)
	{
		return;
	}
	
	//选择了到达流量时提示或断开连接
    if((g_SetData.Internet_nPromptEnable) ||(g_SetData.Internet_nPromptDisconnEnable))
    {
        char szTip[100] = {0};
        CString str;
        COleDateTime datetime;
        datetime.SetDateTime(g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
			, g_SetData.Internet_nPromptDay, 0,0,0);
		
        //Flux
        if(g_SetData.Internet_nPromptFluxOrDuration == 0)
        {
            DWORD dwByte = 0;
            dwByte = m_pLogData->CalculateTotalFlux(datetime); //以Byte为流量统计单位
            
			if(g_SetData.Internet_nPromptDisconnEnable||g_SetData.Internet_nPromptEnable)			// 当到达设置值时直接断开
			{
				if(dwByte >= ((g_SetData.Internet_nPromptFluxVal) * 1024 * 1024)) //超过限制
				{
					WriteLog(_T("reached usagelimit!"));
					g_SetData.Internet_nReachUsageLimit = TRUE;					
				}
				else
				{
					WriteLog(_T("Unreached usagelimit!"));
					g_SetData.Internet_nReachUsageLimit = FALSE;
				}
				SaveIniFile();
			}
        }
        else //Duration
        {
            DWORD dwMin = 0;
            dwMin = m_pLogData->CalculateTotalDuration(datetime);
			
			if(g_SetData.Internet_nPromptDisconnEnable||g_SetData.Internet_nPromptEnable)
			{
				if(dwMin >= g_SetData.Internet_nPromptDurationVal * 60 )  //以秒为单位
				{
					WriteLog(_T("reached usagelimit!"));
					g_SetData.Internet_nReachUsageLimit = TRUE;					
				}
				else
				{
					WriteLog(_T("Unreached usagelimit!"));
					g_SetData.Internet_nReachUsageLimit = FALSE;
				}
				SaveIniFile();
			}
        }
    }
	return;
}

/**************修改连接状态**/
LRESULT CDlgConnect::OnUpdateConnectStatus(WPARAM wParam, LPARAM lParam)
{
    CString str;
    CString strTemp;

	int ntry = 0;

	m_NdisState = (NdisStateRet)wParam;

    CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
  //  CShadeButtonST *pInternetBtn = &(pMainDlg->m_pTabDialog->m_btn4);
	//CButtonST *pConnectBtn=(CButtonST *)GetDlgItem(IDC_BUTTON_CONNECT);
	CWnd* pButtonConnect = GetDlgItem(IDC_BUTTON_CONNECT);
	
	

   
	switch(m_NdisState)
	{
	case NDIS_STATE_CONNECT:
	//	memset(&dTempItem, 0x00, sizeof(stLogItem));		
		
        str.LoadString(IDS_INT_BUTTON_DISC);
	

		pButtonConnect->SetWindowText(str); //把Connect按钮的text 改为 Disconnect

           pButtonConnect->InvalidateRect(NULL);
      pButtonConnect->UpdateWindow();

         pMainDlg->m_pTabDialog->InvalidateRect(NULL);
         pMainDlg->m_pTabDialog->UpdateWindow();
         pMainDlg->UpdateWindow();

        m_strConnStatus.LoadString(IDS_INT_CONNSTS_CONNECTED);
		TRACE(_T("NDIS_STATE_CONNECT: %s\n"), m_strConnStatus);
        UpdateConnTip(m_strConnStatus);
        CloseConnTip();

		GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RAS)->EnableWindow(FALSE);
	//	PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)FALSE, NULL); // all profiles can not modified.

        g_bDataService = TRUE;
		g_bConnLimitTip = TRUE; // 保证连接成功后能够在OnTimer中判断限制
		m_BeforeChageTime = COleDateTime::GetCurrentTime();
		m_lastNdisTxReportDataTime = CTime::GetCurrentTime();
		m_lastNdisRxReportDataTime = CTime::GetCurrentTime();
        SetTimer(IDT_TIMER_DATANDISSERVICEDURATION, 1000 , NULL); 
		for (ntry = 0; ntry < 10; ntry ++)
		{
			if(NW_CALLLOG_NOSRV != ((CHSDPADlg*)AfxGetMainWnd())->m_NetCallLog)
			{
				m_Oldnetwork = ((CHSDPADlg*)AfxGetMainWnd())->m_NetCallLog;
				break;
			}
			else
			{
				Sleep(150);
			}
		}
		
		memset(&rsBeforeChangestatistics, 0x00, sizeof(RAS_STATS));

		break;
	case NDIS_STATE_ABORT:
		m_strConnStatus.LoadString(IDS_INT_CONNSTS_CONNECTING);
		TRACE(_T("NDIS_STATE_ABORT: %s\n"), m_strConnStatus);
		OpenConnTip(m_strConnStatus);

       
		break;
	case NDIS_STATE_DISCONNECT:
        str.LoadString(IDS_INT_BUTTON_CONN);

        pButtonConnect->SetWindowText(str); //把Connect按钮的text 改为 Connect
	     pButtonConnect->SetWindowText(str);
		pButtonConnect->InvalidateRect(NULL);
        pButtonConnect->UpdateWindow();

        pMainDlg->m_pTabDialog->InvalidateRect(NULL);
        pMainDlg->m_pTabDialog->UpdateWindow();
        pMainDlg->UpdateWindow();

		// status edit

		

 		GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(TRUE);
 		GetDlgItem(IDC_RADIO_RAS)->EnableWindow(TRUE);

		g_bDataService = FALSE;
		m_bPassPwdConfirm = false;
		if(m_TempTime != 0)
		{
			StasticNdisLog();
		}
		
        KillTimer(IDT_TIMER_DATANDISSERVICEDURATION);	// stop calculate time when connection is terminated.
	    
		m_uTime = 0;
        m_nLastTxOkCount = 0;
        m_nLastRxOkCount = 0;
		m_TempLastTxOkCount = 0;
		m_TempLastRxOkCount = 0;
		
		TempdwByteIn = 0;
		TempdwByteOut = 0;
		
		m_strConnStatus.Empty();
		m_strDuration.Empty();
			UpdateData(FALSE);
		
        //连接未能建立，活动连接为空
        m_strActiveConnection="";
        m_strActiveProfile="";
	

		break;
	default:
		break;
	}
    UpdateData(FALSE);

	return TRUE;
}

void CDlgConnect::OpenConnTip(CString strTip)
{
	// @@@@@@
	if(m_pConnTip == NULL)
	{
		CInternetConnTip ConnTip(strTip);
		m_pConnTip = &ConnTip;
		m_pConnTip->m_hRasConn=m_hRasConn;
		
		m_pConnTip->DoModal();
		m_hRasConn = m_pConnTip->m_hRasConn;
		m_pConnTip = NULL;
	}
}

void CDlgConnect::UpdateConnTip(CString str)
{
    if(m_pConnTip)
    {
        ((CStatic *)(m_pConnTip->GetDlgItem(IDC_STATIC_INT_TIP)))->SetWindowText(str);
    }
}

void CDlgConnect::CloseConnTip()
{
    if (m_pConnTip)
    {
		Sleep(1000);
        m_pConnTip->PostMessage(WM_CLOSE, NULL, NULL);
    }
}


BOOL CDlgConnect::CreateSystemConnEntry(const StConnProfile &profile)
{
    RASENTRY RasEntry;
    RASDIALPARAMS RasDialParams;
    DWORD dwEntryInfoSize;
    BOOL bPassword = FALSE;
	DWORD  Dres;
	
    WriteLog("Enter CreateSystemConnEntry");
	
    memset(&RasEntry, 0x00, sizeof(RASENTRY));
    memset(&RasDialParams, 0x00, sizeof(RASDIALPARAMS));
	
    RasEntry.dwSize = sizeof(RASENTRY);
    dwEntryInfoSize = sizeof(RASENTRY);
    
    wcscpy(RasEntry.szLocalPhoneNumber, profile.szPhoneNumber);
    wcscpy(RasEntry.szDeviceName, gszModemFriendlyName);
    wcscpy(RasEntry.szDeviceType, g_SetData.Internet_szDeviceType);
	
    RasEntry.dwFramingProtocol = g_SetData.Internet_dwFramingProtocol;
    RasEntry.dwfOptions = g_SetData.Internet_dwfOptions;
    RasEntry.dwCountryID = g_SetData.Internet_dwCountryID;
    RasEntry.dwCountryCode = g_SetData.Internet_dwCountryCode;
    RasEntry.dwfNetProtocols = g_SetData.Internet_dwfNetProtocols;
	
#if WINVER>=0x500
	
	if((wcscmp(g_SetData.Setup_sz3GType,_T("WCDMA"))==0)||
		(wcscmp(g_SetData.Setup_sz3GType,_T("wcdma"))==0))
	{
		if(profile.AuthProtocol == AUTHPROTOCOL_CHAP)
		{
			RasEntry.dwfOptions |= RASEO_RequireCHAP;
		}
		else if(profile.AuthProtocol == AUTHPROTOCOL_PAP)
		{
			RasEntry.dwfOptions |= RASEO_RequirePAP;
		}
	}
    RasEntry.dwType = g_SetData.Internet_dwType;
    RasEntry.dwSubEntries = g_SetData.Internet_dwSubEntries;
    RasEntry.dwEncryptionType = g_SetData.Internet_dwEncryptionType;
#endif
    
    Dres = RasSetEntryProperties(NULL, profile.szEntryName, &RasEntry, 
		dwEntryInfoSize, NULL, 0);
	
	if(Dres)
	{
		WriteLog("RasSetEntryProperties Faild!");
	}
	
	RasDialParams.dwSize = sizeof(RASDIALPARAMS); 
    
#if WINVER>=0x500
    RasDialParams.dwSubEntry = g_SetData.Internet_dwSubEntry;
#endif
	
    wcscpy((PTCHAR)RasDialParams.szEntryName, profile.szEntryName);
    wcscpy((PTCHAR)RasDialParams.szPhoneNumber, profile.szPhoneNumber);
    wcscpy((PTCHAR)RasDialParams.szUserName, profile.szUserName);
    wcscpy((PTCHAR)RasDialParams.szPassword, profile.szPassword);
    
    Dres = RasSetEntryDialParams(NULL, &RasDialParams, bPassword);
	
	if(Dres)
	{
		WriteLog("RasSetEntryDialParams Faild!");
	}
	
    return TRUE;
}

BOOL CDlgConnect::ModifySystemConnEntry(const StConnProfile &profile)
{
    RASENTRY RasEntry; //连接记录
    RASDIALPARAMS RasDialParams;//连接参数
    DWORD dwEntryInfoSize;
    BOOL bPassword = FALSE;
	WORD Dres = 0;
	
	WriteLog("Enter ModifySystemConnEntry");
    
    memset(&RasEntry, 0x00, sizeof(RASENTRY));
    memset(&RasDialParams, 0x00, sizeof(RASDIALPARAMS));
	
    RasEntry.dwSize = sizeof(RASENTRY);
    dwEntryInfoSize = sizeof(RASENTRY);
    
    Dres = RasGetEntryProperties(NULL, profile.szEntryName, &RasEntry,
		&dwEntryInfoSize, NULL, 0);
	if(Dres)
	{
		WriteLog("RasGetEntryProperties faild!");
	}
    
    wcscpy(RasEntry.szLocalPhoneNumber, profile.szPhoneNumber);
    wcscpy(RasEntry.szDeviceName, gszModemFriendlyName);
	wcscpy(RasEntry.szDeviceType, g_SetData.Internet_szDeviceType);
    
    RasEntry.dwFramingProtocol = g_SetData.Internet_dwFramingProtocol;
    RasEntry.dwfOptions = g_SetData.Internet_dwfOptions;
    RasEntry.dwCountryID = g_SetData.Internet_dwCountryID;
    RasEntry.dwCountryCode = g_SetData.Internet_dwCountryCode;
    RasEntry.dwfNetProtocols = g_SetData.Internet_dwfNetProtocols;
	
#if WINVER>=0x500
    if(profile.AuthProtocol == AUTHPROTOCOL_CHAP)
    {
        RasEntry.dwfOptions |= RASEO_RequireCHAP;
    }
    else if(profile.AuthProtocol == AUTHPROTOCOL_PAP)
    {
        RasEntry.dwfOptions |= RASEO_RequirePAP;
    }
	
    RasEntry.dwType = g_SetData.Internet_dwType;
    RasEntry.dwSubEntries = g_SetData.Internet_dwSubEntries;
    RasEntry.dwEncryptionType = g_SetData.Internet_dwEncryptionType;
#endif
	//set entry
    Dres = RasSetEntryProperties(NULL, profile.szEntryName, &RasEntry,
		dwEntryInfoSize, NULL, 0);
    
	if(Dres)
	{
		WriteLog("RasSetEntryProperties faild!");
	}
    RasDialParams.dwSize = sizeof(RASDIALPARAMS);                
    wcscpy(RasDialParams.szEntryName, profile.szEntryName);
    
    Dres = RasGetEntryDialParams(NULL, &RasDialParams, &bPassword);
	if(Dres)
	{
		WriteLog("RasGetEntryDialParams faild!");
	}
	
    bPassword = FALSE;
    wcscpy(RasDialParams.szPhoneNumber, profile.szPhoneNumber);
    wcscpy(RasDialParams.szUserName, profile.szUserName);
    wcscpy(RasDialParams.szPassword, profile.szPassword);
	
    Dres = RasSetEntryDialParams(NULL, &RasDialParams, bPassword);
	
	if(Dres)
	{
		WriteLog("RasSetEntryDialParams faild!");
	}
    return TRUE;
}



void CDlgConnect::RspAtCGDCONT(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgConnect* pDlg = (CDlgConnect*)pWnd;
    pDlg->KillTimer(IDT_CGDCONT_TIMEOUT);
    
    CString RetStr;
    RetStr = strArr[0];
    int nHead, nTail;
    
    BOOL bResult=FALSE;
    
    switch(pDlg->nCGDCONTType)
    {
    case 0:
        if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
        {
            //AfxMessageBox(IDS_SETUP_CONNPOINTOK, MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            //AfxMessageBox(IDS_SETUP_CGDCONT_ERROR);
        } 
        break;
    case 1:
        if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0)
        {
            //AfxMessageBox(IDS_SETUP_CGDCONT_ERROR);
        }
        else
        {
            //success
            //+CGDCONT: 1,"IP","","",0,0
            //+CGDCONT: 1,"IP","cmwap","",0,0
            CString strAPN;
            nHead=RetStr.Find(_T("\",\""),0);
            if (nHead!=-1)
            {
                nTail=RetStr.Find(_T("\",\""),nHead+1);
                if (nTail!=-1)
                {
                    RetStr=strArr[0]+nHead+3;
                    strAPN=RetStr.Left(nTail-nHead-3);
                    bResult=TRUE;
                }
            } 
            else
            {
            }
            pDlg->m_strAPN=strAPN;
        }
        break;
    case 2:
        break;
    }	
    pDlg->nCGDCONTType=-1;
    
    //save last APN
    pDlg->m_strLastAPN = pDlg->m_strAPN;
    //pDlg->PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)0, (LPARAM)TRUE);   
}



BOOL CDlgConnect::SndAtCGDCONT(CString strConnPoint)
{
    char szAtBuf[CONNECTION_APN_LEN+30] = {0};
    
    sprintf(szAtBuf, "AT+CGDCONT=1,\"IP\",\"%s\"\r", strConnPoint);
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        SetTimer(IDT_CGDCONT_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCGDCONT, this);
        return TRUE;
    }
    return FALSE;
}


//将系统拨号网络中的连接项与连接文件中的配置项同步
BOOL CDlgConnect::SyncSystemConnEntry(const StConnProfile &profile)
{
    DWORD i;
    DWORD dwRet = 0;
    DWORD dwRetries = 5;
    DWORD dwEntries = 0;
    BOOL  bNeedCreate = TRUE;
    DWORD dwErr = ERROR_SUCCESS;
    RASENTRYNAME* pRasEntryName = NULL;
    DWORD dwCb = sizeof(RASENTRYNAME);

	WriteLog("Enter SyncSystemConnEntry");

    while(dwRetries--)
    {
        if(NULL != pRasEntryName)
        {
            delete []pRasEntryName;
            pRasEntryName = NULL;
        }

        pRasEntryName = new RASENTRYNAME[dwCb/sizeof(RASENTRYNAME)];
        
        if(NULL == pRasEntryName)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            AfxMessageBox(IDS_ERROR_NOT_ENOUGH_MEMORY);
            return FALSE;
        }
        
        pRasEntryName->dwSize = sizeof(RASENTRYNAME);
        dwErr = RasEnumEntries(NULL, NULL, pRasEntryName, &dwCb, &dwEntries);  //get PB entry names

        //603: ERROR_BUFFER_TOO_SMALL
        if(603 != dwErr)
            break;
    }
	CString str;
	str.Format(_T("dwErr:%d dwRetries:%d"),dwErr,dwRetries);
	str.Insert(0,_T("RasEnumEntries return value and dwRetries  "));
	WriteLog(str);

    if(ERROR_SUCCESS == dwErr)  //RasEnumEntries succ,then compare the entry name
    {
        for(i = 0; i < dwEntries; i++)
        {
            if(wcscmp(pRasEntryName[i].szEntryName, profile.szEntryName) == 0)
            {
				WriteLog("Find the entry name in remote access PB,ModifySystemConnEntry");

                ModifySystemConnEntry(profile);
                break;
            }
        }

        if(i == dwEntries)
        {
			WriteLog("Not find the entry name in remote access PB,CreateSystemConnEntry");

            CreateSystemConnEntry(profile);
			ModifySystemConnEntry(profile);
        }
    }

	/*
	CString strUserAndPass = _T("");
	strUserAndPass.Format("%s","Profile\ruser:\rPass:");
	strUserAndPass.Insert(13,profile.szUserName);
	strUserAndPass.Insert(UNLEN + 1,profile.szPassword);
	MessageBox(strUserAndPass);
	*/

    if(NULL != pRasEntryName)
    {
        delete []pRasEntryName;
        pRasEntryName = NULL;
    }

    if(ERROR_SUCCESS == dwErr)
        return TRUE;
    else
        return FALSE;
}

void CDlgConnect::OnButtonDisconnect()
{
    // TODO: Add your control notification handler code here
    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	CWnd* pButtonConnect = GetDlgItem(IDC_BUTTON_CONNECT);

    CString str;
	
	if(!m_BAutoDisconnect)
	{
		if(AfxMessageBox(IDS_INT_DISC_TIP, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
		{
			return;
		}
	}
    
    UpdateWindow();
	
  //  GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
    
    if(g_bDataService)
    {
        if(m_hRasConn == NULL)
            return;
        
#if WINVER>=0x500		
        DWORD dwConnDuration = 0;
        
		
		//网络类型
		dTempItem.nLogType = m_Oldnetwork;
		//开始的时间
		dTempItem.cTimeStamp = m_BeforeChageTime;
		//持续的时长
		dwConnDuration = rsOldStatistics.dwConnectDuration - rsBeforeChangestatistics.dwConnectDuration;
		CTimeSpan tsdurationTime(dwConnDuration/1000/3600/24, dwConnDuration/1000/3600%24,
			dwConnDuration/1000/60%60, dwConnDuration/1000%60);
		dTempItem.cDuration = tsdurationTime;
		//接收的流量
// 		dTempItem.dwByteIn = rsOldStatistics.dwBytesRcved - rsBeforeChangestatistics.dwBytesRcved;
// 		//发送的流量
 //		dTempItem.dwByteOut = rsOldStatistics.dwBytesXmited - rsBeforeChangestatistics.dwBytesXmited;

// 		//接收的流量
// 		dTempItem.dwByteIn = (DWORD)(m_pDlgUsage->m_cIncoming.m_dCurrTraffic*1024.0);
// 		//发送的流量
//			dTempItem.dwByteOut = (DWORD)(m_pDlgUsage->m_cOutgoing.m_dCurrTraffic*1024.0);

//接收的流量
		dTempItem.dwByteIn = (DWORD)(m_pDlgUsage->m_cIncoming.currentTotalTraffic);
//发送的流量
		dTempItem.dwByteOut = (DWORD)(m_pDlgUsage->m_cOutgoing.currentTotalTraffic);


        if(dTempItem.dwByteIn >= 0.0 && dTempItem.dwByteOut >= 0.0)
        {
            m_pLogData->SaveItem(&dTempItem);
        }
#endif        
        // Hang up and set the handle to NULL.
        RasHangUp(m_hRasConn);
        m_hRasConn = NULL;
        //::Sleep(2000);
        KillTimer(IDT_TIMER_DATASERVICEDURATION);
        KillTimer(IDT_TIMER_DATASERVICECONNECT);
        g_bDataService = FALSE;
		
		GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RAS)->EnableWindow(TRUE);
		
        m_strConnStatus.Format(_T(""));
        m_strDuration.Format(_T(""));
       
		m_pDlgUsage->m_stThroughput=GetThroughtput();
        UpdateData(FALSE);
    }
    else
    {
        RasHangUp(m_hRasConn);
        m_hRasConn = NULL;
        //::Sleep(2000);
        m_strConnStatus.Format(_T(""));      
        UpdateData(FALSE);
    }   	
	
	m_bPassPwdConfirm = false;
    m_strActiveConnection=_T("");
    m_strActiveProfile=_T("");
   // GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(TRUE);
    PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)1, (LPARAM)TRUE);
    pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPFLUX, 0);
    pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWFLUX, 0);
    pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPSPEED, 0);
    pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWSPEED, 0);
    str.LoadString(IDS_INT_BUTTON_CONN);
    pButtonConnect->SetWindowText(str);
    pButtonConnect->InvalidateRect(NULL);
    pButtonConnect->UpdateWindow();
	CShadeButtonST *pInternetBtn = &(pMainWnd->m_pTabDialog->m_btn4);
	pInternetBtn->SetWindowText(str);
	pInternetBtn->InvalidateRect(NULL);
		pInternetBtn->UpdateWindow();
    pMainWnd->m_pTabDialog->InvalidateRect(NULL);
    pMainWnd->m_pTabDialog->UpdateWindow();
    pMainWnd->UpdateWindow();
}


void CDlgConnect::PromptCheck(DWORD dwCurByte, DWORD dwCurSec)
{
	WriteLog("PromptCheck enter");
	if(m_bPassPwdConfirm)
	{
		return;
	}
	
    if(((g_SetData.Internet_nPromptEnable) ||
		(g_SetData.Internet_nPromptDisconnEnable)) && g_bConnLimitTip)
    {
        TCHAR szTip[100] = {0};
        CString str;
        COleDateTime datetime;
        datetime.SetDateTime(g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
			, g_SetData.Internet_nPromptDay, 0,0,0);

		COleDateTime CurrentTime;
		CurrentTime = COleDateTime::GetCurrentTime();
		if(datetime>CurrentTime)
			return;
		
        //Flux
        if(g_SetData.Internet_nPromptFluxOrDuration == 0)
        {
            DWORD dwByte = 0;
            dwByte = m_pLogData->CalculateTotalFlux(datetime) + dwCurByte;
            
            if(dwByte >= (g_SetData.Internet_nPromptFluxVal * 1024 * 1024))  //超过限制
            {
                g_bConnLimitTip = FALSE;
				m_BAutoDisconnect = TRUE;
				
				if(g_SetData.Internet_nPromptDisconnEnable)
				{
					WriteLog(_T("Internet_nPromptDisconnEnable is ture!"));
					g_SetData.Internet_nReachUsageLimit = TRUE;
					SaveIniFile();
					OnButtonDisconnect();
				}
				else
				{
					WriteLog(_T("Internet_nPromptDisconnEnable is false!"));
					str.LoadString(IDS_INT_LIMIT_FLUX);
					swprintf(szTip, str, g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
						, g_SetData.Internet_nPromptDay, g_SetData.Internet_nPromptFluxVal);
					str.LoadString(IDS_INT_DISC_TIP);
					wcscat(szTip, str);
					if(IDYES == AfxMessageBox(szTip, MB_YESNO | MB_ICONQUESTION))
						OnButtonDisconnect();
				}
            }
        }
        else //Duration
        {
            DWORD dwSec = 0;
            dwSec = m_pLogData->CalculateTotalDuration(datetime) + dwCurSec;
			
            if(dwSec >= g_SetData.Internet_nPromptDurationVal * 60)  //超过限制,以秒为单位
            {
                DWORD hour, min;
                hour = g_SetData.Internet_nPromptDurationVal / 60;
                min = g_SetData.Internet_nPromptDurationVal % 60;
                g_bConnLimitTip = FALSE;
				m_BAutoDisconnect = TRUE;
				
				if(g_SetData.Internet_nPromptDisconnEnable)
				{
					g_SetData.Internet_nReachUsageLimit = TRUE;
					SaveIniFile();
					OnButtonDisconnect();
				}
				else
				{
					WriteLog(_T("into duration,Internet_nPromptDisconnEnable is false!"));
					str.LoadString(IDS_INT_LIMIT_DURATION);
					swprintf(szTip, str, g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
						, g_SetData.Internet_nPromptDay, hour, min);
					str.LoadString(IDS_INT_DISC_TIP);
					wcscat(szTip, str);
					if(IDYES == AfxMessageBox(szTip, MB_YESNO | MB_ICONQUESTION))
						OnButtonDisconnect();
				}
            }
        }
    }
}



void CDlgConnect::PromptNDISCheck(DWORD dwCurSec)
{
	if(m_bPassPwdConfirm)
	{
		return;
	}
	
    if(((g_SetData.Internet_nPromptEnable) ||
		(g_SetData.Internet_nPromptDisconnEnable)) && g_bConnLimitTip)
    {
        TCHAR szTip[100] = {0};
        TCHAR *str=_T("");
        COleDateTime datetime;
        datetime.SetDateTime(g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
			, g_SetData.Internet_nPromptDay, 0,0,0);
		
        //Flux
		CHSDPADlg *pDlg = (CHSDPADlg *)AfxGetMainWnd();
        if(g_SetData.Internet_nPromptFluxOrDuration == 0)
        {
            DWORD dwByte = 0;
            dwByte = m_pLogData->CalculateTotalFlux(datetime) + (m_nLastTxOkCount-m_TempLastTxOkCount)+(m_nLastRxOkCount-m_TempLastRxOkCount);
			
            
            if(dwByte >= (g_SetData.Internet_nPromptFluxVal * 1024 * 1024))  //超过限制
            {
                g_bConnLimitTip = FALSE;
				m_BAutoDisconnect = TRUE;
				
				if(g_SetData.Internet_nPromptDisconnEnable)
				{
					g_SetData.Internet_nReachUsageLimit = TRUE;
					SaveIniFile();
                    pDlg->m_connectionPage.OnConnectEx(m_NdisState, NULL); // Ndis disconnect cmd
					
					
				}
				else
				{
					CString(str).LoadString(IDS_INT_LIMIT_FLUX);
					swprintf(szTip, str, g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
						, g_SetData.Internet_nPromptDay, g_SetData.Internet_nPromptFluxVal);
					CString(str).LoadString(IDS_INT_DISC_TIP);
					wcscat(szTip, str);
					if(IDYES == AfxMessageBox(szTip, MB_YESNO | MB_ICONQUESTION))
					{
						pDlg->m_connectionPage.OnConnectEx(m_NdisState, NULL); // Ndis disconnect cmd
						
						
					}
				}
            }
        }
        else //Duration
        {
            DWORD dwSec = 0;
            dwSec = m_pLogData->CalculateTotalDuration(datetime) + dwCurSec;
			
            if(dwSec >= g_SetData.Internet_nPromptDurationVal * 60)  //超过限制,以秒为单位
            {
                DWORD hour, min;
                hour = g_SetData.Internet_nPromptDurationVal / 60;
                min = g_SetData.Internet_nPromptDurationVal % 60;
                g_bConnLimitTip = FALSE;
				m_BAutoDisconnect = TRUE;
				
				if(g_SetData.Internet_nPromptDisconnEnable)
				{
					g_SetData.Internet_nReachUsageLimit = TRUE;
					SaveIniFile();
                    pDlg->m_connectionPage.OnConnectEx(m_NdisState, NULL); // Ndis disconnect cmd
				}
				else
				{
					//					WriteLog("into duration,Internet_nPromptDisconnEnable is false!");
					CString(str).LoadString(IDS_INT_LIMIT_DURATION);
					swprintf(szTip, str, g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
						, g_SetData.Internet_nPromptDay, hour, min);
					CString(str).LoadString(IDS_INT_DISC_TIP);
					wcscat(szTip, str);
					if(IDYES == AfxMessageBox(szTip, MB_YESNO | MB_ICONQUESTION))
					{
						pDlg->m_connectionPage.OnConnectEx(m_NdisState, NULL); // Ndis disconnect cmd
						
					}
				}
            }
        }
    }
}




void CDlgConnect::OnTimer(UINT nIDEvent) 
{
#define FLUXSPEED_UPDATE_SEC 2

    // TODO: Add your message handler code here and/or call default
    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    
#if WINVER>=0x500
    RASCONNSTATUS rsStatus;
    DWORD dwUpSpeed, dwDwSpeed;
    DWORD dwUpFlux, dwDwFlux;
    DWORD dwByte, dwSec;
    
    memset(&rsStatus, 0, sizeof(RASCONNSTATUS));
    rsStatus.dwSize = sizeof(RASCONNSTATUS);
    RasGetConnectStatus(m_hRasConn, &rsStatus);
	
	if (IDT_TIMER_CONNECTIMEOUT == nIDEvent)
	{
		KillTimer(IDT_TIMER_DATASERVICECONNECT);
		KillTimer(IDT_TIMER_CONNECTIMEOUT);
		RasHangUp(m_hRasConn);
		m_hRasConn=NULL;
		g_bDataService=FALSE;
		m_strActiveConnection="";
 		m_strActiveProfile="";
		CloseConnTip();
		if(!g_bDataService)
			AfxMessageBox(_T("Connection error,please reinsert the modem and check the phone number!"));
	}  
     else if(IDT_TIMER_DATASERVICEDURATION == nIDEvent)
    {
        if(rsStatus.rasconnstate == RASCS_Connected)
        {			
            memset(&rsNewStatistics, 0, sizeof(RAS_STATS));
            rsNewStatistics.dwSize = sizeof(RAS_STATS);
            RasGetConnectionStatistics(m_hRasConn, &rsNewStatistics);           

            int nDay,nHour,nMin,nSec;
            nDay = nHour = nMin = nSec = 0;
            nDay = rsOldStatistics.dwConnectDuration/1000/3600/24;
            nHour = rsOldStatistics.dwConnectDuration/1000/3600%24;
            nMin = rsOldStatistics.dwConnectDuration/1000/60%60;
            nSec = rsNewStatistics.dwConnectDuration/1000%60;

            if(nDay==0)
            {
                m_strDuration.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
            } 
            else
            {
                m_strDuration.Format(_T("%02d:%02d:%02d:%2d"), nDay, nHour, nMin, nSec);
            }
            
            m_strSent.Format(_T("%.2f"), rsNewStatistics.dwBytesXmited/1024.0);
            m_strReceived.Format(_T("%.2f"), rsNewStatistics.dwBytesRcved/1024.0);
			
            
            UpdateData(FALSE);

			CString str1 = _T("");
			str1.Format(_T("Ontimer %d"),rsNewStatistics.dwBytesRcved);
			WriteLog(str1);
             
			//网络类型发生改变
			bool bNetSrvChg = false;
			switch (m_Oldnetwork)
			{
				case NW_CALLLOG_GSM:
				{
					if(NW_CALLLOG_WCDMA == pMainWnd->m_NetCallLog)
					{
						bNetSrvChg = true;
					}

					break;
				}
				//case NW_SRV_UMTS:
				case NW_CALLLOG_WCDMA:
				{
					if(NW_CALLLOG_GSM == pMainWnd->m_NetCallLog)
				
					{
						bNetSrvChg = true;
					}
					break;
				}
				default:
					break;
			}
			if(bNetSrvChg)
			{
				COleDateTime CurrentTime;
				CurrentTime = COleDateTime::GetCurrentTime();

				#if WINVER>=0x500
					DWORD dwConnDuration = 0;

					//网络类型
					dTempItem.nLogType = m_Oldnetwork;
					//开始的时间
					dTempItem.cTimeStamp = m_BeforeChageTime;
					//持续的时长
					dwConnDuration = rsNewStatistics.dwConnectDuration - rsBeforeChangestatistics.dwConnectDuration;
					CTimeSpan tsdurationTime(dwConnDuration/1000/3600/24, dwConnDuration/1000/3600%24,
												dwConnDuration/1000/60%60, dwConnDuration/1000%60);
					dTempItem.cDuration = tsdurationTime;

					//接收的流量
					CString str = _T("Net change");
					WriteLog(str);
					str.Format(_T("NEWRcved %d"),rsNewStatistics.dwBytesRcved);
					WriteLog(str);
					str.Format(_T("BeforeChangeRceved %d"),rsBeforeChangestatistics.dwBytesRcved);
					WriteLog(str);

					dTempItem.dwByteIn = rsNewStatistics.dwBytesRcved - rsBeforeChangestatistics.dwBytesRcved;

					//发送的流量
					dTempItem.dwByteOut = rsNewStatistics.dwBytesXmited - rsBeforeChangestatistics.dwBytesXmited;

					if(dTempItem.dwByteIn >= 0 && dTempItem.dwByteOut >= 0)
					{
						m_pLogData->SaveItem(&dTempItem);						
					}	
					m_Oldnetwork = pMainWnd->m_NetCallLog;
					m_BeforeChageTime = CurrentTime;
					rsBeforeChangestatistics = rsNewStatistics;					
				#endif 
				
			}
		     if(rsNewStatistics.dwConnectDuration)
            {
                if(rsNewStatistics.dwBytesXmited > rsOldStatistics.dwBytesXmited)
                    dwUpSpeed = (rsNewStatistics.dwBytesXmited - rsOldStatistics.dwBytesXmited) * 8 / 1024;
                else
                    dwUpSpeed = 0;
                if(rsNewStatistics.dwBytesRcved > rsOldStatistics.dwBytesRcved)
                    dwDwSpeed = (rsNewStatistics.dwBytesRcved - rsOldStatistics.dwBytesRcved) * 8 / 1024;
                else
                    dwDwSpeed = 0;

                dwUpFlux =  rsNewStatistics.dwBytesXmited;
                dwDwFlux =  rsNewStatistics.dwBytesRcved;

                if(++m_nUpdateSec >= FLUXSPEED_UPDATE_SEC)
                {
                    m_nUpdateSec = 0;
                    
                    pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPFLUX, (LPARAM)dwUpFlux);
                    
                    pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWFLUX, (LPARAM)dwDwFlux);

//                     if(dwUpSpeed > 0)
//                         pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPSPEED, (LPARAM)dwUpSpeed);
//                     
//                     if(dwDwSpeed > 0)
//                         pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWSPEED, (LPARAM)dwDwSpeed);
                }
                
                rsOldStatistics.dwConnectDuration = rsNewStatistics.dwConnectDuration;
                rsOldStatistics.dwBps = rsNewStatistics.dwBps;
                rsOldStatistics.dwBytesRcved = rsNewStatistics.dwBytesRcved;
                rsOldStatistics.dwBytesXmited = rsNewStatistics.dwBytesXmited;

                dwByte = (rsOldStatistics.dwBytesRcved + rsOldStatistics.dwBytesXmited);
                            /// (1024 * 1024);
                dwSec = rsOldStatistics.dwConnectDuration / (1000);

                PromptCheck(dwByte, dwSec); //added by wk on 2007-2-12
            }
            else
            {
                pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPFLUX, 0);
                pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWFLUX, 0);
                pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_UPSPEED, 0);
                pMainWnd->PostMessage(WM_ICON_UPDATE, ICON_TYPE_DWSPEED, 0);
            }
            //modified by wk end on 2007-1-3 
        }
        else if(rsStatus.rasconnstate == RASCS_OpenPort)//连接非正常断开
        {
            if(g_bDataService)
            {
                if(m_hRasConn == NULL)
				{
                    return;
				}

				DWORD dwConnDuration;

				//网络类型
				dTempItem.nLogType = m_Oldnetwork;
				//开始的时间
				dTempItem.cTimeStamp = m_BeforeChageTime;
				//持续的时长
				//dTempItem.cDuration = tsCurtime - rsBeforeChangestatistics.dwConnectDuration;
				dwConnDuration = rsOldStatistics.dwConnectDuration - rsBeforeChangestatistics.dwConnectDuration;
				CTimeSpan tsdurationTime(dwConnDuration/1000/3600/24, dwConnDuration/1000/3600%24,
											dwConnDuration/1000/60%60, dwConnDuration/1000%60);
				dTempItem.cDuration = tsdurationTime;

				//接收的流量
				dTempItem.dwByteIn = rsOldStatistics.dwBytesRcved - rsBeforeChangestatistics.dwBytesRcved;
				//发送的流量
				dTempItem.dwByteOut = rsOldStatistics.dwBytesXmited - rsBeforeChangestatistics.dwBytesXmited;
        
				if(dTempItem.dwByteIn >= 0 && dTempItem.dwByteOut >= 0)
				{
					m_pLogData->SaveItem(&dTempItem);
				}                
                // Hang up and set the handle to NULL.
                //RasHangUp( m_hRasConn );
                m_hRasConn = NULL;
                g_bDataService = FALSE;

				// @@@@@@
				GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_RAS)->EnableWindow(TRUE);

				memset(&rsBeforeChangestatistics, 0x00, sizeof(RAS_STATS));
				m_BeforeChageTime.SetStatus(COleDateTime::null);

                //::Sleep(2000); 
                KillTimer(IDT_TIMER_DATASERVICEDURATION);
                KillTimer(IDT_TIMER_DATASERVICECONNECT);
             //   GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(TRUE);
              //  GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
            //    PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)1, (LPARAM)TRUE);                

				CString strConnect;
				CShadeButtonST *pInternetBtn = &(pMainWnd->m_pTabDialog->m_btn4);
				strConnect.LoadString(IDS_INT_BUTTON_CONN);
				pInternetBtn->ShowWindow(SW_HIDE);
				pInternetBtn->SetWindowText(strConnect);
				pInternetBtn->ShowWindow(SW_SHOW);
			//	pInternetBtn->InvalidateRect(NULL);
			//	pInternetBtn->UpdateWindow();
// 				pMainWnd->m_pTabDialog->InvalidateRect(NULL);
// 				pMainWnd->m_pTabDialog->UpdateWindow();
// 				pMainWnd->UpdateWindow();
				CWnd* pButtonConnect = GetDlgItem(IDC_BUTTON_CONNECT);	
				strConnect.LoadString(IDS_INT_BUTTON_CONN);
				pButtonConnect->SetWindowText(strConnect);
				pButtonConnect->InvalidateRect(NULL);
				pButtonConnect->UpdateWindow();

				m_bPassPwdConfirm = false;

                m_strConnStatus.Format(_T(""));
                m_strDuration.Format(_T(""));
                
                UpdateData(FALSE);
            }
        }
    }
    else if(IDT_TIMER_DATASERVICECONNECT == nIDEvent)//连接失败
    {
        if(rsStatus.rasconnstate == RASCS_Connected)
        {           
            g_bDataService = TRUE;
        }
        else
        {
            m_strConnStatus.Format(_T("Connection error,please reinsert the modem and check the phone number!"));
            UpdateConnTip(m_strConnStatus);
//             GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(TRUE);
//             PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)1, (LPARAM)TRUE);
          //  GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
        }
        KillTimer(IDT_TIMER_DATASERVICECONNECT);
        UpdateData(FALSE);
        CloseConnTip();
    }
#endif
	//NDIS statistics
	else if (IDT_TIMER_DATANDISSERVICEDURATION == nIDEvent)  
	{
		m_uTime++;
		m_TempTime++;
        UINT uDays    = m_uTime / 3600 / 24;
        UINT uHours   = m_uTime / 3600 % 24;
        UINT uMinutes = m_uTime / 60 % 60;
        UINT nSeconds = m_uTime % 60;
        if (0 == uDays)
            m_strDuration.Format(_T("%02d:%02d:%02d"), uHours, uMinutes, nSeconds);
        else
            m_strDuration.Format(_T("%02d:%02d:%02d:%2d"), uDays, uHours, uMinutes, nSeconds);

		if (IsNetServerChange()) 
		{
			
			if(m_TempTime != 0)
			{
				StasticNdisLog();
			}

		}

		DWORD dwSec = (DWORD)m_TempTime;
		

		PromptNDISCheck(dwSec); 

	

		UpdateData(FALSE);
	}
	


    CTabedDialog::OnTimer(nIDEvent);
}

BOOL CDlgConnect::IsNetServerChange()
{
	BOOL bNetSrvChg = FALSE;

	CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();

	//网络类型发生改变


	switch (m_Oldnetwork)
	{
		case NW_CALLLOG_GSM:

			if(NW_CALLLOG_WCDMA == pMainWnd->m_NetCallLog)
			{

				bNetSrvChg = TRUE;
			}
			break;
		case NW_CALLLOG_WCDMA:
			if(NW_CALLLOG_GSM == pMainWnd->m_NetCallLog)
			{
				bNetSrvChg = TRUE;
			}
			break;
		default:
			ASSERT(TRUE);
			break;
	}

	return bNetSrvChg;
}

void CDlgConnect::StasticNdisLog() 
{
    CHSDPADlg* pMainDlg = (CHSDPADlg*)AfxGetMainWnd();

	//网络类型
	dTempItem.nLogType = m_Oldnetwork;

	//开始的时间
	dTempItem.cTimeStamp = m_BeforeChageTime;

	//持续的时长
	CTimeSpan tsdurationTime(m_TempTime / 3600 / 24, m_TempTime / 3600 % 24, m_TempTime / 60 % 60, m_TempTime % 60);
	dTempItem.cDuration = tsdurationTime;

/*	//发送的流量
	char szBytes[MAX_PATH] = { 0 };

	
    strcpy(szBytes,(char*)(LPCTSTR)(m_strReceived));
	
	DWORD TempdwByteIn = (DWORD)(atof(szBytes) * 1024);
	dTempItem.dwByteIn = TempdwByteIn - m_TempLastTxOkCount;

	

	//接收的流量
	memset((void *)szBytes, 0, sizeof(szBytes));
	strcpy(szBytes,(char*)(LPCTSTR)(m_strSent));*



	DWORD TempdwByteOut = (DWORD)(atof(szBytes) * 1024);
	dTempItem.dwByteOut = TempdwByteOut - m_TempLastRxOkCount;

  */
	dTempItem.dwByteIn=m_pDlgUsage->m_dCurrDownTraffic*1024;
	dTempItem.dwByteOut=m_pDlgUsage->m_dCurrUpTraffic*1024;
    if (dTempItem.dwByteIn >= 0 && dTempItem.dwByteOut >= 0)
    {
        m_pLogData->SaveItem(&dTempItem);
    }
    m_BeforeChageTime = COleDateTime::GetCurrentTime();
	m_TempTime = 0;
	m_Oldnetwork = pMainDlg->m_NetCallLog;
	m_TempLastTxOkCount = TempdwByteIn;
	m_TempLastRxOkCount = TempdwByteOut;	

}



LRESULT CDlgConnect::OnRasDialEvent(WPARAM wp, LPARAM lp) 
{
    CString strTemp;
    CString str;
    RASCONNSTATE rs = (RASCONNSTATE)wp;
    COleDateTime tm;
    CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
    CWnd* pButtonConnect = GetDlgItem(IDC_BUTTON_CONNECT);
	int ntry = 0;
    CWnd *pInternetBtn = pMainDlg->m_pTabDialog->GetDlgItem(IDC_BUTTON_INTERNET);

    if(rs==RASCS_OpenPort)
    {
		SetTimer(IDT_TIMER_CONNECTIMEOUT,30000, NULL); 
		
		DWORD dwConnDuration;
		
		//网络类型
		dTempItem.nLogType = m_Oldnetwork;
		//开始的时间
		dTempItem.cTimeStamp = m_BeforeChageTime;
		//持续的时长
		//dTempItem.cDuration = tsCurtime - rsBeforeChangestatistics.dwConnectDuration;
		dwConnDuration = rsOldStatistics.dwConnectDuration - rsBeforeChangestatistics.dwConnectDuration;
		CTimeSpan tsdurationTime(dwConnDuration/1000/3600/24, dwConnDuration/1000/3600%24,
			dwConnDuration/1000/60%60, dwConnDuration/1000%60);
	
		dTempItem.cDuration = tsdurationTime;
		
		//接收的流量
		dTempItem.dwByteIn = rsOldStatistics.dwBytesRcved - rsBeforeChangestatistics.dwBytesRcved;
		//发送的流量
		dTempItem.dwByteOut = rsOldStatistics.dwBytesXmited - rsBeforeChangestatistics.dwBytesXmited;
        
		if(dTempItem.dwByteIn >0.0||dTempItem.dwByteOut > 0.0)
			m_pLogData->SaveItem(&dTempItem);
		m_pDlgUsage->m_stThroughput=GetThroughtput();
	}
    else if(rs==RASCS_PortOpened)
	{
        //m_strConnStatus.Format(_T("Connecting..."));
        m_strConnStatus.LoadString(IDS_INT_CONNSTS_CONNECTING);
        UpdateConnTip(m_strConnStatus);
	}
    else if(rs==RASCS_DeviceConnected)
	{
        //m_strConnStatus.Format(_T("Device Connected"));
        m_strConnStatus.LoadString(IDS_INT_CONNSTS_DEVCONNECTED);
        UpdateConnTip(m_strConnStatus);

		// @@@@@@
		GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RAS)->EnableWindow(TRUE);
	}
    else if(rs==RASCS_Authenticate)
	{
        //m_strConnStatus.Format(_T("Authenticating"));
        m_strConnStatus.LoadString(IDS_INT_CONNSTS_AUTHENTICATING);
        UpdateConnTip(m_strConnStatus);
	}
        
    else if(rs==RASCS_Authenticated)
	{
        //m_strConnStatus.Format(_T("Authenticated"));
        m_strConnStatus.LoadString(IDS_INT_CONNSTS_AUTHENTICATED);
        UpdateConnTip(m_strConnStatus);
	}
	else if(rs==RASCS_AuthNotify)
	{
		RASCONNSTATUS rsStatus;
		memset(&rsStatus, 0x00, sizeof(RASCONNSTATUS));
		rsStatus.dwSize = sizeof(RASCONNSTATUS);
		rsStatus.dwError=0;
		RasGetConnectStatus(m_hRasConn, &rsStatus);
		if(rsStatus.dwError!=0)
		{
			RasHangUp(m_hRasConn);

			CloseConnTip();
			m_hRasConn = NULL; 
			 			KillTimer(IDT_TIMER_DATASERVICEDURATION);
						  KillTimer(IDT_TIMER_DATASERVICECONNECT);
			g_bDataService = FALSE;
			m_strActiveConnection=_T("");
			m_strActiveProfile=_T("");
			KillTimer(IDT_TIMER_CONNECTIMEOUT);
			TRACE(_T("%d"),rsStatus.dwError);
			CString strDwerror;
			strDwerror.Format(_T("%d"),rsStatus.dwError);
		//	AfxMessageBox(strDwerror);
				AfxMessageBox(_T("Invalid user name or password!"));
		}
	}
        
    else if(rs==RASCS_Connected)
	{

        //m_strConnStatus.Format(_T("Connected"));
        m_strConnStatus.LoadString(IDS_INT_CONNSTS_CONNECTED);
        UpdateConnTip(m_strConnStatus);
		KillTimer(IDT_TIMER_CONNECTIMEOUT);
        //Modified by lgz 2006-06-23
        tm = COleDateTime::GetCurrentTime();
        dTempItem.cTimeStamp =tm;//保存连接建立时的时间
		m_BeforeChageTime = tm;
        
        g_bDataService = TRUE;


		GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RAS)->EnableWindow(FALSE);

        //GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(FALSE);
        
        //During a data connection forbid the user to use "Network" menu 
        //in order to change the network. 
        //User must not have to have the possibility to change network 
        //during a data connection active.
        //PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)FALSE, NULL);
        
        //GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
        ::SetTimer(this->m_hWnd, IDT_TIMER_DATASERVICEDURATION,1000 , NULL); 
 
        CloseConnTip();
        str.LoadString(IDS_INT_BUTTON_DISC);
        pButtonConnect->SetWindowText(str);
        pButtonConnect->InvalidateRect(NULL);
        pButtonConnect->UpdateWindow();
//         pMainDlg->m_pTabDialog->InvalidateRect(NULL);
//         pMainDlg->m_pTabDialog->UpdateWindow();
//         pMainDlg->UpdateWindow();
		pInternetBtn->ShowWindow(SW_HIDE);
		pInternetBtn->SetWindowText(str);
		pInternetBtn->ShowWindow(SW_SHOW);

		
		for(ntry = 0; ntry < 10; ntry ++)
		{
			if( NW_CALLLOG_NOSRV != ((CHSDPADlg*)AfxGetMainWnd())->m_NetCallLog)
			{
				m_Oldnetwork = ((CHSDPADlg*)AfxGetMainWnd())->m_NetCallLog;
				break;
			}
			else
			{
				Sleep(150);
			}
		}

		memset(&rsBeforeChangestatistics, 0x00, sizeof(RAS_STATS));
		pMainDlg->m_pSetupDlg->GetDlgItem(IDC_BUTTON_BTNNETWORK)->EnableWindow(FALSE);
		
	}
    else if(rs==RASCS_Disconnected)
	{
        str.LoadString(IDS_INT_BUTTON_CONN);
        pButtonConnect->SetWindowText(str);
        pButtonConnect->InvalidateRect(NULL);
        pButtonConnect->UpdateWindow();

		pInternetBtn->SetWindowText(str);
		pInternetBtn->InvalidateRect(NULL);
		pInternetBtn->UpdateWindow();
        pMainDlg->m_pTabDialog->InvalidateRect(NULL);
        pMainDlg->m_pTabDialog->UpdateWindow();
        pMainDlg->UpdateWindow();
	
	

        //m_strConnStatus.Format(_T("Disconnected"));
        m_strConnStatus.LoadString(IDS_INT_CONNSTS_DISCONNECTED);
        UpdateConnTip(m_strConnStatus);
        KillTimer(IDT_TIMER_DATASERVICEDURATION);
        KillTimer(IDT_TIMER_DATASERVICECONNECT);
        g_bDataService = FALSE;


		GetDlgItem(IDC_RADIO_NDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RAS)->EnableWindow(TRUE);

        //连接未能建立，活动连接为空
        m_strActiveConnection=_T("");
        m_strActiveProfile=_T("");

        //GetDlgItem(IDC_BUTTON_DIAL)->EnableWindow(TRUE);    
        //During a data connection forbid the user to use "Network" menu 
        //in order to change the network. 
        //User must not have to have the possibility to change network 
        //during a data connection active.
        //PostMessage(WM_SETUP_UPDATE_INTERNETDLG, (WPARAM)1, (LPARAM)TRUE);    
        //GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
        m_hRasConn=NULL;
        
        CloseConnTip();
    }
    UpdateData(FALSE);
    return (LRESULT)0;
}

void CDlgConnect::onTxOkCount(int nTxOkCount)
{
	CScreenStatic *pStatic = NULL;
    CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	
	nTxOkCount *= 64;
	uint32 uByteCounts = nTxOkCount - m_nLastTxOkCount;
	
	CTime currentTime = CTime::GetCurrentTime();
	CTimeSpan spanDuration = currentTime - m_lastNdisTxReportDataTime;
	LONG  lTotalSeconds = spanDuration.GetTotalSeconds();
	
	// TX Counts
	if (0 != lTotalSeconds) {
		
		CString strInfo;
		strInfo.Format(_T("UP: %.3f Mbytes"), nTxOkCount / 1048576.0); // 1024 * 1024

		pStatic = (CScreenStatic *)(pMainDlg->GetDlgItem(IDC_UI_ICON_UPFLUX));
		pStatic->SetText(strInfo);
		pStatic->Invalidate();
		
		LONG lTxRate = (uByteCounts * 1024) / lTotalSeconds;						// bytes / s
		strInfo.Format(_T("%.3f kbytes"), lTxRate / 1048576.0);							// k bytes / s
		m_dNDISRate_Send=(double)lTxRate/1024.0;
		

		pStatic = (CScreenStatic *)(pMainDlg->GetDlgItem(IDC_UI_ICON_UPSPEED));
		pStatic->SetText(strInfo);
		pStatic->Invalidate();
        m_strSent.Format(_T("%.3f kbytes"), nTxOkCount / 1024.0);
		m_dNDISData_Send=(double)nTxOkCount / 1024.0;
		//m_strSent.Format("%.3f", uByteCounts / 1024.0);
		UpdateData(FALSE);
	}
	
	m_nLastTxOkCount = nTxOkCount;
	m_lastNdisTxReportDataTime = currentTime;
}

void CDlgConnect::onRxOkCount(int nRxOkCount)
{
	CScreenStatic *pStatic = NULL;
    CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
    nRxOkCount *= 64;
	uint32 uByteCounts = nRxOkCount - m_nLastRxOkCount;
	
	CTime currentTime = CTime::GetCurrentTime();
	CTimeSpan spanDuration = currentTime - m_lastNdisRxReportDataTime;
	LONG  lTotalSeconds = spanDuration.GetTotalSeconds();
	
	// RX 
	if (0 != lTotalSeconds) {
		
		CString strInfo;
		strInfo.Format(_T("DW: %.3f Mbytes"), nRxOkCount / 1048576.0); // 1024 * 1024
		pStatic = (CScreenStatic *)(pMainDlg->GetDlgItem(IDC_UI_ICON_DWFLUX));
		pStatic->SetText(strInfo);
		pStatic->Invalidate();
		
		LONG lRxRate = (uByteCounts * 1024) / lTotalSeconds;	// bytes / s
		strInfo.Format(_T("%.3f kbps"), lRxRate / 1048576.0);		// kbps / s
		m_dNDISRate_Rec=(double)lRxRate/1024.0;
		
		pStatic = (CScreenStatic *)(pMainDlg->GetDlgItem(IDC_UI_ICON_DWSPEED));
		pStatic->SetText(strInfo);
		pStatic->Invalidate();
		
		m_strReceived.Format(_T("%.3f"), nRxOkCount / 1024.0);
		m_dNDISData_Rec=(double)nRxOkCount / 1024.0;
		UpdateData(FALSE);
	}
	
	m_nLastRxOkCount = nRxOkCount;
	m_lastNdisRxReportDataTime = currentTime;
}


CString CDlgConnect::GetDurationTime()
{
	return m_strDuration;
}



stThroughput CDlgConnect::GetThroughtput()
{
	COleDateTime CurrentTime;
	CurrentTime = COleDateTime::GetCurrentTime();

	stLogItem dTempItem;
	stThroughput dRtItem;
  
	DWORD m_dailyIn = 0;
    DWORD m_dailyOut = 0;
	DWORD m_monthlyIn = 0;
    DWORD m_monthlyOut = 0;
	DWORD m_yearlyIn = 0;
    DWORD m_yearlyOut = 0;

    CTimeSpan totalDura(0);
	

	for(int i=0; i < m_pLogData->GetCount(); i++)
		{
			dTempItem = m_pLogData->GetItem(i);
			CString strTemp;
			if((dTempItem.cTimeStamp.GetYear()==CurrentTime.GetYear())&&
				(dTempItem.cTimeStamp.GetMonth()==CurrentTime.GetMonth())&&
				(dTempItem.cTimeStamp.GetDay()==CurrentTime.GetDay()))	
			{
				m_dailyIn += dTempItem.dwByteIn;
				m_dailyOut += dTempItem.dwByteOut;
			}
			if((dTempItem.cTimeStamp.GetYear()==CurrentTime.GetYear())&&
				(dTempItem.cTimeStamp.GetMonth()==CurrentTime.GetMonth()))
			{
				m_monthlyIn += dTempItem.dwByteIn;
				m_monthlyOut += dTempItem.dwByteOut;
			}
			if(dTempItem.cTimeStamp.GetYear()==CurrentTime.GetYear())
			{
				m_yearlyIn += dTempItem.dwByteIn;
				m_yearlyOut += dTempItem.dwByteOut;
			}

		}
		dRtItem.dailyIn=m_dailyIn;
		dRtItem.dailyOut=m_dailyOut;
		dRtItem.monthlyIn=m_monthlyIn;
		dRtItem.monthlyOut=m_monthlyOut;
		dRtItem.yearlyIn=m_yearlyIn;
		dRtItem.yearlyOut=m_yearlyOut;
		return dRtItem; 
}

void CDlgConnect::getConnProfile()
{
	m_pConnProf = ((CHSDPAApp *)AfxGetApp())->GetConnProfile();
}

void CDlgConnect::resetConnProfile()
{
	OnRadioRas();
	if (0 == m_connect_type) { 
		OnRadioNdis();
	}
	if (1 == m_connect_type) {
		OnRadioRas();
	}
	

}

BOOL CDlgConnect::ModemLockPrompt()
{
	// 	CString strModemLocked;
	// 	strModemLocked.LoadString(IDS_STR_MODEMLOCKED);
	// 	AfxMessageBox(strModemLocked);
// 	CPwdConfirmDlg PwdConfirmDlg(this);
// 	PwdConfirmDlg.m_bModemLock=TRUE;
// 	int nRes = PwdConfirmDlg.DoModal();
// 	if(nRes==IDOK)
// 	{
// 		//	g_SetData.Setup_nModemLock=0;
// 		g_nModemLock=0;
// 		return TRUE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 		
// 	}
	return true;
}


void CDlgConnect::killTimers()
{
	KillTimer(IDT_TIMER_DATASERVICEDURATION);
	KillTimer(IDT_TIMER_DATASERVICECONNECT);
	KillTimer(IDT_TIMER_CONNECTIMEOUT);
	
}