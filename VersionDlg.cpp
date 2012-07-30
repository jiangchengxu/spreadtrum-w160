// VersionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "HSDPADlg.h"
#include "VersionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVersionDlg dialog


CVersionDlg::CVersionDlg(CWnd* pParent, EnVersionType type)
	: CBaseDialog(CVersionDlg::IDD, pParent), m_VersionType(type)
{
	//{{AFX_DATA_INIT(CVersionDlg)
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
    m_nBkTag = 1;
}


void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVersionDlg)
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	DDV_MaxChars(pDX, m_strVersion, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVersionDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CVersionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVersionDlg message handlers

BOOL CVersionDlg::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	    
    // TODO: Add extra initialization here
    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);

	CHSDPADlg* pDlg = (CHSDPADlg*)AfxGetMainWnd();

    ASSERT(m_VersionType >= VERSION_IMEI && m_VersionType < VERSION_MAX);

    CString strTitle;
    strTitle.LoadString(szVersionTitle[m_VersionType]);
    SetWindowText(strTitle);

    switch(m_VersionType) {
    case VERSION_IMEI:
    	m_strVersion = pDlg->m_strIMEI;
    	break;
    case VERSION_FIRMWARE:
	    m_strVersion = pDlg->m_strFWVersion;
    default:
	    m_strVersion = pDlg->m_strFWVersion;
    }
    
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
