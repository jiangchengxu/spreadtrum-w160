#include "stdafx.h"
#include "HSDPA.h"
#include "PinEx.h"

CPinEx::CPinEx()
{
	m_pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
	m_pRetWnd = (CHSDPADlg*)AfxGetMainWnd();
	
	m_GetPinStatEvent = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	m_nRespIndex = -1;
	m_nPinStat = -1;
	m_nSimStat = -1;
	m_nRemainTimes = -1;
	m_nRemainTimes_puk = -1;
}

CPinEx::~CPinEx()
{
	::CloseHandle(m_GetPinStatEvent);
}

/*��ѯSIM��״̬���Ƿ�����*/
BOOL CPinEx::GetSimStat()
{
	const char ATCPINA[]="AT+CPIN?\x0d\x00";
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCPINA);

	ASSERT(m_pComm);
	if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
	{
		m_nRespIndex = 0;
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespPinHandler, this);
		WaitForSingleObject(m_GetPinStatEvent,5000);

		::ResetEvent(m_GetPinStatEvent);

		return TRUE;
	}
	return FALSE;
}

//���PIN�뼤��״̬
BOOL CPinEx::GetPinStat()
{
	const char ATCLCKsc2[]="AT+CLCK=\"SC\",2\x0d\x00";  
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCLCKsc2);
	
	ASSERT(m_pComm);
	if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
	{
		m_nRespIndex = 3;
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespPinHandler, this);

		WaitForSingleObject(m_GetPinStatEvent,5000);

		::ResetEvent(m_GetPinStatEvent);

		return TRUE;
	}
	return FALSE;

}

BOOL CPinEx::GetPinRemainTimes()
{
	const char ATCLCKsc2[]="AT$CPINS?\x0d\x00";  
	char szAtBuf[512] = {0};
	strcpy(szAtBuf, ATCLCKsc2);
	
	ASSERT(m_pComm);
	if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
	{
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespPinRemainTimes, this);

		WaitForSingleObject(m_GetPinStatEvent,5000);

		::ResetEvent(m_GetPinStatEvent);

		return TRUE;
	}
	return FALSE;

}

/*
BOOL CPinEx::EnablePin(CString& Pin)
{
	const char AtClck1[]="AT+CLCK=\"SC\",1,\"";
	const char Suffix[]="\"\x0d\x00";
	char szAtBuf[512] = {0};
	CString strSend=AtClck1;
	strSend+= Pin;
	strSend+= Suffix;
	strcpy(szAtBuf, strSend);
	ASSERT(m_pComm);
	if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf)))
	{
		m_nRespIndex = 1;
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespPinHandler, this);
		return TRUE;
	}

	return FALSE;
}

BOOL CPinEx::DisablePin(CString& Pin)
{
	const char AtClck1[]="AT+CLCK=\"SC\",0,\"";
	const char Suffix[]="\"\x0d\x00";
	char szAtBuf[512] = {0};
	CString strSend=AtClck1;
	strSend+= Pin;
	strSend+= Suffix;
	strcpy(szAtBuf, strSend);
	ASSERT(m_pComm);
	if(m_pComm->WriteToPort(szAtBuf, strlen(szAtBuf))){
		m_nRespIndex = 2;
		RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespPinHandler, this);
		return TRUE;
	}
	return FALSE;
}


BOOL CPinEx::DeLockPuk(CString& Puk)
{
	return FALSE;
}
*/
const stTXTCode CPinEx::cme_table[] =
{
	CME_SIM_NOT_INSERTED,"SIM not inserted",
	CME_SIM_PIN_REQUIRED,"SIM PIN required",
	CME_SIM_PUK_REQUIRED,"SIM PUK required",
	CME_SIM_FAILURE,"SIM failure",
	CME_SIM_BUSY,"SIM busy",
	CME_SIM_WRONG,"SIM wrong",
	CME_INCORRECT_PASSWORD,"incorrect password",
	CME_SIM_NONE_REQUIRED,"SIM PIN not required",
	CME_SIM_MAX,NULL
};
const stTXTCode CPinEx::cpin_table[] = {
	CPIN_SIM_PIN_REQUIRED,"SIM PIN",
	CPIN_SIM_PUK_REQUIRED,"SIM PUK",
	CPIN_SIM_NONE_REQUIRED,"READY",
	CPIN_SIM_DESTROYED,"SIM DESTROYED",
	CPIN_SIM_MAX,NULL
};

/*AT$CPINS?�Ļص�����*/
void CPinEx::AtRespPinRemainTimes(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	//$CPINS:1,0,10
	CPinEx * pHandle = (CPinEx*)pWnd;
	CString strRet = strArr[0];
	char* pbuf = (char *)strArr[0];
	int nfrom = 0;
	int nto = 0;
	//TCHAR temp[4];
	char temp[4];

	nfrom = strRet.ReverseFind(',') + 1;
	nto = strRet.GetLength();
	memcpy(temp,pbuf + nfrom,(nto - nfrom + 1));

    //pHandle->m_nRemainTimes_puk = _ttoi(temp);
	pHandle->m_nRemainTimes_puk = atoi(temp);

	strRet = strRet.Mid(0,(nfrom - 1));
	
	nfrom = strRet.ReverseFind(',') + 1;
	nto = strRet.GetLength();
	memcpy(temp,pbuf + nfrom,(nto - nfrom + 1));
   // pHandle->m_nRemainTimes = _ttoi(temp);
	pHandle->m_nRemainTimes = atoi(temp);

	::SetEvent(pHandle->m_GetPinStatEvent);
}

/*AT+CPIN?����Ļص�����*/
void CPinEx::AtRespPinHandler(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CPinEx * pHandle = (CPinEx*)pWnd;
	CString strRet =(char*) strArr[0];

	switch(pHandle->m_nRespIndex)
	{
		case 0:  //��ѯSIM����ǰ״̬��AT+CPIN?����
		{
			if(strRet.Find(_T("SIM PIN"),0) != -1)  //Ҫ��PIN����֤
			{
				pHandle->m_nSimStat = CPIN_SIM_PIN_REQUIRED;

				::SetEvent(pHandle->m_GetPinStatEvent);
			}
			else if (strRet.Find(_T("SIM PUK"),0) != -1)   //SIM����
			{
				pHandle->m_nSimStat = CPIN_SIM_PUK_REQUIRED;

				::SetEvent(pHandle->m_GetPinStatEvent);
			}
			else if (strRet.Find(_T("READY"),0) != -1)   //PIN����֤û�д�,��������ʹ��
			{
				pHandle->m_nSimStat = CPIN_SIM_NONE_REQUIRED;

				::SetEvent(pHandle->m_GetPinStatEvent);
			}
			break;
		}
		case 1:  //����PIN
		{
			if(strRet.Find(_T("OK"),0)!=-1)
			{
				pHandle->m_nRemainTimes=PIN_RMTIMES_MAX;
			}
			else if(strRet.Find(_T("incorrect password"),0)!=-1)
			{
				pHandle->m_nRemainTimes--;
			}
			else if(strRet.Find(_T("SIM PUK required"),0)!=-1)
			{
				pHandle->m_nRemainTimes=0;
			}

			break;
		}

		case 2:  //����PIN
		{
			if(strRet.Find(_T("OK"),0)!=-1)
			{
				pHandle->m_nRemainTimes=PIN_RMTIMES_MAX;
			}
			else if(strRet.Find(_T("incorrect password"),0)!=-1)
			{
				pHandle->m_nRemainTimes--;
			}
			else if(strRet.Find(_T("SIM PUK required"),0)!=-1)
			{
				pHandle->m_nRemainTimes=0;
			}

			break;
		}
		case 3:  //��ѯPIN״̬
		{
			if(-1 != strRet.Find(_T("1"),0))
			{
				pHandle->m_nPinStat = 1;  //Ϊ1��ʾPIN�Ѵ�
			}
			else if (-1 != strRet.Find(_T("0"),0))
			{
				pHandle->m_nPinStat = 0;  ////Ϊ0��ʾPIN�ر�
			}
			::SetEvent(pHandle->m_GetPinStatEvent);

			break;
		}
		default:
			{
				break;
			}
			
	}
}
