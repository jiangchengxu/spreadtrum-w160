// SendSms.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "SmsSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsSendDlg dialog


CSmsSendDlg::CSmsSendDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSmsSendDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSmsSendDlg)
    m_strSndTip = _T("");
    //}}AFX_DATA_INIT
}


void CSmsSendDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSmsSendDlg)
    DDX_Control(pDX, IDC_PROGRESS_SEND, m_SndProgress);
    DDX_Text(pDX, IDC_STATIC_TIP, m_strSndTip);
    DDV_MaxChars(pDX, m_strSndTip, 40);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsSendDlg, CDialog)
    //{{AFX_MSG_MAP(CSmsSendDlg)
        // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsSendDlg message handlers
