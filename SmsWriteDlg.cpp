
#include "stdafx.h"
#include "HSDPA.h"
#include "SmsWriteDlg.h"
#include "SelContactDlg.h"
#include "SmsSendDlg.h"
#include "HSDPADlg.h"
#include "resource.h"
#include "SmsTemplateDlg.h"

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
// CSmsWriteDlg dialog


CSmsWriteDlg::CSmsWriteDlg(CWnd* pParent,LPCTSTR lpszNumber, LPCTSTR lpszContent,
                           EnSmsWriteType writeType, LPCTSTR lpszSCNum)
    : CBaseDialog(CSmsWriteDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSmsWriteDlg)
    m_strNumber = _T("");
    m_strSmsDetails = _T("");
    m_strStatistic = _T("");
    m_strSegNum = _T("");
	//}}AFX_DATA_INIT
    
	thelastone = FALSE;
	theLMSSavenow = FALSE;
	bSaveSendSms = TRUE;
    SetOwner(pParent);
    
    if(lpszNumber)
        m_strNumber.Format(_T("%s"), lpszNumber);
    if(lpszContent)
        m_strSmsDetails.Format(_T("%s"), lpszContent);


#ifdef FEATURE_SMS_CONCATENATE
    if(IsAlphabetUnicode(m_strSmsDetails))
        m_strStatistic.Format(_T("(%d / %d)"), GetACSIICharNum(m_strSmsDetails), SMS_CONCAT_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX);
    else
        m_strStatistic.Format(_T("(%d / %d)"), GetUnicodeCharNum(m_strSmsDetails), SMS_CONCAT_GB_MAX * SMS_CONCAT_SEGMENT_MAX);
#else
    /*if(IsAlphabet(m_strSmsDetails))*/
	if(IsAlphabetUnicode(m_strSmsDetails))
        m_strStatistic.Format(_T("(%d / %d)"), GetACSIICharNum(m_strSmsDetails), SMS_CHAR_MAX);
    else
        m_strStatistic.Format(_T("(%d / %d)"), GetUnicodeCharNum(m_strSmsDetails), SMS_CHINESE_CHAR_MAX);
#endif


    UpdateStatistic(); 

    m_pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    m_pSmsData = ((CHSDPAApp *)AfxGetApp())->GetSmsData();

    ASSERT(m_pMainWnd && m_pSmsData);
    
    m_nCurNum = 0;
    m_nNumCount = 0;
    memset(m_szGroupNum, 0x00, sizeof(m_szGroupNum));
	memset(m_szGroupNumSendNum, 0x00, sizeof(m_szGroupNumSendNum));
    m_nBkTag = 1;

    pDlg = NULL;
    m_nLower = 0;
    m_nUpper = SMS_GROUPSEND_MAX;
    m_nStep = 1;

    m_strLastGroupNumber=_T("");

    memset(m_szSCNumber, 0x00, SMS_SC_NUM_MAX);
    m_WriteType = writeType;
    
    if(lpszSCNum && wcslen(lpszSCNum))
    {
        wcsncpy((TCHAR*)m_szSCNumber, lpszSCNum, SMS_SC_NUM_MAX);
    }
	
	else if(wcslen(gSmsCentreNum) > 0)
	{
		wcsncpy((TCHAR*)m_szSCNumber, gSmsCentreNum, SMS_SC_NUM_MAX);
	}
	#ifdef FEATURE_HAIER_SMS
	RegisterAtRespFunc(ATRESP_HCMGSS,AtRespHCMGSS,this);
	#endif
}

#ifdef FEATURE_HAIER_SMS
void CSmsWriteDlg::AtRespHCMGSS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	((CSmsWriteDlg *)pWnd)->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGS, (LPARAM)TRUE);	
}
#endif

void CSmsWriteDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsWriteDlg)
    DDX_Control(pDX, IDC_EDIT_SMSDETAILS, m_editSmsDetails);
    DDX_Control(pDX, IDC_EDIT_NUMBER, m_editNumber);
    DDX_Text(pDX, IDC_EDIT_NUMBER, m_strNumber);
    DDX_Text(pDX, IDC_EDIT_SMSDETAILS, m_strSmsDetails);
    DDX_Text(pDX, IDC_STATIC_STATISTIC, m_strStatistic);
    DDX_Text(pDX, IDC_STATIC_SEGE, m_strSegNum);    
	//}}AFX_DATA_MAP
    /*
    DDX_Control(pDX, IDC_STATIC_NUMBER, m_static_number);
    DDX_Control(pDX, IDC_STATIC_CONTENT, m_static_content);    
    DDX_Control(pDX, IDC_STATIC_STATISTIC, m_static_statistic);
    */
}


BEGIN_MESSAGE_MAP(CSmsWriteDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsWriteDlg)
    ON_EN_CHANGE(IDC_EDIT_SMSDETAILS, OnChangeEditSmsdetails)
    ON_BN_CLICKED(IDC_BUTTON_SMS_PB, OnButtonSmsPb)
    ON_BN_CLICKED(IDC_BUTTON_SMS_PB2, OnButtonSmsPb2)
    ON_BN_CLICKED(ID_BUTTON_SMS_SEND, OnButtonSmsSend)
    ON_BN_CLICKED(ID_BUTTON_SMS_SAVE, OnButtonSmsSave)
    ON_BN_CLICKED(IDC_BUTTON_SMS_CLEAR, OnButtonSmsClear)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE, OnTemplates)
    ON_WM_TIMER()
	ON_EN_UPDATE(IDC_EDIT_NUMBER, OnUpdateEditNumber)
	ON_EN_UPDATE(IDC_EDIT_SMSDETAILS, OnUpdateEditSmsdetails)
    ON_MESSAGE(WM_SEL_CONTACT, OnReceiveNum)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_CREATE_PROGRESS, OnCreateProgress)        
    ON_MESSAGE(WM_SMS_SEND_PROC, OnSmsSendPorc)            
	ON_MESSAGE(WM_SMS_SAVE_MSG_ANSWER_INCALL, SaveSMS)
	ON_MESSAGE(WM_SELCONTENT, OnTemplateSel)
	ON_MESSAGE(WM_UPDATA_SENDSMS_LMS, UpdateProgressStep)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsWriteDlg message handlers


BOOL CSmsWriteDlg::OnInitDialog() 
{    
    CBaseDialog::OnInitDialog();

    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);

#ifdef FEATURE_GCF_SMSREPLY
    if(m_WriteType == SMS_WRITE_REPLY)
        m_editNumber.SetReadOnly();
#endif

#ifndef FEATURE_BUTTON_TEMPLATE
	GetDlgItem(IDC_BUTTON_TEMPLATE)->ShowWindow(SW_HIDE);
#endif

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsWriteDlg::OnChangeEditSmsdetails() 
{
    UpdateData(TRUE);


#ifdef FEATURE_SMS_CONCATENATE
    if(IsAlphabetUnicode(m_strSmsDetails))
       m_strStatistic.Format(_T("(%d / %d)"), GetACSIICharNum(m_strSmsDetails), SMS_CONCAT_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX);
    else
        m_strStatistic.Format(_T("(%d / %d)"), GetUnicodeCharNum(m_strSmsDetails), SMS_CONCAT_GB_MAX * SMS_CONCAT_SEGMENT_MAX);
#else
    if(IsAlphabetUnicode(m_strSmsDetails))
        m_strStatistic.Format(_T("(%d / %d)"), GetACSIICharNum(m_strSmsDetails), SMS_CHAR_MAX);
    else
        m_strStatistic.Format(_T("(%d / %d)"), GetUnicodeCharNum(m_strSmsDetails), SMS_CHINESE_CHAR_MAX);
#endif


    UpdateStatistic(); 
    
    GetDlgItem(IDC_STATIC_STATISTIC)->SetWindowText(m_strStatistic);
    GetDlgItem(IDC_STATIC_STATISTIC)->UpdateWindow();
    GetDlgItem(IDC_STATIC_SEGE)->SetWindowText(m_strSegNum);
    GetDlgItem(IDC_STATIC_SEGE)->UpdateWindow();

    //此处会导致输入255个字符后光标位置错误
    //UpdateData(FALSE);
}

void CSmsWriteDlg::OnButtonSmsPb()
{
    // TODO: Add your control notification handler code here
#ifdef FEATURE_GCF_SMSREPLY
    if(m_WriteType == SMS_WRITE_REPLY)
        return;
#endif
    CSelContactDlg dlg(this, SELCONTACT_SMSNUMBER);
    dlg.DoModal();
}

void CSmsWriteDlg::OnButtonSmsPb2()
{
    CSelContactDlg dlg(this, SELCONTACT_SMSCONTENT);
    dlg.DoModal();
}

LRESULT CSmsWriteDlg::OnReceiveNum(WPARAM wParam, LPARAM lParam)
{
    if(wParam == 0)
        return 0;

    int len;
    EnSelContactType SelType = (EnSelContactType)lParam;

    if(SelType == SELCONTACT_SMSNUMBER)
    {
        UpdateData();
        if((len = m_strNumber.GetLength()) > 0 && m_strNumber[len-1] != ';')
        {
            m_strNumber += _T(";");
        }

        m_strNumber +=(CString)(TCHAR*) wParam;
        UpdateData(FALSE);
    
        UpdateData(TRUE);
        if(IsNumbersOver())
        {
            AfxMessageBox(IDS_ERR_SMS_GROUPNUMBEROVER);
            m_strNumber=m_strLastGroupNumber;
            UpdateData(FALSE);
        }
        else
        {
            m_strLastGroupNumber=m_strNumber;
        }
        //设置在最后
        m_editNumber.SetSel(m_strNumber.GetLength(), m_strNumber.GetLength());
    }
    else if(SelType == SELCONTACT_SMSCONTENT)
    {
        m_editSmsDetails.SetFocus();
        m_editSmsDetails.ReplaceSel((LPCTSTR)wParam);
        UpdateData();
    }

    return 0;
}

void CSmsWriteDlg::OnButtonSmsSend() 
{
	thelastone = FALSE;
    // TODO: Add your control notification handler code here
	MinMaxChar = LMS_Flexi_MinMaxChar[rand()%10];
    UpdateData();
	USES_CONVERSION;
    if(m_strNumber == _T(""))
        AfxMessageBox(IDS_WARNING_INPUTNUM, MB_OK | MB_ICONINFORMATION);
     else if(m_strSmsDetails == _T(""))
         AfxMessageBox(IDS_WARNING_INPUTSMS, MB_OK | MB_ICONINFORMATION);
    else
    {
        CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
        ASSERT(pMainWnd);
        
        CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
        ASSERT(pComm);
#ifndef FEATURE_VERSION_NOSIM
        if(CPIN_SIM_NONE_REQUIRED != pMainWnd->m_cHandlePin.m_nSimStat)
            return;
#endif
        
        if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
            return;
        
        if(pComm->CommIsReady())
        {
            if(RetrieveGroupNum())
            {
                m_nCurNum = 0;
                if(m_nCurNum < m_nNumCount)
                { 
#ifdef FEATURE_SMS_CONCATENATE
					//Modified by Zhou Bin 2008.12.30
                    //if(!DivideSmsConcatenate((const TCHAR*)m_strSmsDetails))
					if(!DivideSmsConcatenate(m_strSmsDetails))
                        return;
#endif                    

//////////////////////////////////////////////////////////////////////////WCDMA modify by liub
//                     if(SndAtSmsQCSMP())
//                     {
//                         ::ResetEvent(g_BGPassEvt);
//                         m_pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
//                     }
//                     else
//                     {
//                         ::SetEvent(g_BGPassEvt);
//                     }
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////CDMA2000 add by liub
					#ifdef FEATURE_HAIER_SMS
					   if(SndAtSmsQHMSGP())
					#else
					   if(SndAtSmsQHMSGL())
					#endif
				       {
				           ::ResetEvent(g_BGPassEvt);
				           m_pMainWnd->CtlAllSmsRcvConcTimer(FALSE);
				       }
				       else
				       {
				           ::SetEvent(g_BGPassEvt);
					   }
//////////////////////////////////////////////////////////////////////////
                }
            }
        }
    }
}

void CSmsWriteDlg::SaveSMS(WPARAM wParam, LPARAM lParam)
{
	//OnButtonSmsSave();
	UpdateData();
	
    if(m_strSmsDetails == _T(""))
        //AfxMessageBox(IDS_WARNING_INPUTSMS, MB_OK | MB_ICONINFORMATION);
		;
    else
    {
        if(m_strNumber.GetLength() == 0 || RetrieveGroupNum())
        {
            if(m_strNumber.GetLength() == 0)
            {
                m_nNumCount = 1;
                memset(m_szGroupNum[0], 0x00, sizeof(m_szGroupNum[0]));
            }
			
            USES_CONVERSION;
			int i = 0;
            while(i < m_nNumCount)
            {
                if(!m_pSmsData->AddSmsRecord(SMS_TYPE_DRAFT,
					SMS_STATE_MO_NOT_SENT,
					A2W(m_szGroupNum[i]),
					COleDateTime::GetCurrentTime(),
					m_strSmsDetails,
					A2W(m_szSCNumber)))
                    break;
                else
                    i++;
            }
            if(i > 0)
            {
                CWnd *pWnd = GetOwner();
                pWnd->SendMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_DRAFT, LOC_PC);
            }
            if(i == m_nNumCount)
                AfxMessageBox(IDS_SMS_SAVESUCC, MB_OK | MB_ICONINFORMATION);                
            else
                AfxMessageBox(IDS_SMS_SAVEFAIL, MB_OK | MB_ICONINFORMATION);            
        }
        else
            AfxMessageBox(IDS_SMS_SAVEFAIL, MB_OK | MB_ICONINFORMATION);
    }
	
	CHSDPADlg* pMainWnd = NULL;
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
void CSmsWriteDlg::OnButtonSmsSave() 
{
    // TODO: Add your control notification handler code here
    UpdateData();

    if(m_strSmsDetails == _T(""))
        AfxMessageBox(IDS_WARNING_INPUTSMS, MB_OK | MB_ICONINFORMATION);
    else
    {
        if(m_strNumber.GetLength() == 0 || RetrieveGroupNum())
        {
            if(m_strNumber.GetLength() == 0)
            {
                m_nNumCount = 1;
                memset(m_szGroupNum[0], 0x00, sizeof(m_szGroupNum[0]));
            }

            USES_CONVERSION;
			int i = 0;
            while(i < m_nNumCount)
            {
                if(!m_pSmsData->AddSmsRecord(SMS_TYPE_DRAFT,
                                             SMS_STATE_MO_NOT_SENT,
                                             A2W(m_szGroupNum[i]),
                                             COleDateTime::GetCurrentTime(),
                                             m_strSmsDetails,
                                             A2W(m_szSCNumber)))
                    break;
                else
                    i++;
            }
            if(i > 0)
            {
                CWnd *pWnd = GetOwner();
                pWnd->SendMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_DRAFT, LOC_PC);
            }
            if(i == m_nNumCount)
                AfxMessageBox(IDS_SMS_SAVESUCC, MB_OK | MB_ICONINFORMATION);                
            else
                AfxMessageBox(IDS_SMS_SAVEFAIL, MB_OK | MB_ICONINFORMATION);            
        }
        else
            AfxMessageBox(IDS_SMS_SAVEFAIL, MB_OK | MB_ICONINFORMATION);
    }

	CHSDPADlg* pMainWnd = NULL;
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

void CSmsWriteDlg::OnButtonSmsClear() 
{
    // TODO: Add your control notification handler code here
#ifdef FEATURE_GCF_SMSREPLY
    if(m_WriteType != SMS_WRITE_REPLY)
        m_strNumber = _T("");
#else
    m_strNumber = _T("");
#endif                          
    m_strSmsDetails = _T("");


#ifdef FEATURE_SMS_CONCATENATE
    m_strStatistic.Format(_T("(%d / %d)"), 0, SMS_CONCAT_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX);
#else
    m_strStatistic.Format(_T("(%d / %d)"), 0, SMS_CHAR_MAX);
#endif

    UpdateStatistic();
    UpdateData(FALSE);
}

//功能：获得群发号码
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::RetrieveGroupNum()
{
    int i, j, cnt, state;
   
    i = j = cnt = state = 0;
    m_nNumCount = 0;
    memset(m_szGroupNum, 0x00, sizeof(m_szGroupNum));

    UpdateData(TRUE);
	USES_CONVERSION;
    if((cnt = m_strNumber.GetLength()) > 0)
    {
        i = 0, state = 0;
        
        while(i < cnt)
        {
            switch(state)
            {
            case 0:
                if(m_strNumber[i] != ';')
                {
                    if(++m_nNumCount > SMS_GROUPSEND_MAX)
                    {
                        i = cnt;
                        m_nNumCount--;
                    }
                    else
                    {
                        j = 0;
                        m_szGroupNum[m_nNumCount-1][j++] = W2A(m_strNumber)[i];
                        state = 1;
                    }
                }
                break;
            case 1:
                if(m_strNumber[i] != ';')
                {
                    if(j < PB_NUM_MAX)
                        m_szGroupNum[m_nNumCount-1][j++] = W2A(m_strNumber)[i];
                    else
                    {
                        i = cnt;
                        m_nNumCount--;
                    }
                }
                else
                {
                    state = 0;
                }    
                break;
            }
            i++;
        }
    }

    if(m_nNumCount > 0)
        return TRUE;
    else
        return FALSE;
}

void CSmsWriteDlg::OnTimer(UINT nIDEvent) 
{
    m_pMainWnd->CtlAllSmsRcvConcTimer(TRUE);
    ::SetEvent(g_BGPassEvt);

    // TODO: Add your message handler code here and/or call default
    DeRegisterAtRespFunc(ATRESP_GENERAL_AT);
    ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
    
    if(IDT_QCSMP_TIMEOUT == nIDEvent)
    {
        KillTimer(IDT_QCSMP_TIMEOUT);
    }
	if (IDT_QHMSGL_TIMEOUT == nIDEvent)//add by liub for CDMA2000
	{
		KillTimer(IDT_QHMSGL_TIMEOUT);
	}
    if(IDT_QCSCA_TIMEOUT == nIDEvent)
    {
        KillTimer(IDT_QCSCA_TIMEOUT);
    }
    else if(IDT_QCMGS_TIMEOUT == nIDEvent)
    {            
        KillTimer(IDT_QCMGS_TIMEOUT);
        SaveSendSmsToOutbox(FALSE);
        CWnd *pWnd = GetOwner();        
        pWnd->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_OUTBOX, LOC_PC);    
        ProgressClose();  
        
        //resetting card to AT cmd state
        char szAtBuf[10] = {0};
        sprintf(szAtBuf, "%c", gccCtrl_Z);
        CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
        ASSERT(pComm);
        if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
        {
            RegisterAtRespFunc(ATRESP_GENERAL_AT, AtRespDummy, this);
        }
        //end
    }
    AfxMessageBox(IDS_USB_PORT_TIMEOUT);
    ((CHSDPAApp*)AfxGetApp())->m_pSerialPort->SetSerialState(SERIAL_STATE_CMD);
    CBaseDialog::OnTimer(nIDEvent);
}

//功能：发送AT$QCSMP
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::SndAtSmsQCSMP()
{
    char szAtBuf[50] = {0};
    BYTE para1, para2, para3, para4;
    para1 = para2 = para3 = para4 = 0;
    
#ifdef FEATURE_SMS_CONCATENATE
    if(gSmsIsConcatenate)
    {
        if(gSmsIsAsciiCode)
        {
            para1 = 113; para2 = 167; para3 = 0; para4 = 0;
        }
        else
        {
            para1 = 113; para2 = 167; para3 = 0; para4 = 8;
        }
    }
    else
    {
        if(IsAlphabetUnicode(m_strSmsDetails))
        {
            para1 = 17; para2 = 167; para3 = 0; para4 = 0;
        }
        else
        {
            para1 = 17; para2 = 167; para3 = 0; para4 = 8;
        }
    }
#else
    if(IsAlphabetUnicode(m_strSmsDetails))
    {
        para1 = 17; para2 = 167; para3 = 0; para4 = 0;
    }
    else
    {
        para1 = 17; para2 = 167; para3 = 0; para4 = 8;
    }
#endif


	if(g_SetData.Messages_nDeliReport == 1)
		para1 |= 0x20;
	else
		para1 &= 0xDF;

	para2 = gc_sms_validity_period[g_SetData.Messages_nValPeriod];

    sprintf(szAtBuf, "%s%d,%d,%d,%d\r", gcstrAtSms[AT_SMS_QCSMP], para1,para2,para3,para4);

    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtBuf, strlen(szAtBuf), FALSE))
    {
        SetTimer(IDT_QCSMP_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCSMP, this);
        return TRUE;
    }
    return FALSE;
}

//功能：发送AT$QHMSGL
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::SndAtSmsQHMSGL()
{
	char szAtBuf[50] = {0};
    BYTE para1, para2;
    para1 = para2 = 0;

#ifdef FEATURE_SMS_CONCATENATE
    if(gSmsIsConcatenate)
    {
        if(gSmsIsAsciiCode)
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
        if(IsAlphabetUnicode(m_strSmsDetails))
        {
            para2 = 2;
        }
        else
        {
            para2 = 4;
        }
    }
#else
    if(IsAlphabetUnicode(m_strSmsDetails))
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
//功能：发送AT$QHMSGP(设置短信其他参数<发送报告>,<短消息类型>,<消息头>,<优先级>)
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::SndAtSmsQHMSGP()
{
    char szAtBuf[50] = {0};
	BYTE para1;
    para1 = 0;
#ifdef FEATURE_HAIER_SMS
#ifdef FEATURE_SMS_CONCATENATE
		if(gSmsIsConcatenate)
		{
			if(gSmsIsAsciiCode)
			{
				m_pMainWnd->m_pSmsDlg->sms_format = 1;//GSM7
			}
			else
			{
				m_pMainWnd->m_pSmsDlg->sms_format = 6;
			}
		}
		else
		{
			if(IsAlphabetUnicode(m_strSmsDetails))
			{
				m_pMainWnd->m_pSmsDlg->sms_format = 1; //GSM7
			}
			else
			{
				m_pMainWnd->m_pSmsDlg->sms_format = 6;
			}
		}
#else
		if(IsAlphabetUnicode(m_strSmsDetails))
		{
			m_pMainWnd->m_pSmsDlg->sms_format = 1;	//ascii
		}
		else
		{
			m_pMainWnd->m_pSmsDlg->sms_format = 6;	//unicode
		}
#endif
#endif

#ifdef FEATURE_SMS_CONCATENATE
	if (gSmsIsConcatenate)
	{
		para1 = 1;//Flexi modify by liub
	}
	else
		para1 = 0;
#endif
	if (m_WriteType == SMS_WRITE_FORWORD)
	{
#ifdef FEATURE_HAIER_SMS
#ifdef FEATURE_SMS_CONCATENATE
		if (gSmsIsConcatenate)
		{
			if (gSmsCurSege +1 == gSmsTotalSege)
			{
				sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority,m_pMainWnd->m_pSmsDlg->sms_format);
			}
			else
				sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], 0,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority,m_pMainWnd->m_pSmsDlg->sms_format);
		}
		else
			sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority,m_pMainWnd->m_pSmsDlg->sms_format);
#else
		sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority,m_pMainWnd->m_pSmsDlg->sms_format);
#endif
#else
#ifdef FEATURE_SMS_CONCATENATE
		if (gSmsIsConcatenate)
		{
			if (gSmsCurSege +1 == gSmsTotalSege)
			{
				sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority);
			}
			else
				sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], 0,para1,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority);
		}
		else
			sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority);
#else
		sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,m_pMainWnd->m_pSmsDlg->ForwardSMSpriority);
#endif
#endif
	}
	else
	{
#ifdef FEATURE_HAIER_SMS
#ifdef FEATURE_SMS_CONCATENATE
		if (gSmsIsConcatenate)
		{
			if (gSmsCurSege +1 == gSmsTotalSege)
			{
				sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,g_SetData.Messages_nPriority,m_pMainWnd->m_pSmsDlg->sms_format);
			}
			else
				sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], 0,g_SetData.Messages_nPriority,m_pMainWnd->m_pSmsDlg->sms_format);
			
			
		}
		else
			sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,g_SetData.Messages_nPriority,m_pMainWnd->m_pSmsDlg->sms_format);


#else
		sprintf(szAtBuf, "%s%d,%d,%d,\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,g_SetData.Messages_nPriority,m_pMainWnd->m_pSmsDlg->sms_format);
#endif
#else
#ifdef FEATURE_SMS_CONCATENATE
		if (gSmsIsConcatenate)
		{
			if (gSmsCurSege +1 == gSmsTotalSege)
			{
				sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,g_SetData.Messages_nPriority);
			}
			else
				sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], 0,para1,g_SetData.Messages_nPriority);
		}
		else
			sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,g_SetData.Messages_nPriority);


#else
		sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,g_SetData.Messages_nPriority);
#endif
#endif
	}
//    sprintf(szAtBuf, "%s%d,,%d,%d\r", gcstrAtSms[AT_SMS_QHMSGP], g_SetData.Messages_nDeliReport,para1,g_SetData.Messages_nPriority);
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

//功能：响应AT$QHMSGP
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsWriteDlg::RspAtSmsQHMSGP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
	CSmsWriteDlg* pDlg = (CSmsWriteDlg*)pWnd;
	pDlg->KillTimer(IDT_QHMSGP_TIMEOUT);
	
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
		//	pDlg->SndAtSmsQCSMP();
	{
		if (pDlg->thelastone == FALSE)
		{
			pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QHMSGP, (LPARAM)TRUE); 
		}
		else
		{
			gSmsCurSege--;
			pDlg->bSaveSendSms = FALSE;
			pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGS, (LPARAM)TRUE);
		}

	}
}

//功能：响应AT$QHMSGL
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsWriteDlg::RspAtSmsQHMSGL(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsWriteDlg* pDlg = (CSmsWriteDlg*)pWnd;
    pDlg->KillTimer(IDT_QHMSGL_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
    {
		pDlg->SndAtSmsQHMSGP();
       // pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QHMSGL, (LPARAM)TRUE);    
        return;
    }
	
    //pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QHMSGL, (LPARAM)TRUE);   
}

//功能：响应AT$QCSMP//设置有效期
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsWriteDlg::RspAtSmsQCSMP(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsWriteDlg* pDlg = (CSmsWriteDlg*)pWnd;
    pDlg->KillTimer(IDT_QCSMP_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
                   strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
        pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCSMP, (LPARAM)FALSE);    
        return;
    }

    pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCSMP, (LPARAM)TRUE);   
}

//功能：发送AT$QCSCA
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::SndAtSmsQCSCA()
{
    char szAtBuf[50] = {0};

	//Modified by Zhou Bin 2008.12.30
//       char *szTemp = new char[_tcslen(m_szSCNumber) + 1];
//      	memset(szTemp, 0, _tcslen(m_szSCNumber) + 1);
//     	int len = WCharToChar(m_szSCNumber, szTemp);
	
    //sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCSCA], szTemp);
	sprintf(szAtBuf, "%s\"%s\"\r", gcstrAtSms[AT_SMS_QCSCA], m_szSCNumber);

	//delete []szTemp;
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

//功能：响应AT$QCSCA
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsWriteDlg::RspAtSmsQCSCA(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsWriteDlg* pDlg = (CSmsWriteDlg*)pWnd;
    pDlg->KillTimer(IDT_QCSCA_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0)
        pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCSCA, (LPARAM)TRUE);
    else
        pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCSCA, (LPARAM)TRUE);
}

//功能：发送AT$QCMGS
//输入：nStep:写号码：1,写内容：2
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::SndAtSmsQCMGS(int nStep)
{    
	memset(m_szGroupNumSendNum, 0x00, sizeof(m_szGroupNumSendNum));
		USES_CONVERSION;
    TCHAR szAtBuf[1600] = {0};
    char szAtAscBuf[1600] = {0};
	int buffsize;
    ASSERT(m_nCurNum < m_nNumCount);

    if(nStep == 1)
    {
        const char *pNumType;
		char *q;
		q = m_szGroupNum[m_nCurNum];
        
		if (m_szGroupNum[m_nCurNum][0] == '+' && m_szGroupNum[m_nCurNum][1] == '6' && m_szGroupNum[m_nCurNum][2] == '2')
		{
			m_szGroupNumSendNum[0] = '0';
			q = q + 3;
			int i = 1;
			while (*q != '\0')
			{
				m_szGroupNumSendNum[i] = *q;
				q++;
				i++;
				
			}
			m_szGroupNumSendNum[i] = '\0';

		}
		else
		{
			strcat(m_szGroupNumSendNum,m_szGroupNum[m_nCurNum]);
		}
		
        if(m_szGroupNum[m_nCurNum][0] == '+')
            pNumType = gcstrNumType[0];
        else
            pNumType = gcstrNumType[1];

	    //Modified by Zhou Bin 2008.12.30
// 		char *szGPTemp = new char[_tcslen(m_szGroupNum[m_nCurNum]) + 1];
//      	memset(szGPTemp, 0, _tcslen(m_szGroupNum[m_nCurNum]) + 1);
// 		WCharToChar(m_szGroupNum[m_nCurNum], szGPTemp);
//         sprintf(szAtAscBuf, "%s\"%s\",%s\r", 
//             gcstrAtSms[AT_SMS_QCMGS], 
//             szGPTemp,
//             pNumType);
// 		delete []szGPTemp;
#ifndef FEATURE_HAIER_SMS
        char szHead[30];
        if(gSmsIsConcatenate)
        {
            memset(szHead, 0x00, sizeof(szHead));
            SetConcatenateSmsParaA(szHead, gSmsRefCnt, gSmsCurSege+1, gSmsTotalSege, MinMaxChar);
		}
		sprintf(szAtAscBuf, "%s\"%s\",0,%s,\r", 
        gcstrAtSms[AT_SMS_QCMGS], 
		m_szGroupNumSendNum,
        szHead);
#else
		sprintf(szAtAscBuf, "%s\"%s\",%s\r", 
        gcstrAtSms[AT_SMS_QCMGS], 
        /*m_szGroupNum[m_nCurNum],*/
		m_szGroupNumSendNum,
        pNumType);
#endif
        buffsize=strlen(szAtAscBuf);
    }
    else
    {
#ifdef FEATURE_SMS_CONCATENATE
        BOOL bConcsms = FALSE;
        if(gSmsIsConcatenate)
            bConcsms = TRUE;

        if(bConcsms)
        {
#ifdef FEATURE_HAIER_SMS
            char szHead[30];
            memset(szHead, 0x00, sizeof(szHead));
            if(SetConcatenateSmsParaA(szHead, gSmsRefCnt, gSmsCurSege+1, gSmsTotalSege, MinMaxChar))
            {
//               TCHAR *unicodStr = new TCHAR[strlen(szHead) * 2 + 1];
//      	//memset(unicodStr, 0, strlen(szHead)  + 2);
//                   ASCToUCS2((char*)szHead,(TCHAR *)unicodStr );
//                   wcscpy(szAtBuf, unicodStr);
// 				      	       delete[] unicodStr;

//				CString unicodStr = A2W(szHead);
//				wcscpy(szAtBuf, BTToUCS2(unicodStr));
				memcpy(szAtAscBuf, szHead, 6);
            }
#endif
#ifdef FEATURE_HAIER_SMS
			if(m_pMainWnd->m_pSmsDlg->sms_format == 1){
				//ascii
				USES_CONVERSION;
				int len = ((CString)gszSmsSege[gSmsCurSege]).GetLength() + 6;
				char* p = T2A(gszSmsSege[gSmsCurSege]);
				strncpy(szAtAscBuf + 6, (char *)p, len - 6);
				szAtAscBuf[len] = gccCtrl_Z;
				buffsize=len+1 ;
				TRACE(_T("len = %d"), len);
			}else{
				//unicode
				wchar_t *buf = GBTOWChar((CString)gszSmsSege[gSmsCurSege]);
				int iLen = wcslen((wchar_t *)buf);
				wcsncpy((wchar_t *)szAtAscBuf, buf, iLen);
				szAtAscBuf[iLen*2] = 0;
				szAtAscBuf[iLen*2+1] = gccCtrl_Z;
				buffsize=iLen*2+2;
			}
#else   
			CString strUc = BTToUCS2((CString)gszSmsSege[gSmsCurSege]);
			wcsncat(szAtBuf, strUc, sizeof(szAtBuf));
			int szhlen=WCharToUnicode(szAtBuf,szAtAscBuf);

			
            szAtAscBuf[szhlen] = gccCtrl_Z;
			buffsize=szhlen+1;
#endif
        }
        else
#endif
        {
#ifdef FEATURE_HAIER_SMS
			if(m_pMainWnd->m_pSmsDlg->sms_format == 1){
				//ascii
				USES_CONVERSION;
				int len = m_strSmsDetails.GetLength();
				
				char* p = T2A(m_strSmsDetails);
				strncpy(szAtAscBuf, (char *)p, len);
				szAtAscBuf[len] = gccCtrl_Z;
				buffsize=len+1;
			}else{
				//unicode
				wchar_t *buf = GBTOWChar((CString)m_strSmsDetails);
				int iLen = wcslen((wchar_t *)buf);
				wcsncpy((wchar_t *)szAtAscBuf, buf, iLen);
				szAtAscBuf[iLen*2] = 0;
				szAtAscBuf[iLen*2+1] = gccCtrl_Z;
				buffsize=iLen*2+2;
			}
#else
			CString strUC = BTToUCS2((CString)m_strSmsDetails);
			int len= WCharToUnicode(strUC, szAtAscBuf);

			szAtAscBuf[len] = gccCtrl_Z;
			
			buffsize=len+1;
#endif
        }
    }
    
    CSerialPort* pComm = ((CHSDPAApp*)AfxGetApp())->m_pSerialPort;
    ASSERT(pComm);
    if(pComm->WriteToPort(szAtAscBuf, buffsize, FALSE))
    {
        SetTimer(IDT_QCMGS_TIMEOUT, 60000, NULL);
        RegisterAtRespFunc(ATRESP_GENERAL_AT, RspAtSmsQCMGS, this);
        return TRUE;
    }
    else
    {
        SaveSendSmsToOutbox(FALSE);
        CWnd *pWnd = GetOwner();
        pWnd->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_OUTBOX, LOC_PC);

		CString strAppName; 
		strAppName.LoadString(IDS_APPNAME);
		CString strSendFail;
		strSendFail.LoadString(IDS_SMS_SEND_FAIL);
		pDlg->MessageBox(strSendFail,strAppName,MB_OK | MB_ICONINFORMATION);
		//AfxMessageBox(IDS_SMS_SEND_FAIL);

        ProgressClose();
        
        return FALSE;
    }
}

//功能：响应AT$QCMGS
//输入：pWnd:窗口指针，strArr:结果行，wStrNum:行数
//输出：无
void CSmsWriteDlg::RspAtSmsQCMGS(LPVOID pWnd, BYTE (*strArr)[DSAT_STRING_COL], WORD wStrNum)
{
    CSmsWriteDlg* pDlg = (CSmsWriteDlg*)pWnd;
    pDlg->KillTimer(IDT_QCMGS_TIMEOUT);
    
    if(strcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_ERROR][gc_dsatmode]) == 0
        || memcmp((const char*)strArr[0], gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode],
        strlen(gc_dsatResCodeTbl[DSAT_CMS_ERROR][gc_dsatmode])) == 0)
    {
		//if (IDOK == AfxMessageBox(_T("Send continue?"),MB_OKCANCEL))
		if(IDOK == pDlg->MessageBox(_T("Send fail,continue?"),NULL,MB_OKCANCEL))
		{
		   	pDlg->bSaveSendSms = FALSE;
#ifdef FEATURE_SMS_CONCATENATE
             if(gSmsIsConcatenate)
    			gSmsCurSege--;
			 else
			 {
				pDlg->m_nCurNum--;
				//pDlg->ProgressPos(pDlg->m_nCurNum-1);
				pDlg->pDlg->m_Progress.SetPos(pDlg->m_nCurNum);
			 }

#else
			pDlg->m_nCurNum--;
			//pDlg->ProgressPos(pDlg->m_nCurNum-1);
			pDlg->pDlg->m_Progress.SetPos(pDlg->m_nCurNum);
#endif
			pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGS, (LPARAM)TRUE);
	       	return;
			
		}
		else
		{
			pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGW, (LPARAM)FALSE);
          	return;
		}

    }
    
    ASSERT(wStrNum == 1 || wStrNum == 2);
    
    //Ready to Write Sms Content
    if(wStrNum == 1 && strcmp((const char*)strArr[0], gcstrSmsPrompt) == 0)
    {
        pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGW, (LPARAM)TRUE);        
    }
    //Write Ok
    else if(wStrNum == 2 
        && strcmp((const char*)strArr[1], gc_dsatResCodeTbl[DSAT_OK][gc_dsatmode]) == 0
        && memcmp((const char*)strArr[0], gcstrResSms[AT_SMS_QCMGS], strlen(gcstrResSms[AT_SMS_QCMGS])) == 0)
    {
		if (TRUE == pDlg->thelastone)
		{
			pDlg->theLMSSavenow = TRUE;
			pDlg->thelastone = FALSE;
			
		}

		#ifdef FEATURE_HAIER_SMS
		::SetEvent(g_BGCSSEvt);
		#else
        pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGS, (LPARAM)TRUE);
		#endif

    }
    else
	{
		if(IDOK == pDlg->MessageBox(_T("Send fail,continue?"),NULL,MB_OKCANCEL))
		{
		   	pDlg->bSaveSendSms = FALSE;

#ifdef FEATURE_SMS_CONCATENATE
			if(gSmsIsConcatenate)
				gSmsCurSege--;
			else
				pDlg->m_nCurNum--;
#else
			pDlg->m_nCurNum--;
#endif
			pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGS, (LPARAM)TRUE);
			
		}
		else
		{
// 			if (TRUE == pDlg->thelastone)
// 			{
// 				pDlg->theLMSSavenow = TRUE;
// 
// 			}
			pDlg->PostMessage(WM_SMS_SEND_PROC, (WPARAM)AT_SMS_QCMGW, (LPARAM)FALSE);
		}

	}
}

//功能：响应SMS发送消息
//输入：wParam:AT命令，lParam:执行结果
//输出：结果
LRESULT CSmsWriteDlg::OnSmsSendPorc(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd = GetOwner();
    EnAtSmsType AtType = (EnAtSmsType)wParam;
    BOOL bProcRst = (BOOL)lParam;
    BOOL bSndRes = TRUE;

    CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    ASSERT(pMainWnd);

    if(bProcRst)
    {
        if(AtType == AT_SMS_QCSMP)
        {
            if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                goto STOPSEND;
            ProgressOpen(m_nNumCount, 1);
            bSndRes = SndAtSmsQCSCA();
        }
        else if(AtType == AT_SMS_QCSCA)
        {
            bSndRes = SndAtSmsQCMGS(1);
        }
		else if(AtType == AT_SMS_QHMSGP)//add by liub
        {
			if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                goto STOPSEND;
            bSndRes = SndAtSmsQCMGS(1);
			ProgressOpen(m_nNumCount, 1);
			PostMessage(WM_UPDATA_SENDSMS_LMS,0,0);//flexi 发长短信刷新进度条
        }
        else if(AtType == AT_SMS_QCMGW)
        {
            if(m_nCurNum == 0)
            {
#ifdef FEATURE_SMS_CONCATENATE
                if(gSmsIsConcatenate) 
                {
                    if(gSmsCurSege == 0)
                        ProgressStep();
                }
                else
                    ProgressStep();
#else
                ProgressStep();
#endif
            }
//            Sleep(200);
            bSndRes = SndAtSmsQCMGS(2);
        }
        else
        {
			//flexi 需求
// 			if (TRUE == bSaveSendSms)
// 			{
// 				SaveSendSmsToOutbox(TRUE);
// 				
// 			}
// 			else
// 				bSaveSendSms = TRUE;
#ifdef FEATURE_SMS_CONCATENATE
			//flexi需求更改，LMS发送成功后完整保存短信至发件箱
			if (gSmsIsConcatenate)
			{
				if (TRUE == theLMSSavenow)
				{
					SaveSMStoOutbox();
					theLMSSavenow = FALSE;
				}
			}
			else
				SaveSMStoOutbox();
#else
			SaveSMStoOutbox();
#endif 
			//////////////////////////////////////////////////////////////////////////
			//尝试修改发短信崩溃的现象而增加
			//CSmsDlg *pSmsDlg = (CSmsDlg *)GetOwner();
			pMainWnd->m_pSmsDlg->m_cmbLocFilter.SetCurSel(LOCFILTER_PC);
			pMainWnd->m_pSmsDlg->OnSelchangeCombolocfilterForOut();
		//	pMainWnd->m_pSmsDlg->m_lstSms.SetDspFilter(SMS_TYPE_OUTBOX);
	        
			//////////////////////////////////////////////////////////////////////////
            //pWnd->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_OUTBOX, LOC_PC);
			pMainWnd->m_pSmsDlg->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_OUTBOX, LOC_PC);

            if(0){}
#ifdef FEATURE_SMS_CONCATENATE

    
            if(gSmsIsConcatenate && ++gSmsCurSege < gSmsTotalSege)
            {
				PostMessage(WM_UPDATA_SENDSMS_LMS,0,0);//flexi 发长短信刷新进度条
				if (gSmsCurSege +1 == gSmsTotalSege && FALSE == thelastone)
				{
					thelastone = TRUE;
					SndAtSmsQHMSGP();
				} 
				else
					bSndRes = SndAtSmsQCMGS(1);
            }
#endif
            else if(++m_nCurNum < m_nNumCount)
            {
			
                if(pMainWnd->m_bInComSms || pMainWnd->m_bInComCall)
                    goto STOPSEND;
#ifdef FEATURE_SMS_CONCATENATE
                if(gSmsIsConcatenate)
                {
                    gSmsRefCnt++;
                    gSmsCurSege = 0;
                }
#endif
                ProgressStep();
				PostMessage(WM_UPDATA_SENDSMS_LMS,0,0);//flexi 发长短信刷新进度条
                bSndRes = SndAtSmsQCMGS(1);
            }
            else
            {
                m_pMainWnd->CtlAllSmsRcvConcTimer(TRUE);
                ::SetEvent(g_BGPassEvt); 
                
				CString strAppName; 
				strAppName.LoadString(IDS_APPNAME);
				CString strSendSucc;
				strSendSucc.LoadString(IDS_SMS_SEND_SUCC);
				pDlg->MessageBox(strSendSucc,strAppName,MB_OK | MB_ICONINFORMATION);
                //AfxMessageBox(IDS_SMS_SEND_SUCC, MB_OK | MB_ICONINFORMATION);

				ProgressClose();
            }    
        } 
        if(!bSndRes)
        {
            m_pMainWnd->CtlAllSmsRcvConcTimer(TRUE);
            ::SetEvent(g_BGPassEvt);
        }
    }
    else
    {
STOPSEND:
        m_pMainWnd->CtlAllSmsRcvConcTimer(TRUE);
        ::SetEvent(g_BGPassEvt); 
        
        //SaveSendSmsToOutbox(FALSE);
		SaveSMStoDraft();//flexi 需求，LMS发送失败后整体保存在草稿箱
		//////////////////////////////////////////////////////////////////////////
		//尝试修改发短信崩溃的现象而增加
		pMainWnd->m_pSmsDlg->m_cmbLocFilter.SetCurSel(LOCFILTER_PC);
	    pMainWnd->m_pSmsDlg->OnSelchangeCombolocfilterForOut();
		//////////////////////////////////////////////////////////////////////////
        pMainWnd->m_pSmsDlg->PostMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_DRAFT, LOC_PC);

//		CString strAppName; 
//		strAppName.LoadString(IDS_APPNAME);
//		CString strSendFail;
//		strSendFail.LoadString(IDS_SMS_SEND_FAIL);
//		pDlg->MessageBox(strSendFail,strAppName,MB_OK | MB_ICONINFORMATION);
		//AfxMessageBox(IDS_SMS_SEND_FAIL);

		ProgressClose();
    }

//	CHSDPADlg* pMainWnd = NULL;
    pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
    CSmsDlg* m_pSmsDlg = NULL;
	m_pSmsDlg = (CSmsDlg*)(pMainWnd->m_pSmsDlg);
	HTREEITEM hRoot = m_pSmsDlg->m_treeSms.GetRootItem();
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
#ifndef FEATURE_VERSION_NOSIM
	hRoot = m_pSmsDlg->m_treeSms.GetNextSiblingItem(hRoot);
	m_pSmsDlg->OnSmsUpdataNumforSmsTree(hRoot);
#endif

    return 0;
}

//flexi 需求，LMS发送成功后整体保存在发件箱
void CSmsWriteDlg::SaveSMStoOutbox()
{   
    // TODO: Add your control notification handler code here
    UpdateData();
	
	if(m_strNumber.GetLength() == 0 || RetrieveGroupNum())
	{
		if(m_strNumber.GetLength() == 0)
		{
			m_nNumCount = 1;
			memset(m_szGroupNum[0], 0x00, sizeof(m_szGroupNum[0]));
		}
		
		USES_CONVERSION;


			m_pSmsData->AddSmsRecord(SMS_TYPE_OUTBOX,
				SMS_STATE_MO_NOT_SENT,
				A2W(m_szGroupNum[m_nCurNum]),
				COleDateTime::GetCurrentTime(),
				m_strSmsDetails,
				A2W(m_szSCNumber));



		if(m_nCurNum)
		{
			CWnd *pWnd = GetOwner();
			pWnd->SendMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_OUTBOX, LOC_PC);
		}
        
	}
	
	
	CHSDPADlg* pMainWnd = NULL;
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

//flexi 需求，LMS发送失败后整体保存在草稿箱
void CSmsWriteDlg::SaveSMStoDraft()
{
    // TODO: Add your control notification handler code here
    UpdateData();
	
        if(m_strNumber.GetLength() == 0 || RetrieveGroupNum())
        {
            if(m_strNumber.GetLength() == 0)
            {
                m_nNumCount = 1;
                memset(m_szGroupNum[0], 0x00, sizeof(m_szGroupNum[0]));
            }
			
            USES_CONVERSION;
			int i = m_nCurNum;
            while(i < m_nNumCount)
            {
                if(!m_pSmsData->AddSmsRecord(SMS_TYPE_DRAFT,
					SMS_STATE_MO_NOT_SENT,
					A2W(m_szGroupNum[i]),
					COleDateTime::GetCurrentTime(),
					m_strSmsDetails,
					A2W(m_szSCNumber)))
                    break;
                else
                    i++;
            }
            if(i > 0)
            {
                CWnd *pWnd = GetOwner();
                pWnd->SendMessage(WM_SMS_SAVE_MSG, (WPARAM)SMS_TYPE_DRAFT, LOC_PC);
            }
        
        }


	CHSDPADlg* pMainWnd = NULL;
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

//功能：创建进度框
//输入：wParam:最大值，lParam:步长
//输出：结果
LRESULT CSmsWriteDlg::OnCreateProgress(WPARAM wParam, LPARAM lParam)
{
    int nUpper = (int)wParam;
    int nStep = (int)lParam;
    
    if(nUpper == 0 || nStep == 0)
    {
        CProgressExDlg dlg(this);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;
    }
    else
    {
        CProgressExDlg dlg(this, nUpper, nStep);
        pDlg = &dlg;
        dlg.DoModal();
        pDlg = NULL;        
    }
    
    return 0;
}

//功能：设置进度框
//输入：nLower:最小值，nUpper:最大值，nStep:步长
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::ProgressSet(int nLower, int nUpper, int nStep)
{
    if(pDlg)
    {
        m_nLower = nLower;
        m_nUpper = nUpper;
        m_nStep  = nStep;
        pDlg->m_strText = "";
        pDlg->m_Progress.SetRange(m_nLower, m_nUpper);
        pDlg->m_Progress.SetStep(m_nStep);
        return TRUE;
    }
    return FALSE;
}

//功能：步进进度框
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::ProgressStep()
{
    if(pDlg && pDlg->m_Progress.GetPos() < m_nUpper)
    {
        CString str;
        str.LoadString(IDS_SMS_SENDING_TIP);
		USES_CONVERSION;
 //        pDlg->m_strText.Format(_T("%s\"%s\", (%d/%d)"), str,
 //                                A2W(m_szGroupNum[m_nCurNum]), m_nCurNum+1, m_nNumCount);
		//flexi需求，增加长短信发送进度
		if (gSmsTotalSege == 0)
		{
			pDlg->m_strText.Format(_T("%s\"%s(%d/%d)\", (%d/%d)"), str,
                                 A2W(m_szGroupNum[m_nCurNum]),1,1, m_nCurNum+1, m_nNumCount);

		}
		else
		{
			if (0 == gSmsCurSege)
			{
				pDlg->m_strText.Format(_T("%s\"%s(%d/%d)\", (%d/%d)"), str,
                                 A2W(m_szGroupNum[m_nCurNum]),gSmsCurSege+1,gSmsTotalSege, m_nCurNum+1, m_nNumCount);
			}
			else
			{
		        pDlg->m_strText.Format(_T("%s\"%s(%d/%d)\", (%d/%d)"), str,
                                 A2W(m_szGroupNum[m_nCurNum]),gSmsCurSege,gSmsTotalSege, m_nCurNum+1, m_nNumCount);
			}
		}
        pDlg->m_Progress.StepIt();
        pDlg->UpdateData(FALSE);
        return TRUE;
    }
    return FALSE;
}

//功能：Flexi 发长短信时刷新步进进度框
//输入：无
//输出：TRUE/FALSE
LRESULT CSmsWriteDlg::UpdateProgressStep(WPARAM wParam, LPARAM lParam)
{
    if(pDlg && pDlg->m_Progress.GetPos() < m_nUpper)
    {
        CString str;
        str.LoadString(IDS_SMS_SENDING_TIP);
		USES_CONVERSION;
		//flexi需求，增加长短信发送进度
		if (gSmsTotalSege == 0)
		{
			pDlg->m_strText.Format(_T("%s\"%s(%d/%d)\", (%d/%d)"), str,
				A2W(m_szGroupNum[m_nCurNum]),1,1, m_nCurNum+1, m_nNumCount);
			
		}
		else
		{
			pDlg->m_strText.Format(_T("%s\"%s(%d/%d)\", (%d/%d)"), str,
				A2W(m_szGroupNum[m_nCurNum]),gSmsCurSege+1,gSmsTotalSege, m_nCurNum+1, m_nNumCount);
		}
       // pDlg->m_Progress.StepIt();
        pDlg->UpdateData(FALSE);
        return TRUE;
    }
    return 0;
}


//功能：关闭进度框
//输入：无
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::ProgressClose()
{
    if(pDlg)
    {
        pDlg->PostMessage(WM_CLOSE);
        return TRUE;
    }
    return FALSE;
}

//功能：打开进度框
//输入：nUpper:最大值，nStep:步长
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::ProgressOpen(int nUpper, int nStep)
{
    PostMessage(WM_CREATE_PROGRESS, (WPARAM)nUpper,(LPARAM)nStep);
    return TRUE;
}

//功能：设置进度框位置
//输入：nPos:位置
//输出：TRUE/FALSE
BOOL CSmsWriteDlg::ProgressPos(int nPos)
{
    if(pDlg && pDlg->m_Progress.GetPos() < nPos && nPos <= m_nUpper)
    {
        pDlg->m_Progress.SetPos(nPos);
        return TRUE;
    }
    return FALSE;
}

/*++

Routine Description:

	更新群发号码输入框通知
    检测输入的号码是否超过指定的数目
    超出数目则使用上次保存的号码列表
	检测输入的单个号码是否超出号码长度
    超出长度则声音报警,并使用上次的号码列表

Arguments:

    None.

Return Value:

    None.

--*/
void CSmsWriteDlg::OnUpdateEditNumber() 
{
/*++

    用复制-粘贴的方法贴入超过10个号码时,产生系统错误
    改用CString后正常

--*/
/*
    int i, j, cnt, state;
    
    int nNumCount = 0;
    BOOL bSingleNumOver = FALSE;
    char szGroupNum[SMS_GROUPSEND_MAX][PB_NUM_MAX + 2];

    memset(szGroupNum, 0x00, sizeof(szGroupNum));

    UpdateData(TRUE);

    if((cnt = m_strNumber.GetLength()) > 0)
    {
        i = 0, state = 0;
        
        while(i < cnt)
        {
            switch(state)
            {
            case 0:
                if(m_strNumber[i] != ';')
                {
                    nNumCount++;
                    j = 0;
                    szGroupNum[nNumCount-1][j++] = m_strNumber[i];
                    state = 1;
                }
                break;
            case 1:
                if(m_strNumber[i] != ';')
                {
                    szGroupNum[nNumCount-1][j++] = m_strNumber[i];
                }
                else
                {
                    state = 0;
                }    
                break;
            }
            i++;
        }
    }

    for(i=0; i<nNumCount; i++)
    {
        if(strlen(szGroupNum[i])>PB_NUM_MAX)
        {
            bSingleNumOver=TRUE;
            break;
        }
    }

    if (nNumCount > SMS_GROUPSEND_MAX)
    {
        //超过10个号码
        AfxMessageBox(IDS_ERR_SMS_GROUPNUMBEROVER);
        m_strNumber=m_strLastGroupNumber;
        UpdateData(FALSE);
        //设置在最后
        m_editNumber.SetSel(m_strNumber.GetLength(), m_strNumber.GetLength());
    }
    else if (bSingleNumOver)
    {
        //有号码长度超长
        Beep( 600, 50 );
        m_strNumber=m_strLastGroupNumber;
        UpdateData(FALSE);
        //设置在最后
        m_editNumber.SetSel(m_strNumber.GetLength(), m_strNumber.GetLength());
    }
    else
    {
        m_strLastGroupNumber=m_strNumber;
    }
*/  
   
    BOOL bUndo = FALSE;
    USHORT length = 0;
    TCHAR szEditNum[PB_NUM_MAX*12];
    DWORD focus = 0;
    memset(szEditNum, 0x0000, sizeof(szEditNum));

 	focus = m_editNumber.GetSel();
    
	UpdateData(TRUE);
    length = m_strNumber.GetLength(); 
    
    for(USHORT i = 0; i < length; i++)
    {
        if(!IsValidSmsNumChar(m_strNumber.GetAt(i)))
        {
			m_strNumber = m_strNumber.Left(i);
            bUndo = TRUE;
            break;
        }
    }
    
    if(bUndo)
    {
		wcsncpy(szEditNum, m_strNumber, m_strNumber.GetLength());
        m_strNumber.Format(_T("%s"), szEditNum);
        Beep(600, 50);
    }
    UpdateData(FALSE);
	if (m_strNumber.GetLength()>255)
	{
		m_editNumber.SetSel(focus, m_strNumber.GetLength());

	}

    

    int len;
    BOOL bSingleNumOver = FALSE;
    UpdateData(TRUE);
    if((len = m_strNumber.GetLength()) > 0 && m_strNumber[len-1] != ';')
    {
        m_strNumber +=_T(";");
    }
    int indexhead,indextail;
    for(indexhead=0,indextail=0;indexhead>=0;)
    {
        indextail=m_strNumber.Find(';', indexhead);
        if (indextail==-1)
        {//代表分号后还有一段号码
            if (m_strNumber.GetLength()-indexhead>PB_NUM_MAX)
            {                
                bSingleNumOver=TRUE;
                break;
            }
            break;
        }
        else if(indextail-indexhead>PB_NUM_MAX)
        {                
            bSingleNumOver=TRUE;
            break;
        }
        indexhead=indextail+1;
    }
    
    if (IsNumbersOver())
    {
        AfxMessageBox(IDS_ERR_SMS_GROUPNUMBEROVER);
        m_strNumber=m_strLastGroupNumber;
        UpdateData(FALSE);
        //设置在最后
        m_editNumber.SetSel(m_strNumber.GetLength(), m_strNumber.GetLength());
    }
    else if (bSingleNumOver)
    {
        //有号码长度超长
        Beep( 600, 50 );
        m_strNumber=m_strLastGroupNumber;
        UpdateData(FALSE);
        //设置在最后
        m_editNumber.SetSel(m_strNumber.GetLength(), m_strNumber.GetLength());
    }
    else
    {
        m_strLastGroupNumber=m_strNumber;
    }
}

/*++

Routine Description:

	发送号码的数目是否超出指定的数目
	
Arguments:

    None.

Return Value:

    True: Over
    False: Not Over

--*/
BOOL CSmsWriteDlg::IsNumbersOver()
{
//
//  delete
//  add number with "+86" will crash the app
//
/*
    int i, j, cnt, state;
    
    int nNumCount = 0;
    char szGroupNum[SMS_GROUPSEND_MAX][PB_NUM_MAX + 2];
    memset(szGroupNum, 0x00, sizeof(szGroupNum));

    UpdateData(TRUE);

    if((cnt = m_strNumber.GetLength()) > 0)
    {
        i = 0, state = 0;
        
        while(i < cnt)
        {
            switch(state)
            {
            case 0:
                if(m_strNumber[i] != ';')
                {
                    nNumCount++;
                    j = 0;
                    szGroupNum[nNumCount-1][j++] = m_strNumber[i];
                    state = 1;
                }
                break;
            case 1:
                if(m_strNumber[i] != ';')
                {
                    if(j < PB_NUM_MAX)
                        szGroupNum[nNumCount-1][j++] = m_strNumber[i];
                    else
                    {
                        i = cnt;
                        nNumCount--;
                    }
                }
                else
                {
                    state = 0;
                }    
                break;
            }
            i++;
        }
    }
	//
	//NEVER delete this line.
	//If you do this, there will be a crash here in Release mode.
	//
	CString temp;
	
    if(nNumCount > SMS_GROUPSEND_MAX)
        return TRUE;
    else
        return FALSE;
*/
	int len;
	BOOL nNumCount = 0;
	if((len = m_strNumber.GetLength()) > 0 && m_strNumber[len-1] != ';')
	{
		m_strNumber += _T(";");
	}
	int indexhead,indextail;
	for(indexhead=0,indextail=0;indexhead>=0;)
	{
		indextail=m_strNumber.Find(';', indexhead);
		if (indextail==-1)
		{
			if (m_strNumber.GetLength()-indexhead>0)
			{                
				nNumCount++;
			}
		}
		else if(indextail-indexhead>0)
		{                
			nNumCount++;
		}
		if(indexhead>=len)
			break;
		indexhead=indextail+1;
	}
	if(nNumCount > SMS_GROUPSEND_MAX)
		return TRUE;
	else
		return FALSE;
}

void CSmsWriteDlg::OnUpdateEditSmsdetails() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBaseDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	USHORT nAsciiLimit;
	USHORT nGbLimit;
#ifdef FEATURE_SMS_CONCATENATE
	nAsciiLimit = SMS_CONCAT_ASCII_MAX * SMS_CONCAT_SEGMENT_MAX;
	nGbLimit = SMS_CONCAT_GB_MAX * SMS_CONCAT_SEGMENT_MAX;
#else
	nAsciiLimit = SMS_CHAR_MAX;
	nGbLimit = SMS_CHINESE_CHAR_MAX;
#endif

    BOOL bUndo = FALSE;
	TCHAR *szDetails = new TCHAR[nAsciiLimit+ 1];
    memset(szDetails, 0x0000, (nAsciiLimit+ 1)*2);
	wcsncpy(szDetails, m_strSmsDetails, nAsciiLimit);
	
    UpdateData(TRUE);

    if(IsAlphabetUnicode(m_strSmsDetails))
    {
        if(GetUnicodeCharNum(m_strSmsDetails) > nAsciiLimit)
            bUndo = TRUE;
    }
    else
    {
        if(GetUnicodeCharNum(m_strSmsDetails) > nGbLimit)
            bUndo = TRUE;
    }
    if(bUndo)
    {
        m_strSmsDetails.Format(_T("%s"), szDetails);
        m_editSmsDetails.SetWindowText(m_strSmsDetails);
        m_editSmsDetails.UpdateWindow();
        m_editSmsDetails.SetSel(m_strSmsDetails.GetLength(), m_strSmsDetails.GetLength());
        m_editSmsDetails.ReplaceSel(_T(""));
        Beep(600, 50);
    }
	delete []szDetails;

    //此处会导致输入255个字符后光标位置错误
    //UpdateData(FALSE);

}

void CSmsWriteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBaseDialog::OnCancel();
}

void CSmsWriteDlg::OnOK() 
{
	// TODO: Add extra cleanup here
	
	//CBaseDialog::OnOK();
}

//功能：保存SMS到发件箱
//输入：bSndSucc:发送是否成功
//输出：无
void CSmsWriteDlg::SaveSendSmsToOutbox(BOOL bSndSucc)
{
#ifdef FEATURE_SMS_CONCATENATE
    BOOL bConcsms = FALSE;
    bConcsms = gSmsIsConcatenate;

    USES_CONVERSION;
	if(bConcsms)
    {
        if(bSndSucc)
        {
            if(gSmsCurSege < gSmsTotalSege)
            {
                //Modified by Zhou Bin 2008.12.30
				m_pSmsData->AddSmsRecord(SMS_TYPE_OUTBOX,
                    SMS_STATE_MO_SENT,
                    A2W(m_szGroupNum[m_nCurNum]),
                    COleDateTime::GetCurrentTime(),
                    &gszSmsSege[gSmsCurSege][0], 
                    A2W(m_szSCNumber), 
                    SMS_RECORD_FLAG_CONCATENATE_SEGE, 
                    gSmsRefCnt, 
                    gSmsCurSege+1, 
                    gSmsTotalSege);
// 				m_pSmsData->AddSmsRecord(SMS_TYPE_OUTBOX,
//                     SMS_STATE_MO_SENT,
//                     m_szGroupNum[m_nCurNum],
//                     COleDateTime::GetCurrentTime(),
//                     (TCHAR*)&gszSmsSege[gSmsCurSege][0], 
//                     (TCHAR*)m_szSCNumber, 
//                     SMS_RECORD_FLAG_CONCATENATE_SEGE, 
//                     gSmsRefCnt, 
//                     gSmsCurSege+1, 
//                     gSmsTotalSege);
            }
        }
        else
        {
            while(gSmsCurSege < gSmsTotalSege)
            {
				m_pSmsData->AddSmsRecord(/*SMS_TYPE_OUTBOX*/SMS_TYPE_DRAFT,//flexi 需求
                    SMS_STATE_MO_NOT_SENT,
                    A2W(m_szGroupNum[m_nCurNum]),
                    COleDateTime::GetCurrentTime(),
                    &gszSmsSege[gSmsCurSege][0], 
                    A2W(m_szSCNumber), 
                    SMS_RECORD_FLAG_CONCATENATE_SEGE, 
                    gSmsRefCnt, 
                    gSmsCurSege+1, 
                    gSmsTotalSege);
                gSmsCurSege++;
//                 m_pSmsData->AddSmsRecord(SMS_TYPE_OUTBOX,
//                     SMS_STATE_MO_NOT_SENT,
//                     m_szGroupNum[m_nCurNum],
//                     COleDateTime::GetCurrentTime(),
//                     (TCHAR*)&gszSmsSege[gSmsCurSege][0], 
//                     (TCHAR*)m_szSCNumber, 
//                     SMS_RECORD_FLAG_CONCATENATE_SEGE, 
//                     gSmsRefCnt, 
//                     gSmsCurSege+1, 
//                     gSmsTotalSege);
//                 gSmsCurSege++;
            }
        }
    }
    else
#endif
    {
        if(bSndSucc)
        {
            m_pSmsData->AddSmsRecord(SMS_TYPE_OUTBOX,
                                        SMS_STATE_MO_SENT,
                                        A2W(m_szGroupNum[m_nCurNum]),
                                        COleDateTime::GetCurrentTime(),
                                        m_strSmsDetails, 
                                        A2W(m_szSCNumber));
        }
        else
        {
            m_pSmsData->AddSmsRecord(/*SMS_TYPE_OUTBOX*/SMS_TYPE_DRAFT,//flexi 需求
                        SMS_STATE_MO_NOT_SENT,
                        A2W(m_szGroupNum[m_nCurNum]),
                        COleDateTime::GetCurrentTime(),
                        m_strSmsDetails, 
                        A2W(m_szSCNumber));
        }
    }
}

//功能：更新统计框
//输入：无
//输出：无
void CSmsWriteDlg::UpdateStatistic()
{
    int nSeg = 0;
    int nRemain = 0;

    if(IsAlphabetUnicode(m_strSmsDetails))
    {
        if(GetACSIICharNum(m_strSmsDetails) <= SMS_CHAR_MAX)
        {
            m_strStatistic.Format(_T("(%d)"), (SMS_CHAR_MAX - GetACSIICharNum(m_strSmsDetails)));
            m_strSegNum.Empty();
        }
        else
        {
            nSeg = GetACSIICharNum(m_strSmsDetails) / SMS_CONCAT_ASCII_MAX; 
            nRemain = GetACSIICharNum(m_strSmsDetails) % SMS_CONCAT_ASCII_MAX; 
            if(nRemain > 0)
            {
                nSeg++;
                nRemain = SMS_CONCAT_ASCII_MAX - nRemain;
            }
            m_strStatistic.Format(_T("(%d)"), nRemain);
            m_strSegNum.Format(_T("%d/%d"), nSeg, SMS_CONCAT_SEGMENT_MAX);
        }
    }
    else
    {
        if(GetUnicodeCharNum(m_strSmsDetails) <= SMS_CHINESE_CHAR_MAX)
        {
            m_strStatistic.Format(_T("(%d)"), (SMS_CHINESE_CHAR_MAX - GetUnicodeCharNum(m_strSmsDetails)));
            m_strSegNum.Empty();
        }
        else
        {
            nSeg = GetUnicodeCharNum(m_strSmsDetails) / SMS_CONCAT_GB_MAX; 
            nRemain = GetUnicodeCharNum(m_strSmsDetails) % SMS_CONCAT_GB_MAX; 
            if(nRemain > 0)
            {
                nSeg++;
                nRemain = SMS_CONCAT_GB_MAX - nRemain;
            }
            m_strStatistic.Format(_T("(%d)"), nRemain);
            m_strSegNum.Format(_T("%d/%d"), nSeg, SMS_CONCAT_SEGMENT_MAX);
        }
    }
}

void CSmsWriteDlg::OnTemplates()
{
	CSmsTemplateDlg dlg(this);
	dlg.DoModal();
}

void CSmsWriteDlg::OnTemplateSel(WPARAM wp, LPARAM lp)
{
	TCHAR *szText = (TCHAR*)wp;

	m_strSmsDetails += szText;

	UpdateData(FALSE);

	OnChangeEditSmsdetails();
}
