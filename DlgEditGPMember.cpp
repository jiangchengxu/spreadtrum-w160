// DlgEditGPMember.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "DlgEditGPMember.h"
#include "ContactDlg.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEditGPMember dialog


CDlgEditGPMember::CDlgEditGPMember(CWnd* pParent, CString groupName)
	: CBaseDialog(CDlgEditGPMember::IDD, pParent)
{
	m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
	m_nBkTag = 1;
	m_strGroupName=groupName;
}


void CDlgEditGPMember::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_PHONEBOOK,m_ListPhoneBook);
	DDX_Control(pDX,IDC_LIST_GROUPMEMBER,m_ListGPMember);
	
}


BEGIN_MESSAGE_MAP(CDlgEditGPMember, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_TORIGHT,OnButtonToRight)
	ON_BN_CLICKED(IDC_BUTTON_TOLEFT,OnButtonToLeft)
	ON_BN_CLICKED(ID_BUTTON_MEMBEROK,OnButtonOK)
//	ON_BN_CLICKED(ID_BUTTON_MEMBERVIEW,OnButtonView)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PHONEBOOK, OnDblclkListPhonebook)
	ON_NOTIFY(NM_DBLCLK,IDC_LIST_GROUPMEMBER,OnDblclkListGPMember)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditGPMember message handlers

BOOL CDlgEditGPMember::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
//	m_bFirst = TRUE;
//	SetSkin(g_SetData.Setup_nSkinStyle);
	InitListPhoneBook();
	InitListGPMember();
	return true;
}

void CDlgEditGPMember::InitListPhoneBook()
{

	m_ListPhoneBook.DeleteAllItems();
	
	int nindex = 0;
	for(int i = 0; i < PB_RECORD_MAX; i++)
	{
		if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,_T(""))==0)
		{
			m_ListPhoneBook.InsertItem(nindex, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]),0);
			m_ListPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
			m_ListPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
			m_ListPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
			m_ListPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
			m_ListPhoneBook.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
			nindex ++;
		}
	}

}


void CDlgEditGPMember::InitListGPMember()
{
	
	m_ListGPMember.DeleteAllItems();
	int nindex = 0;

	for(int i = 0; i < PB_RECORD_MAX; i++)
	{
		if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,m_strGroupName)==0)
		{
			m_ListGPMember.InsertItem(nindex, m_pPbData->PbDatas[i].strName, (DWORD)(&m_pPbData->PbDatas[i]),0);
			m_ListGPMember.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_MOBILE, m_pPbData->PbDatas[i].strMobile);
			m_ListGPMember.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_HOME, m_pPbData->PbDatas[i].strHome);
			m_ListGPMember.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_COMPANY, m_pPbData->PbDatas[i].strCompany);
			m_ListGPMember.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_EMAIL, m_pPbData->PbDatas[i].strEmail);
			m_ListGPMember.SetItemText(nindex, PHONEBOOK_LISTCTRL_HEADERINDEX_REMARK, m_pPbData->PbDatas[i].strMark);
			nindex ++;
		}
	}
	
}

void CDlgEditGPMember::OnButtonToRight()
{
    UpdateData(TRUE);
	int nCount = m_ListPhoneBook.GetSelectedCount();
	LVITEM item;
	item.iSubItem = 0;
	int* nDelItem = NULL;
	nDelItem = new int[nCount];
	memset(nDelItem,0,sizeof(int) * nCount);
	
	DWORD* DelRecords=NULL;
	DelRecords=new DWORD[nCount];
	memset(DelRecords, 0, (sizeof(DWORD) * nCount));
	POSITION  DelSelPos = m_ListPhoneBook.GetFirstSelectedItemPosition();
	int nIndex = 0;
	while(DelSelPos)
	{
		int nSelItem=m_ListPhoneBook.GetNextSelectedItem(DelSelPos);
		DelRecords[nIndex]=m_ListPhoneBook.GetItemData(nSelItem);
		nDelItem[nIndex ++] = nSelItem;
	}

	int nDelCount=nCount;
	int nArrIndex = 0;
	while(nDelCount > 0)
	{
		
		StPbRecord *PbBookTemp = (StPbRecord *)DelRecords[nArrIndex];
		m_ListGPMember.InsertItem(m_ListGPMember.GetItemCount(),PbBookTemp->strName,(DWORD)PbBookTemp,0);
		nDelCount --;
		nArrIndex ++;
		
	}
		
	for(int i = nCount - 1; i >= 0; i --)
	{
		m_ListPhoneBook.DeleteItem(nDelItem[i]);
	}
	delete []nDelItem;
	delete []DelRecords;
}

void CDlgEditGPMember::OnButtonToLeft()
{
	UpdateData(TRUE);
	int nCount = m_ListGPMember.GetSelectedCount();
	LVITEM item;
	item.iSubItem = 0;
	int* nDelItem = NULL;
	nDelItem = new int[nCount];
	memset(nDelItem,0,sizeof(int) * nCount);
	
	DWORD* DelRecords=NULL;
	DelRecords=new DWORD[nCount];
	memset(DelRecords, 0, (sizeof(DWORD) * nCount));
	POSITION  DelSelPos = m_ListGPMember.GetFirstSelectedItemPosition();
	int nIndex = 0;
	while(DelSelPos)
	{
		int nSelItem=m_ListGPMember.GetNextSelectedItem(DelSelPos);
		DelRecords[nIndex]=m_ListGPMember.GetItemData(nSelItem);
		nDelItem[nIndex ++] = nSelItem;
	}
	
	int nDelCount=nCount;
	int nArrIndex = 0;
	while(nDelCount > 0)
	{
		
		StPbRecord *PbBookTemp = (StPbRecord *)DelRecords[nArrIndex];
		m_ListPhoneBook.InsertItem(m_ListPhoneBook.GetItemCount(),PbBookTemp->strName,(DWORD)PbBookTemp,0);
		nDelCount --;
		nArrIndex ++;
		
	}
	
	for(int i = nCount - 1; i >= 0; i --)
	{
		m_ListGPMember.DeleteItem(nDelItem[i]);
	}
	delete []nDelItem;
	delete []DelRecords;
	
}

void CDlgEditGPMember::OnButtonOK()
{
	int nGPMemberCount=m_ListGPMember.GetItemCount();
	for(int i=0;i<nGPMemberCount;i++)
	{
		StPbRecord *GPMemberTemp = (StPbRecord *)m_ListGPMember.GetItemData(i);	
		for(int j=0;j<PB_RECORD_MAX;j++)
		{
			if(m_pPbData->PbDatas[j].index==GPMemberTemp->index)
			{
					wcscpy(m_pPbData->PbDatas[j].strGroup,m_strGroupName);
			}
			
		}
	}

	int nPhoneBookCount=m_ListPhoneBook.GetItemCount();
	for(int k=0;k<nPhoneBookCount;k++)
	{
		StPbRecord *PhoneBookTemp = (StPbRecord *)m_ListPhoneBook.GetItemData(k);	
		for(int j=0;j<PB_RECORD_MAX;j++)
		{
			if(m_pPbData->PbDatas[j].index==PhoneBookTemp->index)
			{
				wcscpy(m_pPbData->PbDatas[j].strGroup,_T(""));
			}
			
		}
	}

	if(m_pPbData->UpdateFile())
	{
		GetParent()->SendMessage(WM_REFRESH_GROUP, NULL, NULL);
			CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
		pMainWnd->m_pPhoneBookDlg->nCurSelIndex=m_nGroupSelected;
		pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_ReDrawListCtrl,NULL,NULL); //Refresh phone book in PhoneBookDlg
		//GetParent()->GetParent()->GetParent()->PostMessage(WM_PB_UPDATECONTACTNUMBER,NULL,NULL);
	
		pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_UPDATECONTACTNUMBER,NULL,NULL);
	}
	OnCancel();
 	return;
	
}

/*void CDlgEditGPMember::OnButtonView() 
{
    // TODO: Add your control notification handler code here

	CFile FileTemp;
    int PbNum1,PbNum2;

	PbNum1 = m_ListPhoneBook.GetNextItem(-1, LVNI_SELECTED);
	PbNum2 = m_ListGPMember.GetNextItem(-1, LVNI_SELECTED);
	if((PbNum1 < 0)&&(PbNum2 < 0))
    {
        AfxMessageBox(IDS_PB_SELNULL);
        return;
    }

	CContactDlg dlg(this,CONTACTDLG_TYPE_NORMAL);

	StPbRecord *PbBookTemp = NULL ;
	if(PbNum1>=0)
		PbBookTemp = (StPbRecord*)m_ListPhoneBook.GetItemData(PbNum1);
	if(PbNum2>=0)
		PbBookTemp = (StPbRecord*)m_ListGPMember.GetItemData(PbNum2);
	dlg.m_strName = PbBookTemp->strName;
	dlg.m_strMobile = PbBookTemp->strMobile;
	dlg.m_strHome = PbBookTemp->strHome;
	dlg.m_strCompany = PbBookTemp->strCompany;
	dlg.m_strEmail = PbBookTemp->strEmail;
	dlg.m_strMark = PbBookTemp->strMark;	
	dlg.m_strPicPath = PbBookTemp->strPicPath;
	dlg.m_strGroup=PbBookTemp->strGroup;
	dlg.m_strGroupBeforechange=PbBookTemp->strGroup;
	dlg.m_pPbData=m_pPbData;
	dlg.DoModal();
				
}*/

LRESULT CDlgEditGPMember::OnDblclkListPhonebook(WPARAM wParam, LPARAM lParam)
{
	OnButtonToRight();
	return 0;

}

LRESULT CDlgEditGPMember::OnDblclkListGPMember(WPARAM wParam, LPARAM lParam)
{
	OnButtonToLeft();
    return 0;
}

