// DiagnosticDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DiagnosticDlg.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticDlg dialog


CDiagnosticDlg::CDiagnosticDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDiagnosticDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiagnosticDlg)
	m_strFWVersion = _T("");
	m_strIMEI = _T("");
	m_strPbmNum = _T("0\0");
	m_strSmsNum = _T("0\0");
	m_strSWVersion = _T("");
	//}}AFX_DATA_INIT
    m_nBkTag = 1;
    m_nPbmMax = m_nPbmNum = 0;
	m_syncevent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_SendATState = -1;
}

CDiagnosticDlg::~CDiagnosticDlg()
{
	CloseHandle(m_syncevent);
}

void CDiagnosticDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagnosticDlg)
 	DDX_Control(pDX, IDC_COMBO_STORAGETYPE, m_cmbStorageType);
	
	DDX_Text(pDX, IDC_STATIC_DIAG_FIRM_VER, m_strFWVersion);
	DDX_Text(pDX, IDC_STATIC_IMEI, m_strIMEI);
	DDX_Text(pDX, IDC_STATIC_DIAG_PBM_NUM, m_strPbmNum);
	DDX_Text(pDX, IDC_STATIC_DIAG_SMS_NUM, m_strSmsNum);
	DDX_Text(pDX, IDC_STATIC_DIAG_SOFT_VER, m_strSWVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiagnosticDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CDiagnosticDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_DIAGNOSTICS_NOTIFY, OnDiagnosticsNotify)
 	ON_CBN_SELCHANGE(IDC_COMBO_STORAGETYPE, OnComboSelStorageType)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticDlg message handlers

BOOL CDiagnosticDlg::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//     m_bFirst = TRUE;
// 
// 	SetSkin(g_SetData.Setup_nSkinStyle);
	InitInterface();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiagnosticDlg::InitInterface()
{
	CString strMessage,strContacts,strIMEI;
	EnLocType loctype = LOC_MAX;  

#ifdef FEATURE_VERSION_NOSIM
	GetDlgItem(IDC_STORAGETYPE)->ShowWindow(SW_HIDE);
	m_cmbStorageType.ShowWindow(SW_HIDE);
	m_strSmsNum.Format(_T("%d / %d"), g_ME_SmsNum, g_ME_SmsMax);
	loctype = LOC_ME;
#else	
	CString strStorageType;
	strStorageType.LoadString(IDS_DATACARD);
	m_cmbStorageType.AddString(strStorageType); 

	if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
	{
		strStorageType.LoadString(IDS_STATIC_USIM);
		m_cmbStorageType.AddString(strStorageType);
		
		strMessage.LoadString(IDS_MESSAGE_USIM);
		strContacts.LoadString(IDS_CONTACTS_USIM);
		strIMEI.LoadString(IDS_IMEI);
	}
	else if(!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
	{
		strStorageType.LoadString(IDS_STATIC_RUIM);
		m_cmbStorageType.AddString(strStorageType);

		strMessage.LoadString(IDS_MESSAGE_RUIM);
		strContacts.LoadString(IDS_CONTACTS_RUIM);
		strIMEI.LoadString(IDS_ESN);
	}	
	GetDlgItem(IDC_TEXT_MESSAGE)->SetWindowText(strMessage);
	GetDlgItem(IDC_TEXT_CONTACTS)->SetWindowText(strContacts);
	GetDlgItem(IDC_TEXT_ESN)->SetWindowText(strIMEI);
	m_strSmsNum.Format(_T("%d / %d"), g_SM_SmsNum, g_SM_SmsMax);
	
	m_cmbStorageType.SetCurSel(0);
	loctype = LOC_UIM;
#endif	
	
	CHSDPADlg* pDlg = (CHSDPADlg*)AfxGetMainWnd();
	m_strIMEI = pDlg->m_strIMEI;
	m_strFWVersion = pDlg->m_strFWVersion;
    m_strSWVersion = pDlg->m_strSWVersion;

    AtSndCPBS(loctype);
	SendMessage(WM_DIAGNOSTICS_NOTIFY, 0, 0);	

    UpdateData(FALSE);
	return;
}

BOOL CDiagnosticDlg::AtSndCPBS(EnLocType loctype)
{
	ResetEvent(m_syncevent);
    ASSERT(loctype == LOC_ME || loctype == LOC_UIM);

    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "AT+CPBS=\"%s\"\r", gcstrLoc[loctype]);
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
		m_SendATState = SEND_AT_SET_STORE;
        SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPBS, this);
		::WaitForSingleObject(m_syncevent,INFINITE);
        return TRUE;
    }
    return FALSE;
}

//AT+CPBS: "SM",1,200
void CDiagnosticDlg::AtRespCPBS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	int Len = 512;
    char szAtBuf[512];
    char buf[10240] = {0};
    CSerialPort* pComm;
    CDiagnosticDlg *pDlg = (CDiagnosticDlg*)pWnd;

	ASSERT(wStrNum == 1 || wStrNum == 2);
    pDlg->KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE);
    
	switch(pDlg->m_SendATState)
	{
		case SEND_AT_SET_STORE:
		{
			if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]))
			{			   
			   ::SetEvent(pDlg->m_syncevent); 
			   return;
			}
			memset(szAtBuf,0,sizeof(char)*Len);
			strcpy(szAtBuf,"AT+CPBS?\r");
			szAtBuf[strlen(szAtBuf)] = 0x0D;
			szAtBuf[strlen(szAtBuf)] = 0;
			pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
			ASSERT(pComm);
			if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
			{ 
				pDlg->m_SendATState = SEND_STORE_CHECK_USEDANDTOTAL;
				pDlg->SetTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE, 30000, NULL);
				RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPBS, pWnd);				
			}
			else
			{
				::SetEvent(pDlg->m_syncevent); 
			}  			
			break;
		}
		case SEND_STORE_CHECK_USEDANDTOTAL:
		{
			if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
				 || strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]))
			{
				::SetEvent(pDlg->m_syncevent); 
				return;
			}
			//AT+CPBS: "SM",1,200
			int cnt = 0;
			char *p[2], *ptr;
			p[0] = p[1] = ptr = NULL;

			ptr = (char*)strArr[0];

			while(*ptr && cnt < 2)
			{
				if(*ptr == ',')
				{
					*ptr = 0;
					p[cnt++] = ++ptr;
				}
				else
					ptr++;
			}
			if(cnt == 2 && p[0] != NULL && p[1] != NULL)
			{
				pDlg->m_nPbmNum = atoi((const char*)p[0]);
				pDlg->m_nPbmMax = atoi((const char*)p[1]);
			}
			::SetEvent(pDlg->m_syncevent); 
			break;		
		}
		default:
			break;
	}
	return;
}

LRESULT CDiagnosticDlg::OnDiagnosticsNotify(WPARAM wParam, LPARAM lParam)
{	
#ifdef FEATURE_VERSION_NOSIM
	m_strSmsNum.Format(_T("%d / %d"), g_ME_SmsNum, g_ME_SmsMax);
#endif
	m_strPbmNum.Format(_T("%d / %d"), m_nPbmNum, m_nPbmMax);
    UpdateData(FALSE);
    return 0;
}

void CDiagnosticDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if(nIDEvent == TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE)
    {
        KillTimer(TIMER_ID_ATRESP_TIMEOUT_CPBS_SEL_STORE);
        DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
        ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
        SendMessage(WM_DIAGNOSTICS_NOTIFY, 0, 0);
		::SetEvent(m_syncevent); 
    }	
	CDialog::OnTimer(nIDEvent);
}

void CDiagnosticDlg::OnComboSelStorageType()
{
	CString strMessage,strContacts;
	   
    int nIndex = m_cmbStorageType.GetCurSel();
    switch (nIndex)
    {
		case 0: //USIM 
		{
			if(!wcsicmp(g_SetData.Setup_sz3GType,_T("WCDMA")))
			{
				strMessage.LoadString(IDS_MESSAGE_USIM);
				strContacts.LoadString(IDS_CONTACTS_USIM);
			}
			else if(!wcsicmp(g_SetData.Setup_sz3GType,_T("CDMA2000")))
			{				
				strMessage.LoadString(IDS_MESSAGE_RUIM);
				strContacts.LoadString(IDS_CONTACTS_RUIM);
			}

			AtSndCPBS(LOC_UIM);
			m_strSmsNum.Format(_T("%d / %d"), g_SM_SmsNum, g_SM_SmsMax);
			
			break;
		}			
		case 1: //USB Modem
		{
			strMessage.LoadString(IDS_MESSAGE_USBMODEM);			
			strContacts.LoadString(IDS_CONTACTS_USBMODEM);

			AtSndCPBS(LOC_ME);	
			m_strSmsNum.Format(_T("%d / %d"), g_ME_SmsNum, g_ME_SmsMax);
			
			break;
		}			
		default:
			break;
	}
	GetDlgItem(IDC_TEXT_MESSAGE)->SetWindowText(strMessage);
	GetDlgItem(IDC_TEXT_CONTACTS)->SetWindowText(strContacts);
	SendMessage(WM_DIAGNOSTICS_NOTIFY, 0, 0);
	UpdateData(FALSE);
	return;
}


