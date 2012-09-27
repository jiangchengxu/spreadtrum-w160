// --------------------------------------------------------------------------
//
// WDSStartNetworkInterfaceMsg.h
//
/// WDSStartNetworkInterfaceReq/WDSStartNetworkInterfaceRsp interface.
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

#include "Message.h"

// --------------------------------------------------------------------------
// Unique identifiers for WDSStartNetworkInterfaceMsg types
// --------------------------------------------------------------------------
static const uint32 WDSStartNetworkInterfaceReqUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_WDS_START_NETWORK_INTERFACE_MSG);
static const uint32 WDSStartNetworkInterfaceRspUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_WDS_START_NETWORK_INTERFACE_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSStartNetworkInterfaceReq;
class WDSStartNetworkInterfaceRsp;
typedef TRCPointer<WDSStartNetworkInterfaceReq> WDSStartNetworkInterfaceReqRCP;
typedef TRCPointer<WDSStartNetworkInterfaceRsp> WDSStartNetworkInterfaceRspRCP;


// --------------------------------------------------------------------------
// WDSStartNetworkInterfaceReq
//
/// This class represents a QMI_WDS_START_NETWORK_INTERFACE_REQ message which
/// binds a control point to the WWAN device’s data session.  The device
/// activates the packet data session if it has not already been established
/// by another control point’s binding.
// --------------------------------------------------------------------------

class WDSStartNetworkInterfaceReq : public Message
{
    // define tlv types
    static const uint8 TECH_PREF_TYPE;
    static const uint8 CONFIGURED_PROFILE_ID_3GPP_TYPE;
    static const uint8 PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;
    static const uint8 SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;
    static const uint8 PRIMARY_NBNS_ADDR_PREF_TYPE;
    static const uint8 SECONDARY_NBNS_ADDR_PREF_TYPE;
    static const uint8 APN_NAME_TYPE;
    static const uint8 IPV4_ADDR_PREF_TYPE;
    static const uint8 AUTH_PREF_TYPE;
    static const uint8 USERNAME_TYPE;
    static const uint8 PASSWORD_TYPE;

public:
    ~WDSStartNetworkInterfaceReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSStartNetworkInterfaceReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildTechPref(std::string& value);
    virtual bool BuildConfiguredProfileId3gpp(std::string& value);
    virtual bool BuildPrimaryDnsIpv4AddrPref(std::string& value);
    virtual bool BuildSecondaryDnsIpv4AddrPref(std::string& value);
    virtual bool BuildPrimaryNbnsAddrPref(std::string& value);
    virtual bool BuildSecondaryNbnsAddrPref(std::string& value);
    virtual bool BuildApnName(std::string& value);
    virtual bool BuildIpv4AddrPref(std::string& value);
    virtual bool BuildAuthPref(std::string& value);
    virtual bool BuildUsername(std::string& value);
    virtual bool BuildPassword(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }

private:
    // Technology Preference, optional tlv 0x30
    uint8 m_techPrefType;
    uint16 m_techPrefLen;
    uint8 m_techPref;

    // 3GPP Configured Profile Identifier, optional tlv 0x31
    uint8 m_configuredProfileId3gppType;
    uint16 m_configuredProfileId3gppLen;
    uint8 m_configuredProfileId3gpp;

    // Primary DNS IPv4 Address Preference, optional tlv 0x10
    uint8 m_primaryDnsIpv4AddrPrefType;
    uint16 m_primaryDnsIpv4AddrPrefLen;
    uint32 m_primaryDnsIpv4AddrPref;

    // Secondary DNS IPv4 Address Preference, optional tlv 0x11
    uint8 m_secondaryDnsIpv4AddrPrefType;
    uint16 m_secondaryDnsIpv4AddrPrefLen;
    uint32 m_secondaryDnsIpv4AddrPref;

    // Primary NBNS Address Preference, optional tlv 0x12
    uint8 m_primaryNbnsAddrPrefType;
    uint16 m_primaryNbnsAddrPrefLen;
    uint32 m_primaryNbnsAddrPref;

    // Secondary NBNS Address Preference, optional tlv 0x13
    uint8 m_secondaryNbnsAddrPrefType;
    uint16 m_secondaryNbnsAddrPrefLen;
    uint32 m_secondaryNbnsAddrPref;

    // APN Name, optional tlv 0x14
    uint8 m_apnNameType;
    uint16 m_apnNameLen;
    std::string m_apnName;

    // IPv4 Address Preference, optional tlv 0x15
    uint8 m_ipv4AddrPrefType;
    uint16 m_ipv4AddrPrefLen;
    uint32 m_ipv4AddrPref;

    // Authentication Preference, optional tlv 0x16
    uint8 m_authPrefType;
    uint16 m_authPrefLen;
    uint8 m_authPref;

    // Username, optional tlv 0x17
    uint8 m_usernameType;
    uint16 m_usernameLen;
    std::string m_username;

    // Password, optional tlv 0x18
    uint8 m_passwordType;
    uint16 m_passwordLen;
    std::string m_password;
};


// --------------------------------------------------------------------------
// WDSStartNetworkInterfaceRsp
//
/// This class represents a QMI_WDS_START_NETWORK_INTERFACE_RESP message
/// which is the response to the activate packet data call request.
// --------------------------------------------------------------------------
// define tlv types

class WDSStartNetworkInterfaceRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 REQUIRED_PARAMETERS_TYPE;

public:
    ~WDSStartNetworkInterfaceRsp();

    virtual void Print(std::ostream& stream);

    uint32 GetPktDataHandle() {
        return m_pktDataHandle;
    }
    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

protected:
    WDSStartNetworkInterfaceRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackRequiredParameters(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Required Parameters, mandatory tlv 0x01
    uint8 m_requiredParametersType;
    uint16 m_requiredParametersLen;
    uint32 m_pktDataHandle;
};
