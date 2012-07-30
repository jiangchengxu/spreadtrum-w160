// SmsExportSel.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SmsExportSel.h"
#include "ExDataExch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsExportSel dialog
CSmsExportSel::CSmsExportSel(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CSmsExportSel::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSmsExportSel)
    m_nSmsType = 0;
    //}}AFX_DATA_INIT
    m_pSmsData = ((CHSDPAApp *)AfxGetApp())->GetSmsData();
    m_nBkTag = 1;
}

void CSmsExportSel::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsExportSel)
    DDX_Radio(pDX, IDC_RADIO1, m_nSmsType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSmsExportSel, CBaseDialog)
    //{{AFX_MSG_MAP(CSmsExportSel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsExportSel message handlers
CString CSmsExportSel::GetSmsStateText(EnSmsState state)
{
    ASSERT(state >= SMS_STATE_MT_NOT_READ && state < SMS_STATE_MAX);

    CString strText;

    switch(state)
    {
    case SMS_STATE_MT_READ:
        strText.LoadString(IDS_SMS_STATE_READ);
        break;
    case SMS_STATE_MT_NOT_READ:
        strText.LoadString(IDS_SMS_STATE_NOT_READ);
        break;
    case SMS_STATE_MO_SENT:
        strText.LoadString(IDS_SMS_STATE_SENT);
        break;
    case SMS_STATE_MO_NOT_SENT:
        strText.LoadString(IDS_SMS_STATE_NOT_SENT);        
        break;
    }
    return strText;
}

void CSmsExportSel::OnOK() 
{
    // TODO: Add extra validation here
    UpdateData();
    EnSmsType SmsType = (EnSmsType)m_nSmsType;
    ASSERT(SmsType >= SMS_TYPE_INBOX && SmsType <= SMS_TYPE_ALL);

    if(m_pSmsData->GetSmsNum(SmsType) == 0)
    {
        AfxMessageBox(IDS_SMS_NULL, MB_OK | MB_ICONINFORMATION);
        return;
    }
    else
    {
        int i, j;
        CString strCell;
        StSmsRecord sms;
        
        memset(&sms, 0x00, sizeof(StSmsRecord));
        sms.m_NoATRspCDS = TRUE;

        ShowWindow(SW_HIDE);
        IGenerator csv(this);
        if(csv.SetPath(EXP_SMS))
        {
            for(i = 0; i < SMS_TYPE_ALL; i++)
            {
                if(!(SmsType == SMS_TYPE_ALL || SmsType == i))
                    continue;
                for(j = 0; j < m_pSmsData->GetSmsNum((EnSmsType)i); j++)
                {
                    sms = m_pSmsData->ReadSmsRecord((EnSmsType)i, j);
                    strCell = GetSmsStateText(sms.state);
                    csv.FillCell(strCell);
                    strCell.Format((LPCTSTR)sms.szNumber);
                    csv.FillCell(strCell);
                    strCell = sms.timestamp.Format(LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
                    csv.FillCell(strCell);
                    strCell.Format((LPCTSTR)sms.szContent);
                    csv.FillCell(strCell);
                    csv.FillTrail();
                }
            }
            csv.MakeFile();
        }
		csv.RecoverPath();
    }

    CDialog::OnOK();
}

BOOL CSmsExportSel::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_bFirst = TRUE;

//	SetSkin(g_SetData.Setup_nSkinStyle);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
