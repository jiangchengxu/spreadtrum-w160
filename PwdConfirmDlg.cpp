// PwdConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "PwdConfirmDlg.h"

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
// CPwdConfirmDlg dialog


CPwdConfirmDlg::CPwdConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPwdConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPwdConfirmDlg)
	m_strCurrPwd = _T("");
	//}}AFX_DATA_INIT
//	m_nBkTag = 1;
}


void CPwdConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPwdConfirmDlg)
	DDX_Text(pDX, IDC_EDIT_PWDCONFIRM, m_strCurrPwd);
	DDV_MaxChars(pDX, m_strCurrPwd, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPwdConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CPwdConfirmDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPwdConfirmDlg message handlers

BOOL CPwdConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
// 	m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPwdConfirmDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	USES_CONVERSION;
	UpdateData(TRUE);
	CString strIniCurrPwd = _T("");
	CString strPwdXor = _T("");
	strIniCurrPwd.Format(g_SetData.Internet_szCurrentPwd);
 	if(strIniCurrPwd.Compare(_T(""))!=0)
 	{
    //char *strPwd=W2A(strIniCurrPwd);
		char *strPwd=W2A(m_strCurrPwd);
		strPwdXor.Format(PWDXOR(strPwd));
		//if(0 != m_strCurrPwd.Compare(strIniCurrPwd))
 	}
	else
	{
		if(m_strCurrPwd.Compare(_T(""))!=0)
		{
			CString strAppName;
			strAppName.LoadString(IDS_APPNAME);
			AddPrefixName(strAppName);
			CString strPwdErr;
			strPwdErr.LoadString(IDS_INTERNET_PWDERR);
			this->MessageBox(strPwdErr,strAppName,MB_OK);
			return;
		}

	}
		if(0 != strPwdXor.Compare(strIniCurrPwd))
		{
			CString strAppName;
			strAppName.LoadString(IDS_APPNAME);
			AddPrefixName(strAppName);
			CString strPwdErr;
			strPwdErr.LoadString(IDS_INTERNET_PWDERR);
			this->MessageBox(strPwdErr,strAppName,MB_OK);
			return;
		}
		

	CDialog::OnOK();
}
