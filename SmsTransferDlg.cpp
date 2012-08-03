// SmsTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "SmsTransferDlg.h"
#include "SmsDlg.h"
#include "Ds.h"
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
// CSmsTransferDlg dialog


CSmsTransferDlg::CSmsTransferDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSmsTransferDlg::IDD, pParent)
{
    SetOwner(pParent);
    
    //{{AFX_DATA_INIT(CSmsTransferDlg)
    m_nRadio = 0;
    //}}AFX_DATA_INIT

    pDlg = NULL;
    m_nLower = 0;
    m_nUpper = 10;
    m_nStep = 1;

    memset(m_szLoc, 0, sizeof(m_szLoc));
    
    m_locType1 = LOC_MAX;
    m_locType2 = LOC_MAX;
    m_locTypeTmp1 = LOC_MAX;
    m_locTypeTmp2 = LOC_MAX;

    m_nDeleteIndex = 0;
    m_nReadIndex = 0;
    memset(&m_WriteRecord, 0, sizeof(StSmsRecord));
    
    m_ME_SmsCnt = 0;
    m_SM_SmsCnt = 0;

    m_pSmsData = ((CHSDPAApp *)AfxGetApp())->GetSmsData();
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_pMainWnd = (CHSDPADlg *)AfxGetMainWnd();

    ASSERT(m_pMainWnd && m_pSmsData && m_pPbData);

    m_TransType = SMS_TYPE_ALL;
    m_nBkTag = 1;
	ResetSmsTranConcatenateData();
	bSetHmsgp = false;
	m_Event = ::CreateEvent(NULL,FALSE,FALSE,NULL);
}


void CSmsTransferDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsTransferDlg)
    DDX_Control(pDX, IDC_COMBO_LEFT_TYPE, m_TypeCombo[CTRL_DIRECTION_LEFT]);
    DDX_Control(pDX, IDC_COMBO_RIGHT_TYPE, m_TypeCombo[CTRL_DIRECTION_RIGHT]);
    DDX_Control(pDX, IDC_COMBO_LEFT_LOC, m_LocCombo[CTRL_DIRECTION_LEFT]);
    DDX_Control(pDX, IDC_COMBO_RIGHT_LOC, m_LocCombo[CTRL_DIRECTION_RIGHT]);
    DDX_Control(pDX, IDC_LIST_LEFT, m_SmsList[CTRL_DIRECTION_LEFT]);
    DDX_Control(pDX, IDC_LIST_RIGHT, m_SmsList[CTRL_DIRECTION_RIGHT]);
    DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	DDX_Control(pDX,IDC_BUTTON_LR,TransUpToDownBtn);
    DDX_Control(pDX,IDC_BUTTON_RL,TransDownToUpBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsTransferDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsTransferDlg)
    ON_BN_CLICKED(IDC_BUTTON_LR, OnButtonLr)
    ON_BN_CLICKED(IDC_BUTTON_RL, OnButtonRl)
    ON_CBN_SELCHANGE(IDC_COMBO_LEFT_LOC, OnSelchangeComboLeftLoc)
    ON_CBN_SELCHANGE(IDC_COMBO_RIGHT_LOC, OnSelchangeComboRightLoc)
    ON_CBN_SELCHANGE(IDC_COMBO_LEFT_TYPE, OnSelchangeComboLeftType)
    ON_CBN_SELCHANGE(IDC_COMBO_RIGHT_TYPE, OnSelchangeComboRightType)
    ON_WM_TIMER()
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SMS_TRANSFER_PROC, OnSmsTransferProc)    
    ON_MESSAGE(WM_CREATE_PROGRESS, OnCreateProgress)        
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsTransferDlg message handlers

void CSmsTransferDlg::OnButtonLr() 
{
    // TODO: Add your control notification handler code here

    MinMaxChar = LMS_Flexi_MinMaxChar[rand()%10];
    StartTransfer(TRUE);
}

void CSmsTransferDlg::OnButtonRl() 
{
    // TODO: Add your control notification handler code here
	MinMaxChar = LMS_Flexi_MinMaxChar[rand()%10];
    StartTransfer(FALSE);
}

BOOL CSmsTransferDlg::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);
	//added by lijiali
	#if 0  //wj test
	if(!SndAtSmsQHMSGP())
	{
		bSetHmsgp =false;
	}
	#endif
	if(!bSetHmsgp)
	{
		OnCancel();
		return FALSE;
	}	
	TransUpToDownBtn.SetIcon(IDI_ICON_DOWNLOAD,IDI_ICON_DOWNLOAD,IDI_ICON_DOWNLOAD);
	TransUpToDownBtn.SetAlign(CButtonST::ST_ALIGN_HORIZ);
    TransUpToDownBtn.DrawBorder(TRUE);

	TransDownToUpBtn.SetIcon(IDI_ICON_UPLOAD,IDI_ICON_UPLOAD,IDI_ICON_UPLOAD);
	TransDownToUpBtn.SetAlign(CButtonST::ST_ALIGN_HORIZ);
    TransDownToUpBtn.DrawBorder(TRUE);
    
    // TODO: Add extra initialization here
    if(m_ilLsts.Create(IDB_SMS_STATE, 16, 1, 255))
    {
        m_SmsList[CTRL_DIRECTION_LEFT].SetImageList(&m_ilLsts, LVSIL_SMALL);
        m_SmsList[CTRL_DIRECTION_RIGHT].SetImageList(&m_ilLsts, LVSIL_SMALL);        
    }
    
    //init two ListCtrls
    CString strColTitle;
    strColTitle.LoadString(IDS_NAME);
    m_SmsList[CTRL_DIRECTION_LEFT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_NAME, strColTitle,LVCFMT_CENTER,80);
    m_SmsList[CTRL_DIRECTION_RIGHT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_NAME, strColTitle,LVCFMT_CENTER,80);
   // strColTitle.LoadString(IDS_NUMBER);
   // m_SmsList[CTRL_DIRECTION_LEFT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_NUMBER, strColTitle,LVCFMT_CENTER,100);
   // m_SmsList[CTRL_DIRECTION_RIGHT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_NUMBER, strColTitle,LVCFMT_CENTER,100);
    strColTitle.LoadString(IDS_SMS_DATETIME);
    m_SmsList[CTRL_DIRECTION_LEFT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_DATETIME, strColTitle,LVCFMT_CENTER,120);
    m_SmsList[CTRL_DIRECTION_RIGHT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_DATETIME, strColTitle,LVCFMT_CENTER,120);
    strColTitle.LoadString(IDS_SMS_CONTENT);
    m_SmsList[CTRL_DIRECTION_LEFT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_CONTENT, strColTitle,LVCFMT_LEFT,100);
    m_SmsList[CTRL_DIRECTION_RIGHT].InsertColumn(SMS_LISTCTRL_HEADERINDEX_CONTENT, strColTitle,LVCFMT_LEFT,100);

    m_SmsList[CTRL_DIRECTION_LEFT].SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_SmsList[CTRL_DIRECTION_RIGHT].SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_SmsList[CTRL_DIRECTION_LEFT].m_headerCtrl.DrawHeader();
    m_SmsList[CTRL_DIRECTION_RIGHT].m_headerCtrl.DrawHeader();

    //init two type ComboBoxs
    CString str;
    str.LoadString(IDS_SMS_INBOX);
    m_TypeCombo[CTRL_DIRECTION_LEFT].AddString(str);    
    m_TypeCombo[CTRL_DIRECTION_RIGHT].AddString(str);
//     str.LoadString(IDS_SMS_OUTBOX);
//     m_TypeCombo[CTRL_DIRECTION_LEFT].AddString(str);    
  //  m_TypeCombo[CTRL_DIRECTION_RIGHT].AddString(str);
    
    

    m_TypeCombo[CTRL_DIRECTION_LEFT].SetCurSel(0);
    m_TypeCombo[CTRL_DIRECTION_RIGHT].SetCurSel(0);
    
    //init two location ComboBoxs
    str.LoadString(IDS_PC);
    wcsncpy(m_szLoc[LOC_PC], str, 9);
    m_LocCombo[CTRL_DIRECTION_LEFT].AddString(str);
    str.LoadString(IDS_ME);
    wcsncpy(m_szLoc[LOC_ME], str, 9);
    m_LocCombo[CTRL_DIRECTION_RIGHT].AddString(str);
#ifndef FEATURE_VERSION_NOSIM
    str.LoadString(IDS_USIM);
    wcsncpy(m_szLoc[LOC_UIM], str, 9);
    m_LocCombo[CTRL_DIRECTION_RIGHT].AddString(str);
    m_LocCombo[CTRL_DIRECTION_LEFT].AddString(str);
#endif
    
    m_LocCombo[CTRL_DIRECTION_LEFT].SetCurSel(0);
    m_LocCombo[CTRL_DIRECTION_RIGHT].SetCurSel(0);

    UpdateListCtrl(CTRL_DIRECTION_LEFT);
	UpdateListCtrl(CTRL_DIRECTION_RIGHT);
    
    return TRUE;
}

void CSmsTransferDlg::OnSelchangeComboLeftLoc() 
{
    // TODO: Add your control notification handler code here
    UpdateData();
    UpdateComboState(CTRL_DIRECTION_RIGHT);
	UpdateComboType(CTRL_DIRECTION_LEFT);
	UpdateComboType(CTRL_DIRECTION_RIGHT);
    UpdateListCtrl(CTRL_DIRECTION_LEFT);
}

void CSmsTransferDlg::OnSelchangeComboRightLoc() 
{
    // TODO: Add your control notification handler code here
    UpdateData();
    UpdateComboState(CTRL_DIRECTION_LEFT);
	UpdateComboType(CTRL_DIRECTION_RIGHT);
	UpdateComboType(CTRL_DIRECTION_LEFT);
    UpdateListCtrl(CTRL_DIRECTION_RIGHT);
	UpdateListCtrl(CTRL_DIRECTION_LEFT);
}

void CSmsTransferDlg::OnSelchangeComboLeftType() 
{
    // TODO: Add your control notification handler code here
    UpdateData();
    UpdateButtonState();
    UpdateListCtrl(CTRL_DIRECTION_LEFT);
}

void CSmsTransferDlg::OnSelchangeComboRightType() 
{
    // TODO: Add your control notification handler code here
    UpdateData();
    UpdateButtonState();
    UpdateListCtrl(CTRL_DIRECTION_RIGHT);
}

void CSmsTransferDlg::UpdateButtonState()
{
    if(m_TypeCombo[CTRL_DIRECTION_LEFT].GetCurSel()
        == m_TypeCombo[CTRL_DIRECTION_RIGHT].GetCurSel())
    {
        GetDlgItem(IDC_RADIO1)->EnableWindow();
        GetDlgItem(IDC_RADIO2)->EnableWindow();    
        GetDlgItem(IDC_BUTTON_LR)->EnableWindow();
        GetDlgItem(IDC_BUTTON_RL)->EnableWindow();        
    }
    else
    {
        GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_LR)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_RL)->EnableWindow(FALSE);
    }
}

void CSmsTransferDlg::UpdateComboType(EnCtrlDirection direction)
{
	CString strME;
	CString strUIM;
	CString strInbox;
//	CString strOutbox;
	CString strnowselect;
	int indexcombo;
	strInbox.LoadString(IDS_SMS_INBOX);
//	strOutbox.LoadString(IDS_SMS_OUTBOX);
	strME.LoadString(IDS_ME);
	strUIM.LoadString(IDS_USIM);
	m_TypeCombo[direction].ResetContent();
//	m_TypeCombo[CTRL_DIRECTION_LEFT].ResetContent();

	indexcombo = m_LocCombo[direction].GetCurSel();
    m_LocCombo[direction].GetLBText(indexcombo,strnowselect);
	if ((strnowselect == strME) || (strnowselect == strUIM))
	{
		m_TypeCombo[direction].AddString(strInbox);
		m_TypeCombo[direction].SetCurSel(0);
	}
	else 
	{
		m_TypeCombo[direction].AddString(strInbox);
	//	m_TypeCombo[direction].AddString(strOutbox);
		m_TypeCombo[direction].SetCurSel(0);
	}

}

void CSmsTransferDlg::UpdateComboState(EnCtrlDirection direction)
{
    ASSERT(direction == CTRL_DIRECTION_LEFT || direction == CTRL_DIRECTION_RIGHT);

    int  index = 0;
    TCHAR szCurLoc[CTRL_DIRECTION_MAX][10];
    memset(szCurLoc, 0, sizeof(szCurLoc));

    m_LocCombo[CTRL_DIRECTION_LEFT].GetLBText(m_LocCombo[CTRL_DIRECTION_LEFT].GetCurSel(), szCurLoc[CTRL_DIRECTION_LEFT]);
    m_LocCombo[CTRL_DIRECTION_RIGHT].GetLBText(m_LocCombo[CTRL_DIRECTION_RIGHT].GetCurSel(), szCurLoc[CTRL_DIRECTION_RIGHT]);

    if(CB_ERR != (index = m_LocCombo[direction].FindStringExact(0, szCurLoc[1-direction])))
    {
        m_LocCombo[direction].DeleteString(index);
        for(int i = 0; i < LOC_MAX; i++)
        {
            if(wcscmp(m_szLoc[i], szCurLoc[CTRL_DIRECTION_LEFT]) && wcscmp(m_szLoc[i], szCurLoc[CTRL_DIRECTION_RIGHT]))
            {
                m_LocCombo[direction].AddString(m_szLoc[i]);
                break;
            }
        }
    }
}

void CSmsTransferDlg::UpdateListCtrl(EnCtrlDirection direction)
{
    ASSERT(direction == CTRL_DIRECTION_LEFT || direction == CTRL_DIRECTION_RIGHT);

    ::SetEvent(g_BGPassEvt); 

    ClearAllSelectMask((EnCtrlDirection)(1-direction));

    WORD  i, cnt;
    WORD  nSmsNum;
    CString strUnknown;
    EnSmsType SmsType;
    EnLocType LocType;
    StSmsRecord SmsRecord;
    TCHAR  szCurLoc[10] = {0};
    TCHAR  szName[PB_NAME_MAX + 2] = {0};

	int SMSNumLength;
	int SMSNumLength1;
	int SMSNumLength2;
  
    memset(&SmsRecord, 0x00, sizeof(StSmsRecord)); 
	SmsRecord.m_NoATRspCDS = TRUE;
    
    SmsType = (EnSmsType)m_TypeCombo[direction].GetCurSel();
    ASSERT(SmsType >= SMS_TYPE_INBOX && SmsType < SMS_TYPE_ALL);
                
    m_LocCombo[direction].GetLBText(m_LocCombo[direction].GetCurSel(), szCurLoc);
    
    if(wcscmp(szCurLoc, m_szLoc[LOC_PC]) == 0)
    {
        LocType = LOC_PC;
        nSmsNum = m_pSmsData->GetSmsNum(SmsType);
        ASSERT(nSmsNum <= SMS_RECORD_MAX);
    }
    else if(wcscmp(szCurLoc, m_szLoc[LOC_ME]) == 0)
    {
        LocType = LOC_ME;
        nSmsNum = g_ME_SmsNum;
    }
    else if(wcscmp(szCurLoc, m_szLoc[LOC_UIM]) == 0)
    {
        LocType = LOC_UIM;
        nSmsNum = g_SM_SmsNum;
    }
    else
    {
        ASSERT(FALSE);
    }
    
    strUnknown.LoadString(IDS_UNKNOWN);

    m_SmsList[direction].SetRedraw(FALSE);

    m_SmsList[direction].DeleteAllItems();

    for(i = 0, cnt = 0; i < nSmsNum; i++)
    {    
        switch(LocType)
        {
        case LOC_PC:
            SmsRecord = m_pSmsData->ReadSmsRecord(SmsType, i);
            break;
        case LOC_ME:
            SmsRecord = g_ME_SmsRecord[i].record;
            break;
        case LOC_UIM:
            SmsRecord = g_SM_SmsRecord[i].record;
            break;
        }

    
        if(LocType != LOC_PC)
        {
            if(SmsType == SMS_TYPE_INBOX)
            {
                if(SmsRecord.state != SMS_STATE_MT_NOT_READ
                    && SmsRecord.state != SMS_STATE_MT_READ)
                    continue;
            }
            else if(SmsType == SMS_TYPE_OUTBOX)
            {
                if(SmsRecord.state != SMS_STATE_MO_SENT
                    && SmsRecord.state != SMS_STATE_MO_NOT_SENT)
                    continue;
            }
            else if(SmsType == SMS_TYPE_DRAFT)
            {
                //if(SmsRecord.state != SMS_STATE_MO_NOT_SENT)
                    continue;
            }
        }

   
        
        memset(szName, 0, sizeof(szName));

        m_SmsList[direction].InsertItem(cnt, _T(""), (int)(SmsRecord.state + 4 * LocType));
        m_SmsList[direction].SetItemData(cnt, i);

		USES_CONVERSION;
        UCHAR chr;
		SMSNumLength2 = wcslen((TCHAR*)SmsRecord.szNumber);
		SMSNumLength1 = strlen((char*)SmsRecord.szNumber);
		if (SMSNumLength1 == SMSNumLength2)
			SMSNumLength = SMSNumLength1;
		else
			SMSNumLength = SMSNumLength2;

        if(SmsRecord.szNumber[0] != '\0')
        {
			chr = *(SmsRecord.szNumber + 1);
			if(chr)
			{
				SMSNumLength = strlen((char*)SmsRecord.szNumber);
            if(m_pPbData->SearchNameByNumber(CString(SmsRecord.szNumber), szName))
                    m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME, szName);
            else
					m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME,A2W(SmsRecord.szNumber));
			}
			else
			{
				SMSNumLength = wcslen((TCHAR*)SmsRecord.szNumber);
				if(m_pPbData->SearchNameByNumber((TCHAR*)SmsRecord.szNumber, szName))
                    m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME, szName);
				else
					m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME,(LPCTSTR)SmsRecord.szNumber);
			}
        }
        else
        {
            m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_NAME, (LPCWSTR)strUnknown);            
			
        }
        
        m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_DATETIME, SmsRecord.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT));

        //Modified by Zhou Bin 2008.12.30
		//TCHAR szHead[30];
		char szHead[30];
        memset(szHead, 0x00, sizeof(szHead));
        //TCHAR szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX + 10];
		char szDspContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX + 10];
        memset(szDspContent, 0x00, sizeof(szDspContent));

		char szContent[SMS_CONCAT_ASCII_MAX*SMS_CONCAT_SEGMENT_MAX*2 + 2];
		
		memset(szContent, 0x0000, sizeof(szContent));
		

		strncat(szContent,(const char*)SmsRecord.szContent,sizeof(szContent));
		char *p;
		p = szContent;
		for (int i=0;i<sizeof(szContent);i++)
		{
			if (*p == '\r' || *p == '\n')
			{
				*p = ' ';
			}
			p++;
		}

        if(SmsRecord.flag & SMS_RECORD_FLAG_CONCATENATE_SEGE)
        {
            if(DspConcatenateSmsPara(szHead, SmsRecord.nRefCnt, SmsRecord.nSeqCnt, SmsRecord.nTotalCnt))
                strcpy(szDspContent, szHead);
				//wcscpy(szDspContent, szHead);
        }
               
		 if(SmsRecord.szNumber[0] == '\0' || chr)
		 {
			 TCHAR *WCharDspContent=A2W(szContent); 

			 m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, WCharDspContent);
		 }
		 else
		 {
			 if (SMSNumLength == 1 && LocType == LOC_PC && SmsType != SMS_TYPE_INBOX)
			 {
				 TCHAR *WCharDspContent=A2W(szContent); 
				 //m_lstSms.SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, szDspContent);
				 m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, WCharDspContent);
			 }
			else
			{
				//modify by lijl 2009.4.13
				m_SmsList[direction].SetItemText(cnt, SMS_LISTCTRL_HEADERINDEX_CONTENT, (CString)szContent);
			}
		 }
        cnt++;
    }

    m_SmsList[direction].SetRedraw(TRUE);
    m_SmsList[direction].Invalidate();
    m_SmsList[direction].UpdateWindow();
}

BOOL CSmsTransferDlg::SndAtSmsQCPMS(EnLocType locType1, EnLocType locType2)
{
   // ASSERT(locType1 == LOC_ME || locType1 == LOC_UIM);
   // ASSERT(locType2 == LOC_ME || locType2 == LOC_UIM || locType2 == LOC_MAX);

    char szAtBuf[50] = {0};
    
    if(locType2 == LOC_MAX)
        sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCPMS], gcstrLoc[locType1]);
    else    
        sprintf(szAtBuf, "%s\"%s\",\"%s\"\r", gcstrAtSms[AT_SMS_QCPMS], gcstrLoc[locType1], gcstrLoc[locType2]);
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        m_locTypeTmp1 = locType1;
        m_locTypeTmp2 = locType2;
        SetTimer(IDT_QCPMS_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCPMS, this);
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::SndAtSmsQCMGR(USHORT index)
{
    if(m_locType1 == LOC_ME)
        ASSERT(index < g_ME_SmsMax);
    else
        ASSERT(index < g_SM_SmsMax);
    
    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGR], index);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {        
        m_nReadIndex = index;
        SetTimer(IDT_QCMGR_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGR, this);
        return TRUE;
    }
    return FALSE;
}

//写号码：pRecord != NULL
//写内容：pRecord == NULL
BOOL CSmsTransferDlg::SndAtSmsQCMGW(StSmsRecord *pRecord)
{
    switch(m_locType2) {
    case LOC_ME:
        if(g_ME_SmsNum >= g_ME_SmsMax)
            return FALSE;
        break;
    case LOC_UIM:
        if(g_SM_SmsNum >= g_SM_SmsMax)
            return FALSE;
        break;
    default:
        ASSERT(FALSE);
    }

    TCHAR szAtBuf[1000] = {0};
	char szAtAscBuf[1600] = {0};
    CString strDateTime;
	int buffsize;

    memset(szAtBuf, 0x00, sizeof(szAtBuf));

    if(pRecord != NULL)
    {
        const char *pNumType;
        if(pRecord->szNumber[0] == '+')
            pNumType = gcstrNumType[0];
        else
            pNumType = gcstrNumType[1];
		//////////////////////////////////////////////////////////////////////////

      
        strDateTime = pRecord->timestamp.Format(_T("%y/%m/%d,%H:%M:%S+00"));
		//Modified by Zhou Bin 2008.12.30
// 	         char *szNumTemp = new char[_tcslen(pRecord->szNumber) + 1];
//      	memset(szNumTemp, 0, _tcslen(pRecord->szNumber) + 1);
//     	WCharToChar(pRecord->szNumber, szNumTemp);

		USES_CONVERSION;
		// char *szTimeTemp = new char[_tcslen(strDateTime) + 1];
		 char *szTimeTemp = W2A(strDateTime);
		
//      	memset(szTimeTemp, 0, _tcslen(strDateTime) + 1);
//     	WCharToChar(strDateTime, szTimeTemp);

//         sprintf(szAtBuf, "%s\"%s\",%s,\"%s\",\"%s\"\r", 
//                 gcstrAtSms[AT_SMS_QCMGW], 
//                 szNumTemp,
//                 pNumType,
//                 GetSmsStrFromState(pRecord->state),
//                 szTimeTemp);//"06/05/23,09:19:55+00" -gsm/wcdma
//                 delete []szNumTemp;
//                 delete []szTimeTemp;	
		 
		 if (m_SrcLoc == LOC_UIM || m_SrcLoc ==LOC_ME || m_SrcLoc ==LOC_PC)
		 {
			 chr = *(pRecord->szNumber + 1);
			 if (chr == 0)
			 {
				 DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(TCHAR*)pRecord->szNumber,-1,NULL,0,NULL,FALSE);
				 char *psText;
				 psText = new char[dwNum+1];
				 if(!psText)
				 {
					 delete []psText;
				 }
				 WideCharToMultiByte (CP_OEMCP,NULL,(TCHAR*)pRecord->szNumber,-1,psText,dwNum,NULL,FALSE);

				 sprintf(szAtAscBuf, "%s\"%s\",%s,\"%s\",\"%s\"\r", 
					 gcstrAtSms[AT_SMS_QCMGW], 
					 psText,
					 pNumType,
					 GetSmsStrFromState(pRecord->state),
					szTimeTemp);//"06/05/23,09:19:55+00" -gsm/wcdma
				  delete []psText;
			 }
			else
			{
			  sprintf(szAtAscBuf, "%s\"%s\",%s,\"%s\",\"%s\"\r", 
			  gcstrAtSms[AT_SMS_QCMGW], 
			  pRecord->szNumber,
			  pNumType,
			  GetSmsStrFromState(pRecord->state),
              szTimeTemp);//"06/05/23,09:19:55+00" -gsm/wcdma
			}
		 }
         else
		 {
		 
			sprintf(szAtAscBuf, "%s\"%s\",%s,\"%s\",\"%s\"\r", 
                gcstrAtSms[AT_SMS_QCMGW], 
			   pRecord->szNumber,
                pNumType,
                GetSmsStrFromState(pRecord->state),
                szTimeTemp);//"06/05/23,09:19:55+00" -gsm/wcdma
		}
	  	buffsize = strlen(szAtAscBuf);
    }
    else//写内容
    {
		char szHead[30];
        memset(szHead, 0x00, sizeof(szHead));
		CString strUc;

		if(gSmsTranIsConcatenate)//长短信
		{
#if 0
            m_WriteRecord.nRefCnt = gSmsTranRefCnt;
			if(SetConcatenateSmsParaA(szHead, m_WriteRecord.nRefCnt, gSmsTranCurSege+1, gSmsTranTotalSege, MinMaxChar))
			{
				TCHAR *unicodStr = new TCHAR[strlen(szHead) * 2 + 1];
				ASCToUCS2((char*)szHead,(TCHAR *)unicodStr );
			    wcscpy(szAtBuf, unicodStr);
				      	       delete[] unicodStr;
			}
#endif
			if (m_SrcLoc == LOC_UIM || m_SrcLoc ==LOC_ME /*|| m_SrcLoc ==LOC_PC*/)
			{
				strUc = BTToUCS2((CString)gszSmsTranSege[gSmsTranCurSege]);

			}
			else
			{
			  
				if (chr == 0)
				{
					
					  strUc = BTToUCS2((TCHAR*)gszSmsTranSege[gSmsTranCurSege]);
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////liub_modify 长短消息拆分后的转换
				    DWORD dwNum1 = WideCharToMultiByte(CP_OEMCP,NULL,(TCHAR*)gszSmsTranSege[gSmsTranCurSege],-1,NULL,0,NULL,FALSE);
					char *psText;
					psText = new char[dwNum1+1];
					if(!psText)
					{
						delete []psText;
					}
			        WideCharToMultiByte (CP_OEMCP,NULL,(TCHAR*)gszSmsTranSege[gSmsTranCurSege],-1,psText,dwNum1,NULL,FALSE);

			        strUc = GBToUCS2((const char *)psText);
					delete []psText;
				}
		   }
	
			
		}
		else
        {
            if(IsConcatenateSms(&m_WriteRecord)
                && SetConcatenateSmsParaA(szHead, m_WriteRecord.nRefCnt, m_WriteRecord.nSeqCnt,
                m_WriteRecord.nTotalCnt, MinMaxChar))
            {	USES_CONVERSION;
				TCHAR *unicodStr = new TCHAR[strlen(szHead) * 2 + 1];
				ASCToUCS2((char*)szHead,(TCHAR *)unicodStr );
				wcscpy(szAtBuf, unicodStr);
			    delete[] unicodStr;
      
            } 
			if (m_SrcLoc == LOC_UIM || m_SrcLoc ==LOC_ME /*|| m_SrcLoc ==LOC_PC*/)
			{
				strUc = BTToUCS2((CString)m_WriteRecord.szContent);
			}
           else
		   {
			   if (chr == 0)
			   {
				   strUc = BTToUCS2((CString)m_WriteRecord.szContent);
				   
			   }
			   else
				   strUc = GBToUCS2((const char *)m_WriteRecord.szContent);
			   
		   }
        }
		int len= WCharToUnicode(strUc, szAtAscBuf);
		szAtAscBuf[len] = gccCtrl_Z;
		buffsize=len+1;

	}
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, buffsize, FALSE))
    {
        if(pRecord != NULL)
		{
            m_WriteRecord = *pRecord;
		}
        SetTimer(IDT_QCMGW_TIMEOUT, 50000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGW, this);
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::SndAtSmsQCMGD(USHORT index)
{
    USHORT nIndex;

    switch(m_locType1) {
    case LOC_ME:
        ASSERT(index < g_ME_SmsNum);
        nIndex = g_ME_SmsRecord[index].index;
        ASSERT(nIndex < g_ME_SmsMax);
        break;
    case LOC_UIM:
        ASSERT(index < g_SM_SmsNum);
        nIndex = g_SM_SmsRecord[index].index;
        ASSERT(nIndex < g_SM_SmsMax);
        break;
    default:
        ASSERT(false);
    }
    
    char szAtBuf[50] = {0};
    sprintf(szAtBuf, "%s%d\r", gcstrAtSms[AT_SMS_QCMGD], nIndex);
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        m_nDeleteIndex = index;
        SetTimer(IDT_QCMGD_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGD, this);
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::SndAtSmsQCSCA(const StSmsRecord *pRecord)
{
    ASSERT(pRecord);

    char szAtBuf[50] = {0};
	//Modified by Zhou Bin 2008.12.30
//          char *szTemp = new char[_tcslen(pRecord->szSCNumber) + 1];
//      	memset(szTemp, 0, _tcslen(pRecord->szSCNumber) + 1);
//     	WCharToChar(pRecord->szSCNumber, szTemp);
    //sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCSCA], szTemp);
	sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCSCA], pRecord->szSCNumber);
	//delete []szTemp;
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);

    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCSCA, (LPVOID)this);
        SetTimer(IDT_QCSCA_TIMEOUT, 60000, NULL);
        m_WriteRecord = *pRecord;
        return TRUE;
    }
    else
        return FALSE;
}

void CSmsTransferDlg::RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QCSCA_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCSCA, (LPARAM)TRUE);
    else
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCSCA, (LPARAM)TRUE);
}


//设置存储器
void CSmsTransferDlg::RspAtSmsQCPMS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QCPMS_TIMEOUT);

    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCPMS, (LPARAM)FALSE);
        return;
    }

    ASSERT(wStrNum == 2);
    
    if(strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
        || memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCPMS], strlen(gcstrResSms[AT_SMS_QCPMS])))
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCPMS, (LPARAM)FALSE);        
        return;
    }

    pDlg->m_locType1 = pDlg->m_locTypeTmp1;

    if(pDlg->m_locTypeTmp2 != LOC_MAX)
        pDlg->m_locType2 = pDlg->m_locTypeTmp2;

    int cnt = 0;
    char *ptr[4], *p;
    ptr[0] = p = (char *)&strArr[0][strlen(gcstrResSms[AT_SMS_QCPMS])];
    
    while(*p != '\0' && cnt < 4)
    {
        if(*p == ',')
        {
            *p++ = '\0';
            ptr[++cnt] = p;
        }
        else
        {
            p++;
        }
    }

    switch(pDlg->m_locType1) {
    case LOC_ME:
        g_ME_SmsNum = atoi((const char*)ptr[0]);
        g_ME_SmsMax = atoi((const char*)ptr[1]);
        break;
    case LOC_UIM:
        g_SM_SmsNum = atoi((const char*)ptr[0]);
        g_SM_SmsMax = atoi((const char*)ptr[1]);
        break;
    }

    if(pDlg->m_locTypeTmp2 != LOC_MAX)
    {
        switch(pDlg->m_locType2) {
        case LOC_ME:
            g_ME_SmsNum = atoi((const char*)ptr[2]);
            g_ME_SmsMax = atoi((const char*)ptr[3]);
            break;
        case LOC_UIM:
            g_SM_SmsNum = atoi((const char*)ptr[2]);
            g_SM_SmsMax = atoi((const char*)ptr[3]);
            break;
        }
    }
    pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCPMS, (LPARAM)TRUE);
}

void CSmsTransferDlg::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default

    switch(nIDEvent) {
    case IDT_QCPMS_TIMEOUT:
            KillTimer(IDT_QCPMS_TIMEOUT);
        break;
    case IDT_QCMGR_TIMEOUT:
            KillTimer(IDT_QCMGR_TIMEOUT);
        break;
    case IDT_QCMGW_TIMEOUT:
            KillTimer(IDT_QCMGW_TIMEOUT);
        break;
    case IDT_QCMGD_TIMEOUT:
            KillTimer(IDT_QCMGD_TIMEOUT);
        break;
    case IDT_QCSMP_TIMEOUT:
            KillTimer(IDT_QCSMP_TIMEOUT);
    case IDT_QCSCA_TIMEOUT:
            KillTimer(IDT_QCSCA_TIMEOUT);
    }

    ProgressClose();    
    UpdateListCtrl(CTRL_DIRECTION_LEFT);
    UpdateListCtrl(CTRL_DIRECTION_RIGHT);
    UpdateOwner();

    DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
    ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);

    if(nIDEvent == IDT_QCMGW_TIMEOUT)  
    {
        char szAtBuf[10] = {0};
        sprintf(szAtBuf, "%c", gccCtrl_Z);
        CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
        ASSERT(pComm);
        if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
        {
            RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespDummy, this);
        }
    }  
 
    AfxMessageBox(IDS_USB_PORT_TIMEOUT);
    ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
    CDialog::OnTimer(nIDEvent);
}

void CSmsTransferDlg::RspAtSmsQCMGR(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGR_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);        
        return;
    }

    //empty sms entry
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)TRUE);            
        return;
    }

    ASSERT(wStrNum == 3 || wStrNum == 2);

    if(strcmp((const char*)strArr[wStrNum-1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode])
        || memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCMGR], strlen(gcstrResSms[AT_SMS_QCMGR])))
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);                
        return;
    }

    StSmsRecord record;
    memset(&record, 0x00, sizeof(StSmsRecord));
	record.m_NoATRspCDS = TRUE;

    if(SmsAtCMGRRspProc(strArr, wStrNum, record))
    {

    switch(pDlg->m_locType1)
    {
    case LOC_ME:
        g_ME_SmsRecord[pDlg->m_ME_SmsCnt].index = pDlg->m_nReadIndex;
        g_ME_SmsRecord[pDlg->m_ME_SmsCnt].record = record;
        pDlg->m_ME_SmsCnt++;
        break;
    case LOC_UIM:
        g_SM_SmsRecord[pDlg->m_SM_SmsCnt].index = pDlg->m_nReadIndex;
        g_SM_SmsRecord[pDlg->m_SM_SmsCnt].record = record;
        pDlg->m_SM_SmsCnt++;
        break;
    }
    pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)TRUE);
    }
    else
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGR, (LPARAM)FALSE);
}

void CSmsTransferDlg::RspAtSmsQCMGW(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGW_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGW, (LPARAM)FALSE);    
        return;
    }
    
    ASSERT(wStrNum == 1 || wStrNum == 2);
    
    //准备写入短信内容
    if(wStrNum == 1 && strcmp((const char*)strArr[0], gcstrSmsPrompt) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGW, (LPARAM)TRUE);        
    }
    //写入完毕
    else if(wStrNum == 2 
            && strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
            && memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCMGW], strlen(gcstrResSms[AT_SMS_QCMGW])) == 0)
    {
        char *ptr = (char *)(strArr[0] + strlen(gcstrResSms[AT_SMS_QCMGW]));
        USHORT index = atoi(ptr);
        
		StSmsRecord temp;
		memset(&temp, 0x00, sizeof(StSmsRecord));
		temp.m_NoATRspCDS = TRUE;

		if(gSmsTranIsConcatenate && gSmsTranCurSege < gSmsTranTotalSege)
		{
			temp = pDlg->m_WriteRecord;
			temp.flag &= (BYTE)~SMS_RECORD_FLAG_CONCATENATE_FULL;
			temp.flag |= (BYTE)SMS_RECORD_FLAG_CONCATENATE_SEGE;
			temp.nSeqCnt = gSmsTranCurSege+1;
			temp.nTotalCnt = gSmsTranTotalSege;
			memset(temp.szContent, 0x00, sizeof(temp.szContent));
	//		if (!IsAlphabetUnicode((TCHAR*)gszSmsTranSege[gSmsTranCurSege]))
	//		{
				DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(TCHAR*)gszSmsTranSege[gSmsTranCurSege],-1,NULL,0,NULL,FALSE);
				char *psText;
				psText = new char[dwNum+1];
				if(!psText)
				{
					delete []psText;
				}
			    WideCharToMultiByte (CP_OEMCP,NULL,(TCHAR*)gszSmsTranSege[gSmsTranCurSege],-1,psText,dwNum,NULL,FALSE);
				wcsncpy((TCHAR*)temp.szContent, (const TCHAR*)psText, SMS_CONCAT_ASCII_MAX);
				delete []psText;
				
	//		}
	//		else
	//			wcsncpy((TCHAR*)temp.szContent, (const TCHAR*)gszSmsTranSege[gSmsTranCurSege], SMS_CONCAT_ASCII_MAX);
		}
        switch(pDlg->m_locType2)
        {
        case LOC_ME:
            ASSERT(index < g_ME_SmsMax);
            g_ME_SmsRecord[g_ME_SmsNum].index = index;
			if(gSmsTranIsConcatenate && gSmsTranCurSege < gSmsTranTotalSege)
				g_ME_SmsRecord[g_ME_SmsNum].record = temp;
			else
				g_ME_SmsRecord[g_ME_SmsNum].record = pDlg->m_WriteRecord;
            g_ME_SmsNum++;
            break;
        case LOC_UIM:
            ASSERT(index < g_SM_SmsMax);
            g_SM_SmsRecord[g_SM_SmsNum].index = index;
			if(gSmsTranIsConcatenate && gSmsTranCurSege < gSmsTranTotalSege)
				g_SM_SmsRecord[g_SM_SmsNum].record = temp;
			else
				g_SM_SmsRecord[g_SM_SmsNum].record = pDlg->m_WriteRecord;
            g_SM_SmsNum++;
            break;
        }
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGS, (LPARAM)TRUE);
    }
    else
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGW, (LPARAM)FALSE);
}

void CSmsTransferDlg::RspAtSmsQCMGD(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGD_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGD, (LPARAM)FALSE);    
        return;
    }

    USHORT cnt;
    switch(pDlg->m_locType1)
    {
    case LOC_ME:
        ASSERT(pDlg->m_nDeleteIndex < g_ME_SmsNum);
        for(cnt = pDlg->m_nDeleteIndex; cnt < g_ME_SmsNum - 1; cnt++)
            g_ME_SmsRecord[cnt] = g_ME_SmsRecord[cnt+1];
        memset(&g_ME_SmsRecord[cnt], 0, sizeof(StSmsCardRecord));
        g_ME_SmsNum--;
        break;
    case LOC_UIM:
        ASSERT(pDlg->m_nDeleteIndex < g_SM_SmsNum);
        for(cnt = pDlg->m_nDeleteIndex; cnt < g_SM_SmsNum - 1; cnt++)
            g_SM_SmsRecord[cnt] = g_SM_SmsRecord[cnt+1];
        memset(&g_SM_SmsRecord[cnt], 0, sizeof(StSmsCardRecord));
        g_SM_SmsNum--;
        break;
    }
    pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCMGD, (LPARAM)TRUE);                
}

void CSmsTransferDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CBaseDialog::OnShowWindow(bShow, nStatus);
	if(bShow)		
	{    
    if(g_bSmsSyncComm)
    {
        UpdateListCtrl(CTRL_DIRECTION_LEFT);
        UpdateListCtrl(CTRL_DIRECTION_RIGHT);
    }
    else
    {
        m_ProcType = PROC_TYPE_INIT;
        ProgressOpen();
        PostMessage(WM_SMS_TRANSFER_PROC,0,(LPARAM)TRUE);
		}
    }
}

//wParam 表示处理哪个命令的响应
//lParam 表示上次命令处理的结果
LRESULT CSmsTransferDlg::OnSmsTransferProc(WPARAM wParam, LPARAM lParam)
{
    StSmsRecord record;    
    EnAtSmsType AtType = (EnAtSmsType)wParam;
    BOOL bProcRst = (BOOL)lParam;
    BOOL bSndRes  = TRUE;
    CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
 
    memset(&record, 0x00, sizeof(StSmsRecord));
	record.m_NoATRspCDS = TRUE;
   
    switch(m_ProcType)
    {
    case PROC_TYPE_INIT:
        m_ProcType = PROC_TYPE_READ;
        nReadIndex = -1;
        m_ME_SmsCnt = 0;
        bSndRes = SndAtSmsQCPMS(LOC_ME, LOC_UIM);
        if(bSndRes)
        {
            ::ResetEvent(g_BGPassEvt); 
            m_pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
        }
        break;

    case PROC_TYPE_READ:
        if(bProcRst || !bProcRst) 
        {
//            if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
//                goto STOPTRANS;

            if(AtType == AT_SMS_QCPMS && m_locType1 == LOC_ME)
                ProgressSet(0, g_ME_SmsNum+g_SM_SmsNum, 1);

            nReadIndex++;
            if(m_locType1 == LOC_ME)
            {
                ProgressPos(m_ME_SmsCnt);
                if(m_ME_SmsCnt < g_ME_SmsNum && nReadIndex < g_ME_SmsMax)
                {
                    bSndRes = SndAtSmsQCMGR(nReadIndex);
                }
                else
                {
    //                AfxMessageBox("Read ME Over!");
                    nReadIndex = -1;
                    m_SM_SmsCnt = 0;
#ifndef FEATURE_VERSION_NOSIM
					bSndRes = SndAtSmsQCPMS(LOC_UIM, LOC_ME); 
#else
					
                    CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
                    g_bSmsSyncComm = TRUE;
                    if(m_pSmsData->DelallRecordbyFlag(SMS_TYPE_INBOX, SMS_RECORD_FLAG_COPY_UE))
                        pSmsDlg->UpdateSmsSortList(SMS_TYPE_INBOX, LOCFILTER_PC);
                    pSmsDlg->InitDspIndexArr(LOCFILTER_ME);
                   // pSmsDlg->InitDspIndexArr(LOCFILTER_UIM);
                    //pSmsDlg->InitDspIndexArr(LOCFILTER_ALL);
                    pSmsDlg->DispSms();
                    pMainWnd->UpdateUnreadSmsIcon(); 
                    pMainWnd->UpdateFullSmsIcon();

					ProgressClose();
#endif
                }
            }
            else
            {
                ProgressPos(m_ME_SmsCnt+m_SM_SmsCnt);
                if(m_SM_SmsCnt < g_SM_SmsNum && nReadIndex < g_SM_SmsMax)
                {
                    bSndRes = SndAtSmsQCMGR(nReadIndex);
                }
                else
                {
                    CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
                    g_bSmsSyncComm = TRUE;
                    if(m_pSmsData->DelallRecordbyFlag(SMS_TYPE_INBOX, SMS_RECORD_FLAG_COPY_UE))
                        pSmsDlg->UpdateSmsSortList(SMS_TYPE_INBOX, LOCFILTER_PC);
                    pSmsDlg->InitDspIndexArr(LOCFILTER_ME);
#ifndef FEATURE_VERSION_NOSIM
                    pSmsDlg->InitDspIndexArr(LOCFILTER_UIM);
#endif
                    //pSmsDlg->InitDspIndexArr(LOCFILTER_ALL);
                    pSmsDlg->DispSms();
                    pMainWnd->UpdateUnreadSmsIcon(); 
                    pMainWnd->UpdateFullSmsIcon();

                    ProgressClose();
//                    AfxMessageBox("Read SM Over!");                    
                    UpdateListCtrl(CTRL_DIRECTION_RIGHT);                
                }
            }
        }
        else
        {
            ProgressClose();            
            UpdateListCtrl(CTRL_DIRECTION_RIGHT);        
        }
        break;

    case PROC_TYPE_COPY:
        if(bProcRst)
        {
            if(AtType == AT_SMS_QCPMS)
                ProgressSet(0, m_nSelCount, 1);
            
            if(m_DesLoc == LOC_PC) //ME->PC SM->PC
            {
                ::SetEvent(g_BGPassEvt);
                do {
                    if(m_pSmsData->GetSmsNum(m_TransType) == SMS_RECORD_MAX)
                    {
                        ProgressClose();
                        UpdateListCtrl(m_DesDirection);
                        UpdateOwner();

						CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
						pSmsDlg->UpdateSortListWhenAddDelete(SMS_TYPE_INBOX, LOC_PC);
						UpTreeSMSCount();
                        AfxMessageBox(IDS_SMS_FULL, MB_OK | MB_ICONINFORMATION);                    
                        return 0;
                    }
                    else
                    {
                        ProgressStep();
                        Sleep(5);
                        if(m_SrcLoc == LOC_ME)
						{
                            StSmsRecord SmsRecordtemp;
							UCHAR chr;
							chr = *(g_ME_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record.szNumber + 1);
							CString strUc;
                            //GBToUCS2((const char *)m_WriteRecord.szContent);
                                          
                  			m_pSmsData->AddSmsRecord(m_TransType, g_ME_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record, FALSE);
						}
                        else
                            m_pSmsData->AddSmsRecord(m_TransType, g_SM_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record, FALSE);                        
                    }
                } while((m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED)) != -1);
                ProgressClose();
                UpdateListCtrl(m_DesDirection);
                UpdateOwner();
				CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
				pSmsDlg->UpdateSortListWhenAddDelete(SMS_TYPE_INBOX, LOC_PC);
				UpTreeSMSCount();
                return 0;
            }
            
            //PC->ME PC->SM ME->SM SM->ME
            if(/*AtType == AT_SMS_QCSMP*/AtType == AT_SMS_QHMSGL)//CSMP为WCDMA使用 modify by liub
            {
                  //bSndRes = SndAtSmsQCSCA(&m_WriteRecord);
				
                bSndRes = SndAtSmsQCMGW(&m_WriteRecord);
            }
            else if(AtType == AT_SMS_QCSCA)
            {
                bSndRes = SndAtSmsQCMGW(&m_WriteRecord);
            }
            else if(AtType == AT_SMS_QCMGW)
            {
                bSndRes = SndAtSmsQCMGW();
            }
            else
            {
                if(AtType == AT_SMS_QCMGS)
                {
					if(gSmsTranIsConcatenate)
					{
						if(++gSmsTranCurSege >= gSmsTranTotalSege)
							m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED);
					}
					else
						m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED);
				}
				
				if(gSmsTranIsConcatenate && gSmsTranCurSege < gSmsTranTotalSege)
				{
					bSndRes = SndAtSmsQCSMP(&m_WriteRecord);
				}
				else
				{
					if(AtType == AT_SMS_QCMGS)
					{
						ProgressStep();
						//Sleep(5);
					}             
                
                    if(m_TransItem != -1)
                    {
                        if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                            goto STOPTRANS;
                        
                        //Des room is full?
                        if((m_DesLoc == LOC_ME && g_ME_SmsNum == g_ME_SmsMax)
                            || (m_DesLoc == LOC_UIM && g_SM_SmsNum == g_SM_SmsMax))
                        {
							//GL
                           /* if(m_DesLoc == LOC_ME)
                                AfxMessageBox(IDS_SMS_ME_FULL, MB_OK | MB_ICONINFORMATION);
                            else
                                AfxMessageBox(IDS_SMS_USIM_FULL, MB_OK | MB_ICONINFORMATION);
                            ProgressClose();
							UpdateListCtrl(m_DesDirection);
							*/
                            ProgressClose();
                            UpdateOwner(); 
                            UpTreeSMSCount();
                            if(m_DesLoc == LOC_ME)
                                AfxMessageBox(IDS_SMS_ME_FULL, MB_OK | MB_ICONINFORMATION);
                            else
                                AfxMessageBox(IDS_SMS_USIM_FULL, MB_OK | MB_ICONINFORMATION);
                            UpdateListCtrl(m_DesDirection); 
                       
							// end

                            //UpdateOwner();
                            return 0;
                        }
                        switch(m_SrcLoc) {
                        case LOC_PC:
                            ASSERT(m_TransItem == m_SmsList[m_SrcDirection].GetItemData(m_TransItem));
                            record = m_pSmsData->ReadSmsRecord(m_TransType, m_TransItem);
                            break;
                        case LOC_ME:
                            record = g_ME_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record;
                            break;
                        case LOC_UIM:
                            record = g_SM_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record;
                            break;
                        }
   //////////////////////////////////////////////////////////////////////////
						DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, record.szContent, -1, NULL, 0);
						wchar_t *pwText;
						pwText = new wchar_t[dwNum];
						if(!pwText)
						{
							delete []pwText;
						}
						MultiByteToWideChar (CP_ACP, 0, record.szContent, -1, pwText, dwNum);
//////////////////////////////////////////////////////////////////////////
                        DivideSmsTranConcatenate((const TCHAR *)pwText);
						//DivideSmsTranConcatenate((const TCHAR *)record.szContent);
			
                        bSndRes = SndAtSmsQCSMP(&record);
						delete []pwText;
                    }
                    else
                    {
                        ProgressClose();
                        UpdateListCtrl(m_DesDirection); 
                        UpdateOwner();
                    }
                }
            }
        }
        else
        {
            ProgressClose();
            UpdateListCtrl(m_DesDirection);
            UpdateOwner();
        }
        break;

    case PROC_TYPE_MOVE:
        if(bProcRst)
        {
            if(AtType == AT_SMS_QCPMS)
                ProgressSet(0, m_nSelCount, 1);

            if(m_DesLoc == LOC_PC) //ME->PC SM->PC
            {
                if(AtType == AT_SMS_QCMGD)
                {
                    if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                        goto STOPTRANS;
                    
                    ProgressStep();
                    Sleep(5);
                    
                    for(int i = m_TransItem+1; i < m_SmsList[m_SrcDirection].GetItemCount(); i++)
                        m_SmsList[m_SrcDirection].SetItemData(i, m_SmsList[m_SrcDirection].GetItemData(i)-1);
                    
                    m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED);                
                }

                if(m_TransItem == -1)
                {
                    ProgressClose();
                    UpdateListCtrl(m_SrcDirection);
                    UpdateListCtrl(m_DesDirection);
                    UpdateOwner();

					CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
					pSmsDlg->m_cmbLocFilter.SetCurSel(LOCFILTER_PC);
	            	pSmsDlg->OnSelchangeCombolocfilterForOut();
					pSmsDlg->UpdateSortListWhenAddDelete(SMS_TYPE_INBOX, LOC_PC);
					UpTreeSMSCount();
                    return 0;
                }
                
                if(m_pSmsData->GetSmsNum(m_TransType) == SMS_RECORD_MAX)
                {
                    ProgressClose();
                    UpdateListCtrl(m_SrcDirection);
                    UpdateListCtrl(m_DesDirection);
                    UpdateOwner();

					CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
					pSmsDlg->UpdateSortListWhenAddDelete(SMS_TYPE_INBOX, LOC_PC);
					UpTreeSMSCount();
                    AfxMessageBox(IDS_SMS_FULL, MB_OK | MB_ICONINFORMATION);                        
                    return 0;
                }
                
                switch(m_SrcLoc) {
                case LOC_ME:
                    record = g_ME_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record;
                    break;
                case LOC_UIM:
                    record = g_SM_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record;
                    break;
                }    
                m_pSmsData->AddSmsRecord(m_TransType, record, FALSE);
                bSndRes = SndAtSmsQCMGD(m_SmsList[m_SrcDirection].GetItemData(m_TransItem));
            }
            else if(m_SrcLoc == LOC_PC) //PC->ME PC->SM
            {
                if(/*AtType == AT_SMS_QCSMP*/AtType == AT_SMS_QHMSGL)
                {
                   // bSndRes = SndAtSmsQCSCA(&m_WriteRecord);
					bSndRes = SndAtSmsQCMGW(&m_WriteRecord);
                }
                else if(AtType == AT_SMS_QCSCA)
                {
                    bSndRes = SndAtSmsQCMGW(&m_WriteRecord);
                }
                else if(AtType == AT_SMS_QCMGW)
                {
                    bSndRes = SndAtSmsQCMGW();
                }
                else
                {
                    if(AtType == AT_SMS_QCMGS)
                    {
                        if(gSmsTranIsConcatenate)
                        {
                            if(++gSmsTranCurSege >= gSmsTranTotalSege)
                            {
                                m_pSmsData->DeleteSmsRecord(m_TransType, m_SmsList[m_SrcDirection].GetItemData(m_TransItem));
                                for(int i = m_TransItem+1; i < m_SmsList[m_SrcDirection].GetItemCount(); i++)
                                    m_SmsList[m_SrcDirection].SetItemData(i, m_SmsList[m_SrcDirection].GetItemData(i)-1);
                                m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED);
                            }
                        }
                        else
                        {
                            m_pSmsData->DeleteSmsRecord(m_TransType, m_SmsList[m_SrcDirection].GetItemData(m_TransItem));
                            for(int i = m_TransItem+1; i < m_SmsList[m_SrcDirection].GetItemCount(); i++)
                                m_SmsList[m_SrcDirection].SetItemData(i, m_SmsList[m_SrcDirection].GetItemData(i)-1);
                            m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED);
                        }
                    }
                    
                    if(gSmsTranIsConcatenate && gSmsTranCurSege < gSmsTranTotalSege)
                    {
                        bSndRes = SndAtSmsQCSMP(&m_WriteRecord);
                    }
                    else
                    {
                        if(AtType == AT_SMS_QCMGS)
                        {
                            ProgressStep();
                        }
                        
                        if(m_TransItem != -1)
                        {
                            if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                                goto STOPTRANS;
                            
                            //Des room is full?
                            if((m_DesLoc == LOC_ME && g_ME_SmsNum == g_ME_SmsMax)
                                || (m_DesLoc == LOC_UIM && g_SM_SmsNum == g_SM_SmsMax))
                            {
								UpTreeSMSCount();
                                if(m_DesLoc == LOC_ME)
                                    AfxMessageBox(IDS_SMS_ME_FULL, MB_OK | MB_ICONINFORMATION);
                                else
                                    AfxMessageBox(IDS_SMS_USIM_FULL, MB_OK | MB_ICONINFORMATION); 
                                ProgressClose();
                                UpdateListCtrl(m_SrcDirection);
                                UpdateListCtrl(m_DesDirection);
                                UpdateOwner();

								
                                return 0;
                            }
                            
                            record = m_pSmsData->ReadSmsRecord(m_TransType, m_SmsList[m_SrcDirection].GetItemData(m_TransItem));

							DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, record.szContent, -1, NULL, 0);
							wchar_t *pwText;
							pwText = new wchar_t[dwNum];
							if(!pwText)
							{
								delete []pwText;
							}
							MultiByteToWideChar (CP_ACP, 0, record.szContent, -1, pwText, dwNum);
							DivideSmsTranConcatenate((const TCHAR *)pwText);
							delete []pwText;
							bSndRes = SndAtSmsQCSMP(&record);
                        }
                        else
                        {
                            ProgressClose();
                            UpdateListCtrl(m_SrcDirection);
                            UpdateListCtrl(m_DesDirection);
                            UpdateOwner();
                        }
                    }
                }
            }
            else //ME->SM SM->ME
            {
                if(/*AtType == AT_SMS_QCSMP*/AtType == AT_SMS_QHMSGL)
                {
                  //bSndRes = SndAtSmsQCSCA(&m_WriteRecord);
					
                bSndRes = SndAtSmsQCMGW(&m_WriteRecord);
                }
                else if(AtType == AT_SMS_QCSCA)
                {
                    bSndRes = SndAtSmsQCMGW(&m_WriteRecord);
                }
                else if(AtType == AT_SMS_QCMGW)
                {
                    bSndRes = SndAtSmsQCMGW();
                }
                else if(AtType == AT_SMS_QCMGS)
                {
                    bSndRes = SndAtSmsQCMGD(m_SmsList[m_SrcDirection].GetItemData(m_TransItem));
                }
                else
                {
                    if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                        goto STOPTRANS;
                    
                    if(AtType == AT_SMS_QCMGD)
                    {
                        ProgressStep();
                        Sleep(5);
                        
                        for(int i = m_TransItem+1; i < m_SmsList[m_SrcDirection].GetItemCount(); i++)
                            m_SmsList[m_SrcDirection].SetItemData(i, m_SmsList[m_SrcDirection].GetItemData(i)-1);
                            
                        m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(m_TransItem, LVNI_SELECTED);                
                    }

                    if(m_TransItem != -1)
                    {
                        //Des room is full?
                        if((m_DesLoc == LOC_ME && g_ME_SmsNum == g_ME_SmsMax)
                            || (m_DesLoc == LOC_UIM && g_SM_SmsNum == g_SM_SmsMax))
                        {
							UpTreeSMSCount();
                            if(m_DesLoc == LOC_ME)
                                AfxMessageBox(IDS_SMS_ME_FULL, MB_OK | MB_ICONINFORMATION);
                            else
                                AfxMessageBox(IDS_SMS_USIM_FULL, MB_OK | MB_ICONINFORMATION);
                            ProgressClose();
                            UpdateListCtrl(m_SrcDirection);
                            UpdateListCtrl(m_DesDirection);
                            UpdateOwner();

							
                            return 0;
                        }
                        
                        switch(m_SrcLoc) {
                        case LOC_ME:
                            record = g_ME_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record;
                            break;
                        case LOC_UIM:
                            record = g_SM_SmsRecord[m_SmsList[m_SrcDirection].GetItemData(m_TransItem)].record;
                            break;
                        }

                        bSndRes = SndAtSmsQCSMP(&record);
                    }
                    else
                    {
                        ProgressClose();
                        UpdateListCtrl(m_SrcDirection);
                        UpdateListCtrl(m_DesDirection);
                        UpdateOwner();
                    }
                }
            }                    
            break;
        }
        else
        {
            ProgressClose();
            UpdateListCtrl(m_SrcDirection);
            UpdateListCtrl(m_DesDirection);
            UpdateOwner();
        }
    }

    if(!bSndRes)
    {
STOPTRANS:  ProgressClose();
        UpdateListCtrl(CTRL_DIRECTION_LEFT);
        UpdateListCtrl(CTRL_DIRECTION_RIGHT);
        UpdateOwner();
    }

    UpTreeSMSCount();
    return 0;
}

LRESULT CSmsTransferDlg::OnCreateProgress(WPARAM wParam, LPARAM lParam)
{
    int nUpper = (int)wParam;
    int nStep = (int)lParam;

    if(nUpper == 0 || nStep == 0)
    {
        CProgressDlg dlg(this);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;
    }
    else
    {
        CProgressDlg dlg(this, nUpper, nStep);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;        
    }

    return 0;
}

BOOL CSmsTransferDlg::StartTransfer(BOOL bFromLToR)
{
    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
 
    if(CPIN_SIM_NONE_REQUIRED != pMainWnd->m_cHandlePin.m_nSimStat)
        return FALSE;
    
    if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
        return FALSE;

    if(!pComm->CommIsReady())
        return FALSE;

    UpdateData();

    if(bFromLToR)
    {
        m_SrcDirection = CTRL_DIRECTION_LEFT;
        m_DesDirection = CTRL_DIRECTION_RIGHT;
    }
    else
    {
        m_SrcDirection = CTRL_DIRECTION_RIGHT;
        m_DesDirection = CTRL_DIRECTION_LEFT;
    }

    if(m_SmsList[m_SrcDirection].GetItemCount() == 0)
        return FALSE;
    else if((m_TransItem = m_SmsList[m_SrcDirection].GetNextItem(-1, LVNI_SELECTED)) == -1)
    {
        AfxMessageBox(IDS_NOITEMSEL, MB_OK | MB_ICONINFORMATION);
        return FALSE;
    }

    m_TransType = (EnSmsType)m_TypeCombo[m_SrcDirection].GetCurSel();
    ASSERT(m_TransType >= SMS_TYPE_INBOX && m_TransType < SMS_TYPE_ALL);

    TCHAR  szSrcLoc[10] = {0};
    TCHAR  szDesLoc[10] = {0};
    m_LocCombo[m_SrcDirection].GetLBText(m_LocCombo[m_SrcDirection].GetCurSel(), szSrcLoc);
    m_LocCombo[m_DesDirection].GetLBText(m_LocCombo[m_DesDirection].GetCurSel(), szDesLoc);    
    m_SrcLoc = GetLocTypeByStr(szSrcLoc);
    m_DesLoc = GetLocTypeByStr(szDesLoc);

    //Des room is full?
    if((m_DesLoc == LOC_PC && m_pSmsData->GetSmsNum(m_TransType) == SMS_RECORD_MAX)
        || (m_DesLoc == LOC_ME && g_ME_SmsNum == g_ME_SmsMax)
        || (m_DesLoc == LOC_UIM && g_SM_SmsNum == g_SM_SmsMax))
    {
        if(m_DesLoc == LOC_PC)
            AfxMessageBox(IDS_SMS_FULL, MB_OK | MB_ICONINFORMATION);
        else if(m_DesLoc == LOC_ME)
            AfxMessageBox(IDS_SMS_ME_FULL, MB_OK | MB_ICONINFORMATION);
        else
            AfxMessageBox(IDS_SMS_USIM_FULL, MB_OK | MB_ICONINFORMATION);
        return FALSE;
    }

    m_nSelCount = m_SmsList[m_SrcDirection].GetSelectedCount();
    BOOL bSndRes = TRUE;
	m_str_sms_Prompt.LoadString(IDS_WAIT_TRANSFER_SMS);
    ProgressOpen(m_nSelCount, 1);
    
    //Copy
    if(m_nRadio == 0)
    {
        m_ProcType = PROC_TYPE_COPY;
        
        if(m_DesLoc == LOC_PC) //ME->PC SM->PC
        {
            bSndRes = SndAtSmsQCPMS(m_locType1, m_locType2);    
        }
        else //PC->ME PC->SM ME->SM SM->ME
        {
            bSndRes = SndAtSmsQCPMS(m_locType1, m_DesLoc);
        }
    }
    else //Move
    {
        m_ProcType = PROC_TYPE_MOVE;        
        if(m_DesLoc == LOC_PC) //ME->PC SM->PC
            bSndRes = SndAtSmsQCPMS(m_SrcLoc);
        else if(m_SrcLoc == LOC_PC) //PC->ME PC->SM
            bSndRes = SndAtSmsQCPMS(m_locType1, m_DesLoc);
        else //ME->SM SM->ME
            bSndRes = SndAtSmsQCPMS(m_SrcLoc, m_DesLoc);
    }

    if(!bSndRes)
    {
        ProgressClose();
        UpdateListCtrl(CTRL_DIRECTION_LEFT);
        UpdateListCtrl(CTRL_DIRECTION_RIGHT);
        UpdateOwner();
    }
    else
    {
        ::ResetEvent(g_BGPassEvt); 
        m_pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
    }

    return bSndRes;
}

EnLocType CSmsTransferDlg::GetLocTypeByStr(LPCTSTR str)
{
    BYTE LocType;

    for(LocType = LOC_PC; LocType < LOC_MAX; LocType++)
        if(wcscmp(str, m_szLoc[LocType]) == 0)
            break;
    return (EnLocType)LocType;
}

BOOL CSmsTransferDlg::ProgressSet(int nLower, int nUpper, int nStep)
{
    if(pDlg)
    {
        m_nLower = nLower;
        m_nUpper = nUpper;
        m_nStep  = nStep;
        pDlg->m_Progress.SetRange(m_nLower, m_nUpper);
        pDlg->m_Progress.SetStep(m_nStep);
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::ProgressStep()
{
    if(pDlg && pDlg->m_Progress.GetPos() < m_nUpper)
    {
        pDlg->m_Progress.StepIt();
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::ProgressClose()
{
    if(pDlg)
    {
        pDlg->PostMessage(WM_CLOSE);
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::ProgressOpen(int nUpper, int nStep)
{
    PostMessage(WM_CREATE_PROGRESS, (WPARAM)nUpper,(LPARAM)nStep);
    return TRUE;
}

BOOL CSmsTransferDlg::ProgressPos(int nPos)
{
    if(pDlg && pDlg->m_Progress.GetPos() < nPos && nPos <= m_nUpper)
    {
        pDlg->m_Progress.SetPos(nPos);
        return TRUE;
    }
    return FALSE;
}

void CSmsTransferDlg::UpdateOwner()
{
    ASSERT(m_TransType >= SMS_TYPE_INBOX && m_TransType <= SMS_TYPE_ALL);

    m_pMainWnd->CtlAllSmsRcvConcTimer(TRUE);
    ::SetEvent(g_BGPassEvt);

    CWnd *pWnd = GetOwner();
    if(pWnd)
    {
        if(m_ProcType == PROC_TYPE_COPY)
            pWnd->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)m_TransType, (LPARAM)m_DesLoc);
        else if(m_ProcType == PROC_TYPE_MOVE)
        {
            pWnd->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)m_TransType, (LPARAM)m_SrcLoc);
            pWnd->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)m_TransType, (LPARAM)m_DesLoc);
        }
    }

    
    if(m_TransType == SMS_TYPE_INBOX && (m_SrcLoc == LOC_PC || m_DesLoc == LOC_PC))
    {
        ((CHSDPADlg*)AfxGetMainWnd())->UpdateUnreadSmsIcon();
        ((CHSDPADlg*)AfxGetMainWnd())->UpdateFullSmsIcon();
    }
   
}

BOOL CSmsTransferDlg::SndAtSmsQCSMP(EnSmsQcsmpType type, BOOL bAscii, BOOL bConcatenate)
{
    ASSERT(type == SMS_QCSMP_TYPE_MT || type == SMS_QCSMP_TYPE_MO);

    char szAtBuf[30] = {0};
    BYTE para1, para2, para3, para4;
    para1 = para2 = para3 = para4 = 0;

    if(type == SMS_QCSMP_TYPE_MT)
    {
        if(bConcatenate)
           // para1 = 68;
	    	para1 = 113;
		
        else
            //para1 = 0;
			para1 = 17;
        //para2 = 0;
		para2 = 167;
        if(bAscii)
            //para3 = 0;
			para4 = 0;
        else
            //para3 = 8;
			para4 = 8;

    }
    else
    {
        if(bConcatenate)
            para1 = 113;
        else
            para1 = 17;
        para2 = 167;
        para3 = 0;
        if(bAscii)
            para4 = 0;
        else
            para4 = 8;
    }
    
    if(type == SMS_QCSMP_TYPE_MT)
        //sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QCSMP], para1,para2,para3);
		sprintf(szAtBuf, "%s%d,%d,%d,%d\r", gcstrAtSms[AT_SMS_QCSMP], para1,para2,para3,para4);
    else
        sprintf(szAtBuf, "%s%d,%d,%d,%d\r", gcstrAtSms[AT_SMS_QCSMP], para1,para2,para3,para4);
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        SetTimer(IDT_QCSMP_TIMEOUT, 30000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCSMP, this);
        return TRUE;
    }
    return FALSE;
}

BOOL CSmsTransferDlg::SndAtSmsQCSMP(StSmsRecord *pRecord)
{
    ASSERT(pRecord);

    EnSmsQcsmpType type;
    BOOL bAscii = TRUE;
    BOOL bConcatenate = FALSE;

    if(pRecord->state == SMS_STATE_MT_NOT_READ || pRecord->state == SMS_STATE_MT_READ)
        type = SMS_QCSMP_TYPE_MT;
    else
        type = SMS_QCSMP_TYPE_MO;

	if(gSmsTranIsConcatenate)
	{
		bAscii = gSmsTranIsAsciiCode;
		bConcatenate = TRUE;
	}
	else
	{
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, pRecord->szContent, -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		if(!pwText)
		{
			delete []pwText;
		}
		MultiByteToWideChar (CP_ACP, 0, pRecord->szContent, -1, pwText, dwNum);

       // if(IsAlphabetUnicode((const TCHAR *)pRecord->szContent))
		if(IsAlphabetUnicode((const TCHAR *)pwText))
            bAscii = TRUE;
        else
            bAscii = FALSE;
        delete []pwText;
        bConcatenate = IsConcatenateSms(pRecord);
	}

	BOOL res = SndAtSmsQHMSGL(bAscii, bConcatenate);

    if(res)
        m_WriteRecord = *pRecord;
    
    return res;
}

void CSmsTransferDlg::RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QCSMP_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCSMP, (LPARAM)FALSE);    
        return;
    }

    pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QCSMP, (LPARAM)TRUE);   
}

CSmsTransferDlg::~CSmsTransferDlg()
{
    ::SetEvent(g_BGPassEvt);
	CloseHandle(m_Event);
}

void CSmsTransferDlg::ClearAllSelectMask(EnCtrlDirection direction)
{
    ASSERT(direction == CTRL_DIRECTION_LEFT || direction == CTRL_DIRECTION_RIGHT);

    int nSelIndex = -1;

    while((nSelIndex = m_SmsList[direction].GetNextItem(nSelIndex, LVNI_SELECTED)) != -1)
    {
        m_SmsList[direction].SetItemState(nSelIndex, 0, LVIS_SELECTED);
    }
}

void CSmsTransferDlg::OnOK() 
{
	// TODO: Add extra cleanup here
	
	//CBaseDialog::OnOK();
}


//功能：发送AT$QHMSGL
//输入：无
//输出：TRUE/FALSE
BOOL CSmsTransferDlg::SndAtSmsQHMSGL(BOOL bAscii, BOOL bConcatenate)
{
	char szAtBuf[50] = {0};
    BYTE para1, para2;
    para1 = para2 = 0;
    
#ifdef FEATURE_SMS_CONCATENATE
    if(bConcatenate)
    {
        if(bAscii)
        {
            para2 = 2;
        }
        else
        {
            para2 = 4;
        }
    }
    else
    {
        if(bAscii)
        {
            para2 = 2;
        }
        else
        {
            para2 = 4;
        }
    }
#else
    if(bAscii)
    {
        para2 = 2;
    }
    else
    {
        para2 = 4;
    }
#endif
	
    sprintf(szAtBuf, "%s,%d\r", gcstrAtSms[AT_SMS_QHMSGL],para2);
	
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        SetTimer(IDT_QHMSGL_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQHMSGL, this);
        return TRUE;
    }
     return FALSE;
	
}

//功能：响应AT$QHMSGL
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsTransferDlg::RspAtSmsQHMSGL(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
    pDlg->KillTimer(IDT_QHMSGL_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {

	   pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QHMSGL, (LPARAM)TRUE);   
       return;
    }
	pDlg->PostMessage(WM_SMS_TRANSFER_PROC, (WPARAM)AT_SMS_QHMSGL, (LPARAM)FALSE);

}

void CSmsTransferDlg::UpTreeSMSCount()
{
	CHSDPADlg *pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
	pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    CSmsDlg* m_pSmsDlg = NULL;
	m_pSmsDlg = (CSmsDlg*)(pMainWnd->m_pSmsDlg);
	HTREEITEM hRoot = m_pSmsDlg->m_treeSms.GetRootItem();
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
}

//功能：发送AT$QHMSGP(设置短信其他参数<发送报告>,<短消息类型>,<消息头>,<优先级>)
//输入：无
//输出：TRUE/FALSE
BOOL CSmsTransferDlg::SndAtSmsQHMSGP()
{
	ResetEvent(m_Event);
    char szAtBuf[50] = {0};
	//CString strHMSGP;
	//strHMSGP.Format(_T("AT+HMSGP=,,0"));
	//wcscpy(szAtBuf,strHMSGP);
	sprintf(szAtBuf, "%s,,%d\r", gcstrAtSms[AT_SMS_QHMSGP],0);
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
	
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQHMSGP, (LPVOID)this);
        SetTimer(IDT_QHMSGP_TIMEOUT, 60000, NULL);
		::WaitForSingleObject(m_Event,INFINITE);
        return TRUE;
    }
    else
        return FALSE;
}

//功能：响应AT$QHMSGP
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsTransferDlg::RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CSmsTransferDlg* pDlg = (CSmsTransferDlg*)pWnd;
	pDlg->KillTimer(IDT_QHMSGP_TIMEOUT);
	
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
	{
		pDlg->bSetHmsgp = true;
	}
	else
	{
		pDlg->bSetHmsgp = false;
	}
	SetEvent(pDlg->m_Event);
	return;
}
