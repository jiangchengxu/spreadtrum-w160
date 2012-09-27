// --------------------------------------------------------------------------
//
// ProfileGeneralPage.cpp
//
/// ProfileGeneralPage implementation.
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

#ifndef ATLCONV_H
#define ATLCONV_H
#include <atlconv.h>
#endif


#include "ProfileGeneralPage.h"
#include "Resource.h"



BEGIN_MESSAGE_MAP(ProfileGeneralPage, CPropertyPage)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileGeneralPage::ProfileGeneralPage(WDSGetDefaultSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_GENERAL_PAGE),
    m_defaultSettingsRspRCP(rspRCP),
    m_profileSettingsRspRCP(NULL),
    m_initPdpType(0),
    m_initAuthPref(0)
{}

// --------------------------------------------------------------------------
// ctor
// --------------------------------------------------------------------------
ProfileGeneralPage::ProfileGeneralPage(WDSGetProfileSettingsRspRCP rspRCP) :
    ProfileSheetPage(IDD_PROFILE_GENERAL_PAGE),
    m_defaultSettingsRspRCP(NULL),
    m_profileSettingsRspRCP(rspRCP),
    m_initPdpType(0),
    m_initAuthPref(0)
{}

// --------------------------------------------------------------------------
// DoDataExchange
// --------------------------------------------------------------------------
void ProfileGeneralPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROFILE_NAME_EDIT, m_profileNameEdit);
    DDX_Control(pDX, IDC_PROFILE_PDP_TYPE_COMBO, m_pdpTypeCombo);
    DDX_Control(pDX, IDC_PROFILE_APN_NAME_EDIT, m_apnNameEdit);
    DDX_Control(pDX, IDC_PROFILE_PRIMARY_DNS_IPADDRESS, m_primaryDnsIpAddress);
    DDX_Control(pDX, IDC_PROFILE_SECONDARY_DNS_IPADDRESS, m_secondaryDnsIpAddress);
    DDX_Control(pDX, IDC_PROFILE_USERNAME_EDIT, m_usernameEdit);
    DDX_Control(pDX, IDC_PROFILE_PASSWORD_EDIT, m_passwordEdit);
    DDX_Control(pDX, IDC_PROFILE_AUTH_PREF_COMBO, m_authPrefCombo);
    DDX_Control(pDX, IDC_PROFILE_IP_ADDR_IPADDRESS, m_ipAddrIpAddress);
}

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ProfileGeneralPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // set pdp type combo box options
    m_pdpTypeCombo.AddString(_T("3GPP"));

    // set authentication preference combo box options
    m_authPrefCombo.AddString(_T("None"));
    m_authPrefCombo.AddString(_T("PAP"));
    m_authPrefCombo.AddString(_T("CHAP"));
    m_authPrefCombo.AddString(_T("PAP and CHAP"));

    // initialize member data
    if (!m_defaultSettingsRspRCP.IsNull()) {
        DefaultInit();
    }
    if (!m_profileSettingsRspRCP.IsNull()) {
        ProfileInit();
    }

    // set initial values for the controls on this page
    USES_CONVERSION;

    m_profileNameEdit.SetWindowText(A2W(m_initProfileName.c_str()));
    m_pdpTypeCombo.SetCurSel(m_initPdpType);
    m_apnNameEdit.SetWindowText(A2W(m_initApnName.c_str()));
    m_primaryDnsIpAddress.SetWindowText(A2W(m_initPrimaryDns.c_str()));
    m_secondaryDnsIpAddress.SetWindowText(A2W(m_initSecondaryDns.c_str()));
    m_usernameEdit.SetWindowText(A2W(m_initUsername.c_str()));
    m_passwordEdit.SetWindowText(A2W(m_initPassword.c_str()));
    m_authPrefCombo.SetCurSel(m_initAuthPref);
    m_ipAddrIpAddress.SetWindowText(A2W(m_initIpAddr.c_str()));


    return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// DefaultInit
//
/// Initialize the values for this page using the default settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileGeneralPage::DefaultInit()
{
    if (m_defaultSettingsRspRCP->IsProfileName()) {
        m_initProfileName = m_defaultSettingsRspRCP->GetProfileName();
    }

    if (m_defaultSettingsRspRCP->IsPdpType()) {
        m_initPdpType = m_defaultSettingsRspRCP->GetPdpType();
    }

    if (m_defaultSettingsRspRCP->IsApnName()) {
        m_initApnName = m_defaultSettingsRspRCP->GetApnName();
    }

    if (m_defaultSettingsRspRCP->IsPrimaryDnsIpv4AddrPref()) {
        std::stringstream stream;
        m_defaultSettingsRspRCP->PrintIPv4Addr(m_defaultSettingsRspRCP->GetPrimaryDnsIpv4AddrPref(),stream);
        m_initPrimaryDns = stream.str();
    }

    if (m_defaultSettingsRspRCP->IsSecondaryDnsIpv4AddrPref()) {
        std::stringstream stream;
        m_defaultSettingsRspRCP->PrintIPv4Addr(m_defaultSettingsRspRCP->GetSecondaryDnsIpv4AddrPref(),stream);
        m_initSecondaryDns = stream.str();
    }

    if (m_defaultSettingsRspRCP->IsUsername()) {
        m_initUsername = m_defaultSettingsRspRCP->GetUsername();
    }

    if (m_defaultSettingsRspRCP->IsAuthPref()) {
        m_initAuthPref = m_defaultSettingsRspRCP->GetAuthPref();
    }

    if (m_defaultSettingsRspRCP->IsIpv4AddrPref()) {
        std::stringstream stream;
        m_defaultSettingsRspRCP->PrintIPv4Addr(m_defaultSettingsRspRCP->GetIpv4AddrPref(),stream);
        m_initIpAddr = stream.str();
    }
}

// --------------------------------------------------------------------------
// ProfileInit
//
/// Initialize the values for this page using the profile settings rsp rcp.
// --------------------------------------------------------------------------
void ProfileGeneralPage::ProfileInit()
{
    if (m_profileSettingsRspRCP->IsProfileName()) {
        m_initProfileName = m_profileSettingsRspRCP->GetProfileName();
    }

    if (m_profileSettingsRspRCP->IsPdpType()) {
        m_initPdpType = m_profileSettingsRspRCP->GetPdpType();
    }

    if (m_profileSettingsRspRCP->IsApnName()) {
        m_initApnName = m_profileSettingsRspRCP->GetApnName();
    }

    if (m_profileSettingsRspRCP->IsPrimaryDnsIpv4AddrPref()) {
        std::stringstream stream;
        m_profileSettingsRspRCP->PrintIPv4Addr(m_profileSettingsRspRCP->GetPrimaryDnsIpv4AddrPref(),stream);
        m_initPrimaryDns = stream.str();
    }

    if (m_profileSettingsRspRCP->IsSecondaryDnsIpv4AddrPref()) {
        std::stringstream stream;
        m_profileSettingsRspRCP->PrintIPv4Addr(m_profileSettingsRspRCP->GetSecondaryDnsIpv4AddrPref(),stream);
        m_initSecondaryDns = stream.str();
    }

    if (m_profileSettingsRspRCP->IsUsername()) {
        m_initUsername = m_profileSettingsRspRCP->GetUsername();
    }

    if (m_profileSettingsRspRCP->IsAuthPref()) {
        m_initAuthPref = m_profileSettingsRspRCP->GetAuthPref();
    }

    if (m_profileSettingsRspRCP->IsIpv4AddrPref()) {
        std::stringstream stream;
        m_profileSettingsRspRCP->PrintIPv4Addr(m_profileSettingsRspRCP->GetIpv4AddrPref(),stream);
        m_initIpAddr = stream.str();
    }
}

// --------------------------------------------------------------------------
// BuildTlvString
//
/// Build a string of the message tlv's represented on this page.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfileGeneralPage::BuildTlvString()
{
    std::stringstream stream;

    // add tlvs to stream if value has changed

    // profile name tlv
    USES_CONVERSION;

    CString profileName;
    m_profileNameEdit.GetWindowText(profileName);
    if (profileName.Compare(A2W(m_initProfileName.c_str())) != 0) {
        if (profileName.Compare(_T("")) == 0) {
            profileName = _T("NULL");
        }
        stream << "  ProfileName " << W2A(profileName) << std::endl;
    }

    // pdp type tlv
    int pdpType = m_pdpTypeCombo.GetCurSel();
    if (pdpType != m_initPdpType) {
        stream << "  PdpType " << pdpType << std::endl;
    }

    // apn name tlv
    CString apnName;
    m_apnNameEdit.GetWindowText(apnName);
    if (apnName.Compare(A2W(m_initApnName.c_str())) != 0) {
        if (apnName.Compare(_T("")) == 0) {
            apnName = _T("NULL");
        }
        stream << "  ApnName " << apnName << std::endl;
    }

    // primary dns tlv
    CString primaryDns;
    m_primaryDnsIpAddress.GetWindowText(primaryDns);
    if (primaryDns.Compare(A2W(m_initPrimaryDns.c_str())) != 0) {
        stream << "  PrimaryDnsIpv4AddrPref " << primaryDns << std::endl;
    }

    // secondary dns tlv
    CString secondaryDns;
    m_secondaryDnsIpAddress.GetWindowText(secondaryDns);
    if (secondaryDns.Compare(A2W(m_initSecondaryDns.c_str())) != 0) {
        stream << "  SecondaryDnsIpv4AddrPref " << secondaryDns << std::endl;
    }

    // username tlv
    CString username;
    m_usernameEdit.GetWindowText(username);
    if (username.Compare(A2W(m_initUsername.c_str()) )!= 0) {
        if (username.Compare(_T("")) == 0) {
            username = _T("NULL");
        }
        stream << "  Username " << username << std::endl;
    }

    // password tlv
    CString password;
    m_passwordEdit.GetWindowText(password);
    if (password.Compare(A2W(m_initPassword.c_str())) != 0) {
        if (password.Compare(_T("")) == 0) {
            password = _T("NULL");
        }
        stream << "  Password " << password << std::endl;
    }

    // authentication preference tlv
    int authPref = m_authPrefCombo.GetCurSel();
    if (authPref != m_initAuthPref) {
        stream << "  AuthPref " << authPref << std::endl;
    }

    // ipv4 address preference tlv
    CString ipv4AddrPref;
    m_ipAddrIpAddress.GetWindowText(ipv4AddrPref);
    if (ipv4AddrPref.Compare(A2W(m_initIpAddr.c_str())) != 0) {
        stream << "  Ipv4AddrPref "<< ipv4AddrPref << std::endl;
    }

    return stream.str();
}
