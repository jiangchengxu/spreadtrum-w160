// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "ProgressDlg.h"
#include "Resource.h"
#include "Ds.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/, int nUpper, int nStep)
: m_nUpper(nUpper), m_nLower(0), m_nStep(nStep), CDialog(CProgressDlg::IDD, pParent)
{
//	m_nBkTag = 1;
	//m_str_sms_Prompt = _T("Waiting ...");
    //{{AFX_DATA_INIT(CProgressDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT 
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	//m_str_sms_Prompt = "Waiting...";
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProgressDlg)
    DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_STATIC_SMS, m_str_sms_Prompt);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressDlg)
        // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN)
    {
        switch(pMsg->wParam)
        {
        case VK_RETURN:
        case VK_ESCAPE:
            return true;
        }
    }    
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CProgressDlg::OnInitDialog()
{
    
	
    CDialog::OnInitDialog();

//     m_bFirst = TRUE;
// 
// 	SetSkin(g_SetData.Setup_nSkinStyle);

    m_Progress.SetRange(m_nLower, m_nUpper);
    m_Progress.SetStep(m_nStep);
    return TRUE;
}