// --------------------------------------------------------------------------
//
// WDSGetProfileSettingsMsg.h
//
/// WDSGetProfileSettingsReq/WDSGetProfileSettingsRsp interface.
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
// Unique identifiers for WDSGetProfileSettingsMsg types
// --------------------------------------------------------------------------
static const uint32 WDSGetProfileSettingsReqUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_WDS_GET_PROFILE_SETTINGS_MSG);
static const uint32 WDSGetProfileSettingsRspUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_WDS_GET_PROFILE_SETTINGS_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSGetProfileSettingsReq;
class WDSGetProfileSettingsRsp;
typedef TRCPointer<WDSGetProfileSettingsReq> WDSGetProfileSettingsReqRCP;
typedef TRCPointer<WDSGetProfileSettingsRsp> WDSGetProfileSettingsRspRCP;


// --------------------------------------------------------------------------
// WDSGetProfileSettingsReq
//
/// This class represents a QMI_WDS_GET_PROFILE_SETTINGS_REQ message which
/// retrieves the settings from a configured profile.
// --------------------------------------------------------------------------

class WDSGetProfileSettingsReq : public Message
{
    // define tlv types
    static const uint8 PROFILE_IDENTIFIER_TYPE;

public:
    ~WDSGetProfileSettingsReq();

    virtual void Print(std::ostream& stream);

    virtual uint8 GetProfileType() {
        return m_profileType;
    }
    virtual uint8 GetProfileIndex() {
        return m_profileIndex;
    }

protected:
    WDSGetProfileSettingsReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildProfileType(std::string& value);
    virtual bool BuildProfileIndex(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }

private:
    // Profile Identifier, mandatory tlv 0x01
    uint8 m_profileIdentifierType;
    uint16 m_profileIdentifierLen;
    uint8 m_profileType;
    uint8 m_profileIndex;
};


// --------------------------------------------------------------------------
// WDSGetProfileSettingsRsp
//
/// This class represents a QMI_WDS_GET_PROFILE_SETTINGS_RESP message which
/// is the response to the wds get profile settings request.
// --------------------------------------------------------------------------
// define tlv types

class WDSGetProfileSettingsRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE ;
    static const uint8 PROFILE_NAME_TYPE;
    static const uint8 PDP_TYPE_TYPE;
    static const uint8 APN_NAME_TYPE;
    static const uint8 PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;
    static const uint8 SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;
    static const uint8 UMTS_REQUESTED_QOS_TYPE;
    static const uint8 UMTS_MINIMUM_QOS_TYPE;
    static const uint8 GPRS_REQUESTED_QOS_TYPE;
    static const uint8 GPRS_MINIMUM_QOS_TYPE;
    static const uint8 USERNAME_TYPE;
    static const uint8 AUTH_PREF_TYPE;
    static const uint8 IPV4_ADDR_PREF_TYPE;

public:
    ~WDSGetProfileSettingsRsp();

    virtual void Print(std::ostream& stream);

    virtual bool IsProfileName() {
        return m_profileNameType == PROFILE_NAME_TYPE;
    }
    virtual bool IsPdpType() {
        return m_pdpTypeType == PDP_TYPE_TYPE;
    }
    virtual bool IsApnName() {
        return m_apnNameType == APN_NAME_TYPE;
    }
    virtual bool IsPrimaryDnsIpv4AddrPref() {
        return m_primaryDnsIpv4AddrPrefType == PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;
    }
    virtual bool IsSecondaryDnsIpv4AddrPref() {
        return m_secondaryDnsIpv4AddrPrefType == SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;
    }
    virtual bool IsUmtsRequestedQos() {
        return m_umtsRequestedQosType == UMTS_REQUESTED_QOS_TYPE;
    }
    virtual bool IsUmtsMinimumQos() {
        return m_umtsMinimumQosType == UMTS_MINIMUM_QOS_TYPE;
    }
    virtual bool IsGprsRequestedQos() {
        return m_gprsRequestedQosType == GPRS_REQUESTED_QOS_TYPE;
    }
    virtual bool IsGprsMinimumQos() {
        return m_gprsMinimumQosType == GPRS_MINIMUM_QOS_TYPE;
    }
    virtual bool IsUsername() {
        return m_usernameType == USERNAME_TYPE;
    }
    virtual bool IsAuthPref() {
        return m_authPrefType == AUTH_PREF_TYPE;
    }
    virtual bool IsIpv4AddrPref() {
        return m_ipv4AddrPrefType == IPV4_ADDR_PREF_TYPE;
    }

    virtual uint16 GetResult() {
        return m_result;
    }
    virtual uint16 GetError() {
        return m_error;
    }
    virtual std::string GetProfileName() {
        return m_profileName;
    }
    virtual uint8 GetPdpType() {
        return m_pdpType;
    }
    virtual std::string GetApnName() {
        return m_apnName;
    }
    virtual uint32 GetPrimaryDnsIpv4AddrPref() {
        return m_primaryDnsIpv4AddrPref;
    }
    virtual uint32 GetSecondaryDnsIpv4AddrPref() {
        return m_secondaryDnsIpv4AddrPref;
    }
    virtual uint8 GetReqTrafficClass() {
        return m_reqTrafficClass;
    }
    virtual uint32 GetReqMaxUplinkBitrate() {
        return m_reqMaxUplinkBitrate;
    }
    virtual uint32 GetReqMaxDownlinkBitrate() {
        return m_reqMaxDownlinkBitrate;
    }
    virtual uint32 GetReqGtdUplinkBitrate() {
        return m_reqGtdUplinkBitrate;
    }
    virtual uint32 GetReqGtdDownlinkBitrate() {
        return m_reqGtdDownlinkBitrate;
    }
    virtual uint8 GetReqQosDeliveryOrder() {
        return m_reqQosDeliveryOrder;
    }
    virtual uint32 GetReqMaxSduSize() {
        return m_reqMaxSduSize;
    }
    virtual uint8 GetReqSduErrorRatio() {
        return m_reqSduErrorRatio;
    }
    virtual uint8 GetReqResidualBerRatio() {
        return m_reqResidualBerRatio;
    }
    virtual uint8 GetReqDeliveryErroneousSdu() {
        return m_reqDeliveryErroneousSdu;
    }
    virtual uint32 GetReqTransferDelay() {
        return m_reqTransferDelay;
    }
    virtual uint32 GetReqTrafficHandlingPriority() {
        return m_reqTrafficHandlingPriority;
    }
    virtual uint8 GetMinTrafficClass() {
        return m_minTrafficClass;
    }
    virtual uint32 GetMinMaxUplinkBitrate() {
        return m_minMaxUplinkBitrate;
    }
    virtual uint32 GetMinMaxDownlinkBitrate() {
        return m_minMaxDownlinkBitrate;
    }
    virtual uint32 GetMinGtdUplinkBitrate() {
        return m_minGtdUplinkBitrate;
    }
    virtual uint32 GetMinGtdDownlinkBitrate() {
        return m_minGtdDownlinkBitrate;
    }
    virtual uint8 GetMinQosDeliveryOrder() {
        return m_minQosDeliveryOrder;
    }
    virtual uint32 GetMinMaxSduSize() {
        return m_minMaxSduSize;
    }
    virtual uint8 GetMinSduErrorRatio() {
        return m_minSduErrorRatio;
    }
    virtual uint8 GetMinResidualBerRatio() {
        return m_minResidualBerRatio;
    }
    virtual uint8 GetMinDeliveryErroneousSdu() {
        return m_minDeliveryErroneousSdu;
    }
    virtual uint32 GetMinTransferDelay() {
        return m_minTransferDelay;
    }
    virtual uint32 GetMinTrafficHandlingPriority() {
        return m_minTrafficHandlingPriority;
    }
    virtual uint32 GetReqPrecedenceClass() {
        return m_reqPrecedenceClass;
    }
    virtual uint32 GetReqDelayClass() {
        return m_reqDelayClass;
    }
    virtual uint32 GetReqReliabilityClass() {
        return m_reqReliabilityClass;
    }
    virtual uint32 GetReqPeakThroughputClass() {
        return m_reqPeakThroughputClass;
    }
    virtual uint32 GetReqMeanThroughputClass() {
        return m_reqMeanThroughputClass;
    }
    virtual uint32 GetMinPrecedenceClass() {
        return m_minPrecedenceClass;
    }
    virtual uint32 GetMinDelayClass() {
        return m_minDelayClass;
    }
    virtual uint32 GetMinReliabilityClass() {
        return m_minReliabilityClass;
    }
    virtual uint32 GetMinPeakThroughputClass() {
        return m_minPeakThroughputClass;
    }
    virtual uint32 GetMinMeanThroughputClass() {
        return m_minMeanThroughputClass;
    }
    virtual std::string GetUsername() {
        return m_username;
    }
    virtual uint8 GetAuthPref() {
        return m_authPref;
    }
    virtual uint32 GetIpv4AddrPref() {
        return m_ipv4AddrPref;
    }

protected:
    WDSGetProfileSettingsRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackProfileName(MsgBuf& msgBuf);
    virtual bool UnpackPdpType(MsgBuf& msgBuf);
    virtual bool UnpackApnName(MsgBuf& msgBuf);
    virtual bool UnpackPrimaryDnsIpv4AddrPref(MsgBuf& msgBuf);
    virtual bool UnpackSecondaryDnsIpv4AddrPref(MsgBuf& msgBuf);
    virtual bool UnpackUmtsRequestedQos(MsgBuf& msgBuf);
    virtual bool UnpackUmtsMinimumQos(MsgBuf& msgBuf);
    virtual bool UnpackGprsRequestedQos(MsgBuf& msgBuf);
    virtual bool UnpackGprsMinimumQos(MsgBuf& msgBuf);
    virtual bool UnpackUsername(MsgBuf& msgBuf);
    virtual bool UnpackAuthPref(MsgBuf& msgBuf);
    virtual bool UnpackIpv4AddrPref(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

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

    // Authentication Preference, optional tlv 0x1D
    uint8 m_authPrefType;
    uint16 m_authPrefLen;
    uint8 m_authPref;

    // IPv4 Address Preference, optional tlv 0x1E
    uint8 m_ipv4AddrPrefType;
    uint16 m_ipv4AddrPrefLen;
    uint32 m_ipv4AddrPref;
};
