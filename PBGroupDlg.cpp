// PBNewGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Common_DataCrad\Common_DataCrad.h"
#include "PBGroupDlg.h"
#include "PBNewGroupDlg.h"
#include "HSDPADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPBGroupDlg dialog


CPBGroupDlg::CPBGroupDlg(CWnd* pParent /*=NULL*/)
    : CBaseDialog(CPBGroupDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPBGroupDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    m_nBkTag = 1;
}


void CPBGroupDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    DDX_Control(pDX,IDC_LIST_PBGROUP,m_ListGroup);

}


BEGIN_MESSAGE_MAP(CPBGroupDlg, CBaseDialog)

    ON_WM_PAINT()
    ON_BN_CLICKED(ID_BUTTON_NEWGP,OnButtonNewGP)
    ON_BN_CLICKED(ID_BUTTON_EDITGP,OnButtonEdit)
    ON_BN_CLICKED(ID_BUTTON_DELETEGP,OnButtonDelete)
    ON_MESSAGE(WM_REFRESH_GROUP, OnRefreshGroup)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPBGroupDlg message handlers

BOOL CPBGroupDlg::OnInitDialog()
{
    CBaseDialog::OnInitDialog();
// 	m_bFirst = TRUE;
// 	SetSkin(g_SetData.Setup_nSkinStyle);
    initGroupList();

    return true;
}

void CPBGroupDlg::initGroupList()
{
    m_ListGroup.ResetContent();
    for(int i = 0; i < PB_GROUP_MAX; i++) {
        int nindex = 0;
        if(-1 != m_pPbData->PbGroups[i].index) {
            m_ListGroup.InsertString(nindex,m_pPbData->PbGroups[i].strGroupName);
            nindex ++;
        }

    }
}

void CPBGroupDlg::OnPaint()
{
    CBaseDialog::OnPaint();
}

void CPBGroupDlg::OnButtonNewGP()
{
    if( m_pPbData->CheckGroupFull()) {
        CString strGroupFull;
        strGroupFull.LoadString(IDS_GROUP_FULL);
        AfxMessageBox(strGroupFull);

        return;
    }

    CPBNewGroupDlg dlg(this,_T(""));
    dlg.m_iEditFlag=0; //Ìí¼Ó
    dlg.m_nGroupSelected=m_nGroupSelected;
    dlg.DoModal();
}

void CPBGroupDlg::OnButtonEdit()
{
    UpdateData(TRUE);
    int selectedItem=m_ListGroup.GetCurSel();
    if(selectedItem!=LB_ERR) {
        CString groupName;
        groupName.Format(_T(""));
        m_ListGroup.GetText(selectedItem,groupName);
        CPBNewGroupDlg dlg(this,groupName);
        dlg.m_iEditFlag=1; //±à¼­
        dlg.m_nGroupSelected=m_nGroupSelected;
        dlg.DoModal();

    } else {
        CString StrNoItemSelect;
        StrNoItemSelect.LoadString(IDS_NOITEMSEL);
        AfxMessageBox(StrNoItemSelect);
        //AfxMessageBox(_T("Please select a group!"));
        return;
    }

}

void CPBGroupDlg::OnButtonDelete()
{
    UpdateData(TRUE);

    int selectedItem=m_ListGroup.GetCurSel();
    if(selectedItem!=LB_ERR) {
        if (AfxMessageBox(IDS_DELETECONFIRM, MB_YESNO) == IDNO) {
            return;
        }
        CString groupName;
        m_ListGroup.GetText(selectedItem,groupName);
        m_pPbData->DelSigleGroup(groupName);

        //Set group name of those members in the removed group to empty
        for(int i = 0; i < PB_RECORD_MAX; i++) {
            if(-1 != m_pPbData->PbDatas[i].index && wcscmp(m_pPbData->PbDatas[i].strGroup,groupName)==0)
                wcscpy(m_pPbData->PbDatas[i].strGroup,_T(""));
        }
        m_pPbData->UpdateFile();
        CHSDPADlg* pMainWnd=((CHSDPADlg *)AfxGetMainWnd());
        pMainWnd->m_pPhoneBookDlg->m_strGPDelete=groupName;
        pMainWnd->m_pPhoneBookDlg->nCurSelIndex=m_nGroupSelected;
        pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_REDRAWTREECTRL,2,NULL);
        pMainWnd->m_pPhoneBookDlg->PostMessage(WM_PB_ReDrawListCtrl,NULL,NULL);
        initGroupList();
    } else {
        CString StrNoItemSelect;
        StrNoItemSelect.LoadString(IDS_NOITEMSEL);
        //AfxMessageBox(_T("Please select a group!"));
        AfxMessageBox(StrNoItemSelect);
        return;
    }

}

LRESULT CPBGroupDlg::OnRefreshGroup(WPARAM wParam, LPARAM lParam)
{

    m_pPbData = ((CHSDPAApp *)AfxGetApp())->GetPbData();
    initGroupList();
    return 0;
}

void CPBGroupDlg::OnCancel()
{
    CHSDPADlg* pMainWnd=((CHSDPADlg *)AfxGetMainWnd());
    pMainWnd->m_pPhoneBookDlg->nCurSelIndex=m_nGroupSelected;
    CDialog::OnCancel();

}