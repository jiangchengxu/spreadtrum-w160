// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "AboutDialog.h"
#include "string.h"
#include "BaseDialog.h"
#include "HSDPADlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog

CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/) 
: CBaseDialog(CAboutDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAboutDialog)
	m_strSWVersion = _T("");
	m_strFWVersion = _T("");
	m_strIMEI = _T("");
	m_strCopyright = _T("");
	m_strAppName = _T("");
	//}}AFX_DATA_INIT
    m_nBkTag = 1;    
}

BOOL CAboutDialog::OnInitDialog() 
{    
    CBaseDialog::OnInitDialog();
    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);

	#ifdef FEATURE_VERSION_NASHUA
		GetDlgItem(IDC_STATIC_FREENUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HOMEPAGE)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_BUTTON_MAILTO)->ShowWindow(SW_SHOW);
	#else
		GetDlgItem(IDC_STATIC_FREENUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HOMEPAGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAILTO)->ShowWindow(SW_HIDE);
	#endif

	//CString strUrl;
	//strUrl.LoadString(IDS_URL);
    //m_Linkurl.SetURL(strUrl);
	m_Linkurl.SetURL(g_SetData.Main_szURL);
	//CString strEmail;
	//strEmail.LoadString(IDS_EMAIL);
    //m_EMail.SetURL(strEmail);
	m_EMail.SetURL(g_SetData.Main_szEMail);

	CHSDPADlg* pDlg = (CHSDPADlg*)AfxGetMainWnd();
	m_strIMEI = pDlg->m_strIMEI;
	m_strFWVersion = pDlg->m_strFWVersion;
    m_strSWVersion = pDlg->m_strSWVersion;

	CString temp;
	temp.LoadString(IDS_ABOUT_COPYRIGHT);
	//m_strCopyright.Format(temp, g_SetData.Main_szCompany);

	temp.LoadString(IDS_APPNAME);
	m_strAppName.Format(_T("%s %s"), g_SetData.Main_szPrefixName, temp);
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDialog)
	DDX_Text(pDX, IDC_SWVERSION, m_strSWVersion);
	DDX_Text(pDX, IDC_FWVERSION, m_strFWVersion);
	DDX_Text(pDX, IDC_IMEI, m_strIMEI);
	DDX_Text(pDX, IDC_COPYRIGHT, m_strCopyright);
    DDX_Control(pDX, IDC_STATIC_URL, m_Linkurl);
    DDX_Control(pDX, IDC_STATIC_EMAIL, m_EMail);
	DDX_Text(pDX, IDC_STA_APPNAME, m_strAppName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDialog, CBaseDialog)
    //{{AFX_MSG_MAP(CAboutDialog)
    ON_BN_CLICKED(IDC_BUTTON_HOMEPAGE, OnButtonHomePage)
	ON_BN_CLICKED(IDC_BUTTON_MAILTO, OnButtonMailTo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDialog::OnButtonHomePage()
{
	m_Linkurl.OnClicked();
}

void CAboutDialog::OnButtonMailTo()
{
	m_EMail.OnClicked();
}
