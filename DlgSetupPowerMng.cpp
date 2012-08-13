// DlgSetupPowerMng.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "DlgSetupPowerMng.h"
#include "SetupData.h"
#include "Dev.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL SaveIniFile();
/////////////////////////////////////////////////////////////////////////////
// CDlgSetupPowerMng dialog


CDlgSetupPowerMng::CDlgSetupPowerMng(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSetupPowerMng::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetupPowerMng)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_nBkTag = 1;
	m_bAlertWindowSmsSet = FALSE;//add by liub 1104 for SMSSettings
	m_bAlertToneSMSSet = FALSE;
	SetOK = FALSE;
}


void CDlgSetupPowerMng::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetupPowerMng)
	DDX_Control(pDX, IDC_EDIT_SETUP_SCA, m_NumEditSCA);
	DDX_Text(pDX, IDC_EDIT_SETUP_SCA, m_strSCA);
	DDX_Text(pDX, IDC_EDI_VOICEMAILNUMBER, m_strVoicemailNumber);
	DDV_MaxChars(pDX, m_strSCA, 32);
	DDX_Control(pDX, IDC_COMBO_PREFSTOR, m_cmbPrefStor);
	DDX_Control(pDX, IDC_COMBO_VALPERIOD, m_cmbValPeriod);
	DDX_Control(pDX, IDC_COMBO_DELIREPORT, m_cmbDeliReport);
	DDX_Control(pDX, IDC_COMBO_DEFDELIVERY, m_cmbDefDelivery);//add by liub for CDMA2000 SMSSettings
	DDX_Control(pDX, IDC_COMBO_SMS_PRIORITY, m_Priority);//add by liub for CDMA2000 SMSSettings
	DDX_Check(pDX, IDC_CHECK_PREFER_ALERTWINDOWS_SMSSET, m_bAlertWindowSmsSet);//add by liub 1104 for SMSSettings
	DDX_Check(pDX, IDC_CHECK_PREFER_ALERTTONE_SMSSET, m_bAlertToneSMSSet);//add by liub 1104 for SMSSettings
	DDX_Text(pDX, IDC_EDIT_PREFER_SMSSOUND_FILE_SMSSET, m_strSmsSoundFileSMSTone);//add by liub 1104 for SMSSettings

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupPowerMng, CBaseDialog)
	//{{AFX_MSG_MAP(CDlgSetupPowerMng)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_MSGSET, OnButtonSetupMsgSet)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PREFER_SMSSOUND_BROWSE_SMSSET, OnButtonSmsSoundBrowseSMSSet)//add by liub 1104 for SMSSettings
	ON_BN_CLICKED(IDC_BUTTON_PREFER_SMSSOUND_PLAY_SMSSET, OnButtonSmsSoundPlaySMSSet)//add by liub 1104 for SMSSettings
	//}}AFX_MSG_MAP
    //Modified by lgz 2006-06-20
    ON_MESSAGE(WM_SETUP_UPDATE_POWERDLG, OnUpdatePowerDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupPowerMng message handlers
//Modified by lgz 2006-06-20
BOOL CDlgSetupPowerMng::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_NumEditSCA.m_bFlag = TRUE;	
    ReadDataFromFile();//SMSSettings
	RECT rect;
	RECT rectWin;

	CString wstr1hour;
	CString wstr6hour;
	CString wstr1day;
	CString wstr3day;
	CString wstr1week;
	CString wstrmax;

	CString cstr30min;
	CString cstr1hour;
	CString cstr2hour;
	CString cstr6hour;
	CString cstr12hour;
	CString cstr1day;
	CString cstr1week;
	CString cstrmax;
	CString cstrdefault;

	CString cssmssetup_pri1;
	CString cssmssetup_pri2;
	CString cssmssetup_pri3;
	CString cssmssetup_pri4;
    
	cssmssetup_pri1.LoadString(IDS_SETUP_SMS_PRI1);
	cssmssetup_pri2.LoadString(IDS_SETUP_SMS_PRI2);
	cssmssetup_pri3.LoadString(IDS_SETUP_SMS_PRI3);
	cssmssetup_pri4.LoadString(IDS_SETUP_SMS_PRI4);



	wstr1hour.LoadString(IDS_SETSMSW_1HOUR);
	wstr6hour.LoadString(IDS_SETSMSW_6HOUR);
	wstr1day.LoadString(IDS_SETSMSW_1DAY);
	wstr3day.LoadString(IDS_SETSMSW_3DAY);
	wstr1week.LoadString(IDS_SETSMSW_1WEEK);
	wstrmax.LoadString(IDS_SETSMSW_MAX);

	cstr30min.LoadString(IDS_SETSMSC_30MIN);
	cstr1hour.LoadString(IDS_SETSMSC_1HOUR);
	cstr2hour.LoadString(IDS_SETSMSC_2HOUR);
	cstr6hour.LoadString(IDS_SETSMSC_6HOUR);
	cstr12hour.LoadString(IDS_SETSMSC_12HOUR);
	cstr1day.LoadString(IDS_SETSMSC_1DAY);
	cstr1week.LoadString(IDS_SETSMSC_1WEEK);
	cstrmax.LoadString(IDS_SETSMSC_MAX);
	cstrdefault.LoadString(IDS_SETSMSC_DEFAULT);

	GetClientRect(&rect);
	GetWindowRect(&rectWin);
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
		CString str[] = {wstr1hour,
			             wstr6hour,
						 wstr1day,
						 wstr3day,
						 wstr1week,
						 wstrmax
		};
		GetDlgItem(IDC_STATIC_SMS_CENTER)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_SETUP_SCA)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBO_SMS_PRIORITY)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PRIORITY)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DEFDELIVERY)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_DEFDELIVERY)->ShowWindow(FALSE);
		m_cmbValPeriod.ResetContent();
		for(int i=0;i<6;i++)
		{
			m_cmbValPeriod.AddString(str[i]);
		}
		m_cmbValPeriod.SetCurSel(i-1);
			
	}
	else if(!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
	{
		CString str[] = {cstr30min,
			             cstr1hour,
						 cstr2hour,
						 cstr6hour,
			             cstr12hour,
						 cstr1day,
						 cstr1week,
						 cstrmax,
						 cstrdefault
		};
		GetDlgItem(IDC_STATIC_SMS_CENTER)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_SETUP_SCA)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_SMS_PRIORITY)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PRIORITY)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_STATIC_DEFDELIVERY)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_COMBO_DEFDELIVERY)->ShowWindow(TRUE);
	//	m_Priority.SetCurSel(0);
		m_cmbValPeriod.ResetContent();
		for(int i=0;i<9;i++)
		{
			m_cmbValPeriod.AddString(str[i]);
		    m_cmbDefDelivery.AddString(str[i]);
	

		}
		m_cmbDefDelivery.SetCurSel(i-1);
        m_cmbValPeriod.SetCurSel(i-1);

		CString str1[] = {cssmssetup_pri1,
			             cssmssetup_pri2,
						 cssmssetup_pri3,
						 cssmssetup_pri4
		};
		for(i=0;i<4;i++)
		{
			m_Priority.AddString(str1[i]);
		   
		}

		
	}
	InitData();
//	MoveWindow(rectWin.left,rect.top,rect.right,rect.bottom);
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgSetupPowerMng::OnUpdatePowerDlg(WPARAM wParam, LPARAM lParam)
{
    BOOL res = (BOOL)wParam;

	GetDlgItem(IDC_BUTTON_SETUP_MSGSET)->EnableWindow(TRUE);

    if(res)
    {
	    g_SetData.Messages_nPreStor = (int)m_PreStor;
	    SaveIniFile();
		AfxMessageBox(IDS_SETOK, MB_OK | MB_ICONINFORMATION);
    }

    return 0;
}



BOOL CDlgSetupPowerMng::InitData()
{
    //m_cmbPrefStor.SetCurSel(g_SetData.Messages_nPreStor);
	m_cmbPrefStor.SetCurSel(2);//（新短信只存储在PC上）
	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
	{
		m_Priority.SetCurSel(g_SetData.Messages_nPriority);
		if (2 == g_SetData.Messages_nDeliReport)//(CDMA2000 设置项为0、2；WCDMA为0、1)
		{
			m_cmbDeliReport.SetCurSel(1);
		}
		else
			m_cmbDeliReport.SetCurSel(g_SetData.Messages_nDeliReport);

	}
	else
		m_cmbDeliReport.SetCurSel(g_SetData.Messages_nDeliReport);

    if(!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
	{
		switch (g_SetData.Messages_nValPeriod)
		{
		case 5:
			m_cmbValPeriod.SetCurSel(0);
			break;
		case 11:
			m_cmbValPeriod.SetCurSel(1);
			break;
		case 23:
			m_cmbValPeriod.SetCurSel(2);
			break;
		case 71:
			m_cmbValPeriod.SetCurSel(3);
			break;
		case 144:
			m_cmbValPeriod.SetCurSel(4);
			break;
		case 167:
			m_cmbValPeriod.SetCurSel(5);
			break;
		case 173:
			m_cmbValPeriod.SetCurSel(6);
			break;
		case 245:
			m_cmbValPeriod.SetCurSel(7);
			break;
		case 246:
			m_cmbValPeriod.SetCurSel(8);
			break;
		default:
			break;
		}
      ////////////
		switch (g_SetData.Messages_nDefDelivery)
		{
		case 5:
			m_cmbDefDelivery.SetCurSel(0);
			break;
		case 11:
			m_cmbDefDelivery.SetCurSel(1);
			break;
		case 23:
			m_cmbDefDelivery.SetCurSel(2);
			break;
		case 71:
			m_cmbDefDelivery.SetCurSel(3);
			break;
		case 144:
			m_cmbDefDelivery.SetCurSel(4);
			break;
		case 167:
			m_cmbDefDelivery.SetCurSel(5);
			break;
		case 173:
			m_cmbDefDelivery.SetCurSel(6);
			break;
		case 245:
			m_cmbDefDelivery.SetCurSel(7);
			break;
		case 246:
			m_cmbDefDelivery.SetCurSel(8);
			break;
		default:
			break;
		}
	}
	else
	{
		m_cmbValPeriod.SetCurSel(g_SetData.Messages_nValPeriod);
	    m_strSCA=gSmsCentreNum;
		strLastSCA=m_strSCA;
	}

    m_strSmsSoundFileSMSTone = g_SetData.Messages_szSoundFile;
    m_bAlertWindowSmsSet = g_SetData.Messages_nAlertWindow;
	m_bAlertToneSMSSet = g_SetData.Messages_nAlertTone;
	//SndAtCSCA("?");
	m_strVoicemailNumber = g_SetData.Messages_szVoiceMailNumber;
    UpdateData(FALSE);
    return true;
}

BOOL CDlgSetupPowerMng::SndAtCSCA(CString strSCA)
{
    const TCHAR szATSCA[]=_T("AT+CSCA");
    TCHAR szAtBuf[50] = {0};
    
	if (!wcscmp(strSCA, _T("?")))
	{
		swprintf(szAtBuf, _T("%s%s\r"), szATSCA, strSCA);
	} 
	else
	{
		swprintf(szAtBuf, _T("%s=\"%s\"\r"), szATSCA, strSCA);
	}
	char szAtAscBuf[512] = {0};
	WCharToChar(szAtBuf, szAtAscBuf);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf)))
    {
        SetTimer(IDT_CSCA_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCSCA, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgSetupPowerMng::RspAtCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupPowerMng* pDlg = (CDlgSetupPowerMng*)pWnd;
    pDlg->KillTimer(IDT_CSCA_TIMEOUT);

	CString RetStr;
	CString strSCA;
	RetStr = strArr[0];
	int nHead, nTail;
	int i;
	BOOL bResult=FALSE;

	nHead=RetStr.Find(_T("\""),0);

	if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
		AfxMessageBox(IDS_SETUP_SCA_OK, MB_OK | MB_ICONINFORMATION);
    }
    else if (nHead!=-1)
    {
		nTail=RetStr.Find(_T("\""),nHead+1);
		if (nTail!=-1)
		{
			RetStr=strArr[0]+nHead+1;
			strSCA=RetStr.Left(nTail-nHead-1);
			bResult=TRUE;
		}
		
		CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
		pDlg->m_strSCA="";
//		if (pMainDlg->m_bChineseChSet == TRUE)
		{
			//UCS2
			int nLen=strSCA.GetLength();
			for (i=3; i<nLen; i+=4)
			{
				pDlg->m_strSCA+=strSCA.GetAt(i);
				pDlg->strLastSCA=pDlg->m_strSCA;
			}
		}
        /*
		else 
		{
			//ira
			pDlg->m_strSCA=strSCA;
		}
        */
		
		if(bResult)
		{
			//AfxMessageBox(IDS_SETUP_CONNPOINTOK);
		}
		else
		{
			//设置失败
			AfxMessageBox(IDS_SETUP_SCA_ERROR);
			pDlg->m_strSCA="";
		} 
	} 
	else
	{

	}
    pDlg->PostMessage(WM_SETUP_UPDATE_POWERDLG, (WPARAM)TRUE, (LPARAM)TRUE);   
}

void CDlgSetupPowerMng::OnButtonSetupMsgSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//设置消息有效期和发送状态报告、优先级
	
    if(!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
	{
		g_SetData.Messages_nPriority = m_Priority.GetCurSel();//当前选中的行。
		if (1 == m_cmbDeliReport.GetCurSel())
		{
			g_SetData.Messages_nDeliReport = 2;

		}
		else
			g_SetData.Messages_nDeliReport = m_cmbDeliReport.GetCurSel();

	}else
		g_SetData.Messages_nDeliReport = m_cmbDeliReport.GetCurSel();

	g_SetData.Messages_nAlertWindow = m_bAlertWindowSmsSet;//for SMSSettings
	g_SetData.Messages_nAlertTone = m_bAlertToneSMSSet;//SMSSettings

	//wcscpy(g_SetData.Messages_szSoundFile, m_strSmsSoundFileSMSTone);

	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
		if(m_strSCA.IsEmpty())
		{
		   AfxMessageBox(IDS_SCA_TIP);
		   m_strSCA.Format(_T("%s"), gSmsCentreNum);
		   UpdateData(FALSE);
		   return;
		}
		g_SetData.Messages_nValPeriod = m_cmbValPeriod.GetCurSel();
		    //设置消息中心
		   SndAtSmsQCSCA();
	}
	else
	{
		int iPos = m_cmbValPeriod.GetCurSel();
		switch (iPos)
		{
		case 0:
            g_SetData.Messages_nValPeriod = 5;
			break;
		case 1:
            g_SetData.Messages_nValPeriod = 11;
			break;
		case 2:
			g_SetData.Messages_nValPeriod = 23;
			break;
		case 3:
			g_SetData.Messages_nValPeriod = 71;
			break;
		case 4:
			g_SetData.Messages_nValPeriod = 144;
			break;
		case 5:
			g_SetData.Messages_nValPeriod = 167;
			break;
		case 6:
			g_SetData.Messages_nValPeriod = 173;
			break;
	    case 7:
			g_SetData.Messages_nValPeriod = 245;
			break;
		case 8:
			g_SetData.Messages_nValPeriod = 246;
            break;
		default:
			break;
		}

		iPos = m_cmbDefDelivery.GetCurSel();
		switch (iPos)
		{
		case 0:
            g_SetData.Messages_nDefDelivery = 5;
			break;
		case 1:
            g_SetData.Messages_nDefDelivery = 11;
			break;
		case 2:
			g_SetData.Messages_nDefDelivery = 23;
			break;
		case 3:
			g_SetData.Messages_nDefDelivery = 71;
			break;
		case 4:
			g_SetData.Messages_nDefDelivery = 144;
			break;
		case 5:
			g_SetData.Messages_nDefDelivery = 167;
			break;
		case 6:
			g_SetData.Messages_nDefDelivery = 173;
			break;
		case 7:
			g_SetData.Messages_nDefDelivery = 245;
			break;
		case 8:
			g_SetData.Messages_nDefDelivery = 246;
            break;
		default:
			break;
		}
		 	SndAtSmsQHMSGP();
			SndAtSmsQCSMP();

	}
	wcscpy(g_SetData.Messages_szVoiceMailNumber, m_strVoicemailNumber); //add by liub
    SaveIniFile();
    
    //设置消息优先保存位置
	if (0)
    m_PreStor = (EnSmsPreStor)m_cmbPrefStor.GetCurSel();
	else
    m_PreStor = SMS_PRESTOR_PC;

    if(m_PreStor == SMS_PRESTOR_ME)
        m_PreLocType = LOC_ME;
    else if(m_PreStor == SMS_PRESTOR_UIM)
        m_PreLocType = LOC_UIM;
    else
        m_PreLocType = LOC_PC;

    ASSERT(m_PreLocType >= LOC_PC && m_PreLocType < LOC_MAX);
	if (g_bIsExist == FALSE)//无卡时
	{
		SetOK = TRUE;
	}
if (SetOK)
{
	AfxMessageBox(IDS_SMS_SETSUCC);
}

}

BOOL CDlgSetupPowerMng::SndAtSmsQHMSGP()
{
    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d,,,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,g_SetData.Messages_nPriority);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
	
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQHMSGP, (LPVOID)this);
        SetTimer(IDT_QHMSGP_TIMEOUT, 60000, NULL);
        return TRUE;
    }
    else
        return FALSE;
}

void CDlgSetupPowerMng::RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CDlgSetupPowerMng* pDlg = (CDlgSetupPowerMng*)pWnd;
	pDlg->KillTimer(IDT_QHMSGP_TIMEOUT);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
     	pDlg->SndAtSmsQCSMP();
}

//CDMA2000的+CSMP，与WCDMA完全不同
BOOL CDlgSetupPowerMng::SndAtSmsQCSMP()
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
        SetTimer(IDT_QHCSMP_TIMEOUT, 60000, NULL);
        return TRUE;
    }
    else
        return FALSE;
}

void CDlgSetupPowerMng::RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CDlgSetupPowerMng* pDlg = (CDlgSetupPowerMng*)pWnd;
	pDlg->KillTimer(IDT_QHCSMP_TIMEOUT);
	
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
		pDlg->SetOK = TRUE;
		//::MessageBox(NULL, _T("Set success"), _T(""), MB_OK);
		

}

BOOL CDlgSetupPowerMng::SndAtSmsQCSCA()
{
    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCSCA], m_strSCA);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCSCA, (LPVOID)this);
        SetTimer(IDT_QCSCA_TIMEOUT, 60000, NULL);
        return TRUE;
    }
    else
        return FALSE;
}

void CDlgSetupPowerMng::RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupPowerMng* pDlg = (CDlgSetupPowerMng*)pWnd;
    pDlg->KillTimer(IDT_QCSCA_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
	    if(wcscmp(pDlg->m_strSCA, gSmsCentreNum) != 0)
	    {
		    wcsncpy(gSmsCentreNum, pDlg->m_strSCA, SMS_SC_NUM_MAX);
		    //AfxMessageBox(IDS_SETUP_SCA_OK, MB_OK | MB_ICONINFORMATION);
	    }
    }

    if(pDlg->m_PreLocType == LOC_PC)
    {
        if(pDlg->SndAtCNMI(1,2))
 		    pDlg->GetDlgItem(IDC_BUTTON_SETUP_MSGSET)->EnableWindow(FALSE);       
    }
    else
    {
	    CString strCPMS;
	    strCPMS = gcstrLoc[pDlg->m_PreLocType];

	    if(pDlg->SndAtCPMS(strCPMS))
		    pDlg->GetDlgItem(IDC_BUTTON_SETUP_MSGSET)->EnableWindow(FALSE);
    }    
}

BOOL CDlgSetupPowerMng::SndAtCPMS(CString strCPMS)
{
    const TCHAR szATCPMS[]=_T("AT+CPMS");
    TCHAR szAtBuf[50] = {0};
    
	if (!wcscmp(strCPMS, _T("?")))
	{
		swprintf(szAtBuf, _T("%s%s\r"), szATCPMS, strCPMS);
	} 
	else
	{
		//="',"","ME"
		swprintf(szAtBuf, _T("%s=\"\",\"\",\"%s\"\r"), szATCPMS, strCPMS);
	}
    char szAtAscBuf[512] = {0};
	WCharToChar(szAtBuf, szAtAscBuf);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf)))
    {
        SetTimer(IDT_CPMS_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCPMS, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgSetupPowerMng::RspAtCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupPowerMng* pDlg = (CDlgSetupPowerMng*)pWnd;
    pDlg->KillTimer(IDT_CPMS_TIMEOUT);

	if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
        if(!pDlg->SndAtCNMI(2,1))
           pDlg->PostMessage(WM_SETUP_UPDATE_POWERDLG, (WPARAM)FALSE, 0);
    }
	else
	{
        pDlg->PostMessage(WM_SETUP_UPDATE_POWERDLG, (WPARAM)FALSE, 0);
	}
}

BOOL CDlgSetupPowerMng::SndAtCNMI(int para1, int para2)
{
    char szAtBuf[30] = {0};
	
    sprintf(szAtBuf, "%s%d,%d\r", gcstrAtSms[AT_SMS_QCNMI], para1, para2);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        SetTimer(IDT_CNMI_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtCNMI, this);
        return TRUE;
    }
    return FALSE;
}

void CDlgSetupPowerMng::RspAtCNMI(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupPowerMng *pDlg = (CDlgSetupPowerMng*)pWnd;
    
    pDlg->KillTimer(IDT_CNMI_TIMEOUT);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
        pDlg->PostMessage(WM_SETUP_UPDATE_POWERDLG, (WPARAM)TRUE, 0);
    else
        pDlg->PostMessage(WM_SETUP_UPDATE_POWERDLG, (WPARAM)FALSE, 0);
}

void CDlgSetupPowerMng::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    KillTimer(nIDEvent);
    
    switch(nIDEvent)
    {
    case IDT_CPMS_TIMEOUT:
    case IDT_CNMI_TIMEOUT:
        DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
        ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
        PostMessage(WM_SETUP_UPDATE_POWERDLG, (WPARAM)FALSE, 0);
        break;
    }

	CBaseDialog::OnTimer(nIDEvent);
}

void CDlgSetupPowerMng::OnOK()
{
}

void CDlgSetupPowerMng::OnCancel()
{
	CDialog::OnCancel();//add by liub

}

//add by liub 1104 for SMSSettings
void CDlgSetupPowerMng::ReadDataFromFile()
{
    if((g_SetData.Setup_nAutoLaunchApp == 0 || g_SetData.Setup_nAutoLaunchApp == 1)
        && (g_SetData.Setup_nAutoConnect == 0 || g_SetData.Setup_nAutoConnect == 1)
        && (g_SetData.Setup_nStartMini == 0 || g_SetData.Setup_nStartMini == 1)
        && (g_SetData.Setup_nAllowSleep == 0 || g_SetData.Setup_nAllowSleep == 1)
        && (g_SetData.Messages_nAlertWindow == 0 || g_SetData.Messages_nAlertWindow == 1)
        && (g_SetData.Messages_nAlertTone == 0 || g_SetData.Messages_nAlertTone == 1))
    {
        //m_bLaunchApp = g_SetData.Setup_nAutoLaunchApp;
        //m_bConncet = g_SetData.Setup_nAutoConnect;
       // m_bStartMini = g_SetData.Setup_nStartMini;
       // m_nSleepRadio = g_SetData.Setup_nAllowSleep;
        m_bAlertWindowSmsSet = g_SetData.Messages_nAlertWindow;
        m_bAlertToneSMSSet = g_SetData.Messages_nAlertTone;
        //m_bAlertTone = g_SetData.Messages_nAlertTone;
        if(wcslen(g_SetData.Messages_szSoundFile) > 0)
            m_strSmsSoundFileSMSTone.Format(_T("%s"), g_SetData.Messages_szSoundFile);
        else
            m_strSmsSoundFileSMSTone = GetDefaultSMSSoundFilePath();
    }
    else
    {
        DefaultSet();
        WriteDataToFile();
    }
    UpdateData(FALSE);
}

void CDlgSetupPowerMng::WriteDataToFile()
{
    //g_SetData.Setup_nAutoLaunchApp = m_bLaunchApp;
    //g_SetData.Setup_nAutoConnect = m_bConncet;
    //g_SetData.Setup_nStartMini = m_bStartMini;
    //g_SetData.Setup_nAllowSleep = m_nSleepRadio;
    g_SetData.Messages_nAlertWindow = m_bAlertWindowSmsSet;
    g_SetData.Messages_nAlertWindow = m_bAlertToneSMSSet;
    //g_SetData.Messages_nAlertTone = m_bAlertTone;
    //memset(g_SetData.Messages_szSoundFile, 0x00, MAX_PATH);
    //wcsncpy(g_SetData.Messages_szSoundFile, m_strSmsSoundFile, MAX_PATH);
    SaveIniFile();
}

void CDlgSetupPowerMng::DefaultSet()
{    m_strSmsSoundFileSMSTone = GetDefaultSMSSoundFilePath();
     //m_bAlertTone = TRUE;
     m_bAlertWindowSmsSet = TRUE;
	 m_bAlertToneSMSSet = TRUE;
     //m_bConncet = FALSE;
     //m_bLaunchApp = FALSE;
     //m_bStartMini = FALSE;
    // m_nSleepRadio = 1;
    UpdateData(FALSE);
}

void CDlgSetupPowerMng::OnButtonSmsSoundBrowseSMSSet()
{
    UpdateData(TRUE);
	CString strTitle;
    CFileDialog fileDlg(TRUE, _T("wav"), NULL, 0,
		_T("wav|*.wav||"), this);
	
	TCHAR fDefault[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, fDefault);
	CString strDefFolder;
	strDefFolder = fDefault;
    
    strTitle.LoadString(IDS_CHOOSE_SOUNDFILE);

	TCHAR szDefoultPath[MAX_PATH] = _T("");
	wcscpy(szDefoultPath, fDefault);
	wcscat(szDefoultPath, _T("\\wav"));
	
    fileDlg.m_ofn.lStructSize = 88;
    fileDlg.m_ofn.lpstrTitle = strTitle;
	fileDlg.m_ofn.lpstrInitialDir = szDefoultPath;
	
    if(fileDlg.DoModal() == IDOK)
    {
		m_strSmsSoundFileSMSTone = fileDlg.GetPathName();
		wcscpy(g_SetData.Messages_szSoundFile,m_strSmsSoundFileSMSTone);
        UpdateData(FALSE);
    }
	if(strDefFolder != _T(""))
	{
		::SetCurrentDirectory(strDefFolder);
	}
	return;

}

void CDlgSetupPowerMng::OnButtonSmsSoundPlaySMSSet()
{
	UpdateData(TRUE);
    PlaySound((LPCTSTR)m_strSmsSoundFileSMSTone, NULL, SND_ASYNC | SND_NOSTOP);

}

CString CDlgSetupPowerMng::GetDefaultSMSSoundFilePath()
{
    TCHAR *ptr = NULL;
    TCHAR szAppPath[MAX_PATH];
    CString strAppPath;
    
    memset(szAppPath, 0x00, MAX_PATH);
    ::GetModuleFileName(NULL, szAppPath, MAX_PATH);
	
    ptr = szAppPath + wcslen(szAppPath) - 1;
	
    while(ptr >= szAppPath)
    {
        if(*ptr == '\\')
        {
            *ptr = '\0';
            break;
        }
        else
            ptr--;
    }
	
    wcsncat(szAppPath, _T("\\wav\\sms.wav"), MAX_PATH);
    
    strAppPath.Format(_T("%s"), szAppPath);
    return strAppPath;
}