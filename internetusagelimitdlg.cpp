// InternetUsageLimitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InternetUsageLimitDlg.h"

#include "PwdConfirmDlg.h"
#include "PwdModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInternetUsageLimitDlg dialog


CInternetUsageLimitDlg::CInternetUsageLimitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInternetUsageLimitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInternetUsageLimitDlg)
	m_nFluxOrDuration = -1;
	m_bPrompt = FALSE;
	m_bDisconnect  = FALSE;
	m_bPassword = FALSE;
	m_nHours = 0;
	m_nMinutes = 0;
	m_nFlux = 0;
	m_DateTime = COleDateTime::GetCurrentTime();
    m_DateTime.SetTime(0,0,0);
	//}}AFX_DATA_INIT
  //  m_nBkTag = 1;
}


void CInternetUsageLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInternetUsageLimitDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nFluxOrDuration);
	DDX_Check(pDX, IDC_CHECK_INT_USAGE_PROMPT, m_bPrompt);
	DDX_Check(pDX, IDC_CHECK_INT_USAGE_DISCONN, m_bDisconnect);
	DDX_Check(pDX, IDC_CHECK_INT_USAGE_PWD, m_bPassword);
	DDX_Text(pDX, IDC_EDIT_INT_USAGE_DURA_H, m_nHours);
	DDV_MaxChars(pDX, m_nHours, 4);
	DDX_Text(pDX, IDC_EDIT_INT_USAGE_DURA_M, m_nMinutes);
	DDV_MaxChars(pDX, m_nMinutes, 4);
	DDX_Text(pDX, IDC_EDIT_INT_USAGE_FLUX, m_nFlux);
	DDV_MaxChars(pDX, m_nFlux, 4);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_USAGE, m_DateTime);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInternetUsageLimitDlg, CDialog)
	//{{AFX_MSG_MAP(CInternetUsageLimitDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_CHECK_INT_USAGE_DISCONN, OnCheckDisconn)
	ON_BN_CLICKED(IDC_CHECK_INT_USAGE_PWD, OnCheckPWD)
	ON_BN_CLICKED(IDC_BUTTON_MODIFYPWD,OnModifyPWD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetUsageLimitDlg message handlers

BOOL CInternetUsageLimitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//     m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);
    
    ReadFromFile(); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInternetUsageLimitDlg::WriteToFile()
{
    //UpdateData(TRUE);
    g_SetData.Internet_nPromptEnable = m_bPrompt;						// 当到达设置值时提示用户
	g_SetData.Internet_nPromptDisconnEnable = m_bDisconnect;			// 当到达设置值时直接断开
	g_SetData.Internet_nPromptPwdConfirm = m_bPassword;					// 在再次发起连接时要求密码确认
    g_SetData.Internet_nPromptYear = m_DateTime.GetYear();				// 开始限制起始时间
    g_SetData.Internet_nPromptMonth = m_DateTime.GetMonth();			// 开始限制起始时间
    g_SetData.Internet_nPromptDay = m_DateTime.GetDay();				// 开始限制起始时间
    g_SetData.Internet_nPromptFluxOrDuration = m_nFluxOrDuration;		// 流量限制还是时间限制
    g_SetData.Internet_nPromptFluxVal = m_nFlux;						// 流量值
    g_SetData.Internet_nPromptDurationVal = m_nHours * 60 + m_nMinutes;	// 时间限制总和
    SaveIniFile();
}

void CInternetUsageLimitDlg::ReadFromFile()
{
    if((g_SetData.Internet_nPromptEnable == 0 || g_SetData.Internet_nPromptEnable == 1)
        && (g_SetData.Internet_nPromptFluxOrDuration == 0 || g_SetData.Internet_nPromptFluxOrDuration == 1)
        && (g_SetData.Internet_nPromptYear !=0 && g_SetData.Internet_nPromptMonth != 0
        && g_SetData.Internet_nPromptDay != 0)
		&&((g_SetData.Internet_nPromptDisconnEnable == 0) || (g_SetData.Internet_nPromptDisconnEnable == 1))
		&&((g_SetData.Internet_nPromptPwdConfirm == 0) ||(g_SetData.Internet_nPromptPwdConfirm ==1)))
    {
        m_bPrompt = (BOOL)g_SetData.Internet_nPromptEnable;
		m_bDisconnect = (BOOL)g_SetData.Internet_nPromptDisconnEnable;
		m_bPassword = (BOOL)g_SetData.Internet_nPromptPwdConfirm;
        m_DateTime.SetDate(g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
                            , g_SetData.Internet_nPromptDay);
        m_nFluxOrDuration = g_SetData.Internet_nPromptFluxOrDuration;
        m_nFlux = g_SetData.Internet_nPromptFluxVal;
        m_nHours = g_SetData.Internet_nPromptDurationVal / 60;
        m_nMinutes = g_SetData.Internet_nPromptDurationVal % 60;
    }
    else
        ResetToDefault();

    if(m_nFluxOrDuration == 0)
        OnRadio1();
    else
        OnRadio2();	

	ChangePWDCheckBoxStatus();
    UpdateData(FALSE);
}

void CInternetUsageLimitDlg::ResetToDefault()
{
    COleDateTime currtime = COleDateTime::GetCurrentTime();
    
    g_SetData.Internet_nPromptEnable = m_bPrompt = FALSE;
	g_SetData.Internet_nPromptDisconnEnable = m_bDisconnect = FALSE;
	g_SetData.Internet_nPromptPwdConfirm = m_bPassword = FALSE;
    g_SetData.Internet_nPromptYear = currtime.GetYear();
    g_SetData.Internet_nPromptMonth = currtime.GetMonth();
    g_SetData.Internet_nPromptDay = currtime.GetDay();
    m_DateTime.SetDate(g_SetData.Internet_nPromptYear, g_SetData.Internet_nPromptMonth
                        , g_SetData.Internet_nPromptDay);
    g_SetData.Internet_nPromptFluxOrDuration = m_nFluxOrDuration = 0;
    g_SetData.Internet_nPromptFluxVal = 50;
    m_nHours = 10; m_nMinutes = 0;
    g_SetData.Internet_nPromptDurationVal = m_nHours * 60 + m_nMinutes;
    SaveIniFile();
}

void CInternetUsageLimitDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
    GetDlgItem(IDC_EDIT_INT_USAGE_FLUX)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_INT_USAGE_DURA_H)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_INT_USAGE_DURA_M)->EnableWindow(FALSE);
}

void CInternetUsageLimitDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
    GetDlgItem(IDC_EDIT_INT_USAGE_FLUX)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_INT_USAGE_DURA_H)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_INT_USAGE_DURA_M)->EnableWindow(TRUE);
}

void CInternetUsageLimitDlg::OnOK() 
{
	// TODO: Add extra validation here

    if(!UpdateData(TRUE))
	{
		return;
	}
	
		WriteToFile();
		AfxMessageBox(IDS_SAVEOK, MB_ICONINFORMATION);
}

void CInternetUsageLimitDlg::ChangePWDCheckBoxStatus()
{
	if(m_bDisconnect)
	{
		m_bPrompt = FALSE;
		GetDlgItem(IDC_CHECK_INT_USAGE_PROMPT)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_INT_USAGE_PWD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_INT_USAGE_PROMPT)->EnableWindow(TRUE);

		m_bPassword = FALSE;		
		GetDlgItem(IDC_CHECK_INT_USAGE_PWD)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	return;
}

void CInternetUsageLimitDlg::OnCheckDisconn()
{
	UpdateData(TRUE);
	ChangePWDCheckBoxStatus();
	
	return;
}

void CInternetUsageLimitDlg::OnModifyPWD()
{
	CPwdModifyDlg ModifyPwdDlg(this);
	int nRes = ModifyPwdDlg.DoModal();
	if(nRes == IDOK)
	{
		CString strAppName;
		strAppName.LoadString(IDS_APPNAME);
		AddPrefixName(strAppName);
		CString strModifyPwdSucc;
		strModifyPwdSucc.LoadString(IDS_INTERNET_MODIFYPWDSUCC);
		this->MessageBox(strModifyPwdSucc,strAppName,MB_OK);
	}
	return;
}

void CInternetUsageLimitDlg::OnCheckPWD()
{
	return;
	CPwdConfirmDlg PwdConfirmDlg(this);
	int nRes = PwdConfirmDlg.DoModal();
	if(nRes == IDCANCEL)
	{
		UpdateData(FALSE);
	}
	return;
}
