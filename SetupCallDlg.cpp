// SmsTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetupCallDlg.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupCallDlg dialog

CSetupCallDlg::CSetupCallDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSetupCallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmsTemplateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nBkTag = 1;
	m_hCallSetupEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_nPrivacy = 0;
	m_nPrivacySet = 0;
}

CSetupCallDlg::~CSetupCallDlg()
{
    CloseHandle(m_hCallSetupEvt);
}

void CSetupCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmsTemplateDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupCallDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CSmsTemplateDlg)
	ON_BN_CLICKED(IDC_RADIO_PRIVACY_ON, OnRadioPrivacyOn)
	ON_BN_CLICKED(IDC_RADIO_PRIVACY_OFF, OnRadioPrivacyOff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsTemplateDlg message handlers
void CSetupCallDlg::OnOK() 
{
	// TODO: Add extra cleanup here
	CHSDPADlg *pMainDlg = NULL;
	pMainDlg = (CHSDPADlg*)AfxGetMainWnd();	
	if (m_nPrivacySet != m_nPrivacy && pMainDlg && pMainDlg->m_pCallDlg->m_blIsInCalling == FALSE)
	{
		AtSndSetPrivacy(m_nPrivacySet);
	}
	else if (pMainDlg && pMainDlg->m_pCallDlg->m_blIsInCalling)
	{
		AtSndSetPrivacy(m_nPrivacySet);
	}

	AfxMessageBox(IDS_SMS_SETSUCC);
	
	CBaseDialog::OnOK();
}

BOOL CSetupCallDlg::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AtSndPrivacy();
	if (m_nPrivacy)
	{
		m_nPrivacySet = 1;
		((CButton*)GetDlgItem(IDC_RADIO_PRIVACY_ON))->SetCheck(1);
	}
	else
	{
		m_nPrivacySet = 0;
		((CButton*)GetDlgItem(IDC_RADIO_PRIVACY_OFF))->SetCheck(1);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetupCallDlg::AtSndPrivacy()
{
	char szAtBuf[20] = {0};
    strcpy(szAtBuf, "AT$HVPRIV?\r");
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespPrivacy, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hCallSetupEvt, SYNCINIT_TIMEOUT_SHORT))
            return TRUE;
    }

    return FALSE;
}

void CSetupCallDlg::AtRespPrivacy(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{	
	if(!strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
		&& !memcmp((const char*)strArr[0], "$HVPRIV: ", strlen("$HVPRIV: ")))
    {
        char *ptr = (char*)strArr[0] + strlen("$HVPRIV: ");

        ((CSetupCallDlg*)pWnd)->m_nPrivacy = atoi(ptr);
    }

	SetEvent(((CSetupCallDlg*)pWnd)->m_hCallSetupEvt);
}

void CSetupCallDlg::OnRadioPrivacyOn() 
{
	m_nPrivacySet = 1;
}

void CSetupCallDlg::OnRadioPrivacyOff() 
{
	m_nPrivacySet = 0;
}

BOOL CSetupCallDlg::AtSndSetPrivacy(int nPrivacy)
{
	CHSDPADlg *pMainDlg = NULL;
	pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	char szAtBuf[20] = {0};  

	if (pMainDlg && pMainDlg->m_pCallDlg->m_blIsInCalling)
	{
		const char szATSetPower[]="AT+CPRIV=";
		sprintf(szAtBuf, "%s%d\r", szATSetPower, nPrivacy);
	}
	else
	{
		const char szATSetPower[]="AT$HVPRIV=";
		sprintf(szAtBuf, "%s%d\r", szATSetPower, nPrivacy);
	}
  
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespSetPrivacy, (LPVOID)this);
        if(WAIT_OBJECT_0 == WaitForSingleObject(m_hCallSetupEvt, SYNCINIT_TIMEOUT_SHORT))
            return TRUE;
    }
	
    return FALSE;
}

void CSetupCallDlg::AtRespSetPrivacy(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{	
	if( !strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) )
    {
    }
	
	SetEvent(((CSetupCallDlg*)pWnd)->m_hCallSetupEvt);
}