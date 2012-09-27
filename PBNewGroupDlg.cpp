// PBNewGroup.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "PBNewGroupDlg.h"
#include  "DlgEditGPMember.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPBNewGroup dialog


CPBNewGroupDlg::CPBNewGroupDlg(CWnd* pParent, CString groupName)
    : CBaseDialog(CPBNewGroupDlg::IDD, pParent)
{
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_nBkTag = 1;
    m_strGroupName=groupName;
    m_strPreGPName=groupName;
}


void CPBNewGroupDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_GROUPNAME, m_EditGroupName);
    DDX_Text(pDX,IDC_EDIT_GROUPNAME,m_strGroupName);
    DDV_MaxChars(pDX, m_strGroupName, 20);
    DDX_Control(pDX,IDC_LIST_GROUPMEMBERS,m_ListGroupMembers);

    //{{AFX_DATA_MAP(CPBNewGroup)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPBNewGroupDlg, CBaseDialog)

    ON_BN_CLICKED(ID_BUTTON_GROUP_OK,OnButtonOK)
    ON_BN_CLICKED(IDCANCEL,OnButtonCancel)
    ON_BN_CLICKED(ID_BUTTON_EDITMEMBER,OnButtonEdit)
    ON_MESSAGE(WM_REFRESH_GROUP,OnRefreshGroup)
END_MESSAGE_MAP()


BOOL CPBNewGroupDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
// 	m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);
    UpdateData(FALSE);
    initGroupList();
    if(m_iEditFlag==1) { //add group
        SetWindowText(_T("Edit Group"));
    }
    m_EditGroupName.SetFocus();

    return false;
}
/////////////////////////////////////////////////////////////////////////////
// CPBNewGroup message handlers
void CPBNewGroupDlg::OnPaint()
{
    CBaseDialog::OnPaint();
}

void CPBNewGroupDlg::initGroupList()
{
    if(m_strGroupName==_T(""))
        return;
    m_ListGroupMembers.ResetContent();
    for(int i = 0; i < PB_RECORD_MAX; i++) {
        int nindex = 0;
        if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,m_strGroupName)==0) {
            m_ListGroupMembers.InsertString(nindex,m_pPbData->PbDatas[i].strName);
            nindex ++;
        }

    }
}

void CPBNewGroupDlg::OnButtonOK()
{
    UpdateData(TRUE);
    StPbGroup *PbGroupTemp = new StPbGroup();
    memset(PbGroupTemp,0,sizeof(StPbGroup));
    CString StrWarningInputGroup;
    CString StrWarningGroupExisted;
    StrWarningInputGroup.LoadString(IDS_WARNING_INPUTGROUP);
    StrWarningGroupExisted.LoadString(IDS_WARNING_GROUPEXISTED);

    wcscpy(PbGroupTemp->strGroupName,(LPCTSTR)m_strGroupName);
    if(m_iEditFlag==0) { //add group
        if(m_strGroupName==_T("")) {
            //AfxMessageBox(_T("Please input a group name!"));
            AfxMessageBox(StrWarningInputGroup);
            return;
        }
        if(GroupExisted()) {
            //AfxMessageBox(_T("The same group already existed!"));
            AfxMessageBox(StrWarningGroupExisted);
            return;
        }
        DWORD PbArrItemAds = 0;
        if( !m_pPbData->AddGroup(*PbGroupTemp,&PbArrItemAds ) ) {
            if(NULL != PbGroupTemp) {
                delete PbGroupTemp;
            }
            return;
        } else {
            GetParent()->SendMessage(WM_REFRESH_GROUP, NULL, NULL);
            CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
            pMainWnd->m_pPhoneBookDlg->m_bNewGroupFlagUSIM=TRUE;
            pMainWnd->m_pPhoneBookDlg->m_bNewGroupFlagUSB=TRUE;
            pMainWnd->m_pPhoneBookDlg->nCurSelIndex=m_nGroupSelected;
            pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_REDRAWTREECTRL,1000,NULL);
            delete PbGroupTemp;
            OnCancel();
        }
    } else { //edit group
        if(m_strGroupName==_T("")) {
            //AfxMessageBox(_T("Group name could not be empty!"));
            AfxMessageBox(StrWarningInputGroup);
            return;
        }
        if(!m_EditGroupName.GetModify()) {
            CDialog::OnCancel();
            return;
        }
        if(m_strGroupName.Compare(m_strPreGPName)==0) {
            CDialog::OnCancel();
            return;
        }
        if(GroupExisted()) {
            //AfxMessageBox(_T("The same group already existed!"));
            //	AfxMessageBox(StrWarningGroupExisted);
            CDialog::OnOK();
            return;
        }
        for(int i = 0; i < PB_RECORD_MAX; i++) {
            if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,m_strPreGPName)==0)
                wcscpy(m_pPbData->PbDatas[i].strGroup,m_strGroupName);
        }
        for(int j= 0; j < PB_GROUP_MAX; j++) {
            if(-1 != m_pPbData->PbGroups[j].index && wcscmp(m_pPbData->PbGroups[j].strGroupName,m_strPreGPName)==0)
                wcscpy(m_pPbData->PbGroups[j].strGroupName,m_strGroupName);
        }

        if(m_pPbData->UpdateFile()) {
            CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
            pMainWnd->m_pPhoneBookDlg->m_strGPBeforeUpdated=m_strPreGPName;
            pMainWnd->m_pPhoneBookDlg->m_strGPAfterUpdated=m_strGroupName;
            pMainWnd->m_pPhoneBookDlg->nCurSelIndex=m_nGroupSelected;
            pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_REDRAWTREECTRL,1,NULL);

            pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_ReDrawListCtrl,NULL,NULL); //Refresh phone book in PhoneBookDlg

        }
        if(m_pPbData->UpdateGroupFile()) {
            GetParent()->SendMessage(WM_REFRESH_GROUP, NULL, NULL); //update group list
        }
        OnCancel();
    }
}


void CPBNewGroupDlg::OnButtonCancel()
{

    CBaseDialog::OnCancel();
}

void CPBNewGroupDlg::OnButtonEdit()
{
    UpdateData(TRUE);
    CString StrWarningInputGroup;
    CString StrWarningGroupExisted;
    StrWarningInputGroup.LoadString(IDS_WARNING_INPUTGROUP);
    StrWarningGroupExisted.LoadString(IDS_WARNING_GROUPEXISTED);
    if(m_strGroupName!=_T("")) {
        if(m_iEditFlag==0) { //add group
            if(GroupExisted()) {
                //AfxMessageBox(_T("The same group already existed!"));
                AfxMessageBox(StrWarningGroupExisted);
                return;
            }
            StPbGroup *PbGroupTemp = new StPbGroup();
            memset(PbGroupTemp,0,sizeof(StPbGroup));
            wcscpy(PbGroupTemp->strGroupName,(LPCTSTR)m_strGroupName);

            DWORD PbArrItemAds = 0;
            if( !m_pPbData->AddGroup(*PbGroupTemp,&PbArrItemAds ) ) {
                if(NULL != PbGroupTemp) {
                    delete PbGroupTemp;
                }
                return;
            } else {
                GetParent()->SendMessage(WM_REFRESH_GROUP, NULL, NULL);
                CHSDPADlg* pMainWnd = (CHSDPADlg*)AfxGetMainWnd();
                pMainWnd->m_pPhoneBookDlg->m_bNewGroupFlagUSIM=TRUE;
                pMainWnd->m_pPhoneBookDlg->m_bNewGroupFlagUSB=TRUE;
                pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_REDRAWTREECTRL,1000,NULL);
                delete PbGroupTemp;
                //	OnCancel();
            }
            m_iEditFlag=1;
            CDlgEditGPMember dlg(this, m_strGroupName);
            dlg.m_nGroupSelected=m_nGroupSelected;
            dlg.DoModal();
        } else { //Edit group member
            m_iEditFlag=1;
            CDlgEditGPMember dlg(this, m_strGroupName);
            dlg.m_nGroupSelected=m_nGroupSelected;
            dlg.DoModal();
        }



    } else {
        //AfxMessageBox(_T("Please input a group name!"));
        AfxMessageBox(StrWarningInputGroup);
        return;
    }


}

LRESULT CPBNewGroupDlg::OnRefreshGroup(WPARAM wParam, LPARAM lParam)
{
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    initGroupList();
    return 0;
}

BOOL CPBNewGroupDlg::GroupExisted()
{
    for(int j= 0; j < PB_GROUP_MAX; j++) {
        if(wcscmp(m_pPbData->PbGroups[j].strGroupName,m_strGroupName)==0)
            return TRUE;
    }
    return FALSE;
}