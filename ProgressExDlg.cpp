// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "ProgressExDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressExDlg dialog


CProgressExDlg::CProgressExDlg(CWnd* pParent /*=NULL*/, int nUpper, int nStep)
    : m_nUpper(nUpper), m_nLower(0), m_nStep(nStep), CDialog(CProgressExDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CProgressExDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CProgressExDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProgressExDlg)
    DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
    DDX_Text(pDX, IDC_STATIC_TEXT, m_strText);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressExDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressExDlg)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressExDlg message handlers

BOOL CProgressExDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) {
        switch(pMsg->wParam) {
        case VK_RETURN:
        case VK_ESCAPE:
            return true;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CProgressExDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_Progress.SetRange(m_nLower, m_nUpper);
    m_Progress.SetStep(m_nStep);
    m_strText.Empty();
    return TRUE;
}