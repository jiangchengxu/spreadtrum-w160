// DlgSignIn.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "DlgSignIn.h"
#include "ds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSignIn dialog


CDlgSignIn::CDlgSignIn(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSignIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSignIn)
	m_strKey = _T("");
	m_strLeaveTime = _T("");
	//}}AFX_DATA_INIT
	m_nBkTag = 1;
	//m_pParent = pParent;
	m_pMWnd = (CHSDPADlg*)pParent;

	m_pHandlePin = &(((CHSDPADlg*)AfxGetMainWnd())->m_cHandlePin);
	m_strLeaveTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes);
}

CDlgSignIn::~CDlgSignIn()
{
	::CloseHandle(m_hSignInEvt);
}

void CDlgSignIn::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSignIn)
	DDX_Text(pDX, IDC_EDIT_SIGNIN_KEY, m_strKey);
//	DDV_MaxChars(pDX, m_strKey, 8);
	DDX_Text(pDX, IDC_STATIC_LEAVETIME, m_strLeaveTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSignIn, CBaseDialog)
	//{{AFX_MSG_MAP(CDlgSignIn)
	ON_BN_CLICKED(IDC_BUTTON_SIGNIN, OnBtnSignin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSignIn message handlers

BOOL CDlgSignIn::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();

	m_hSignInEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	// TODO: Add extra initialization here
	m_bFirst = TRUE;
	//SetSkin(g_SetData.Setup_nSkinStyle);
	((CEdit*)GetDlgItem(IDC_EDIT_SIGNIN_KEY))->SetLimitText(PIN_LEN);

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSignIn::OnOK() 
{	
	CBaseDialog::OnOK();
}

void CDlgSignIn::AtRespCPinVerify(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CDlgSignIn* pdlg= (CDlgSignIn*)pWnd;
	//CPinEx * pHandle = (CDlgSignIn*)(((CModifyPinDlg*)pWnd)->m_pHandlePin);
	CString RetStr;
	RetStr = strArr[0];
	
	if(RetStr.Find(_T("OK"),0) != -1)
	{
		pdlg->m_pHandlePin->m_nSimStat = CPIN_SIM_NONE_REQUIRED;

		pdlg->m_pHandlePin->m_nRemainTimes = 3;
	}
	else if (RetStr.Find(_T("incorrect password"),0)!=-1)
	{
		(pdlg->m_pHandlePin->m_nRemainTimes)--;
	}
	else if(RetStr.Find(_T("SIM PUK required"),0) != -1)
	{
		pdlg->m_pHandlePin->m_nRemainTimes = 0;
	}

	::SetEvent(pdlg->m_hSignInEvt);
	
}

EnSyncInitFuncRetType CDlgSignIn::OnSignin()
{
	const TCHAR ATCPIN_V[]=_T("AT+CPIN=\"");
	const TCHAR DQuote[]= _T("\"\x0d\x00");
	TCHAR szAtBuf[512] = {0};
	char szAtAscBuf[512] = {0};
	CString cVal = ATCPIN_V;
	UpdateData();
	cVal += m_strKey;
	cVal += DQuote;
	wcscpy(szAtBuf, cVal);  //发送PIN码验证的AT命令


    WCharToChar(szAtBuf, szAtAscBuf);
	
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf)))
	{
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPinVerify, this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hSignInEvt, 5000))
		{
			::ResetEvent(m_hSignInEvt);
            return SYNCINITFUNCRET_DONE;
		}
        else
		{
			::ResetEvent(m_hSignInEvt);
            return SYNCINITFUNCRET_RSP_TO;
		}
	}
    else
	{
	    return SYNCINITFUNCRET_SND_ERR;
	}

}

void CDlgSignIn::OnBtnSignin()
{
	UpdateData();
	int len = m_strKey.GetLength();
	if ((len > 8)||(len < 4))
	{
		AfxMessageBox(IDS_PIN_ERR_LEN);

		return;
	}

	if (SYNCINITFUNCRET_DONE == OnSignin())  //返回正确
	{
		m_strLeaveTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes);
		UpdateData(FALSE);

		if(3 == m_pHandlePin->m_nRemainTimes)  //设置成功，则剩余的次数为3
		{
			OnOK();
		}
		else if (0 == m_pHandlePin->m_nRemainTimes)
		{
			//被锁了
			m_pHandlePin->m_nSimStat = CPIN_SIM_PUK_REQUIRED;
			CString strAppName;
			strAppName.LoadString(IDS_APPNAME);
			AddPrefixName(strAppName);
			CString strPuk;
			strPuk.LoadString(IDS_SIGNIN_NEEDPUK);
			m_Hwnd = GetSafeHwnd();
			::MessageBox(m_Hwnd,strPuk,strAppName,MB_OK);

			OnOK();
		}
		else
		{
			CString strAppName;
			strAppName.LoadString(IDS_APPNAME);
			AddPrefixName(strAppName);
			CString strErrPin;
			strErrPin.LoadString(IDS_PIN_ERR_PIN1);
			
			m_Hwnd = GetSafeHwnd();
			::MessageBox(m_Hwnd,strErrPin,strAppName,MB_OK);
		}
	}
}
