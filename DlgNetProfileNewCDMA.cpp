// DlgNetProfileNewCDMA.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "DlgNetProfileNewCDMA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNetProfileNewCDMA dialog


CDlgNetProfileNewCDMA::CDlgNetProfileNewCDMA(CWnd* pParent /*=NULL*/, INT nProfileType)
	: CDialog(CDlgNetProfileNewCDMA::IDD, pParent)
{
	m_strConnName = _T("");
	m_strUserName = _T("");
	m_strPassWord = _T("");
    m_strPhoneNumber = _T("");
	m_bSetDefault = FALSE;
    m_nAuthRadio = 0;
	//}}AFX_DATA_INIT

    m_nAuthRadio = g_SetData.Internet_nAuthType;
	
	//	m_strConnName.Format("%s (Edit.)", _T("TIM WEB"));
	
    m_strPhoneNumber.Format(_T("%s"), g_SetData.Internet_szLocalPhoneNumber);
	m_nProfileType = nProfileType;
	if (0 == nProfileType) {
		m_pNdisConnProf = ((CHSDPAApp *)AfxGetApp())->GetConnProfile();
	}
	else if (1 == nProfileType) {
		m_strPhoneNumber.Format(_T("%s"), g_SetData.Internet_szLocalPhoneNumber);
		m_pConnProf = ((CHSDPAApp *)AfxGetApp())->GetConnProfile();
	}
	else {
		ASSERT(TRUE);
	}
}


void CDlgNetProfileNewCDMA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INTERNET_NEWCONN_PHONENUMBER, m_NumEditPhoneNumber);
    DDX_Text(pDX, IDC_EDIT_INTERNET_NEWCONN_CONNNAME, m_strConnName);
	DDV_MaxChars(pDX, m_strConnName, RAS_MaxEntryName);
	DDX_Text(pDX, IDC_EDIT_INTERNET_NEWCONN_USERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, UNLEN);
	DDX_Text(pDX, IDC_EDIT_INTERNET_NEWCONN_PASSWORD, m_strPassWord);
	DDV_MaxChars(pDX, m_strPassWord, PWLEN);
	DDX_Text(pDX, IDC_EDIT_INTERNET_NEWCONN_PHONENUMBER, m_strPhoneNumber);
	DDV_MaxChars(pDX, m_strPhoneNumber, RAS_MaxPhoneNumber);
	DDX_Check(pDX, IDC_CHECK_INTERNET_NEWCONN_SETDFT, m_bSetDefault);
	
	DDX_Radio(pDX, IDC_RADIO_CHAP, m_nAuthRadio);
}


BEGIN_MESSAGE_MAP(CDlgNetProfileNewCDMA, CDialog)
	//{{AFX_MSG_MAP(CDlgNetProfileNewCDMA)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetProfileNewCDMA message handlers
BOOL CDlgNetProfileNewCDMA::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_NumEditPhoneNumber.m_bFlag = TRUE;
	
	if (0 == m_nProfileType) {
		GetDlgItem(IDC_EDIT_INTERNET_NEWCONN_PHONENUMBER)->EnableWindow(FALSE);
	}
	

	
    return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgNetProfileNewCDMA::OnOK() 
{
    UpdateData(TRUE);
    
    //判断输入组合是否合法
	if (0 == m_nProfileType) {
		if(m_strConnName.GetLength() == 0
			/*|| m_strPhoneNumber.GetLength() == 0*/)
		{
			AfxMessageBox(IDS_INT_ERR_INPUTNULL);
		}
		else if(m_pNdisConnProf->IsExistent(m_strConnName))
		{
			AfxMessageBox(IDS_INT_ERR_SAMENAME);
		}
		else if((m_strConnName.GetLength() > RAS_MaxEntryName)
			//|| (m_strPhoneNumber.GetLength() > RAS_MaxPhoneNumber)
			|| (m_strUserName.GetLength() > UNLEN)
			|| (m_strPassWord.GetLength() > PWLEN))
		{
			AfxMessageBox(IDS_INT_ERR_INPUTLONG);
		}
		else
		{
			CDialog::OnOK();
		}
	}
	else if (1 == m_nProfileType) {
		if(m_strConnName.GetLength() == 0
			|| m_strPhoneNumber.GetLength() == 0)
		{
			AfxMessageBox(IDS_INT_ERR_INPUTNULL);
		}
		else if(m_pConnProf->IsExistent(m_strConnName))
		{
			AfxMessageBox(IDS_INT_ERR_SAMENAME);
		}
		else if((m_strConnName.GetLength() > RAS_MaxEntryName)
			|| (m_strPhoneNumber.GetLength() > RAS_MaxPhoneNumber)
			|| (m_strUserName.GetLength() > UNLEN)
			|| (m_strPassWord.GetLength() > PWLEN))
		{
			AfxMessageBox(IDS_INT_ERR_INPUTLONG);
		}
		else
		{
			CDialog::OnOK();
		}
	}
}
