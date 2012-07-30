// --------------------------------------------------------------------------
//
// WDSModifyProfileSettingsMsg.h
//
/// WDSModifyProfileSettingsReq/WDSModifyProfileSettingsRsp interface.
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
// Unique identifiers for WDSModifyProfileSettingsMsg types
// --------------------------------------------------------------------------
static const uint32 WDSModifyProfileSettingsReqUID =
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_WDS_MODIFY_PROFILE_SETTINGS_MSG);
static const uint32 WDSModifyProfileSettingsRspUID =
    (QMUX_TYPE_WDS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_WDS_MODIFY_PROFILE_SETTINGS_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSModifyProfileSettingsReq;
class WDSModifyProfileSettingsRsp;
typedef TRCPointer<WDSModifyProfileSettingsReq> WDSModifyProfileSettingsReqRCP;
typedef TRCPointer<WDSModifyProfileSettingsRsp> WDSModifyProfileSettingsRspRCP;


// --------------------------------------------------------------------------
// WDSModifyProfileSettingsReq
//
/// This class represents a QMI_WDS_MODIFY_PROFILE_SETTINGS_REQ message which
/// changes the settings in a configured profile.
// --------------------------------------------------------------------------

class WDSModifyProfileSettingsReq : public Message
{
    // define tlv types
    static const uint8 PROFILE_IDENTIFIER_TYPE;
    static const uint8 PROFILE_NAME_TYPE;
    static const uint8 PDP_TYPE_TYPE ;
    static const uint8 APN_NAME_TYPE;
    static const uint8 PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;
    static const uint8 SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;
    static const uint8 UMTS_REQUESTED_QOS_TYPE;
    static const uint8 UMTS_MINIMUM_QOS_TYPE;
    static const uint8 GPRS_REQUESTED_QOS_TYPE;
    static const uint8 GPRS_MINIMUM_QOS_TYPE;
    static const uint8 USERNAME_TYPE;
    static const uint8 PASSWORD_TYPE;
    static const uint8 AUTH_PREF_TYPE;
    static const uint8 IPV4_ADDR_PREF_TYPE;

public:
    ~WDSModifyProfileSettingsReq();
    
    virtual void Print(std::ostream& stream);

protected:
    WDSModifyProfileSettingsReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildProfileType(std::string& value);
    virtual bool BuildProfileIndex(std::string& value);
    virtual bool BuildProfileName(std::string& value);
    virtual bool BuildPdpType(std::string& value);
    virtual bool BuildApnName(std::string& value);
    virtual bool BuildPrimaryDnsIpv4AddrPref(std::string& value);
    virtual bool BuildSecondaryDnsIpv4AddrPref(std::string& value);
    virtual bool BuildReqTrafficClass(std::string& value);
    virtual bool BuildReqMaxUplinkBitrate(std::string& value);
    virtual bool BuildReqMaxDownlinkBitrate(std::string& value);
    virtual bool BuildReqGtdUplinkBitrate(std::string& value);
    virtual bool BuildReqGtdDownlinkBitrate(std::string& value);
    virtual bool BuildReqQosDeliveryOrder(std::string& value);
    virtual bool BuildReqMaxSduSize(std::string& value);
    virtual bool BuildReqSduErrorRatio(std::string& value);
    virtual bool BuildReqResidualBerRatio(std::string& value);
    virtual bool BuildReqDeliveryErroneousSdu(std::string& value);
    virtual bool BuildReqTransferDelay(std::string& value);
    virtual bool BuildReqTrafficHandlingPriority(std::string& value);
    virtual bool BuildMinTrafficClass(std::string& value);
    virtual bool BuildMinMaxUplinkBitrate(std::string& value);
    virtual bool BuildMinMaxDownlinkBitrate(std::string& value);
    virtual bool BuildMinGtdUplinkBitrate(std::string& value);
    virtual bool BuildMinGtdDownlinkBitrate(std::string& value);
    virtual bool BuildMinQosDeliveryOrder(std::string& value);
    virtual bool BuildMinMaxSduSize(std::string& value);
    virtual bool BuildMinSduErrorRatio(std::string& value);
    virtual bool BuildMinResidualBerRatio(std::string& value);
    virtual bool BuildMinDeliveryErroneousSdu(std::string& value);
    virtual bool BuildMinTransferDelay(std::string& value);
    virtual bool BuildMinTrafficHandlingPriority(std::string& value);
    virtual bool BuildReqPrecedenceClass(std::string& value);
    virtual bool BuildReqDelayClass(std::string& value);
    virtual bool BuildReqReliabilityClass(std::string& value);
    virtual bool BuildReqPeakThroughputClass(std::string& value);
    virtual bool BuildReqMeanThroughputClass(std::string& value);
    virtual bool BuildMinPrecedenceClass(std::string& value);
    virtual bool BuildMinDelayClass(std::string& value);
    virtual bool BuildMinReliabilityClass(std::string& value);
    virtual bool BuildMinPeakThroughputClass(std::string& value);
    virtual bool BuildMinMeanThroughputClass(std::string& value);
    virtual bool BuildUsername(std::string& value);
    virtual bool BuildPassword(std::string& value);
    virtual bool BuildAuthPref(std::string& value);
    virtual bool BuildIpv4AddrPref(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }

private:
    // PROFILE_IDENTIFIER, mandatory tlv 0x01
    uint8 m_profileIdentifierType;
    uint16 m_profileIdentifierLen;
    uint8 m_profileType;
    uint8 m_profileIndex;

    // Profile Name, optional tlv 0x10
    uint8 m_profileNameType;
    uint16 m_profileNameLen;
    std::string m_profileName;

    // PDP Type, optional tlv 0x11
    uint8 m_pdpTypeType;
    uint16 m_pdpTypeLen;
    uint8 m_pdpType;

    // APN Name, optional tlv 0x14
    uint8 m_apnNameType;
    uint16 m_apnNameLen;
    std::string m_apnName;

    // Primary DNS IPv4 Address Preference, optional tlv 0x15
    uint8 m_primaryDnsIpv4AddrPrefType;
    uint16 m_primaryDnsIpv4AddrPrefLen;
    uint32 m_primaryDnsIpv4AddrPref;

    // Secondary DNS IPv4 Address Preference, optional tlv 0x16
    uint8 m_secondaryDnsIpv4AddrPrefType;
    uint16 m_secondaryDnsIpv4AddrPrefLen;
    uint32 m_secondaryDnsIpv4AddrPref;

    // UMTS Requested QoS, optional tlv 0x17
    uint8 m_umtsRequestedQosType;
    uint16 m_umtsRequestedQosLen;
    uint8 m_reqTrafficClass;
    uint32 m_reqMaxUplinkBitrate;
    uint32 m_reqMaxDownlinkBitrate;
    uint32 m_reqGtdUplinkBitrate;
    uint32 m_reqGtdDownlinkBitrate;
    uint8 m_reqQosDeliveryOrder;
    uint32 m_reqMaxSduSize;
    uint8 m_reqSduErrorRatio;
    uint8 m_reqResidualBerRatio;
    uint8 m_reqDeliveryErroneousSdu;
    uint32 m_reqTransferDelay;
    uint32 m_reqTrafficHandlingPriority;

    // UMTS Minimum QoS, optional tlv 0x18
    uint8 m_umtsMinimumQosType;
    uint16 m_umtsMinimumQosLen;
    uint8 m_minTrafficClass;
    uint32 m_minMaxUplinkBitrate;
    uint32 m_minMaxDownlinkBitrate;
    uint32 m_minGtdUplinkBitrate;
    uint32 m_minGtdDownlinkBitrate;
    uint8 m_minQosDeliveryOrder;
    uint32 m_minMaxSduSize;
    uint8 m_minSduErrorRatio;
    uint8 m_minResidualBerRatio;
    uint8 m_minDeliveryErroneousSdu;
    uint32 m_minTransferDelay;
    uint32 m_minTrafficHandlingPriority;

    // GPRS Requested QoS, optional tlv 0x19
    uint8 m_gprsRequestedQosType;
    uint16 m_gprsRequestedQosLen;
    uint32 m_reqPrecedenceClass;
    uint32 m_reqDelayClass;
    uint32 m_reqReliabilityClass;
    uint32 m_reqPeakThroughputClass;
    uint32 m_reqMeanThroughputClass;

    // GPRS Minimum QoS, optional tlv 0x1A
    uint8 m_gprsMinimumQosType;
    uint16 m_gprsMinimumQosLen;
    uint32 m_minPrecedenceClass;
    uint32 m_minDelayClass;
    uint32 m_minReliabilityClass;
    uint32 m_minPeakThroughputClass;
    uint32 m_minMeanThroughputClass;

    // Username, optional tlv 0x1B
    uint8 m_usernameType;
    uint16 m_usernameLen;
    std::string m_username;

    // Password, optional tlv 0x1C
    uint8 m_passwordType;
    uint16 m_passwordLen;
    std::string m_password;

    // Authentication Preference, optional tlv 0x1D
    uint8 m_authPrefType;
    uint16 m_authPrefLen;
    uint8 m_authPref;

    // IPv4 Address Preference, optional tlv 0x1E
    uint8 m_ipv4AddrPrefType;
    uint16 m_ipv4AddrPrefLen;
    uint32 m_ipv4AddrPref;
};


// --------------------------------------------------------------------------
// WDSModifyProfileSettingsRsp
//
/// This class represents a QMI_WDS_MODIFY_PROFILE_SETTINGS_RESP message 
/// which is the response to the wds modify profile settings request.
// --------------------------------------------------------------------------

class WDSModifyProfileSettingsRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~WDSModifyProfileSettingsRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() { return m_result; }
    virtual uint16 GetError() { return m_error; }

protected:
    WDSModifyProfileSettingsRsp();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;
};
