// SmsReadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "SmsReadDlg.h"
#include "SmsWriteDlg.h"
#include "SMSDlg.h"

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
// CSmsReadDlg dialog

CSmsReadDlg::CSmsReadDlg(EnSmsType type, EnLocFilter locFilter, WORD index, StDspEntityType (*pDspEntity)[LOCFILTER_MAX][SMS_TOTAL_RECORD_MAX], CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSmsReadDlg::IDD, pParent), m_DispFilter(type), m_LocFilter(locFilter), m_nCurIndex(index), m_pDspEntity(pDspEntity)
{
    //{{AFX_DATA_INIT(CSmsReadDlg)
    m_strContent = _T("");
    m_strDatetime = _T("");
    m_strName = _T("");
    m_strNumber = _T("");
    m_strByteCount = _T("");
    //}}AFX_DATA_INIT

    SetOwner(pParent);
    
    m_pSmsData = ((CHSDPAApp *)AfxGetApp())->GetSmsData();
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_nBkTag = 1;
}


void CSmsReadDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsReadDlg)
    DDX_Text(pDX, IDC_EDIT_SMS_CONTENT, m_strContent);
    DDX_Text(pDX, IDC_EDIT_SMS_DATETIME, m_strDatetime);
    DDX_Text(pDX, IDC_EDIT_SMS_NAME, m_strName);
    DDX_Text(pDX, IDC_EDIT_SMS_NUM, m_strNumber);
    DDX_Text(pDX, IDC_STATIC_SMS_BYTECOUNT, m_strByteCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSmsReadDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsReadDlg)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
    ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
    ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_EN_UPDATE(IDC_EDIT_SMS_CONTENT, OnUpdateEditSmsContent)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_READSMS_UPDATE, OnUpdateReadSms)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsReadDlg message handlers

BOOL CSmsReadDlg::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    ((CEdit*)GetDlgItem(IDC_EDIT_SMS_CONTENT))->SetReadOnly(FALSE);
    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);

    if(m_DispFilter == SMS_TYPE_INBOX)
    {
        CString str;
        str.LoadString(IDS_SMS_READ_REPLAY);
        GetDlgItem(IDC_BUTTON_FIRST)->SetWindowText(str);
        str.LoadString(IDS_SMS_READ_EXIT);
        GetDlgItem(IDC_BUTTON_LAST)->SetWindowText(str);        
    }

    UpdateSmsDsp();    

		GetDlgItem(IDC_BUTTON_FIRST)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_PREV)->ShowWindow(SW_NORMAL);		
		GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_BUTTON_LAST)->ShowWindow(SW_NORMAL);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsReadDlg::OnPaint() 
{
    // TODO: Add your message handler code here
    CBaseDialog::OnPaint();
    // Do not call CDialog::OnPaint() for painting messages
}

void CSmsReadDlg::OnButtonFirst() 
{
//	UCHAR chr;
    // TODO: Add your control notification handler code here
    if(m_DispFilter == SMS_TYPE_INBOX)
    {
// 		StSmsRecord record = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, m_nCurIndex);
// 
// 		chr = *(record.szNumber +1);
// 
// 		if(record.szNumber[0] == '\0' || chr)
// 		{
// 		///add by liub for Unicode显示，字节转换（短——》宽）
// 		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, record.szNumber, -1, NULL, 0);
// 		wchar_t *pwText;
// 		pwText = new wchar_t[dwNum];
// 		if(!pwText)
// 		{
// 			delete []pwText;
// 		}
// 		MultiByteToWideChar (CP_ACP, 0, record.szNumber, -1, pwText, dwNum);
// 		////////////////
// 		m_strNumber = pwText;
//         CSmsWriteDlg dlg(this, (LPCTSTR)pwText, NULL, 
//                             SMS_WRITE_REPLY);
//         dlg.DoModal();    
// 	    delete []pwText;
//         return;
// 		}
//		else
		{
			//m_strNumber
			CSmsWriteDlg dlg(this, (LPCTSTR)m_strNumber, NULL, 
				SMS_WRITE_REPLY);
			dlg.DoModal();    
		    return;

		}
    }

    m_nCurIndex = 0;
    UpdateSmsDsp();        
}

void CSmsReadDlg::OnButtonPrev() 
{
    // TODO: Add your control notification handler code here
    if(m_nCurIndex > 0)
    {
        m_nCurIndex--;
        UpdateSmsDsp();            
    }
}

void CSmsReadDlg::OnButtonNext() 
{
    // TODO: Add your control notification handler code here
    if(m_nCurIndex < GetTotalSmsNum(m_DispFilter, m_LocFilter) - 1)
    {
        m_nCurIndex++;
        UpdateSmsDsp();        
    }
}

void CSmsReadDlg::OnButtonLast() 
{
    // TODO: Add your control notification handler code here
    if(m_DispFilter == SMS_TYPE_INBOX)
    {
        OnCancel();
        return;
    }

    m_nCurIndex = GetTotalSmsNum(m_DispFilter, m_LocFilter) - 1;
    UpdateSmsDsp();    
}

//功能：显示消息记录
//输入：无
//输出：无
void CSmsReadDlg::DspSmsRecord()
{
    ASSERT(m_DispFilter >= SMS_TYPE_INBOX && m_DispFilter < SMS_TYPE_ALL);
    ASSERT(m_nCurIndex < GetTotalSmsNum(m_DispFilter, m_LocFilter));
USES_CONVERSION;
    CString strUnknown;    
    TCHAR  szName[PB_NAME_MAX + 2];
	UCHAR chr;


	StSmsRecord SmsRecord = GetTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, m_nCurIndex);
	char szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*4 + 4];
	memset(szContent, 0x0000, sizeof(szContent));

	switch (SmsRecord.SMSpriority)
	{
	case  1:
		strncpy(szContent, W2A(_T("<Interactive!> \r\n")), 17);
		break;
	case  2:
		//record.szContent = _T("Urgent! ");
		strncpy(szContent, W2A(_T("<Urgent!> \r\n")), 12);
		break;
	case  3:
		//record.szContent = _T("Emergency! ");
		strncpy(szContent, W2A(_T("<Emergency!> \r\n")), 15);
		break;
	default:
		break;
		
	}

	strncat(szContent,(const char*)SmsRecord.szContent,sizeof(szContent));
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

    chr = *(SmsRecord.szNumber + 1);

    if(SmsRecord.state == SMS_STATE_MT_NOT_READ)
    {
        SmsRecord.state = SMS_STATE_MT_READ;
        EditTotalSmsRecordFromDspIndex(m_DispFilter, m_LocFilter, m_nCurIndex, SmsRecord);
//        m_pSmsData->EditSmsRecord(m_DispFilter, nIndex, SmsRecord); 
        ((CHSDPADlg*)AfxGetMainWnd())->UpdateUnreadSmsIcon();
    }

    memset(szName, 0, sizeof(szName));    
    strUnknown.LoadString(IDS_UNKNOWN);
        
    if(SmsRecord.szNumber[0] != '\0')
    {
		if(chr)
		{
			if(m_pPbData->SearchNameByNumber((CString)SmsRecord.szNumber, szName))
				m_strName.Format(_T("%s"), szName);
			else
				m_strName = strUnknown;
			///add by liub for Unicode显示，字节转换（短——》宽）
			DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, SmsRecord.szNumber, -1, NULL, 0);
			wchar_t *pwText;
			pwText = new wchar_t[dwNum];
			if(!pwText)
			{
				delete []pwText;
			}
			MultiByteToWideChar (CP_ACP, 0, SmsRecord.szNumber, -1, pwText, dwNum);
			////////////////
			m_strNumber = pwText;
            delete []pwText;


		}
		else
		{
			if(m_pPbData->SearchNameByNumber((TCHAR*)SmsRecord.szNumber, szName))
				m_strName.Format(_T("%s"), szName);
			else
				m_strName = strUnknown;
			m_strNumber.Format(_T("%s"), SmsRecord.szNumber);
		}
       // m_strNumber.Format(_T("%s"), SmsRecord.szNumber);


    }
    else
    {
        m_strName = strUnknown;
        m_strNumber = strUnknown;
    }
    
    m_strDatetime = SmsRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);

//	USES_CONVERSION;
    //Modified by Zhou Bin 2008.12.30
	//TCHAR szHead[30];
	char szHead[30];
    memset(szHead, 0x00, sizeof(szHead));
    //TCHAR szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 20];
	char szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 20];
    memset(szDspContent, 0x00, sizeof(szDspContent));
	int SMSNumLength;

    if(SmsRecord.flag & SMS_RECORD_FLAG_CONCATENATE_SEGE)
    {
        if(DspConcatenateSmsPara(szHead, SmsRecord.nRefCnt, SmsRecord.nSeqCnt, SmsRecord.nTotalCnt))
			strcpy(szDspContent, szHead);
			//	wcscpy(szDspContent, szHead);
    }
        
    //wcsncat(szDspContent, (const TCHAR*)SmsRecord.szContent, sizeof(szDspContent));
	strncat(szDspContent, SmsRecord.szContent, sizeof(szDspContent));
	///add by liub for Unicode显示，字节转换（短——》宽）
	if(SmsRecord.szNumber[0] == '\0' || chr)
	{
		if (m_LocFilter == LOCFILTER_UIM && chr == 0)
		{
			TCHAR *WCharDspContent=(TCHAR*)szContent; 
			m_strContent = WCharDspContent;
		}
		else
		{
			m_strContent = (CString)szContent;
		}
	}
	else
	{
		SMSNumLength = wcslen((TCHAR*)SmsRecord.szNumber);
		m_strContent = (CString)szContent;

	}
    //m_strByteCount.Format(_T("(%d)"), GetUnicodeCharNum((const TCHAR*)SmsRecord.szContent));
	m_strByteCount.Format(_T("(%d)"), GetUnicodeCharNum(A2W(szContent)));

    UpdateData(FALSE);
}

//功能：更新按钮状态
//输入：无
//输出：无
void CSmsReadDlg::UpdateButtonStatus()
{
    WORD nNum = GetTotalSmsNum(m_DispFilter, m_LocFilter);

    ASSERT(m_DispFilter >= SMS_TYPE_INBOX && m_DispFilter < SMS_TYPE_ALL);
    ASSERT(m_nCurIndex < nNum);

    if(nNum == 1)
    {
        GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(FALSE);
        
        if(m_DispFilter == SMS_TYPE_INBOX)
        {
            GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(TRUE);
        }
        return;
    }

    if(m_nCurIndex == 0)
    {
        GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(TRUE);
    }
    else if(m_nCurIndex == nNum - 1)
    {
        GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(FALSE);        
    }
    else
    {
        GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(TRUE);
    }

    if(m_DispFilter == SMS_TYPE_INBOX)
    {
        GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(TRUE);
    }
}

//功能：设置窗口标题
//输入：无
//输出：无
void CSmsReadDlg::SetGroupBoxTitle()
{
    WORD nNum = GetTotalSmsNum(m_DispFilter, m_LocFilter);
    ASSERT(m_DispFilter >= SMS_TYPE_INBOX && m_DispFilter < SMS_TYPE_ALL);
    ASSERT(m_nCurIndex < nNum);
    
    CString str;
    CString strv;

    strv.Format(_T(": (%d/%d)"), m_nCurIndex+1, nNum);

    switch(m_DispFilter) {
    case SMS_TYPE_INBOX:
        str.LoadString(IDS_SMS_INBOX);
        break;
    case SMS_TYPE_OUTBOX:
        str.LoadString(IDS_SMS_OUTBOX);
        break;
    case SMS_TYPE_DRAFT:
        str.LoadString(IDS_SMS_DRAFT);
        break;
    }
    str += strv;
    GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(str);
}

//功能：更新SMS显示
//输入：无
//输出：无
void CSmsReadDlg::UpdateSmsDsp()
{
    CWnd *pWnd = GetOwner();
    SetGroupBoxTitle();    
    UpdateButtonStatus();
    pWnd->PostMessage(WM_SMS_READ_MSG, (WPARAM)m_nCurIndex, 0);
    DspSmsRecord();
}

//功能：更新读SMS
//输入：wParam:显示类型和位置，lParam:当前位置
//输出：结果
LRESULT CSmsReadDlg::OnUpdateReadSms(WPARAM wParam, LPARAM lParam)
{
    m_DispFilter = (EnSmsType)(wParam >> 16);
    m_LocFilter  = (EnLocFilter)(wParam & 0x0000ffff);
    m_nCurIndex  = (WORD)lParam;
    UpdateSmsDsp();
    return 0;
}

void CSmsReadDlg::OnOK() 
{
	// TODO: Add extra cleanup here
	
	//CBaseDialog::OnOK();
}

//功能：更新编辑框
//输入：无
//输出：无
void CSmsReadDlg::OnUpdateEditSmsContent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CAmoiBaseDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

    TCHAR *szDetails = new TCHAR[SMS_CONCAT_SEGMENT_MAX*SMS_CONCAT_ASCII_MAX*2 + 2];
    memset(szDetails, 0x0000, SMS_CONCAT_SEGMENT_MAX*SMS_CONCAT_ASCII_MAX*2+2);
    wcsncpy(szDetails, m_strContent, SMS_CONCAT_SEGMENT_MAX*SMS_CONCAT_ASCII_MAX*2);
	
    UpdateData(TRUE);

    m_strContent.Format(_T("%s"), szDetails);
//    Beep(600, 50);
	
    delete []szDetails;
    UpdateData(FALSE); 
	// TODO: Add your control notification handler code here
}

//功能：获得SMS条数
//输入：type:类型，locFilter:位置
//输出：条数
WORD CSmsReadDlg::GetTotalSmsNum(EnSmsType type, EnLocFilter locFilter)
{
	ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
	ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);

	WORD nPCNum = 0;
	WORD nMENum = 0;
	WORD nSMNum = 0;

	if(locFilter == LOCFILTER_PC || locFilter == LOCFILTER_ALL)
		nPCNum = m_pSmsData->GetSmsNum(type);

	if(g_bSmsSyncComm && type != SMS_TYPE_DRAFT && locFilter != LOCFILTER_PC)
	{
		WORD cnt;
		EnSmsState state;
		EnSmsState state1;
		EnSmsState state2;
		StSmsCardRecord CardRecord;

		if(type == SMS_TYPE_INBOX)
		{
			state1 = SMS_STATE_MT_NOT_READ;
			state2 = SMS_STATE_MT_READ;
		}
		else if(type == SMS_TYPE_OUTBOX)
		{
			state1 = SMS_STATE_MO_NOT_SENT;
			state2 = SMS_STATE_MO_SENT;
		}

		if(locFilter != LOCFILTER_UIM)
		{
			for(cnt=0; cnt<g_ME_SmsNum; cnt++)
			{
				CardRecord = UE_ReadSmsCardRecord(LOC_ME, cnt);
				state = CardRecord.record.state;
				if(state == state1 || state == state2)
					nMENum++;
			}
		}

		if(locFilter != LOCFILTER_ME)
		{
			for(cnt=0; cnt<g_SM_SmsNum; cnt++)
			{
				CardRecord = UE_ReadSmsCardRecord(LOC_UIM, cnt);
				state = CardRecord.record.state;
				if(state == state1 || state == state2)
					nSMNum++;
			}
		}
	}

	if(locFilter == LOCFILTER_PC)
		return nPCNum;
	else if(locFilter == LOCFILTER_ME)
		return nMENum;
	else if(locFilter == LOCFILTER_UIM)
		return nSMNum;
	else
		return (nPCNum+nMENum+nSMNum);
}

//功能：从索引获得SMS记录
//输入：type:类型，locFilter:位置，dspIndex:索引
//输出：SMS记录
StSmsRecord CSmsReadDlg::GetTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex)
{
	ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
	ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);
	ASSERT(dspIndex < GetTotalSmsNum(type, locFilter));

	StDspEntityType SmsDspEntity = m_pDspEntity[type][locFilter][dspIndex];

	if(SmsDspEntity.LocType == LOC_PC)
		return m_pSmsData->ReadSmsRecord(type, SmsDspEntity.nIndex);
	else
	{
		StSmsCardRecord CardRecord = UE_ReadSmsCardRecord(SmsDspEntity.LocType, SmsDspEntity.nIndex);
		return CardRecord.record;
	}
}

//功能：从索引编辑SMS记录
//输入：type:类型，locFilter:位置，dspIndex:索引，record:记录
//输出：无
void CSmsReadDlg::EditTotalSmsRecordFromDspIndex(EnSmsType type, EnLocFilter locFilter, WORD dspIndex, const StSmsRecord &record)
{
	ASSERT(type >= SMS_TYPE_INBOX && type < SMS_TYPE_ALL);
	ASSERT(locFilter >= LOCFILTER_PC && locFilter <= LOCFILTER_ALL);
	ASSERT(dspIndex < GetTotalSmsNum(type, locFilter));

	StDspEntityType SmsDspEntity = m_pDspEntity[type][locFilter][dspIndex];

	if(SmsDspEntity.LocType == LOC_PC)
		m_pSmsData->EditSmsRecord(type, SmsDspEntity.nIndex, record);
	else
		UE_EditSmsCardRecord(SmsDspEntity.LocType, SmsDspEntity.nIndex, record);
}