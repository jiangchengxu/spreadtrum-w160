// PbSelPositon.cpp : implementation file
//

#include "stdafx.h"
#include "HSDPA.h"
#include "PbSelPositon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPbSelPositon dialog


CPbSelPositon::CPbSelPositon(CString Name,CString Num,CWnd* pParent /*=NULL*/)
    : CBaseDialog(CPbSelPositon::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPbSelPositon)
        // NOTE: the ClassWizard will add member initialization here
    StrName = Name;
    StrNum  = Num;
    //}}AFX_DATA_INIT
	m_nBkTag = 1;
}


void CPbSelPositon::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPbSelPositon)
    
    DDX_Control(pDX, IDC_STATIC_Set, m_StaText );
    DDX_Control(pDX, IDC_STATIC_NAME, m_StaName);
    DDX_Control(pDX, IDC_STATIC_NUM, m_staNum);
    DDX_Control(pDX, IDC_STATIC_NAMEIN, m_StaNameIn);
    DDX_Control(pDX, IDC_STATIC_NUMIN, m_staNumIn);
        
    DDX_Text(pDX, IDC_STATIC_NAMEIN, StrName);
    DDX_Text(pDX, IDC_STATIC_NUMIN, StrNum);
    
    DDX_Control(pDX, IDC_CHECK_SET, m_Check);

    DDX_Control(pDX, IDC_RADIO_MOBILE, m_RMobile);
    DDX_Control(pDX, IDC_RADIO_HOME, m_RHome);
    DDX_Control(pDX, IDC_RADIO_COMPANY, m_RCompany);



    
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPbSelPositon, CBaseDialog)
    //{{AFX_MSG_MAP(CPbSelPositon)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPbSelPositon message handlers

BOOL CPbSelPositon::OnInitDialog() 
{
    CBaseDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    m_bFirst = TRUE;

	//SetSkin(g_SetData.Setup_nSkinStyle);

//    m_Flag = 1;
    
    switch (m_Flag)
    {
    case 1:
        {
            m_RMobile.SetCheck(1);

            break;
        }
    case 2:
        {
            m_RHome.SetCheck(1);

            break;
        }
    case 3:
        {
            m_RCompany.SetCheck(1);

            break;
        }
    default:
        {
            m_RMobile.SetCheck(1);    
            break;
        }
    }

    return TRUE; 
    // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPbSelPositon::OnOK() 
{
    // TODO: Add extra validation here
    
    if (1 == m_RMobile.GetCheck())
    {    
        m_Flag = 1;
    }
    else if(1 == m_RHome.GetCheck())
    {
        m_Flag = 2;
    }
    else
    {
        m_Flag = 3;        
    }
    m_nSet = m_Check.GetCheck();
    CBaseDialog::OnOK();
    Sleep(500);
}
