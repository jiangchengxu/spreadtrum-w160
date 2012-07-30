// SearchContactDlg.cpp : implementation file
/*=================================================================
FILE��   SearchContactDlg.cpp                         VERSION : 


DESCRIPTION
  ʵ�ְ�������Ĳ�ѯ�����ҵ��绰����¼�Ĺ��ܡ�

=================================================================*/

/*=================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
 Notice that changes are listed in reverse chronological order.

 when        who         what, where, why
 --------   -----       ---------------------------------------------
 
=================================================================*/
//Function List:  // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵����
	/*
	CSearchContactDlg:: OnButtonSearch()���������ڲ��������ѯ�����ĵ绰����¼
	CSearchContactDlg::OnButtonAhead()������������ʾ������������һ���绰����¼	
	CSearchContactDlg::OnButtonNext()������������ʾ������������һ���绰����¼
	*/

	

#include "stdafx.h"
#include "HSDPA.h"
#include "SearchContactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchContactDlg dialog

CSearchContactDlg::CSearchContactDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSearchContactDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSearchContactDlg)
        // NOTE: the ClassWizard will add member initialization here
    m_strName = _T("");
    m_strMobile = _T("");
    m_strFindNum = "0 / 0";
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    TotalIndexNum = 0;
    CurIndexNum = 0;
	m_IndexArr = NULL;
	m_PBListTotalCount = 0;
    //}}AFX_DATA_INIT
 // m_nBkTag = 1;
}

CSearchContactDlg::~CSearchContactDlg()
{
	if (NULL != m_IndexArr)
	{
		delete []m_IndexArr;
		m_IndexArr = NULL;
	}
}

void CSearchContactDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSearchContactDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
	
	DDX_Control(pDX, IDC_EDIT_MOBILE, m_EditSerachNum);
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDV_MaxChars(pDX, m_strName, 12);
    DDX_Text(pDX, IDC_EDIT_MOBILE, m_strMobile);
    DDV_MaxChars(pDX, m_strMobile, 32);
    DDX_Text(pDX,IDC_STATIC_NUM,m_strFindNum);
    
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchContactDlg, CDialog)
    //{{AFX_MSG_MAP(CSearchContactDlg)
    ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
    ON_BN_CLICKED(IDC_BUTTON_AHEAD, OnButtonAhead)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
//    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
    ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchContactDlg message handlers

BOOL CSearchContactDlg::OnInitDialog() 
{

    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
//     m_bFirst = TRUE;
// 
// 	SetSkin(g_SetData.Setup_nSkinStyle);

	m_EditSerachNum.m_bFlag = true;
    
    GetDlgItem(IDC_BUTTON_AHEAD)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchContactDlg::OnPaint() 
{
    CDialog::OnPaint();    
}

void CSearchContactDlg::OnButtonSearch() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
	
	//if name and mobile are all empty,disable "Ahead" and "Next" button
    if(m_strName.GetLength() == 0 && m_strMobile.GetLength() == 0)
    {
        CurIndexNum = 0;
        TotalIndexNum = 0;
        
        GetDlgItem(IDC_BUTTON_AHEAD)->EnableWindow(false);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);    
        
        m_strFindNum = "0 / 0";
        UpdateData(FALSE);
        return;
    }
	//get total list count
	m_PBListTotalCount = 0;
	WPARAM wParam = (DWORD)&m_PBListTotalCount;
    LPARAM lParam = 0; 
	::SendMessage(GetParent()->GetSafeHwnd(), WM_SEARCH_GETTOTALCOUNT, wParam, lParam);
	   
	if(0 < m_PBListTotalCount)
	{
		if (NULL != m_IndexArr)
		{
			delete []m_IndexArr;
			m_IndexArr = NULL;
		}
		m_IndexArr = new int[m_PBListTotalCount];
		memset(m_IndexArr, 0, (sizeof(int) * m_PBListTotalCount));
	}

	wParam = (DWORD)m_IndexArr;
	lParam = (DWORD)&TotalIndexNum;
    ::SendMessage(GetParent()->GetSafeHwnd(),WM_SEARCH_CONTACT,wParam, lParam);

	//���ҵ�����ϵ��������0
    if(TotalIndexNum > 0)
    {
        CurIndexNum = 0;

        GetDlgItem(IDC_BUTTON_AHEAD)->EnableWindow(true);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);    
        
        m_strFindNum.Format(_T("%d / %d"),CurIndexNum + 1,TotalIndexNum);

        //����Ϣ��������
        WPARAM wParam = (long)m_IndexArr[CurIndexNum];
        LPARAM lParam = 0;    
        ::SendMessage(GetOwner()->GetSafeHwnd(),WM_SEL_CONTACT_INDEX,wParam, lParam);
    }
	//δ���ҵ���ϵ��
    else
    {
        CurIndexNum = 0;
        TotalIndexNum = 0;
        //memset(m_IndexArr,0,sizeof(int)*PB_RECORD_MAX);
        
        GetDlgItem(IDC_BUTTON_AHEAD)->EnableWindow(false);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);    
        
        m_strFindNum = "0 / 0";
    }
	
    UpdateData(FALSE);    
    Invalidate();
}

//����"Ahead"��ť
void CSearchContactDlg::OnButtonAhead() 
{
    // TODO: Add your control notification handler code here
    if(CurIndexNum > 0)
    {
        CurIndexNum --;
        m_strFindNum.Format(_T("%d / %d"),CurIndexNum + 1,TotalIndexNum);
        UpdateData(FALSE);
        //����Ϣ��������
        WPARAM wParam = (long)m_IndexArr[CurIndexNum];
        LPARAM lParam = 0;    
        ::SendMessage(GetOwner()->GetSafeHwnd(),WM_SEL_CONTACT_INDEX,wParam, lParam);
    }

    Invalidate();
}

//����"Next"��ť
void CSearchContactDlg::OnButtonNext() 
{
    // TODO: Add your control notification handler code here
    if(CurIndexNum < TotalIndexNum-1)
    {
        CurIndexNum++;
        m_strFindNum.Format(_T("%d / %d"),CurIndexNum+1,TotalIndexNum);
        UpdateData(FALSE);
        //����Ϣ��������
        WPARAM wParam = (long)m_IndexArr[CurIndexNum];
        LPARAM lParam = 0;    
        ::SendMessage(GetOwner()->GetSafeHwnd(),WM_SEL_CONTACT_INDEX,wParam, lParam);
    }    
    Invalidate();
}
