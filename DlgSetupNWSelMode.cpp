// DlgSetupNWSelMode.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "DlgSetupNWSelMode.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSelMode dialog


CDlgSetupNWSelMode::CDlgSetupNWSelMode(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSetupNWSelMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetupNWSelMode)
		// NOTE: the ClassWizard will add member initialization here

    m_nBkTag = 1;
	m_nAutoMode = 3;
	nSetPREFOK = 0;
	m_nMsgBeat = 0;
	//}}AFX_DATA_INIT

	//wyw
	pdlgMsg = NULL;
}


void CDlgSetupNWSelMode::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetupNWSelMode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Radio(pDX, IDC_RADIO_SETUP_NWSEL_GEONLY_MODE, m_nAutoMode);//add by liub 1027 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupNWSelMode, CBaseDialog)
	//{{AFX_MSG_MAP(CDlgSetupNWSelMode)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED(IDC_BUTTON_NWSET, OnButtonNwselSet)
		ON_MESSAGE(WM_SETUP_UPDATE_NWSELDLG, OnUpdateNWSelDlgMode)
		ON_WM_TIMER()
		ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupNWSelMode message handlers


//add by liub 1028 for NWSET
void CDlgSetupNWSelMode::RspAtPHPREF(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CDlgSetupNWSelMode* pDlg = (CDlgSetupNWSelMode*)pWnd;
    pDlg->KillTimer(IDT_PHPREF_TIMEOUT);
	
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
        gCurAutoPref = pDlg->m_nAutoMode;
		pDlg->PostMessage(WM_SETUP_UPDATE_NWSELDLG, (WPARAM)3, (LPARAM)TRUE);
    }
    else
    {
		pDlg->nSetPREFOK = 0;
		pDlg->PostMessage(WM_SETUP_UPDATE_NWSELDLG, (WPARAM)3, (LPARAM)FALSE);
    }
}

BOOL CDlgSetupNWSelMode::SndAtPHPREF(CString strPHPREF)
{
    const char szATPHPREF[]="AT+PHPREF=";
    char szAtBuf[50] = {0};
    
	   char *StrPre = new char[_tcslen(strPHPREF) * 2 + 1];
	   memset(StrPre, 0, _tcslen(strPHPREF) * 2 + 1);
	   int len = WCharToUnicode(strPHPREF, StrPre);
	   sprintf(szAtBuf, "%s%s\r", szATPHPREF, StrPre);
       delete []StrPre;
	   CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	   ASSERT(pComm);
	   if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
	   {
		   SetTimer(IDT_PHPREF_TIMEOUT, 60000, NULL);
		   RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtPHPREF, this);
		   return TRUE;
	   }
	   return FALSE;
}

void CDlgSetupNWSelMode::OnButtonNwselSet()
{
	CString strPr;
	strPr.Format(_T("%d"), m_nAutoMode+1);
	//MessageBox(strPr);
	if (SndAtPHPREF(strPr))
	{
		//disable all buttons
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_MODEPRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSPRE_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEPRE_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSONLY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEONLY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NWSET)->EnableWindow(FALSE);
		
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(FALSE);
	}

}



BOOL CDlgSetupNWSelMode::InitData()
{
	//during a data connection forbid the user to use "Network" menu in order to change the network.
	CHSDPADlg* pMainDlg = (CHSDPADlg*)(theApp.GetMainWnd());
	if(g_bDataService)
	{
	/*	GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MODEPRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSPRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEPRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SET)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_SETUP_NWSEL_NWLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_REFRESH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SELECT)->EnableWindow(FALSE);
		AfxMessageBox(IDS_DATASERVICING);*/
	}
	else
	{
	/*	GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MODEPRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSPRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEPRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SET)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(TRUE);*/
	}
    m_nAutoMode = gCurAutoPref;
    UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgSetupNWSelMode::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

 
	m_nMsgBeat=0;
	pdlgMsg = new CDlgMsg(this);

	InitData();

	return TRUE;

}

//wyw
void CDlgSetupNWSelMode::OnDestroy()
{
	if (pdlgMsg)
	{
		delete pdlgMsg;
		pdlgMsg = NULL;
	}
}

LRESULT CDlgSetupNWSelMode::OnUpdateNWSelDlgMode(WPARAM wParam, LPARAM lParam)
{

    KillTimer(IDT_COPS_MSG);
    if(pdlgMsg->m_hWnd)
        pdlgMsg->EndDialog(0);
    m_nMsgBeat=0;
	switch((int)wParam)
	{
	case 0:	
		if (m_nSelMode==0)
		{
			GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(TRUE);
			
			GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SELECT)->EnableWindow(TRUE);
		} 
		else if(m_nSelMode==1)
		{
			GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(TRUE);
			
			GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_LIST_SETUP_NWSEL_NWLIST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SETUP_NWSEL_REFRESH)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SELECT)->EnableWindow(TRUE);
		}
		break;
	case 2:
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_SETUP_NWSEL_NWLIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_REFRESH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETUP_NWSEL_SELECT)->EnableWindow(TRUE);
		break;
	case 3:
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_AUTOSEL)->EnableWindow(TRUE);
//		GetDlgItem(IDC_STATIC_MODEPRE)->EnableWindow(TRUE);//liub_test
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSPRE_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEPRE_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_UMTSONLY_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SETUP_NWSEL_GEONLY_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NWSET)->EnableWindow(TRUE);
        if(lParam)
			AfxMessageBox(IDS_SETOK, MB_OK | MB_ICONINFORMATION);
        else
            AfxMessageBox(IDS_SETUP_NWAUTOSETERROR);
		
		//GetDlgItem(IDC_RADIO_SETUP_NWSEL_MANSEL)->EnableWindow(TRUE);
		break;
	}
	return TRUE;
}

void CDlgSetupNWSelMode::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if(nIDEvent == IDT_COPS_MSG)
    {
		CString strTemp;
		CString strMsg;
		UINT nLen;
		
        if(nCOPSType==0)
            strTemp.LoadString(IDS_SEARCH_NWLIST);
        else
			strTemp.LoadString(IDS_REFRESH_NWLIST);
		
		nLen=strTemp.GetLength();
		strMsg=strTemp.Left(nLen-6+m_nMsgBeat);
		//OutputDebugString(strMsg);
		pdlgMsg->m_strMsg=strMsg;
		m_nMsgBeat++;
		if(m_nMsgBeat>6)
		{
			m_nMsgBeat=0;
		}
		if (pdlgMsg->m_hWnd)
		{
			pdlgMsg->UpdateData(FALSE);
		}
    }
    else if(nIDEvent == IDT_COPS_TIMEOUT)
    {
        KillTimer(IDT_COPS_TIMEOUT);
		bSetNWSucc = false;
        DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
        ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
		PostMessage(WM_SETUP_UPDATE_NWSELDLG, (WPARAM)(nCOPSType), (LPARAM)TRUE);		
    }
	CBaseDialog::OnTimer(nIDEvent);
}