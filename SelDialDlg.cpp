// SelDialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SelDialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelDialDlg dialog


CSelDialDlg::CSelDialDlg(CWnd* pParent/*=NULL*/)
    : CBaseDialog(CSelDialDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelDialDlg)
        // NOTE: the ClassWizard will add member initialization here

    //}}AFX_DATA_INIT

    m_strMobile      = _T("");
    m_strHome        = _T("");
    m_strCompany     = _T("");
    m_strSelCallNum  = _T("");
	m_strName        = _T("");

	m_nBkTag = 1;
}


void CSelDialDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelDialDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_COMBO_SELNUM, m_ComboNum);
	DDX_Text(pDX, IDC_STATIC_NAME, m_strName);
	DDX_Text(pDX, IDC_STATIC_NUMBER, m_strSelCallNum);
    
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelDialDlg, CBaseDialog)
    //{{AFX_MSG_MAP(CSelDialDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelDialDlg message handlers

BOOL CSelDialDlg::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();

//	SetSkin(g_SetData.Setup_nSkinStyle);

    CString CNumTemp;
    // TODO: Add extra initialization here
    if (m_strMobile.CompareNoCase(_T("")) != 0)
    {
        CNumTemp.LoadString(IDS_MOBILE);
        m_ComboNum.AddString(CNumTemp);
    }
    if (m_strHome.CompareNoCase(_T("")) != 0)
    {
        CNumTemp.LoadString(IDS_HOME);
        m_ComboNum.AddString(CNumTemp);
    }
    if (m_strCompany.CompareNoCase(_T("")) != 0)
    {
        CNumTemp.LoadString(IDS_COMPANY);
        m_ComboNum.AddString(CNumTemp);
    }    
    
    m_ComboNum.SetCurSel(0);

	GetSelNumber();	
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

//获取下拉框中当前选择的列表的号码
void CSelDialDlg::GetSelNumber() 
{
	/*
    0:mobile;
    1:Home;
    2:Company;
    */
	CString strText;

	int nIndex = m_ComboNum.GetCurSel();
    m_ComboNum.GetLBText(nIndex,strText);

	//fix get number in Italy version
	CString strMobile, strHome, strCompany;
	strMobile.LoadString(IDS_MOBILE);
	strHome.LoadString(IDS_HOME);
	strCompany.LoadString(IDS_COMPANY);
	if (!strText.CompareNoCase(strMobile))
    {
        m_strSelCallNum = m_strMobile;
    }
    else if (!strText.CompareNoCase(strHome))
    {
        m_strSelCallNum = m_strHome;
    }
    else if (!strText.CompareNoCase(strCompany))
    {
        m_strSelCallNum = m_strCompany;
    }

	UpdateData(FALSE);

	return;
}

void CSelDialDlg::OnOK() 
{
    
    // TODO: Add extra validation here
 
    //GetSelNumber();

    CBaseDialog::OnOK();
}

void CSelDialDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	GetSelNumber();	
}
