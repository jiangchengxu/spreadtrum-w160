// DlgNetSettingCDMA.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "DlgNetSettingCDMA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNetSettingCDMA dialog


CDlgNetSettingCDMA::CDlgNetSettingCDMA(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgNetSettingCDMA::IDD, pParent)
{
	m_nRoamingMode=0;
	m_nBkTag = 1;
}


void CDlgNetSettingCDMA::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Radio(pDX,IDC_RADIO_AUTOMATIC,m_nRoamingMode);
}


BEGIN_MESSAGE_MAP(CDlgNetSettingCDMA, CBaseDialog)

  ON_BN_CLICKED(IDC_RADIO_AUTOMATIC,OnButtonAutomatic)   
  ON_BN_CLICKED(IDC_RADIO_AUTOMATIC,OnButtonManual)
  ON_BN_CLICKED(IDC_BUTTON2,OnCancel)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetSettingCDMA message handlers

BOOL CDlgNetSettingCDMA::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
  m_nRoamingMode=0;
  UpdateData(FALSE);
  return TRUE;
}

void CDlgNetSettingCDMA::OnButtonManual()
{

}

void CDlgNetSettingCDMA::OnButtonAutomatic()
{

}

void CDlgNetSettingCDMA::OnCancel()
{
	AfxMessageBox(_T("Save successfully!"));
	CDialog::OnCancel();

}