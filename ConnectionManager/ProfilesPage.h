// --------------------------------------------------------------------------
//
// ProfilesPage.h
//
/// ProfilesPage interface.
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

#pragma once

#include <string>
#include <vector>
#include "CommonDefs.h"
#include "WDSGetProfileListMsg.h"
#include "WDSGetProfileSettingsMsg.h"
#include "WDSGetDefaultSettingsMsg.h"
#include "WDSCreateProfileMsg.h"
#include "WDSModifyProfileSettingsMsg.h"
#include "WDSDeleteProfileMsg.h"


// @@@@@@
#ifndef OPEN_PAGE_UI
class CHSDPADlg;
#else
class ConnectionManagerDlg;
#endif
class ProfileSheet;


// --------------------------------------------------------------------------
// ProfilesPage
//
/// The ProfilesPage class represents the controls under the profiles
/// tab of the ConnectionManager dialog. Input from the user and output to
/// the user under the profiles tab is controlled by this class. 
// --------------------------------------------------------------------------

class ProfilesPage : public CPropertyPage
{
public:
    ProfilesPage();
// @@@@@@
#ifndef OPEN_PAGE_UI
    void SetHSDPADlg(CHSDPADlg* pDlg);
#else
    void SetCMDlg(ConnectionManagerDlg* pCMDlg);
#endif
    void EnableSelectedState();
    void DisableSelectedState();
    void ProcessWDSGetProfileListRsp(WDSGetProfileListRspRCP rspRCP);
    void ProcessWDSGetDefaultSettingsReq(WDSGetDefaultSettingsReqRCP reqRCP);
    void ProcessWDSGetDefaultSettingsRsp(WDSGetDefaultSettingsRspRCP rspRCP);
    void ProcessWDSGetProfileSettingsReq(WDSGetProfileSettingsReqRCP reqRCP);
    void ProcessWDSGetProfileSettingsRsp(WDSGetProfileSettingsRspRCP rspRCP);
    void ProcessWDSCreateProfileRsp(WDSCreateProfileRspRCP rspRCP);
    void ProcessWDSModifyProfileSettingsRsp(WDSModifyProfileSettingsRspRCP rspRCP);
    void ProcessWDSDeleteProfileReq(WDSDeleteProfileReqRCP reqRCP);
    void ProcessWDSDeleteProfileRsp();

protected:
    enum { UNKNOWN = -1 };
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    std::string BuildWDSGetProfileListString();
    std::string BuildWDSGetDefaultSettingsString();
    std::string BuildWDSGetProfileSettingsString();
    std::string BuildWDSDeleteProfileString();
    afx_msg void OnProfilesListSelChange();
    afx_msg void OnCreate();
    afx_msg void OnViewModify();
    afx_msg void OnDelete();
    
	DECLARE_MESSAGE_MAP()

private:
// @@@@@@
#ifndef OPEN_PAGE_UI
	CHSDPADlg* m_pCMDlg;
#else
    ConnectionManagerDlg* m_pCMDlg;
#endif
    ProfileSheet* m_pProfileSheet;
    uint16 m_settingsReqTxId;
    uint8 m_profileType;
    uint8 m_profileIndex;
    int m_listIndex;


    CListBox m_profilesList;
// @@@@@@
#ifdef OPEN_PAGE_UI
    CStatic m_configuredProfilesGroup;

    CButton m_createButton;
    CButton m_viewModifyButton;
    CButton m_deleteButton;
#endif
};