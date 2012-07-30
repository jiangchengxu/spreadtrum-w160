// PwdModifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hsdpa.h"
#include "PwdModifyDlg.h"

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
// CPwdModifyDlg dialog


CPwdModifyDlg::CPwdModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPwdModifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPwdModifyDlg)
	m_strPwdCurr = _T("");
	m_strPwdVerify = _T("");
	m_strPwdNew = _T("");
	//}}AFX_DATA_INIT
//	m_nBkTag = 1;
}


void CPwdModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPwdModifyDlg)
	DDX_Text(pDX, IDC_EDIT_MPWD_CURR, m_strPwdCurr);
	DDV_MaxChars(pDX, m_strPwdCurr, 32);
	DDX_Text(pDX, IDC_EDIT_MPWD_CONF, m_strPwdVerify);
	DDV_MaxChars(pDX, m_strPwdVerify, 32);
	DDX_Text(pDX, IDC_EDIT_MPWD_NEW, m_strPwdNew);
	DDV_MaxChars(pDX, m_strPwdNew, 32);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPwdModifyDlg, CDialog)
	//{{AFX_MSG_MAP(CPwdModifyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPwdModifyDlg message handlers

BOOL CPwdModifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

// 	m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPwdModifyDlg::OnOK() 
{
	// TODO: Add extra validation here
	USES_CONVERSION;

	UpdateData(TRUE);
	CString strAppName;
	strAppName.LoadString(IDS_APPNAME);
	AddPrefixName(strAppName);

	CString strIniCurrPwd = _T("");
	CString strPwdXor = _T("");
	strIniCurrPwd.Format(g_SetData.Internet_szCurrentPwd);
	if(strIniCurrPwd.Compare(_T(""))!=0)
	{
		char *strPwd=W2A(m_strPwdCurr);
		strPwdXor.Format(PWDXOR(strPwd));
	}
	//if(0 != strIniCurrPwd.Compare(m_strPwdCurr))
	if(0 != strIniCurrPwd.Compare(strPwdXor))
	{
		//PWD incorrect		
		CString strPwdErr;
		strPwdErr.LoadString(IDS_INTERNET_PWDERR);
		this->MessageBox(strPwdErr,strAppName,MB_OK);

		return;
	}
	if(0 != m_strPwdNew.Compare(m_strPwdVerify))
	{
		//PWd diff
		CString strPwdDiff;
		strPwdDiff.LoadString(IDS_INTERNET_PWDDIFF);
		this->MessageBox(strPwdDiff,strAppName,MB_OK);

		return;
	}
	WritePwdtoINIFile();
	
	CDialog::OnOK();
}

void CPwdModifyDlg::WritePwdtoINIFile()
{
	wcscpy(g_SetData.Internet_szCurrentPwd,m_strPwdNew);
	SaveIniFilePwd();

	return;
}




