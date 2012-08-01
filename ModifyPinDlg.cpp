// ModifyPinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "ModifyPinDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyPinDlg dialog

/*tag为0代表Modify PIN,为1代表Unlock PIN*/
CModifyPinDlg::CModifyPinDlg(CWnd* pParent,UINT tag)
: CBaseDialog(CModifyPinDlg::IDD, pParent)
{
	m_pinConf = _T("");
	m_pinCurr = _T("");
	m_pinNew = _T("");
	m_cRemainTime = _T("");
	m_nTag = tag;
    m_nBkTag = 1;
	m_pHandlePin = &(((CHSDPADlg*)AfxGetMainWnd())->m_cHandlePin);
	m_Hwnd = NULL;
}
CModifyPinDlg::~CModifyPinDlg()
{
	::CloseHandle(m_hCPwdEvt);
}

void CModifyPinDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModifyPinDlg)
	DDX_Text(pDX, IDC_EDIT_MPIN_CONF, m_pinConf);
//	DDV_MaxChars(pDX, m_pinConf, 8);
	DDX_Text(pDX, IDC_EDIT_MPIN_CURR, m_pinCurr);
//	DDV_MaxChars(pDX, m_pinCurr, 8);
	DDX_Text(pDX, IDC_EDIT_MPIN_NEW, m_pinNew);
//	DDV_MaxChars(pDX, m_pinNew, 8);
	DDX_Text(pDX, IDC_STATIC_MPIN_REMAIN, m_cRemainTime);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CModifyPinDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CModifyPinDlg)
    //ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_MPIN_OK, OnBtnMpinOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyPinDlg message handlers
void CModifyPinDlg::OnOK() 
{
    CBaseDialog::OnOK();
}

BOOL CModifyPinDlg::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
	m_bFirst = TRUE;
	//SetSkin(g_SetData.Setup_nSkinStyle);

	CString strT1,strT2,strBT1,strBT2;
	strT1.LoadString(IDS_PIN_MODIFY_T);
	strT2.LoadString(IDS_PIN_UNLOCK_T);
	strBT1.LoadString(IDS_PIN_BTNT);
	strBT2.LoadString(IDS_PUK_BTNT);
	UpdateData(TRUE);
	
	if (0 == m_nTag)
	{
		//SetWindowText("Modify PIN Code");//rc_update shang
		SetWindowText(strT1);
		SetDlgItemText(IDC_STATIC_PINPUK_TITLE,strBT1);
		m_cRemainTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes);
	}
	else
	{
		//SetWindowText("PUK Unlock PIN Code");//rc_update shang
		SetWindowText(strT2);
		SetDlgItemText(IDC_STATIC_PINPUK_TITLE,strBT2);
		m_cRemainTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes_puk);
	}
	UpdateData(FALSE);

	m_hCPwdEvt = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	((CEdit*)GetDlgItem(IDC_EDIT_MPIN_NEW))->SetLimitText(PIN_LEN);
	((CEdit*)GetDlgItem(IDC_EDIT_MPIN_CONF))->SetLimitText(PIN_LEN);
	if (0 == m_nTag)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_MPIN_CURR))->SetLimitText(PIN_LEN);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_MPIN_CURR))->SetLimitText(8);
	}

    return TRUE;  
	// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyPinDlg::OnBtnMpinOk() 
{
	int nRect;
	UpdateData();

	if(CheckCorrect())
	{
		if(m_nTag == 0)
		{
			nRect = OnPinModify();
			if(SYNCINITFUNCRET_DONE == nRect)
			{
				m_cRemainTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes);
				UpdateData(FALSE);
				
				if(m_pHandlePin->m_nRemainTimes == 3)
				{
					//::MessageBox(m_Hwnd,"Modify pin success!","HSDPA",MB_OK);
					//rc_update shang
					AfxMessageBox(IDS_PIN_MODIFY_OK);
					OnOK();
				}
				else if (m_pHandlePin->m_nRemainTimes == 0)
				{
					/*
					CString strPinLock;
					strPinLock.LoadString(IDS_PIN_LOCK);
					m_Hwnd = GetSafeHwnd();
					::MessageBox(m_Hwnd,strPinLock,"HSDPA",MB_OK);
					*/
					//rc_update shang
					AfxMessageBox(IDS_PIN_LOCK);
					OnOK();
				}
				else
				{
					/*
					m_Hwnd = GetSafeHwnd();
					::MessageBox(m_Hwnd,"Incorrect PIN Code!","HSDPA",MB_OK);
					*/
					//rc_update shang
					AfxMessageBox(IDS_PIN_ERR_PIN1);
				}
			}
		}
		else if(m_nTag == 1)
		{
			nRect = OnPinUnlock();
			if(SYNCINITFUNCRET_DONE == nRect)
			{
				m_cRemainTime.Format(_T("%d"),m_pHandlePin->m_nRemainTimes_puk);
				UpdateData(FALSE);
				
				if(10 == m_pHandlePin->m_nRemainTimes_puk)
				{
					OnOK();
				}
				else if (0 == m_pHandlePin->m_nRemainTimes_puk)
				{
					/*
					m_Hwnd = GetSafeHwnd();
					::MessageBox(m_Hwnd,"SIM Destroyed!","HSDPA",MB_OK);
					*/
					//rc_update shang
					AfxMessageBox(IDS_SIM_DESTROYED);

					OnCancel();
				}
				else
				{
					/*
					m_Hwnd = GetSafeHwnd();
					::MessageBox(m_Hwnd,"Incorrect PUK Code!","HSDPA",MB_OK);
					*/
					//rc_update shang
					AfxMessageBox(IDS_PUK_ERR);
				  
				}
			}
		} 	
	}
}

int CModifyPinDlg::OnPinModify()
{
	const TCHAR AtCpwd[]=_T("AT+CPWD=\"SC\",\"");
	const TCHAR Suffix[]=_T("\"\x0d\x00");
	TCHAR szAtBuf[512] = {0};
	CString strSend = AtCpwd;
	strSend += m_pinCurr;
	strSend +=_T("\",\"");
	strSend +=m_pinNew;
	strSend += Suffix;
	wcscpy(szAtBuf, strSend);
    char szAtAscBuf[512] = {0};
	WCharToChar(szAtBuf, szAtAscBuf);
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf)))
	{
		
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPwd, this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hCPwdEvt, 30000))
		{
			::ResetEvent(m_hCPwdEvt);
            return SYNCINITFUNCRET_DONE;  //同步正确
		}
        else
		{
			::ResetEvent(m_hCPwdEvt);
            return SYNCINITFUNCRET_RSP_TO;  //同步异常
		}
	}
    else
	{
	    return SYNCINITFUNCRET_SND_ERR;  //写端口失败
	}
}

int CModifyPinDlg::OnPinUnlock()
{
	const TCHAR AtCpin[]=_T("AT+CPIN=\"");
	const TCHAR Suffix[]=_T("\"\x0d\x00");
	TCHAR szAtBuf[512] = {0};
	CString strSend=AtCpin;
	strSend += m_pinCurr;
	strSend +=_T("\",\"");
	strSend +=m_pinNew;
	strSend += Suffix;
	wcscpy(szAtBuf, strSend);
	
	    char szAtAscBuf[512] = {0};
	WCharToChar(szAtBuf, szAtAscBuf);
	CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	ASSERT(pComm);
	if(pComm->WriteToPort(szAtAscBuf, wcslen(szAtBuf)))
	{		
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespCPwd, this);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hCPwdEvt, 30000))
		{
			::ResetEvent(m_hCPwdEvt);

            return SYNCINITFUNCRET_DONE;
		}
        else
		{
			::ResetEvent(m_hCPwdEvt);

            return SYNCINITFUNCRET_RSP_TO;
		}
	}
    else
	{
		return SYNCINITFUNCRET_SND_ERR;
	}
}

BOOL CModifyPinDlg::CheckCorrect()
{
	int len = m_pinNew.GetLength();
	int prelen = m_pinCurr.GetLength();
	if((len > 8)||(len < 4)||(prelen > 8)||(prelen < 4))
	{/*
		m_Hwnd = GetSafeHwnd();
		::MessageBox(m_Hwnd,"Length Error!","HSDPA",MB_OK);
	*/
		//rc_update shang
		AfxMessageBox(IDS_PIN_ERR_LEN);
	}
	else
	{
		if(m_pinConf == m_pinNew)
		{
			return TRUE;
		}
		else
		{
			/*
			m_Hwnd = GetSafeHwnd();
			::MessageBox(m_Hwnd,"Not The Same!","HSDPA",MB_OK);
			*/
			//rc_update shang
			AfxMessageBox(IDS_PIN_ERR_UNSAME);
		}
	}
	return FALSE;
}

/*Modify和Unlock的回调函数*/
void CModifyPinDlg::AtRespCPwd(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CModifyPinDlg *pdlgCPwd = (CModifyPinDlg *)pWnd;
	CPinEx * pHandle = (CPinEx*)(((CModifyPinDlg*)pWnd)->m_pHandlePin);

	CString strRet = strArr[0];
	if (pdlgCPwd->m_nTag == 0)  //Modify
	{
		if(-1 != strRet.Find(_T("OK"),0))
		{
			pHandle->m_nRemainTimes=PIN_RMTIMES_MAX;

		}
		else if(-1 != strRet.Find(_T("incorrect password"),0))
		{
			pHandle->m_nRemainTimes--;
		}
#ifdef FEATURE_HAIER_PINMANAGE
		else if(-1 != strRet.Find(_T("ERROR"),0))
		{
			pHandle->m_nRemainTimes--;
		}else if(-1 != strRet.Find(_T("SIM PUK"),0))
		{
			pHandle->m_nRemainTimes = 0;
			pHandle->m_nSimStat = CPIN_SIM_PUK_REQUIRED;
		}
#endif
		else if(-1 != strRet.Find(_T("SIM PUK required"),0))
		{
			pHandle->m_nRemainTimes = 0;
			pHandle->m_nSimStat = CPIN_SIM_PUK_REQUIRED;
		}
	}
	else if(pdlgCPwd->m_nTag == 1)  //Unlock
	{
		if(-1 != strRet.Find(_T("OK"),0))
		{
			pHandle->m_nSimStat = CPIN_SIM_NONE_REQUIRED;

			pHandle->m_nRemainTimes_puk = PUK_RMTIMES_MAX;
			pHandle->m_nRemainTimes = PIN_RMTIMES_MAX;
		}
		else if(-1 != strRet.Find(_T("incorrect password"),0))
		{
			pHandle->m_nRemainTimes_puk--;
		}
#ifdef FEATURE_HAIER_PINMANAGE
		else if(-1 != strRet.Find(_T("ERROR"),0))
		{
			pHandle->m_nRemainTimes_puk--;
		}
#endif
		else if(-1 != strRet.Find(_T("SIM Destroyed"),0))
		{
			pHandle->m_nRemainTimes_puk = 0;
			pHandle->m_nSimStat = CPIN_SIM_DESTROYED;
		}
	}
	SetEvent(pdlgCPwd->m_hCPwdEvt);
}
