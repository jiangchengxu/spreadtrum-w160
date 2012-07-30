// DlgInternetNewConn.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "DlgInternetNewConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInternetNewConn dialog


CDlgInternetNewConn::CDlgInternetNewConn(CWnd* pParent /*=NULL*/, INT nProfileType)
    : CBaseDialog(CDlgInternetNewConn::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgInternetNewConn)
    m_strConnName = _T("");
	m_strUserName = _T("");
	m_strPassWord = _T("");
    m_strPhoneNumber = _T("");
    m_strAPN = _T("");
	m_bSetDefault = FALSE;
    m_nAuthRadio = 0;
	//}}AFX_DATA_INIT
    m_nBkTag = 1;
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

    m_strAPN.Format(_T("%s"), g_SetData.Internet_szAPN);
}


void CDlgInternetNewConn::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgInternetNewConn)
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
	DDX_Text(pDX, IDC_EDIT_INTERNET_NEWCONN_APN, m_strAPN);
	DDV_MaxChars(pDX, m_strAPN, CONNECTION_APN_LEN);
	DDX_Radio(pDX, IDC_RADIO_CHAP, m_nAuthRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInternetNewConn, CBaseDialog)
    //{{AFX_MSG_MAP(CDlgInternetNewConn)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInternetNewConn message handlers

BOOL CDlgInternetNewConn::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_NumEditPhoneNumber.m_bFlag = TRUE;

	if (0 == m_nProfileType) {
		GetDlgItem(IDC_EDIT_INTERNET_NEWCONN_PHONENUMBER)->EnableWindow(FALSE);
	}

	//SetSkin(g_SetData.Setup_nSkinStyle);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgInternetNewConn::OnOK() 
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
			|| (m_strPassWord.GetLength() > PWLEN)
			|| (m_strAPN.GetLength() > CONNECTION_APN_LEN))
		{
			AfxMessageBox(IDS_INT_ERR_INPUTLONG);
		}
		else
		{
			CBaseDialog::OnOK();
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
			|| (m_strPassWord.GetLength() > PWLEN)
			|| (m_strAPN.GetLength() > CONNECTION_APN_LEN))
		{
			AfxMessageBox(IDS_INT_ERR_INPUTLONG);
		}
		else
		{
			CBaseDialog::OnOK();
		}
	}
}
