// --------------------------------------------------------------------------
//
// ProfileGeneralPage.h
//
/// ProfileGeneralPage interface.
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

#include "CommonDefs.h"
#include "ProfileSheetPage.h"
#include "WDSGetProfileSettingsMsg.h"
#include "WDSGetDefaultSettingsMsg.h"


// --------------------------------------------------------------------------
// ProfileGeneralPage
//
/// The ProfileGeneralPage class represents the controls under the general
/// tab of the profile sheet. Input from the user and output to the user 
/// under the general tab is controlled by this class. 
// --------------------------------------------------------------------------

class ProfileGeneralPage : public ProfileSheetPage
{
public:
    ProfileGeneralPage(WDSGetDefaultSettingsRspRCP rspRCP);
    ProfileGeneralPage(WDSGetProfileSettingsRspRCP rspRCP);

    virtual std::string BuildTlvString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    virtual void DefaultInit();
    virtual void ProfileInit();
    
	DECLARE_MESSAGE_MAP()

private:
    WDSGetDefaultSettingsRspRCP m_defaultSettingsRspRCP;
    WDSGetProfileSettingsRspRCP m_profileSettingsRspRCP;

    std::string m_initProfileName;
    uint8 m_initPdpType;
    std::string m_initApnName;
    std::string m_initPrimaryDns;
    std::string m_initSecondaryDns;
    std::string m_initUsername;
    std::string m_initPassword;
    uint8 m_initAuthPref;
    std::string m_initIpAddr;

    CEdit m_profileNameEdit;
    CComboBox m_pdpTypeCombo;
    CEdit m_apnNameEdit;
    CIPAddressCtrl m_primaryDnsIpAddress;
    CIPAddressCtrl m_secondaryDnsIpAddress;
    CEdit m_usernameEdit;
    CEdit m_passwordEdit;
    CComboBox m_authPrefCombo;
    CIPAddressCtrl m_ipAddrIpAddress;
};