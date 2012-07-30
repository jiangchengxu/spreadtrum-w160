// --------------------------------------------------------------------------
//
// ProfilesPage.cpp
//
/// ProfilesPage implementation.
///
/// @file
//
// Copyright (c) 2006 QUALCOMM Inc.
//
// All Rights Reserved.  QUALCOMM Proprietary
//
// Export of this technology or software is regulated by the U.S. Government.
// Diversion contrary to U.S. law prohibited.
//
// --------------------------------------------------------------------------
#include "stdafx.h"
#include <sstream>


#include "ProfilesPage.h"
#include "ConnectionManagerDlg.h"
#include "Resource.h"
#include "Message.h"
#include "ProfileSheet.h"
#include "ProfileGeneralPage.h"
#include "ProfileUmtsReqPage.h"
#include "ProfileUmtsMinPage.h"
#include "ProfileGprsReqPage.h"
#include "ProfileGprsMinPage.h"

// @@@@@@
#ifndef OPEN_PAGE_UI
#include "HSDPADlg.h"
#endif

BEGIN_MESSAGE_MAP(ProfilesPage, CPropertyPage)
    ON_LBN_SELCHANGE(IDC_PROFILES_LIST,OnProfilesListSelChange)
    ON_BN_CLICKED(IDC_CREATE_BUTTON,OnCreate)
    ON_BN_CLICKED(IDC_VIEW_MODIFY_BUTTON,OnViewModify)
    ON_LBN_DBLCLK(IDC_PROFILES_LIST,OnViewModify)
    ON_BN_CLICKED(IDC_DELETE_BUTTON,OnDelete)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfilesPage::ProfilesPage() : 
    CPropertyPage(IDD_PROFILES_PAGE),
    m_pCMDlg(NULL),
    m_pProfileSheet(NULL),
    m_settingsReqTxId(0),
    m_profileType(-1),
    m_profileIndex(-1),
    m_listIndex(0)
{}

// --------------------------------------------------------------------------
// SetCMDlg
// --------------------------------------------------------------------------
// @@@@@@
#ifndef OPEN_PAGE_UI
void ProfilesPage::SetHSDPADlg(CHSDPADlg* pDlg)
{
    m_pCMDlg = pDlg;
}
#else
void ProfilesPage::SetCMDlg(ConnectionManagerDlg* pCMDlg)
{
    m_pCMDlg = pCMDlg;
}
#endif

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void ProfilesPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

// @@@@@@
#ifdef OPEN_PAGE_UI
    DDX_Control(pDX, IDC_PROFILES_LIST, m_profilesList);
    DDX_Control(pDX, IDC_CONFIGURED_PROFILES_GROUP, m_configuredProfilesGroup);

    DDX_Control(pDX, IDC_CREATE_BUTTON, m_createButton);
    DDX_Control(pDX, IDC_VIEW_MODIFY_BUTTON, m_viewModifyButton);
    DDX_Control(pDX, IDC_DELETE_BUTTON, m_deleteButton);
#endif
}

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ProfilesPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    
    DisableSelectedState();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// EnableSelectedState
//
/// Show availabe fields and send requests to populate them.
// --------------------------------------------------------------------------
void ProfilesPage::EnableSelectedState()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // enable/disable appropriate fields
    m_configuredProfilesGroup.EnableWindow(TRUE);
    m_profilesList.EnableWindow(TRUE);
    m_createButton.EnableWindow(TRUE);
#endif
    // send get profile list request
    m_pCMDlg->SendRequest(BuildWDSGetProfileListString());
}

// --------------------------------------------------------------------------
// DisableSelectedState
//
/// Dim fields that should not be accessed.
// --------------------------------------------------------------------------
void ProfilesPage::DisableSelectedState()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // enable/disable appropriate fields
    m_configuredProfilesGroup.EnableWindow(FALSE);
    m_profilesList.EnableWindow(FALSE);
    m_profilesList.ResetContent();
    m_createButton.EnableWindow(FALSE);
    m_viewModifyButton.EnableWindow(FALSE);
    m_deleteButton.EnableWindow(FALSE);
#endif
}

// --------------------------------------------------------------------------
// BuildWDSGetProfileListString
//
/// Build a string of message type and name/value pairs representing a
/// WDSGetProfileListReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfilesPage::BuildWDSGetProfileListString()
{
    std::stringstream stream;

    // msg type
    stream << "QMI_WDS_GET_PROFILE_LIST_REQ" << std::endl
           << "{}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildWDSGetDefaultSettingsString
//
/// Build a string of message type and name/value pairs representing a
/// WDSGetDefaultSettingsReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfilesPage::BuildWDSGetDefaultSettingsString()
{
    std::stringstream stream;

    // !!! there is only one profile type now, but there will be more.
    // !!! consider a modal dialog to select profile type here.
    int type = 0;

    // msg type
    stream << "QMI_WDS_GET_DEFAULT_SETTINGS_REQ" << std::endl
           << "{" << std::endl
           << "  ProfileType " << type << std::endl
           << "}";

    return stream.str();
}

// --------------------------------------------------------------------------
// BuildWDSGetProfileSettingsString
//
/// Build a string of message type and name/value pairs representing a
/// WDSGetProfileSettingsReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfilesPage::BuildWDSGetProfileSettingsString()
{
    std::stringstream stream;
// @@@@@@
#ifdef OPEN_PAGE_UI
    // get the index of the profile selected
    int result = m_profilesList.GetCurSel();

    if (result != LB_ERR)
    {
        // get the type/index information stored in item data

		// @@
        //DWORD_PTR itemData = m_profilesList.GetItemData(result);
		DWORD itemData = m_profilesList.GetItemData(result);

        int idx = (int)(itemData & 0x0000FFFF);
        int type = (int)(itemData >> 16);

        // msg type
        stream << "QMI_WDS_GET_PROFILE_SETTINGS_REQ" << std::endl
               << "{" << std::endl
               << "  ProfileType " << type << std::endl
               << "  ProfileIndex " << idx << std::endl
               << "}";
    }
#endif
    return stream.str();
}

// --------------------------------------------------------------------------
// BuildWDSDeleteProfileString
//
/// Build a string of message type and name/value pairs representing a
/// WDSDeleteProfileReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfilesPage::BuildWDSDeleteProfileString()
{
    std::stringstream stream;
// @@@@@@
#ifdef OPEN_PAGE_UI
    // get the index of the profile selected
    int result = m_profilesList.GetCurSel();

    if (result != LB_ERR)
    {
        // get the type/index information stored in item data

        // @@
		//DWORD_PTR itemData = m_profilesList.GetItemData(result);
		DWORD itemData = m_profilesList.GetItemData(result);

        int idx = (int)(itemData & 0x0000FFFF);
        int type = (int)(itemData >> 16);

        // msg type
        stream << "QMI_WDS_DELETE_PROFILE_REQ" << std::endl
               << "{" << std::endl
               << "  ProfileType " << type << std::endl
               << "  ProfileIndex " << idx << std::endl
               << "}";
    }
#endif
    return stream.str();
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileListRsp
//
/// Actions to take when a WDS get profile list responce is received.
///
/// @params indRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSGetProfileListRsp(WDSGetProfileListRspRCP rspRCP)
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // reset content and disable buttons that rely on selection
    m_profilesList.ResetContent();
// @@@@@@
#ifdef OPEN_PAGE_UI
    m_viewModifyButton.EnableWindow(FALSE);
    m_deleteButton.EnableWindow(FALSE);
#endif
    // populate profiles list
    std::vector<ProfileListInstance*> profileList = 
        rspRCP->GetProfileListInstances();

    size_t numProfiles = profileList.size();
    for (uint32 i = 0; i < numProfiles; i++)
    {
        int type = profileList[i]->GetProfileType();
        int idx = profileList[i]->GetProfileIndex();

        std::stringstream stream;
        stream << "Index: " << idx 
               << "  Type: " << PROFILE_TYPE_STRINGS[type]
               << "  Name: " << profileList[i]->GetProfileName();
        
        int result = m_profilesList.AddString(stream.str().c_str());

        if (result != LB_ERR)
        {
			// @@
            //DWORD_PTR itemData = (type << 16) | idx;
			DWORD itemData = (type << 16) | idx;

            // set index as item data
            m_profilesList.SetItemData(result,itemData);
        }
    }

    // set selection to last selected, if available
    int listCount = m_profilesList.GetCount();
    if (listCount > 0)
    {
        if (m_listIndex == UNKNOWN)
        {
            // when creating list index is unknown, must search
            for (int i = 0; i < listCount; i++)
            {
				// @@
                //DWORD_PTR itemData = m_profilesList.GetItemData(i);
				DWORD itemData = m_profilesList.GetItemData(i);

                int idx = (int)(itemData & 0x0000FFFF);
                int type = (int)(itemData >> 16);
                if (idx == m_profileIndex && type == m_profileType)
                {
                    // set list index to new profile location
                    m_profilesList.SetCurSel(i);
                    m_listIndex = i;
                }
            }
        }
        else if (m_listIndex < listCount)
        {
            // when modifying and deleting list index is known
            m_profilesList.SetCurSel(m_listIndex);
        }
        else
        {
            // when deleting the last profile list index == list count
            m_profilesList.SetCurSel(listCount - 1);
        }

        // SetCurSel does not send selection change message so do it manually
        OnProfilesListSelChange();
    }
#endif
}

// --------------------------------------------------------------------------
// ProcessWDSGetDefaultSettingsReq
//
/// Actions to take when a WDS get default settings request is received.
///
/// @params reqRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSGetDefaultSettingsReq(WDSGetDefaultSettingsReqRCP reqRCP)
{
    m_settingsReqTxId = reqRCP->GetTxId();
    m_profileType = reqRCP->GetProfileType();
}

// --------------------------------------------------------------------------
// ProcessWDSGetDefaultSettingsRsp
//
/// Actions to take when a WDS get default settings response is received.
///
/// @params rspRCP - the response messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSGetDefaultSettingsRsp(WDSGetDefaultSettingsRspRCP rspRCP)
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // make sure this response matches the request
    if (rspRCP->GetTxId() != m_settingsReqTxId)
    {
        return;
    }

    // create the profile sheet and profile sheet pages
    ProfileSheet profileSheet(m_pCMDlg,m_profileType);
    m_pProfileSheet = &profileSheet;
    ProfileGeneralPage generalPage(rspRCP);
    ProfileUmtsReqPage umtsReqPage(rspRCP);
    ProfileUmtsMinPage umtsMinPage(rspRCP);
    ProfileGprsReqPage gprsReqPage(rspRCP);
    ProfileGprsMinPage gprsMinPage(rspRCP);

    // turn off the apply and help buttons
    profileSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    profileSheet.m_psh.dwFlags &= ~PSH_HASHELP;
    generalPage.m_psp.dwFlags &= ~PSP_HASHELP;
    umtsReqPage.m_psp.dwFlags &= ~PSP_HASHELP;
    umtsMinPage.m_psp.dwFlags &= ~PSP_HASHELP;
    gprsReqPage.m_psp.dwFlags &= ~PSP_HASHELP;
    gprsMinPage.m_psp.dwFlags &= ~PSP_HASHELP;

    // add pages to the profile sheet
    profileSheet.AddPage(&generalPage);
    profileSheet.AddPage(&umtsReqPage);
    profileSheet.AddPage(&umtsMinPage);
    profileSheet.AddPage(&gprsReqPage);
    profileSheet.AddPage(&gprsMinPage);

    INT_PTR result = profileSheet.DoModal();

    if (result == IDOK)
    {
        // refresh the profiles list
        m_pCMDlg->SendRequest(BuildWDSGetProfileListString());
    }
#endif
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileSettingsReq
//
/// Actions to take when a WDS get profile settings request is received.
///
/// @params reqRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSGetProfileSettingsReq(WDSGetProfileSettingsReqRCP reqRCP)
{
    m_settingsReqTxId = reqRCP->GetTxId();
    m_profileType = reqRCP->GetProfileType();
    m_profileIndex = reqRCP->GetProfileIndex();
}

// --------------------------------------------------------------------------
// ProcessWDSGetProfileSettingsRsp
//
/// Actions to take when a WDS get profile settings response is received.
///
/// @params rspRCP - the response messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSGetProfileSettingsRsp(WDSGetProfileSettingsRspRCP rspRCP)
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // make sure this response matches the request
    if (rspRCP->GetTxId() != m_settingsReqTxId)
    {
        return;
    }

    // create the profile sheet and profile sheet pages
    ProfileSheet profileSheet(m_pCMDlg,m_profileType,m_profileIndex);
    m_pProfileSheet = &profileSheet;
    ProfileGeneralPage generalPage(rspRCP);
    ProfileUmtsReqPage umtsReqPage(rspRCP);
    ProfileUmtsMinPage umtsMinPage(rspRCP);
    ProfileGprsReqPage gprsReqPage(rspRCP);
    ProfileGprsMinPage gprsMinPage(rspRCP);

    // turn off the apply and help buttons
    profileSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    profileSheet.m_psh.dwFlags &= ~PSH_HASHELP;
    generalPage.m_psp.dwFlags &= ~PSP_HASHELP;
    umtsReqPage.m_psp.dwFlags &= ~PSP_HASHELP;
    umtsMinPage.m_psp.dwFlags &= ~PSP_HASHELP;
    gprsReqPage.m_psp.dwFlags &= ~PSP_HASHELP;
    gprsMinPage.m_psp.dwFlags &= ~PSP_HASHELP;

    // add pages to the profile sheet
    profileSheet.AddPage(&generalPage);
    profileSheet.AddPage(&umtsReqPage);
    profileSheet.AddPage(&umtsMinPage);
    profileSheet.AddPage(&gprsReqPage);
    profileSheet.AddPage(&gprsMinPage);

    INT_PTR result = profileSheet.DoModal();

    if (result == IDOK)
    {
        // refresh the profiles list
        m_pCMDlg->SendRequest(BuildWDSGetProfileListString());
    }
#endif
}

// --------------------------------------------------------------------------
// ProcessWDSCreateProfileRsp
//
/// Actions to take when a WDS create profile response is received.
///
/// @params reqRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSCreateProfileRsp(WDSCreateProfileRspRCP rspRCP)
{
    // verify success
    if (rspRCP->GetResult() == QMI_RESULT_SUCCESS)
    {
        // set to determine list box selection after creation
        m_settingsReqTxId = rspRCP->GetTxId();
        m_profileType = rspRCP->GetProfileType();
        m_profileIndex = rspRCP->GetProfileIndex();

        // exit the modal profile sheet dialog
        if (m_pProfileSheet != NULL)
        {
            m_pProfileSheet->EndDialog(IDOK);
            m_pProfileSheet = NULL;
        }
    }

    // on failure, profile sheet dialog will continue, allowing user to 
    // make corrections and attempt creating again
}

// --------------------------------------------------------------------------
// ProcessWDSModifyProfileSettingsRsp
//
/// Actions to take when a WDS modify profile settings response is received.
///
/// @params reqRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSModifyProfileSettingsRsp(WDSModifyProfileSettingsRspRCP rspRCP)
{
    // verify success
    if (rspRCP->GetResult() == QMI_RESULT_SUCCESS)
    {
        // exit the modal profile sheet dialog
        if (m_pProfileSheet != NULL)
        {
            m_pProfileSheet->EndDialog(IDOK);
            m_pProfileSheet = NULL;
        }
    }
    
    // on failure, profile sheet dialog will continue, allowing user to 
    // make corrections and attempt modifying again
}

// --------------------------------------------------------------------------
// ProcessWDSDeleteProfileReq
//
/// Actions to take when a WDS delete profile request is received.
///
/// @params reqRCP - the indicator messge to process
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSDeleteProfileReq(WDSDeleteProfileReqRCP reqRCP)
{
    m_settingsReqTxId = reqRCP->GetTxId();
    m_profileType = reqRCP->GetProfileType();
    m_profileIndex = reqRCP->GetProfileIndex();
}

// --------------------------------------------------------------------------
// ProcessWDSDeleteProfileRsp
//
/// Actions to take when a wds delete profile response is received.
// --------------------------------------------------------------------------
void ProfilesPage::ProcessWDSDeleteProfileRsp()
{
    // refresh the profiles list
    m_pCMDlg->SendRequest(BuildWDSGetProfileListString());
}

// --------------------------------------------------------------------------
// OnProfilesListSelChange
//
/// Actions to take when selection changes in the profiles list.
// --------------------------------------------------------------------------
void ProfilesPage::OnProfilesListSelChange()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    m_viewModifyButton.EnableWindow(TRUE);
    m_deleteButton.EnableWindow(TRUE);
#endif
}

// --------------------------------------------------------------------------
// OnCreate
//
/// Actions to take when when the create button is pressed.
// --------------------------------------------------------------------------
void ProfilesPage::OnCreate()
{
    // send get profile settings request and set list index to unknown
    m_pCMDlg->SendRequest(BuildWDSGetDefaultSettingsString());
    m_listIndex = UNKNOWN;
}

// --------------------------------------------------------------------------
// OnViewModify
//
/// Actions to take when when the view/modify button is pressed or an item in
/// the profiles list is double clicked.
// --------------------------------------------------------------------------
void ProfilesPage::OnViewModify()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // send get profile settings request and set list index to selection
    m_pCMDlg->SendRequest(BuildWDSGetProfileSettingsString());
    m_listIndex = m_profilesList.GetCurSel();
#endif
}

// --------------------------------------------------------------------------
// OnDelete
//
/// Actions to take when when the delete button is pressed.
// --------------------------------------------------------------------------
void ProfilesPage::OnDelete()
{
// @@@@@@
#ifdef OPEN_PAGE_UI
    // send get profile settings request and set list index to selection
    m_pCMDlg->SendRequest(BuildWDSDeleteProfileString());
    m_listIndex = m_profilesList.GetCurSel();
#endif
}