// ReadFalshSMS.cpp : implementation file
//

#include "stdafx.h"
//#include "common_datacrad.h"
#include "ReadFalshSMS.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadFalshSMS dialog


CReadFalshSMS::CReadFalshSMS(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CReadFalshSMS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReadFalshSMS)
	m_strCFTContent = _T("");
    m_strCFTDatetime = _T("");
    m_strCFTName = _T("");
    m_strCFTNumber = _T("");
    m_strCFTByteCount = _T("");
	DlgDestoryTime = 30;//自动销毁时间
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSmsData = ((CHSDPAApp *)AfxGetApp())->GetSmsData();
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_nBkTag = 1;
}


void CReadFalshSMS::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReadFalshSMS)
	DDX_Text(pDX, IDC_EDIT_CFTSMS_CONTENT, m_strCFTContent);
    DDX_Text(pDX, IDC_EDIT_CFTSMS_DATETIME, m_strCFTDatetime);
    DDX_Text(pDX, IDC_EDIT_CFTSMS_NAME, m_strCFTName);
    DDX_Text(pDX, IDC_EDIT_CFTSMS_NUM, m_strCFTNumber);
    DDX_Text(pDX, IDC_STATIC_CFTSMS_BYTECOUNT, m_strCFTByteCount);
	DDX_Text(pDX,IDC_STATIC_FLASHSMS_TIME,DlgDestoryTime);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReadFalshSMS, CBaseDialog)
	//{{AFX_MSG_MAP(CReadFalshSMS)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED(ID_READCFT_SAVE, OnOK)
		ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadFalshSMS message handlers
BOOL CReadFalshSMS::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();

// 	RECT rect1;
// 	GetClientRect(&rect1);

   // MoveWindow(50,50,rect1.right,rect1.bottom,TRUE);

	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	
// 	if (pMainDlg->VoiceMail == FALSE)
// 	{
// 	    SetTimer(IDT_CFTDLG_DESTORY,1000,NULL);
// 	}
	UpdataSMS();
	return true;
}

void CReadFalshSMS::UpdataSMS()
{
	USES_CONVERSION;
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();

	if (pMainDlg->VoiceMail == FALSE)
	{
		//DlgDestoryTime = 30;//更新自动销毁时间
		//SetTimer(IDT_CFTDLG_DESTORY,1000,NULL);
	}

	CString strUnknown;    
    TCHAR  szName[PB_NAME_MAX + 2];
	UCHAR chr;
    StSmsRecord SmsRecord;
	memset(&SmsRecord, 0x0000, sizeof(StSmsRecord));
    char szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*4 + 4];
	memset(szContent, 0x0000, sizeof(szContent));
	
	strUnknown.LoadString(IDS_UNKNOWN);
	if (pMainDlg->VoiceMail == FALSE)
	{
// 		if (pMainDlg->CFTTempRecord.szNumber)
// 		{
// 			switch (pMainDlg->CFTTempRecord.SMSpriority)
// 			{
// 			case  2:
// 				//record.szContent = _T("Urgent! ");
// 				strncpy(szContent, W2A(_T("<Urgent!> \r\n")), 12);
// 				break;
// 			case  3:
// 				//record.szContent = _T("Emergency! ");
// 				strncpy(szContent, W2A(_T("<Emergency!> \r\n")), 15);
// 				break;
// 			default:
// 				break;
// 				
// 			}
// 			strncat(szContent,(const char*)pMainDlg->CFTTempRecord.szContent,sizeof(szContent));
// 			char *p,*q,*temp;
// 			p = szContent;
// 			q = szContent + 1;
// 			temp = szContent;
// 			while(*temp != '\0')
// 				temp++;
// 			for (int i=0;i<sizeof(szContent);i++)
// 			{
// 				if (*p == '\r' && *q == '\n')
// 				{
// 					p = p +2;
// 					q = q +2;
// 				}
// 				else if (*p == '\n')
// 				{
// 					while(temp!= p)
// 					{
// 						*temp = *(temp - 1);
// 						temp--;
// 					}
// 					*temp = '\r';
// 					while(*temp != '\0')
// 						temp++;
// 					
// 					p = p +2;
// 					q = q +2;
// 				}
// 				else if (*p == '\r')
// 				{
// 					*p = ' ';
// 					p++;
// 					q++;
// 				}
// 				else
// 				{
// 					p++;
// 					q++;
// 				}
// 		}
// 			m_strCFTContent = szContent;
// 			m_strCFTDatetime = pMainDlg->CFTTempRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
// 			m_strCFTNumber = pMainDlg->CFTTempRecord.szNumber;
// 			UpdateData(FALSE);
// 		}
// 	//////////////////////////////////////////////////////////////////////////
// 	if(pMainDlg->CFTTempRecord.szNumber[0] != '\0')
//     {
// 		chr = *(pMainDlg->CFTTempRecord.szNumber + 1);
// 		if(chr)
// 		{
// 			if(m_pPbData->SearchNameByNumber((CString)pMainDlg->CFTTempRecord.szNumber, szName))
// 				m_strCFTName.Format(_T("%s"), szName);
// 			else
// 				m_strCFTName = strUnknown;
// 			///add by liub for Unicode显示，字节转换（短——》宽）
// 			DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, pMainDlg->CFTTempRecord.szNumber, -1, NULL, 0);
// 			wchar_t *pwText;
// 			pwText = new wchar_t[dwNum];
// 			if(!pwText)
// 			{
// 				delete []pwText;
// 			}
// 			MultiByteToWideChar (CP_ACP, 0, pMainDlg->CFTTempRecord.szNumber, -1, pwText, dwNum);
// 			////////////////
// 			m_strCFTNumber = pwText;
//             delete []pwText;
// 			
// 			
// 		}
// 		else
// 		{
// 			if(m_pPbData->SearchNameByNumber((TCHAR*)pMainDlg->CFTTempRecord.szNumber, szName))
// 				m_strCFTName.Format(_T("%s"), szName);
// 			else
// 				m_strCFTName = strUnknown;
// 			m_strCFTNumber.Format(_T("%s"), pMainDlg->CFTTempRecord.szNumber);
// 		}
// 		// m_strNumber.Format(_T("%s"), SmsRecord.szNumber);
// 		
// 		
//     }
//     else
//     {
//         m_strCFTName = strUnknown;
//         m_strCFTNumber = strUnknown;
//     }
// 
// 	m_strCFTDatetime = pMainDlg->CFTTempRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
// 	
// 	USES_CONVERSION;
//     //Modified by Zhou Bin 2008.12.30
// 	//TCHAR szHead[30];
// 	char szHead[30];
//     memset(szHead, 0x00, sizeof(szHead));
//     //TCHAR szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 20];
// 	char szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 20];
//     memset(szDspContent, 0x00, sizeof(szDspContent));
// 	
// //     if(SmsRecord.flag & SMS_RECORD_FLAG_CONCATENATE_SEGE)
// //     {
// //         if(DspConcatenateSmsPara(szHead, SmsRecord.nRefCnt, SmsRecord.nSeqCnt, SmsRecord.nTotalCnt))
// // 			strcpy(szDspContent, szHead);
// // 		//	wcscpy(szDspContent, szHead);
// //     }
// 	
//     //wcsncat(szDspContent, (const TCHAR*)SmsRecord.szContent, sizeof(szDspContent));
// 	strncat(szDspContent, pMainDlg->CFTTempRecord.szContent, sizeof(szDspContent));
// 	///add by liub for Unicode显示，字节转换（短——》宽）
// 	if(chr)
// 	{
// 		TCHAR *WCharDspContent=A2W(szContent); 
// 		m_strCFTContent = WCharDspContent;
// 	}
// 	else
// 	{
// 		m_strCFTContent = (LPCTSTR)szContent;
// 		
// 	}
//     //m_strByteCount.Format(_T("(%d)"), GetUnicodeCharNum((const TCHAR*)SmsRecord.szContent));
// 	m_strCFTByteCount.Format(_T("(%d)"), GetUnicodeCharNum(A2W(szContent)));

	}
	else
	{
// 		m_strCFTContent = pMainDlg->VoicemailRecord.szContent;
// 		m_strCFTDatetime = pMainDlg->VoicemailRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
// 		m_strCFTNumber = pMainDlg->VoicemailRecord.szNumber;
			UpdateData(FALSE);

			//////////////////////////////////////////////////////////////////////////
			if(pMainDlg->VoicemailRecord.szNumber[0] != '\0')
			{
				chr = *(pMainDlg->VoicemailRecord.szNumber + 1);
				if(chr)
				{
					if(m_pPbData->SearchNameByNumber((CString)pMainDlg->VoicemailRecord.szNumber, szName))
						m_strCFTName.Format(_T("%s"), szName);
					else
						m_strCFTName = strUnknown;
					///add by liub for Unicode显示，字节转换（短——》宽）
					DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, pMainDlg->VoicemailRecord.szNumber, -1, NULL, 0);
					wchar_t *pwText;
					pwText = new wchar_t[dwNum];
					if(!pwText)
					{
						delete []pwText;
					}
					MultiByteToWideChar (CP_ACP, 0, pMainDlg->VoicemailRecord.szNumber, -1, pwText, dwNum);
					////////////////
					m_strCFTNumber = pwText;
					delete []pwText;
					
					
				}
				else
				{
					if(m_pPbData->SearchNameByNumber((TCHAR*)pMainDlg->VoicemailRecord.szNumber, szName))
						m_strCFTName.Format(_T("%s"), szName);
					else
						m_strCFTName = strUnknown;
					m_strCFTNumber.Format(_T("%s"), pMainDlg->VoicemailRecord.szNumber);
				}
				// m_strNumber.Format(_T("%s"), SmsRecord.szNumber);
				
				
			}
			else
			{
				m_strCFTName = strUnknown;
				m_strCFTNumber = strUnknown;
			}
			
			m_strCFTDatetime = pMainDlg->VoicemailRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
			
			USES_CONVERSION;
			//Modified by Zhou Bin 2008.12.30
			//TCHAR szHead[30];
			char szHead[30];
			memset(szHead, 0x00, sizeof(szHead));
			//TCHAR szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 20];
			char szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 20];
			memset(szDspContent, 0x00, sizeof(szDspContent));
			strncat(szContent,(const char*)pMainDlg->VoicemailRecord.szContent,sizeof(szContent));
			char *p,*q,*temp;
			p = szContent;
			q = szContent + 1;
			temp = szContent;
			while(*temp != '\0')
				temp++;
			for (int i=0;i<sizeof(szContent);i++)
			{
				if (*p == '\r' && *q == '\n')
				{
					p = p +2;
					q = q +2;
				}
				else if (*p == '\n')
				{
					while(temp!= p)
					{
						*temp = *(temp - 1);
						temp--;
					}
					*temp = '\r';
					while(*temp != '\0')
						temp++;
					
					p = p +2;
					q = q +2;
				}
				else if (*p == '\r')
				{
					*p = ' ';
					p++;
					q++;
				}
				else
				{
					p++;
					q++;
				}
		}
			
			//     if(SmsRecord.flag & SMS_RECORD_FLAG_CONCATENATE_SEGE)
			//     {
			//         if(DspConcatenateSmsPara(szHead, SmsRecord.nRefCnt, SmsRecord.nSeqCnt, SmsRecord.nTotalCnt))
			// 			strcpy(szDspContent, szHead);
			// 		//	wcscpy(szDspContent, szHead);
			//     }
			
			//wcsncat(szDspContent, (const TCHAR*)SmsRecord.szContent, sizeof(szDspContent));
			strncat(szDspContent, pMainDlg->VoicemailRecord.szContent, sizeof(szDspContent));
			///add by liub for Unicode显示，字节转换（短——》宽）
			if(pMainDlg->tempvoicemailASCUS == _T("2"))
			{
				TCHAR *WCharDspContent=A2W(szContent); 
				m_strCFTContent = WCharDspContent;
			}
			else
			{
				m_strCFTContent = (LPCTSTR)szContent;
				
			}
			//m_strByteCount.Format(_T("(%d)"), GetUnicodeCharNum((const TCHAR*)SmsRecord.szContent));
	m_strCFTByteCount.Format(_T("(%d)"), GetUnicodeCharNum(A2W(szContent)));

	}



	SmsRecord.voicemail =pMainDlg->VoicemailRecord.voicemail;

	if (pMainDlg->VoiceMail == TRUE)
	{
		GetDlgItem(ID_READCFT_SAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FLASHSMS_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SECONDS)->ShowWindow(SW_HIDE);
	}
	else
	{
	   // GetDlgItem(ID_READCFT_SAVE)->ShowWindow(SW_NORMAL);
	    //GetDlgItem(IDC_STATIC_FLASHSMS_TIME)->ShowWindow(SW_NORMAL);
		//GetDlgItem(IDC_STATIC_SECONDS)->ShowWindow(SW_NORMAL);
	}

	
	
    UpdateData(FALSE);

}

void CReadFalshSMS::OnOK()
{
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
//	pMainDlg->FlashSMSSave = 1;
	pMainDlg->RcvNewSmsProc(LOC_PC, pMainDlg->CFTTempRecord);
	memset(&pMainDlg->CFTTempRecord,0x00,sizeof(StSmsRecord));
	DestroyWindow();
	pMainDlg->m_pReadFalshSMSDlg = NULL;
//	pMainDlg->FlashSMSSave = 0;


}
void CReadFalshSMS::OnCancel()
{
	CHSDPADlg *pMainDlg = (CHSDPADlg*)AfxGetMainWnd();
	pMainDlg->VoiceMail = FALSE;
	memset(&pMainDlg->CFTTempRecord,0x00,sizeof(StSmsRecord));
	DestroyWindow();
	pMainDlg->m_pReadFalshSMSDlg = NULL;
	memset(&pMainDlg->VoicemailRecord,0x00,sizeof(StSmsRecord));
}

// void CReadFalshSMS::OnTimer(UINT nIDEvent)
// {
// // 	if (nIDEvent == IDT_CFTDLG_DESTORY)
// // 	{
// // 		if (DlgDestoryTime > 0 && DlgDestoryTime <= 30)
// // 		{
// // 			DlgDestoryTime--;
// // 			UpdateData(FALSE);
// // 		}
// // 		if (DlgDestoryTime == 0)
// // 		{
// // 			KillTimer(IDT_CFTDLG_DESTORY);
// // 			OnCancel();
// // 		}
// // 
// // 	}
// }
