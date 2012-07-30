// --------------------------------------------------------------------------
//
// WDSStartNetworkInterfaceMsg.cpp
//
/// WDSStartNetworkInterfaceReq/WDSStartNetworkInterfaceRsp implementation.
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
#include <assert.h>
#include <sstream>
#include "WDSStartNetworkInterfaceMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSStartNetworkInterfaceReq> ReqStringCreator("QMI_WDS_START_NETWORK_INTERFACE_REQ");
static MessageCreator<WDSStartNetworkInterfaceReq> ReqUint32Creator(WDSStartNetworkInterfaceReqUID);

static MessageCreator<WDSStartNetworkInterfaceRsp> RspStringCreator("QMI_WDS_START_NETWORK_INTERFACE_RSP");
static MessageCreator<WDSStartNetworkInterfaceRsp> RspUint32Creator(WDSStartNetworkInterfaceRspUID);

// --------------------------------------------------------------------------
// WDSStartNetworkInterfaceReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSStartNetworkInterfaceReq.
// --------------------------------------------------------------------------
const uint8 WDSStartNetworkInterfaceReq::TECH_PREF_TYPE = 0x30;
const uint8 WDSStartNetworkInterfaceReq::CONFIGURED_PROFILE_ID_3GPP_TYPE = 0x31;
const uint8 WDSStartNetworkInterfaceReq::PRIMARY_DNS_IPV4_ADDR_PREF_TYPE = 0x10;
const uint8 WDSStartNetworkInterfaceReq::SECONDARY_DNS_IPV4_ADDR_PREF_TYPE = 0x11;
const uint8 WDSStartNetworkInterfaceReq::PRIMARY_NBNS_ADDR_PREF_TYPE = 0x12;
const uint8 WDSStartNetworkInterfaceReq::SECONDARY_NBNS_ADDR_PREF_TYPE = 0x13;
const uint8 WDSStartNetworkInterfaceReq::APN_NAME_TYPE = 0x14;
const uint8 WDSStartNetworkInterfaceReq::IPV4_ADDR_PREF_TYPE = 0x15;
const uint8 WDSStartNetworkInterfaceReq::AUTH_PREF_TYPE = 0x16;
const uint8 WDSStartNetworkInterfaceReq::USERNAME_TYPE = 0x17;
const uint8 WDSStartNetworkInterfaceReq::PASSWORD_TYPE = 0x18;
WDSStartNetworkInterfaceReq::WDSStartNetworkInterfaceReq() : 
    Message(QMUX_TYPE_WDS,QMI_WDS_START_NETWORK_INTERFACE_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_techPrefType(TLV_TYPE_INVALID),
    m_techPrefLen(0),
    m_techPref(0),
    m_configuredProfileId3gppType(TLV_TYPE_INVALID),
    m_configuredProfileId3gppLen(0),
    m_configuredProfileId3gpp(0),
    m_primaryDnsIpv4AddrPrefType(TLV_TYPE_INVALID),
    m_primaryDnsIpv4AddrPrefLen(0),
    m_primaryDnsIpv4AddrPref(0),
    m_secondaryDnsIpv4AddrPrefType(TLV_TYPE_INVALID),
    m_secondaryDnsIpv4AddrPrefLen(0),
    m_secondaryDnsIpv4AddrPref(0),
    m_primaryNbnsAddrPrefType(TLV_TYPE_INVALID),
    m_primaryNbnsAddrPrefLen(0),
    m_primaryNbnsAddrPref(0),
    m_secondaryNbnsAddrPrefType(TLV_TYPE_INVALID),
    m_secondaryNbnsAddrPrefLen(0),
    m_secondaryNbnsAddrPref(0),
    m_apnNameType(TLV_TYPE_INVALID),
    m_apnNameLen(0),
    m_ipv4AddrPrefType(TLV_TYPE_INVALID),
    m_ipv4AddrPrefLen(0),
    m_ipv4AddrPref(0),
    m_authPrefType(TLV_TYPE_INVALID),
    m_authPrefLen(0),
    m_authPref(0),
    m_usernameType(TLV_TYPE_INVALID),
    m_usernameLen(0),
    m_passwordType(TLV_TYPE_INVALID),
    m_passwordLen(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSStartNetworkInterfaceReq.
// --------------------------------------------------------------------------
WDSStartNetworkInterfaceReq::~WDSStartNetworkInterfaceReq()
{}

// --------------------------------------------------------------------------
// Build
//
/// Parse a string into this Message.
///
/// @param nameValue - string of name value pairs.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::Build(std::string& nameValue)
{
    // call the base build function
    return Message::Build(nameValue);
}

// --------------------------------------------------------------------------
// GetBuilderMap
//
/// Return a reference to the string to builder map for this Message.
///
/// @returns StringBuilderMap& - reference to the string to builder map for
///     this Message type.
// --------------------------------------------------------------------------
Message::StringBuilderMap& WDSStartNetworkInterfaceReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty())
    {
        bool bSuccess = SBMap.insert(SBPair("TechPref",(Builder)BuildTechPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("ProfileId",(Builder)BuildConfiguredProfileId3gpp)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("PrimaryDnsPref",(Builder)BuildPrimaryDnsIpv4AddrPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("SecondaryDnsPref",(Builder)BuildSecondaryDnsIpv4AddrPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("PrimaryNbnsPref",(Builder)BuildPrimaryNbnsAddrPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("SecondaryNbnsPref",(Builder)BuildSecondaryNbnsAddrPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("ApnName",(Builder)BuildApnName)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("Ipv4AddrPref",(Builder)BuildIpv4AddrPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("AuthPref",(Builder)BuildAuthPref)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("Username",(Builder)BuildUsername)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("Password",(Builder)BuildPassword)).second;
        assert(bSuccess);
    }
    return SBMap;
}

// --------------------------------------------------------------------------
// BuildTechPref
//
/// Build the technology preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildTechPref(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num < 1 && num > 2)
    {
        // report invalid technology preference
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Technology Preference '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid preference values are 1 and 2.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_techPrefType = TECH_PREF_TYPE;
    m_techPrefLen = 1;
    m_techPref = (uint8)num;
    m_length += (m_techPrefLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildConfiguredProfileId3gpp
//
/// Build the 3GPP Configured Profile Id attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildConfiguredProfileId3gpp(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num < 0 && num > 255)
    {
        // report invalid technology preference
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid 3GPP Configured Profile Id '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid id values are 0 to 255 inclusive.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_configuredProfileId3gppType = CONFIGURED_PROFILE_ID_3GPP_TYPE;
    m_configuredProfileId3gppLen = 1;
    m_configuredProfileId3gpp = (uint8)num;
    m_length += (m_configuredProfileId3gppLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildPrimaryDnsIpv4AddrPref
//
/// Build the Primary DNS IPv4 Address Preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildPrimaryDnsIpv4AddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_primaryDnsIpv4AddrPref))
    {
        // set member data
        m_primaryDnsIpv4AddrPrefType = PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;
        m_primaryDnsIpv4AddrPrefLen = 4;
        m_length += (m_primaryDnsIpv4AddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildSecondaryDnsIpv4AddrPref
//
/// Build the Secondary DNS IPv4 Address Preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildSecondaryDnsIpv4AddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_secondaryDnsIpv4AddrPref))
    {
        // set member data
        m_secondaryDnsIpv4AddrPrefType = SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;
        m_secondaryDnsIpv4AddrPrefLen = 4;
        m_length += (m_secondaryDnsIpv4AddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildPrimaryNbnsAddrPref
//
/// Build the Primary NBNS Address Preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildPrimaryNbnsAddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_primaryNbnsAddrPref))
    {
        // set member data
        m_primaryNbnsAddrPrefType = PRIMARY_NBNS_ADDR_PREF_TYPE;
        m_primaryNbnsAddrPrefLen = 4;
        m_length += (m_primaryNbnsAddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildSecondaryNbnsAddrPref
//
/// Build the Secondary NBNS Address Preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildSecondaryNbnsAddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_secondaryNbnsAddrPref))
    {
        // set member data
        m_secondaryNbnsAddrPrefType = SECONDARY_NBNS_ADDR_PREF_TYPE;
        m_secondaryNbnsAddrPrefLen = 4;
        m_length += (m_secondaryNbnsAddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildApnName
//
/// Build the apn name attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildApnName(std::string& value)
{
    // check for null string indication
    if (value == "NULL") { value = ""; }

    // set member data
    m_apnNameType = APN_NAME_TYPE;
    m_apnName = value;
    m_apnNameLen = (uint16)m_apnName.length();
    m_length += (m_apnNameLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildIpv4AddrPref
//
/// Build the IPv4 Address Preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildIpv4AddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_ipv4AddrPref))
    {
        // set member data
        m_ipv4AddrPrefType = IPV4_ADDR_PREF_TYPE;
        m_ipv4AddrPrefLen = 4;
        m_length += (m_ipv4AddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildAuthPref
//
/// Build the authentication preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildAuthPref(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num < 1 && num > 3)
    {
        // report invalid technology preference
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Authenication Preference '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid preference values are 1, 2 and 3.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_authPrefType = AUTH_PREF_TYPE;
    m_authPrefLen = 1;
    m_authPref = (uint8)num;
    m_length += (m_authPrefLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildUsername
//
/// Build the username attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildUsername(std::string& value)
{
    // check for null string indication
    if (value == "NULL") { value = ""; }

    // set member data
    m_usernameType = USERNAME_TYPE;
    m_username = value;
    m_usernameLen = (uint8)m_username.length();
    m_length += (m_usernameLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildPassword
//
/// Build the password attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildPassword(std::string& value)
{
    // check for null string indication
    if (value == "NULL") { value = ""; }

    // set member data
    m_passwordType = PASSWORD_TYPE;
    m_password = value;
    m_passwordLen = (uint8)m_password.length();
    m_length += (m_passwordLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // optional tlv, technology preference
    if (m_techPrefType == TECH_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_techPrefType);
        m_pMsgBuf->PutWord(m_techPrefLen);
        m_pMsgBuf->PutByte(m_techPref);
    }

    // optional tlv, 3gpp configured profile id
    if (m_configuredProfileId3gppType == CONFIGURED_PROFILE_ID_3GPP_TYPE)
    {
        m_pMsgBuf->PutByte(m_configuredProfileId3gppType);
        m_pMsgBuf->PutWord(m_configuredProfileId3gppLen);
        m_pMsgBuf->PutByte(m_configuredProfileId3gpp);
    }

    // optional tlv, primary dns ipv4 address preference
    if (m_primaryDnsIpv4AddrPrefType == PRIMARY_DNS_IPV4_ADDR_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_primaryDnsIpv4AddrPrefType);
        m_pMsgBuf->PutWord(m_primaryDnsIpv4AddrPrefLen);
        m_pMsgBuf->PutDWord(m_primaryDnsIpv4AddrPref);
    }

    // optional tlv, secondary dns ipv4 address preference
    if (m_secondaryDnsIpv4AddrPrefType == SECONDARY_DNS_IPV4_ADDR_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_secondaryDnsIpv4AddrPrefType);
        m_pMsgBuf->PutWord(m_secondaryDnsIpv4AddrPrefLen);
        m_pMsgBuf->PutDWord(m_secondaryDnsIpv4AddrPref);
    }

    // optional tlv, primary nbns address preference
    if (m_primaryNbnsAddrPrefType == PRIMARY_NBNS_ADDR_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_primaryNbnsAddrPrefType);
        m_pMsgBuf->PutWord(m_primaryNbnsAddrPrefLen);
        m_pMsgBuf->PutDWord(m_primaryNbnsAddrPref);
    }

    // optional tlv, secondary nbns address preference
    if (m_secondaryNbnsAddrPrefType == SECONDARY_NBNS_ADDR_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_secondaryNbnsAddrPrefType);
        m_pMsgBuf->PutWord(m_secondaryNbnsAddrPrefLen);
        m_pMsgBuf->PutDWord(m_secondaryNbnsAddrPref);
    }

    // optional tlv, apn name
    if (m_apnNameType == APN_NAME_TYPE)
    {
        m_pMsgBuf->PutByte(m_apnNameType);
        m_pMsgBuf->PutWord(m_apnNameLen);
        m_pMsgBuf->PutCopy(m_apnName.c_str(),m_apnNameLen);
    }

    // optional tlv, ipv4 address preference
    if (m_ipv4AddrPrefType == IPV4_ADDR_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_ipv4AddrPrefType);
        m_pMsgBuf->PutWord(m_ipv4AddrPrefLen);
        m_pMsgBuf->PutDWord(m_ipv4AddrPref);
    }

    // optional tlv, authentication preference
    if (m_authPrefType == AUTH_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_authPrefType);
        m_pMsgBuf->PutWord(m_authPrefLen);
        m_pMsgBuf->PutByte(m_authPref);
    }

    // optional tlv, username
    if (m_usernameType == USERNAME_TYPE)
    {
        m_pMsgBuf->PutByte(m_usernameType);
        m_pMsgBuf->PutWord(m_usernameLen);
        m_pMsgBuf->PutCopy(m_username.c_str(),m_usernameLen);
    }

    // optional tlv, password
    if (m_passwordType == PASSWORD_TYPE)
    {
        m_pMsgBuf->PutByte(m_passwordType);
        m_pMsgBuf->PutWord(m_passwordLen);
        m_pMsgBuf->PutCopy(m_password.c_str(),m_passwordLen);
    }

    assert(m_length == m_pMsgBuf->GetIndex() - QMI_SDU_HEADER_BYTES);
    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSStartNetworkInterfaceReq::Print(std::ostream& stream)
{
    stream << _T("QMI_WDS_START_NETWORK_INTERFACE_REQ") << std::endl
           << _T("{") << std::endl;
    
    if (m_techPrefType == TECH_PREF_TYPE)
    {
        stream << _T("  TechPref ") << (int)m_techPref << std::endl;
    }

    if (m_configuredProfileId3gppType == CONFIGURED_PROFILE_ID_3GPP_TYPE)
    {
        stream << _T("  ProfileId ") << (int)m_configuredProfileId3gpp << std::endl;
    }

    if (m_primaryDnsIpv4AddrPrefType == PRIMARY_DNS_IPV4_ADDR_PREF_TYPE)
    {
        stream << _T("  PrimaryDnsPref ");
        PrintIPv4Addr(m_primaryDnsIpv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_secondaryDnsIpv4AddrPrefType == SECONDARY_DNS_IPV4_ADDR_PREF_TYPE)
    {
        stream << _T("  SecondaryDnsPref ");
        PrintIPv4Addr(m_secondaryDnsIpv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_primaryNbnsAddrPrefType == PRIMARY_NBNS_ADDR_PREF_TYPE)
    {
        stream << _T("  PrimaryNbnsPref ");
        PrintIPv4Addr(m_primaryNbnsAddrPref,stream);
        stream << std::endl;
    }

    if (m_secondaryNbnsAddrPrefType == SECONDARY_NBNS_ADDR_PREF_TYPE)
    {
        stream << _T("  SecondaryNbnsPref ");
        PrintIPv4Addr(m_secondaryNbnsAddrPref,stream);
        stream << std::endl;
    }

    if (m_apnNameType == APN_NAME_TYPE)
    {
        std::string apnName = m_apnName;
        if (apnName == "") { apnName = "NULL"; }
        stream << _T("  ApnName ") << apnName << std::endl;
    }

    if (m_ipv4AddrPrefType == IPV4_ADDR_PREF_TYPE)
    {
        stream << _T("  Ipv4AddrPref ");
        PrintIPv4Addr(m_ipv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_authPrefType == AUTH_PREF_TYPE)
    {
        stream << _T("  AuthPref ") << (int)m_authPref << std::endl;
    }

    if (m_usernameType == USERNAME_TYPE)
    {
        std::string username = m_username;
        if (username == "") { username = "NULL"; }
        stream << _T("  Username ") << username << std::endl;
    }

    if (m_passwordType == PASSWORD_TYPE)
    {
        std::string password = m_password;
        if (password == "") { password = "NULL"; }
        stream << _T("  Password ") << password << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSStartNetworkInterfaceRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSStartNetworkInterfaceRsp.
// --------------------------------------------------------------------------
const uint8 WDSStartNetworkInterfaceRsp::RESULT_CODE_TYPE = 0x02;
const uint8 WDSStartNetworkInterfaceRsp::REQUIRED_PARAMETERS_TYPE = 0x01;
WDSStartNetworkInterfaceRsp::WDSStartNetworkInterfaceRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_START_NETWORK_INTERFACE_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_requiredParametersType(TLV_TYPE_INVALID),
    m_requiredParametersLen(0),
    m_pktDataHandle(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSStartNetworkInterfaceRsp.
// --------------------------------------------------------------------------
WDSStartNetworkInterfaceRsp::~WDSStartNetworkInterfaceRsp()
{}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into this Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }
    
    // validate message length
    if (m_result == QMI_RESULT_SUCCESS)
    {
        // mandatory tlvs
        if (m_length != 14)
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("Expected message length is 14 bytes, unpacked length is ")
                << m_length << _T(" bytes.") << std::endl 
                << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false; 
        }
    }
    else
    {
        // only result code tlv on failure
        if (m_length != 7) 
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("Expected message length is 7 bytes, unpacked length is ")
                << m_length << _T(" bytes.") << std::endl 
                << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false; 
        }
    }

    return true;
}

// --------------------------------------------------------------------------
// GetUnpackerMap
//
/// Return a reference to the uint8 to unpacker map for this Message.
///
/// @returns Uint*UnpackerMap& - reference to the uint8 to unpacker map for
///     this Message type.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& WDSStartNetworkInterfaceRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(REQUIRED_PARAMETERS_TYPE,(Unpacker)UnpackRequiredParameters)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackResultCode
//
/// Unpack the result code tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceRsp::UnpackResultCode(MsgBuf& msgBuf)
{
    m_resultCodeType = RESULT_CODE_TYPE;

    m_resultCodeLen = msgBuf.GetWord();
    if (m_resultCodeLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Result Code length is 4 bytes, unpacked length is ")
               << m_resultCodeLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_result = msgBuf.GetWord();
    m_error = msgBuf.GetWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackRequiredParameters
//
/// Unpack the required parameters tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSStartNetworkInterfaceRsp::UnpackRequiredParameters(MsgBuf& msgBuf)
{
    m_requiredParametersType = REQUIRED_PARAMETERS_TYPE;

    m_requiredParametersLen = msgBuf.GetWord();
    if (m_requiredParametersLen != 4)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Required Parameter length is 4 bytes, unpacked length is ")
               << m_requiredParametersLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_pktDataHandle = msgBuf.GetDWord();

    // all tlvs are mandatory, so we should be at end of buffer
    if (!msgBuf.EOB())
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Finished unpacking message but end of buffer not reached")
               << std::endl << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    return true;
}
// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSStartNetworkInterfaceRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_START_NETWORK_INTERFACE_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    // only print other mandatory tlvs if result code success
    if (m_result == QMI_RESULT_SUCCESS)
    {
        stream << _T("  PktDataHandle ") << (int)m_pktDataHandle << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
