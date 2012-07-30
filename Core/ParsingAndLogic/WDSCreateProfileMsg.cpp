// --------------------------------------------------------------------------
//
// WDSCreateProfileMsg.cpp
//
/// WDSCreateProfileReq/WDSCreateProfileRsp implementation.
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
#include "WDSCreateProfileMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSCreateProfileReq> ReqStringCreator("QMI_WDS_CREATE_PROFILE_REQ");
static MessageCreator<WDSCreateProfileReq> ReqUint32Creator(WDSCreateProfileReqUID);

static MessageCreator<WDSCreateProfileRsp> RspStringCreator("QMI_WDS_CREATE_PROFILE_RSP");
static MessageCreator<WDSCreateProfileRsp> RspUint32Creator(WDSCreateProfileRspUID);

// --------------------------------------------------------------------------
// WDSCreateProfileReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSCreateProfileReq.
// --------------------------------------------------------------------------
const uint8 WDSCreateProfileReq::PROFILE_TYPE_TYPE = 0x01;
const uint8 WDSCreateProfileReq::PROFILE_NAME_TYPE = 0x10;
const uint8 WDSCreateProfileReq::PDP_TYPE_TYPE = 0x11;
const uint8 WDSCreateProfileReq::APN_NAME_TYPE = 0x14;
const uint8 WDSCreateProfileReq::PRIMARY_DNS_IPV4_ADDR_PREF_TYPE = 0x15;
const uint8 WDSCreateProfileReq::SECONDARY_DNS_IPV4_ADDR_PREF_TYPE = 0x16;
const uint8 WDSCreateProfileReq::UMTS_REQUESTED_QOS_TYPE = 0x17;
const uint8 WDSCreateProfileReq::UMTS_MINIMUM_QOS_TYPE = 0x18;
const uint8 WDSCreateProfileReq::GPRS_REQUESTED_QOS_TYPE = 0x19;
const uint8 WDSCreateProfileReq::GPRS_MINIMUM_QOS_TYPE = 0x1A;
const uint8 WDSCreateProfileReq::USERNAME_TYPE = 0x1B;
const uint8 WDSCreateProfileReq::PASSWORD_TYPE = 0x1C;
const uint8 WDSCreateProfileReq::AUTH_PREF_TYPE = 0x1D;
const uint8 WDSCreateProfileReq::IPV4_ADDR_PREF_TYPE = 0x1E;
WDSCreateProfileReq::WDSCreateProfileReq() : 
    Message(QMUX_TYPE_WDS,QMI_WDS_CREATE_PROFILE_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_profileTypeType(TLV_TYPE_INVALID),
    m_profileTypeLen(0),
    m_profileType(-1),
    m_profileNameType(TLV_TYPE_INVALID),
    m_profileNameLen(0),
    m_pdpTypeType(TLV_TYPE_INVALID),
    m_pdpTypeLen(0),
    m_pdpType(-1),
    m_apnNameType(TLV_TYPE_INVALID),
    m_apnNameLen(0),
    m_primaryDnsIpv4AddrPrefType(TLV_TYPE_INVALID),
    m_primaryDnsIpv4AddrPrefLen(0),
    m_primaryDnsIpv4AddrPref(0),
    m_secondaryDnsIpv4AddrPrefType(TLV_TYPE_INVALID),
    m_secondaryDnsIpv4AddrPrefLen(0),
    m_secondaryDnsIpv4AddrPref(0),
    m_umtsRequestedQosType(TLV_TYPE_INVALID),
    m_umtsRequestedQosLen(0),
    m_reqTrafficClass(-1),
    m_reqMaxUplinkBitrate(-1),
    m_reqMaxDownlinkBitrate(-1),
    m_reqGtdUplinkBitrate(-1),
    m_reqGtdDownlinkBitrate(-1),
    m_reqQosDeliveryOrder(-1),
    m_reqMaxSduSize(-1),
    m_reqSduErrorRatio(-1),
    m_reqResidualBerRatio(-1),
    m_reqDeliveryErroneousSdu(-1),
    m_reqTransferDelay(-1),
    m_reqTrafficHandlingPriority(-1),
    m_umtsMinimumQosType(TLV_TYPE_INVALID),
    m_umtsMinimumQosLen(0),
    m_minTrafficClass(-1),
    m_minMaxUplinkBitrate(-1),
    m_minMaxDownlinkBitrate(-1),
    m_minGtdUplinkBitrate(-1),
    m_minGtdDownlinkBitrate(-1),
    m_minQosDeliveryOrder(-1),
    m_minMaxSduSize(-1),
    m_minSduErrorRatio(-1),
    m_minResidualBerRatio(-1),
    m_minDeliveryErroneousSdu(-1),
    m_minTransferDelay(-1),
    m_minTrafficHandlingPriority(-1),
    m_gprsRequestedQosType(TLV_TYPE_INVALID),
    m_gprsRequestedQosLen(0),
    m_reqPrecedenceClass(-1),
    m_reqDelayClass(-1),
    m_reqReliabilityClass(-1),
    m_reqPeakThroughputClass(-1),
    m_reqMeanThroughputClass(-1),
    m_gprsMinimumQosType(TLV_TYPE_INVALID),
    m_gprsMinimumQosLen(0),
    m_minPrecedenceClass(-1),
    m_minDelayClass(-1),
    m_minReliabilityClass(-1),
    m_minPeakThroughputClass(-1),
    m_minMeanThroughputClass(-1),
    m_usernameType(TLV_TYPE_INVALID),
    m_usernameLen(0),
    m_passwordType(TLV_TYPE_INVALID),
    m_passwordLen(0),
    m_authPrefType(TLV_TYPE_INVALID),
    m_authPrefLen(0),
    m_authPref(-1),
    m_ipv4AddrPrefType(TLV_TYPE_INVALID),
    m_ipv4AddrPrefLen(0),
    m_ipv4AddrPref(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSCreateProfileReq.
// --------------------------------------------------------------------------
WDSCreateProfileReq::~WDSCreateProfileReq()
{}

// --------------------------------------------------------------------------
// Build
//
/// Parse a string into this Message.
///
/// @param nameValue - string of name value pairs.
///
/// @returns bool - true if build successful
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::Build(std::string& nameValue)
{
    // mandatory tlv PktDataHandle is expected
    m_nextName = "ProfileType";

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
Message::StringBuilderMap& WDSCreateProfileReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty())
    {
        bool bSuccess = SBMap.insert(SBPair("ProfileType",(Builder)BuildProfileType)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ProfileName",(Builder)BuildProfileName)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("PdpType",(Builder)BuildPdpType)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ApnName",(Builder)BuildApnName)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("PrimaryDnsIpv4AddrPref",(Builder)BuildPrimaryDnsIpv4AddrPref)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("SecondaryDnsIpv4AddrPref",(Builder)BuildSecondaryDnsIpv4AddrPref)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqTrafficClass",(Builder)BuildReqTrafficClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqMaxUplinkBitrate",(Builder)BuildReqMaxUplinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqMaxDownlinkBitrate",(Builder)BuildReqMaxDownlinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqGtdUplinkBitrate",(Builder)BuildReqGtdUplinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqGtdDownlinkBitrate",(Builder)BuildReqGtdDownlinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqQosDeliveryOrder",(Builder)BuildReqQosDeliveryOrder)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqMaxSduSize",(Builder)BuildReqMaxSduSize)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqSduErrorRatio",(Builder)BuildReqSduErrorRatio)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqResidualBerRatio",(Builder)BuildReqResidualBerRatio)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqDeliveryErroneousSdu",(Builder)BuildReqDeliveryErroneousSdu)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqTransferDelay",(Builder)BuildReqTransferDelay)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqTrafficHandlingPriority",(Builder)BuildReqTrafficHandlingPriority)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinTrafficClass",(Builder)BuildMinTrafficClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinMaxUplinkBitrate",(Builder)BuildMinMaxUplinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinMaxDownlinkBitrate",(Builder)BuildMinMaxDownlinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinGtdUplinkBitrate",(Builder)BuildMinGtdUplinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinGtdDownlinkBitrate",(Builder)BuildMinGtdDownlinkBitrate)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinQosDeliveryOrder",(Builder)BuildMinQosDeliveryOrder)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinMaxSduSize",(Builder)BuildMinMaxSduSize)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinSduErrorRatio",(Builder)BuildMinSduErrorRatio)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinResidualBerRatio",(Builder)BuildMinResidualBerRatio)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinDeliveryErroneousSdu",(Builder)BuildMinDeliveryErroneousSdu)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinTransferDelay",(Builder)BuildMinTransferDelay)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinTrafficHandlingPriority",(Builder)BuildMinTrafficHandlingPriority)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqPrecedenceClass",(Builder)BuildReqPrecedenceClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqDelayClass",(Builder)BuildReqDelayClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqReliabilityClass",(Builder)BuildReqReliabilityClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqPeakThroughputClass",(Builder)BuildReqPeakThroughputClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ReqMeanThroughputClass",(Builder)BuildReqMeanThroughputClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinPrecedenceClass",(Builder)BuildMinPrecedenceClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinDelayClass",(Builder)BuildMinDelayClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinReliabilityClass",(Builder)BuildMinReliabilityClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinPeakThroughputClass",(Builder)BuildMinPeakThroughputClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("MinMeanThroughputClass",(Builder)BuildMinMeanThroughputClass)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("Username",(Builder)BuildUsername)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("Password",(Builder)BuildPassword)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("AuthPref",(Builder)BuildAuthPref)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("Ipv4AddrPref",(Builder)BuildIpv4AddrPref)).second;
        assert(bSuccess);
    }
    return SBMap;
}

// --------------------------------------------------------------------------
// BuildProfileType
//
/// Build the profile type attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildProfileType(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num != 0)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Profile Type '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_profileTypeType = PROFILE_TYPE_TYPE;
    m_profileTypeLen += sizeof(m_profileType);
    m_profileType = (uint8)num;
    m_length += (m_profileTypeLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildProfileName
//
/// Build the profile name attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildProfileName(std::string& value)
{
    // check for null string indication
    if (value == "NULL") { value = ""; }

    // set member data
    m_profileNameType = PROFILE_NAME_TYPE;
    m_profileName = value;
    m_profileNameLen = (uint16)m_profileName.length();
    m_length += (m_profileNameLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildPdpType
//
/// Build the pdp type attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildPdpType(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num != 0)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid PDP Type '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_pdpTypeType = PDP_TYPE_TYPE;
    m_pdpTypeLen += sizeof(m_pdpType);
    m_pdpType = (uint8)num;
    m_length += (m_pdpTypeLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
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
bool WDSCreateProfileReq::BuildApnName(std::string& value)
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
// BuildPrimaryDnsIpv4AddrPref
//
/// Build the primary dns ipv4 address preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildPrimaryDnsIpv4AddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_primaryDnsIpv4AddrPref))
    {
        // set member data
        m_primaryDnsIpv4AddrPrefType = PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;
        m_primaryDnsIpv4AddrPrefLen += sizeof(m_primaryDnsIpv4AddrPref);
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
/// Build the secondary dns ipv4 address preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildSecondaryDnsIpv4AddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_secondaryDnsIpv4AddrPref))
    {
        // set member data
        m_secondaryDnsIpv4AddrPrefType = SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;
        m_secondaryDnsIpv4AddrPrefLen += sizeof(m_secondaryDnsIpv4AddrPref);
        m_length += (m_secondaryDnsIpv4AddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildReqTrafficClass
//
/// Build the requested traffic class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqTrafficClass(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 4)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Traffic Class '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 4.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsRequestedQosType = UMTS_REQUESTED_QOS_TYPE;
    m_umtsRequestedQosLen += sizeof(m_reqTrafficClass);
    m_reqTrafficClass = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqMaxUplinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqMaxUplinkBitrate
//
/// Build the requested maximum uplink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqMaxUplinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqTrafficClass") != 0)
    {
        ReportInvalidSequence("ReqTrafficClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqMaxUplinkBitrate);
    m_reqMaxUplinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqMaxDownlinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqMaxDownlinkBitrate
//
/// Build the requested maximum downlink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqMaxDownlinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqMaxUplinkBitrate") != 0)
    {
        ReportInvalidSequence("ReqMaxUplinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqMaxDownlinkBitrate);
    m_reqMaxDownlinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqGtdUplinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqGtdUplinkBitrate
//
/// Build the requested guaranteed uplink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqGtdUplinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqMaxDownlinkBitrate") != 0)
    {
        ReportInvalidSequence("ReqMaxDownlinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqGtdUplinkBitrate);
    m_reqGtdUplinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqGtdDownlinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqGtdDownlinkBitrate
//
/// Build the requested guaranteed downlink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqGtdDownlinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqGtdUplinkBitrate") != 0)
    {
        ReportInvalidSequence("ReqGtdUplinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqGtdDownlinkBitrate);
    m_reqGtdDownlinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqQosDeliveryOrder";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqQosDeliveryOrder
//
/// Build the requested qos delivery order attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqQosDeliveryOrder(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqGtdDownlinkBitrate") != 0)
    {
        ReportInvalidSequence("ReqGtdDownlinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested QoS Delivery Order '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 4.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqQosDeliveryOrder);
    m_reqQosDeliveryOrder = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqMaxSduSize";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqMaxSduSize
//
/// Build the requested maximum sdu size attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqMaxSduSize(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqQosDeliveryOrder") != 0)
    {
        ReportInvalidSequence("ReqQosDeliveryOrder");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqMaxSduSize);
    m_reqMaxSduSize = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqSduErrorRatio";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqSduErrorRatio
//
/// Build the requested sdu error ratio attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqSduErrorRatio(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqMaxSduSize") != 0)
    {
        ReportInvalidSequence("ReqMaxSduSize");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested SDU Error Ratio '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqSduErrorRatio);
    m_reqSduErrorRatio = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqResidualBerRatio";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqResidualBerRatio
//
/// Build the requested residual ber ratio attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqResidualBerRatio(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqSduErrorRatio") != 0)
    {
        ReportInvalidSequence("ReqSduErrorRatio");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 9)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Residual BER Ratio '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 9.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqResidualBerRatio);
    m_reqResidualBerRatio = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqDeliveryErroneousSdu";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqDeliveryErroneousSdu
//
/// Build the requested delivery erroneous sdu attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqDeliveryErroneousSdu(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqResidualBerRatio") != 0)
    {
        ReportInvalidSequence("ReqResidualBerRatio");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 3)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Delivery of Erroneous SDUs '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 3.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqDeliveryErroneousSdu);
    m_reqDeliveryErroneousSdu = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqTransferDelay";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqTransferDelay
//
/// Build the requested transfer delay attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqTransferDelay(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqDeliveryErroneousSdu") != 0)
    {
        ReportInvalidSequence("ReqDeliveryErroneousSdu");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqTransferDelay);
    m_reqTransferDelay = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqTrafficHandlingPriority";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqTrafficHandlingPriority
//
/// Build the requested traffic handling priority attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqTrafficHandlingPriority(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqTransferDelay") != 0)
    {
        ReportInvalidSequence("ReqTransferDelay");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsRequestedQosLen += sizeof(m_reqTrafficHandlingPriority);
    m_reqTrafficHandlingPriority = (uint32)num;
    m_length += (m_umtsRequestedQosLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildMinTrafficClass
//
/// Build the minimum traffic class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinTrafficClass(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 4)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Traffic Class '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 4.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsMinimumQosType = UMTS_MINIMUM_QOS_TYPE;
    m_umtsMinimumQosLen += sizeof(m_minTrafficClass);
    m_minTrafficClass = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinMaxUplinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinMaxUplinkBitrate
//
/// Build the minimum maximum uplink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinMaxUplinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinTrafficClass") != 0)
    {
        ReportInvalidSequence("MinTrafficClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minMaxUplinkBitrate);
    m_minMaxUplinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinMaxDownlinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinMaxDownlinkBitrate
//
/// Build the minimum maximum downlink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinMaxDownlinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinMaxUplinkBitrate") != 0)
    {
        ReportInvalidSequence("MinMaxUplinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minMaxDownlinkBitrate);
    m_minMaxDownlinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinGtdUplinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinGtdUplinkBitrate
//
/// Build the minimum guaranteed uplink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinGtdUplinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinMaxDownlinkBitrate") != 0)
    {
        ReportInvalidSequence("MinMaxDownlinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minGtdUplinkBitrate);
    m_minGtdUplinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinGtdDownlinkBitrate";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinGtdDownlinkBitrate
//
/// Build the minimum guaranteed downlink bitrate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinGtdDownlinkBitrate(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinGtdUplinkBitrate") != 0)
    {
        ReportInvalidSequence("MinGtdUplinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minGtdDownlinkBitrate);
    m_minGtdDownlinkBitrate = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinQosDeliveryOrder";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinQosDeliveryOrder
//
/// Build the minimum qos delivery order attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinQosDeliveryOrder(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinGtdDownlinkBitrate") != 0)
    {
        ReportInvalidSequence("MinGtdDownlinkBitrate");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum QoS Delivery Order '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 4.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minQosDeliveryOrder);
    m_minQosDeliveryOrder = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinMaxSduSize";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinMaxSduSize
//
/// Build the minimum maximum sdu size attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinMaxSduSize(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinQosDeliveryOrder") != 0)
    {
        ReportInvalidSequence("MinQosDeliveryOrder");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minMaxSduSize);
    m_minMaxSduSize = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinSduErrorRatio";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinSduErrorRatio
//
/// Build the minimum sdu error ratio attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinSduErrorRatio(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinMaxSduSize") != 0)
    {
        ReportInvalidSequence("MinMaxSduSize");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum SDU Error Ratio '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minSduErrorRatio);
    m_minSduErrorRatio = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinResidualBerRatio";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinResidualBerRatio
//
/// Build the minimum residual ber ratio attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinResidualBerRatio(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinSduErrorRatio") != 0)
    {
        ReportInvalidSequence("MinSduErrorRatio");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 9)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Residual BER Ratio '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 9.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minResidualBerRatio);
    m_minResidualBerRatio = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinDeliveryErroneousSdu";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinDeliveryErroneousSdu
//
/// Build the minimum delivery erroneous sdu attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinDeliveryErroneousSdu(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinResidualBerRatio") != 0)
    {
        ReportInvalidSequence("MinResidualBerRatio");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 3)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Delivery of Erroneous SDUs '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 3.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minDeliveryErroneousSdu);
    m_minDeliveryErroneousSdu = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinTransferDelay";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinTransferDelay
//
/// Build the minimum transfer delay attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinTransferDelay(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinDeliveryErroneousSdu") != 0)
    {
        ReportInvalidSequence("MinDeliveryErroneousSdu");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minTransferDelay);
    m_minTransferDelay = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinTrafficHandlingPriority";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinTrafficHandlingPriority
//
/// Build the minimum traffic handling priority attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinTrafficHandlingPriority(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinTransferDelay") != 0)
    {
        ReportInvalidSequence("MinTransferDelay");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_umtsMinimumQosLen += sizeof(m_minTrafficHandlingPriority);
    m_minTrafficHandlingPriority = (uint32)num;
    m_length += (m_umtsMinimumQosLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildReqPrecedenceClass
//
/// Build the requested precedence class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqPrecedenceClass(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 3 && num != 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Precedence Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 3 and 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsRequestedQosType = GPRS_REQUESTED_QOS_TYPE;
    m_gprsRequestedQosLen += sizeof(m_reqPrecedenceClass);
    m_reqPrecedenceClass = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqDelayClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqDelayClass
//
/// Build the requested delay class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqDelayClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqPrecedenceClass") != 0)
    {
        ReportInvalidSequence("ReqPrecedenceClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 4 && num != 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Delay Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 4 and 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsRequestedQosLen += sizeof(m_reqDelayClass);
    m_reqDelayClass = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqReliabilityClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqReliabilityClass
//
/// Build the requested reliability class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqReliabilityClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqDelayClass") != 0)
    {
        ReportInvalidSequence("ReqDelayClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 5 && num != 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Reliability Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 5 and 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsRequestedQosLen += sizeof(m_reqReliabilityClass);
    m_reqReliabilityClass = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqPeakThroughputClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqPeakThroughputClass
//
/// Build the requested peak throughput class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqPeakThroughputClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqReliabilityClass") != 0)
    {
        ReportInvalidSequence("ReqReliabilityClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 9 && num != 15)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Peak Throughput Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 9 and 15.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsRequestedQosLen += sizeof(m_reqPeakThroughputClass);
    m_reqPeakThroughputClass = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ReqMeanThroughputClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildReqMeanThroughputClass
//
/// Build the requested mean throughput class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildReqMeanThroughputClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ReqPeakThroughputClass") != 0)
    {
        ReportInvalidSequence("ReqPeakThroughputClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 18 && num != 30 && num != 31)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Requested Mean Throughput Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 18, 30 and 31.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsRequestedQosLen += sizeof(m_reqMeanThroughputClass);
    m_reqMeanThroughputClass = (uint32)num;
    m_length += (m_gprsRequestedQosLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildMinPrecedenceClass
//
/// Build the minimum precedence class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinPrecedenceClass(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 3 && num != 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Precedence Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 3 and 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsMinimumQosType = GPRS_MINIMUM_QOS_TYPE;
    m_gprsMinimumQosLen += sizeof(m_minPrecedenceClass);
    m_minPrecedenceClass = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinDelayClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinDelayClass
//
/// Build the minimum delay class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinDelayClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinPrecedenceClass") != 0)
    {
        ReportInvalidSequence("MinPrecedenceClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 4 && num != 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Delay Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 4 and 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsMinimumQosLen += sizeof(m_minDelayClass);
    m_minDelayClass = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinReliabilityClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinReliabilityClass
//
/// Build the minimum reliability class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinReliabilityClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinDelayClass") != 0)
    {
        ReportInvalidSequence("MinDelayClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 5 && num != 7)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Reliability Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 5 and 7.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsMinimumQosLen += sizeof(m_minReliabilityClass);
    m_minReliabilityClass = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinPeakThroughputClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinPeakThroughputClass
//
/// Build the minimum peak throughput class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinPeakThroughputClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinReliabilityClass") != 0)
    {
        ReportInvalidSequence("MinReliabilityClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 9 && num != 15)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Peak Throughput Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 9 and 15.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsMinimumQosLen += sizeof(m_minPeakThroughputClass);
    m_minPeakThroughputClass = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "MinMeanThroughputClass";
    return true;
}

// --------------------------------------------------------------------------
// BuildMinMeanThroughputClass
//
/// Build the minimum mean throughput class attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMinMeanThroughputClass(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("MinPeakThroughputClass") != 0)
    {
        ReportInvalidSequence("MinPeakThroughputClass");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 18 && num != 30 && num != 31)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Minimum Mean Throughput Class '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 18, 30 and 31.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_gprsMinimumQosLen += sizeof(m_minMeanThroughputClass);
    m_minMeanThroughputClass = (uint32)num;
    m_length += (m_gprsMinimumQosLen + QMI_TLV_HEADER_BYTES);

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
bool WDSCreateProfileReq::BuildUsername(std::string& value)
{
    // check for null string indication
    if (value == "NULL") { value = ""; }

    // set member data
    m_usernameType = USERNAME_TYPE;
    m_username = value;
    m_usernameLen = (uint16)m_username.length();
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
bool WDSCreateProfileReq::BuildPassword(std::string& value)
{
    // check for null string indication
    if (value == "NULL") { value = ""; }

    // set member data
    m_passwordType = PASSWORD_TYPE;
    m_password = value;
    m_passwordLen = (uint16)m_password.length();
    m_length += (m_passwordLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
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
bool WDSCreateProfileReq::BuildAuthPref(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 3)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Authentication Preference '") << (uint32)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 - 3.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_authPrefType = AUTH_PREF_TYPE;
    m_authPrefLen += sizeof(m_authPref);
    m_authPref = (uint8)num;
    m_length += (m_authPrefLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildIpv4AddrPref
//
/// Build the ipv4 address preference attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildIpv4AddrPref(std::string& value)
{
    if (ExtractIpv4Addr(value,m_ipv4AddrPref))
    {
        // set member data
        m_ipv4AddrPrefType = IPV4_ADDR_PREF_TYPE;
        m_ipv4AddrPrefLen += sizeof(m_ipv4AddrPref);
        m_length += (m_ipv4AddrPrefLen + QMI_TLV_HEADER_BYTES);

        // set previous name to current name and set expectations for next name.
        m_prevName = m_name;
        m_nextName.erase(m_nextName.begin(), m_nextName.end());
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool WDSCreateProfileReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // Mandatory tlv, profile identifier
    m_pMsgBuf->PutByte(m_profileTypeType);
    m_pMsgBuf->PutWord(m_profileTypeLen);
    m_pMsgBuf->PutByte(m_profileType);

    // optional tlv, profile name
    if (m_profileNameType == PROFILE_NAME_TYPE)
    {
        m_pMsgBuf->PutByte(m_profileNameType);
        m_pMsgBuf->PutWord(m_profileNameLen);
        m_pMsgBuf->PutCopy(m_profileName.c_str(),m_profileNameLen);
    }

    // optional tlv, pdp type
    if (m_pdpTypeType == PDP_TYPE_TYPE)
    {
        m_pMsgBuf->PutByte(m_pdpTypeType);
        m_pMsgBuf->PutWord(m_pdpTypeLen);
        m_pMsgBuf->PutByte(m_pdpType);
    }

    // optional tlv, apn name
    if (m_apnNameType == APN_NAME_TYPE)
    {
        m_pMsgBuf->PutByte(m_apnNameType);
        m_pMsgBuf->PutWord(m_apnNameLen);
        m_pMsgBuf->PutCopy(m_apnName.c_str(),m_apnNameLen);
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

    // optional tlv, umts requested qos
    if (m_umtsRequestedQosType == UMTS_REQUESTED_QOS_TYPE)
    {
        m_pMsgBuf->PutByte(m_umtsRequestedQosType);
        m_pMsgBuf->PutWord(m_umtsRequestedQosLen);
        m_pMsgBuf->PutByte(m_reqTrafficClass);
        m_pMsgBuf->PutDWord(m_reqMaxUplinkBitrate);
        m_pMsgBuf->PutDWord(m_reqMaxDownlinkBitrate);
        m_pMsgBuf->PutDWord(m_reqGtdUplinkBitrate);
        m_pMsgBuf->PutDWord(m_reqGtdDownlinkBitrate);
        m_pMsgBuf->PutByte(m_reqQosDeliveryOrder);
        m_pMsgBuf->PutDWord(m_reqMaxSduSize);
        m_pMsgBuf->PutByte(m_reqSduErrorRatio);
        m_pMsgBuf->PutByte(m_reqResidualBerRatio);
        m_pMsgBuf->PutByte(m_reqDeliveryErroneousSdu);
        m_pMsgBuf->PutDWord(m_reqTransferDelay);
        m_pMsgBuf->PutDWord(m_reqTrafficHandlingPriority);
    }

    // optional tlv, umts minimum qos
    if (m_umtsMinimumQosType == UMTS_MINIMUM_QOS_TYPE)
    {
        m_pMsgBuf->PutByte(m_umtsMinimumQosType);
        m_pMsgBuf->PutWord(m_umtsMinimumQosLen);
        m_pMsgBuf->PutByte(m_minTrafficClass);
        m_pMsgBuf->PutDWord(m_minMaxUplinkBitrate);
        m_pMsgBuf->PutDWord(m_minMaxDownlinkBitrate);
        m_pMsgBuf->PutDWord(m_minGtdUplinkBitrate);
        m_pMsgBuf->PutDWord(m_minGtdDownlinkBitrate);
        m_pMsgBuf->PutByte(m_minQosDeliveryOrder);
        m_pMsgBuf->PutDWord(m_minMaxSduSize);
        m_pMsgBuf->PutByte(m_minSduErrorRatio);
        m_pMsgBuf->PutByte(m_minResidualBerRatio);
        m_pMsgBuf->PutByte(m_minDeliveryErroneousSdu);
        m_pMsgBuf->PutDWord(m_minTransferDelay);
        m_pMsgBuf->PutDWord(m_minTrafficHandlingPriority);
    }

    // optional tlv, gprs requested qos
    if (m_gprsRequestedQosType == GPRS_REQUESTED_QOS_TYPE)
    {
        m_pMsgBuf->PutByte(m_gprsRequestedQosType);
        m_pMsgBuf->PutWord(m_gprsRequestedQosLen);
        m_pMsgBuf->PutDWord(m_reqPrecedenceClass);
        m_pMsgBuf->PutDWord(m_reqDelayClass);
        m_pMsgBuf->PutDWord(m_reqReliabilityClass);
        m_pMsgBuf->PutDWord(m_reqPeakThroughputClass);
        m_pMsgBuf->PutDWord(m_reqMeanThroughputClass);
    }

    // optional tlv, gprs minimum qos
    if (m_gprsMinimumQosType == GPRS_MINIMUM_QOS_TYPE)
    {
        m_pMsgBuf->PutByte(m_gprsMinimumQosType);
        m_pMsgBuf->PutWord(m_gprsMinimumQosLen);
        m_pMsgBuf->PutDWord(m_minPrecedenceClass);
        m_pMsgBuf->PutDWord(m_minDelayClass);
        m_pMsgBuf->PutDWord(m_minReliabilityClass);
        m_pMsgBuf->PutDWord(m_minPeakThroughputClass);
        m_pMsgBuf->PutDWord(m_minMeanThroughputClass);
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

    // optional tlv, authentication preference
    if (m_authPrefType == AUTH_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_authPrefType);
        m_pMsgBuf->PutWord(m_authPrefLen);
        m_pMsgBuf->PutByte(m_authPref);
    }

    // optional tlv, ipv4 address preference
    if (m_ipv4AddrPrefType == IPV4_ADDR_PREF_TYPE)
    {
        m_pMsgBuf->PutByte(m_ipv4AddrPrefType);
        m_pMsgBuf->PutWord(m_ipv4AddrPrefLen);
        m_pMsgBuf->PutDWord(m_ipv4AddrPref);
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
void WDSCreateProfileReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_CREATE_PROFILE_REQ" << std::endl
           << _T("{") << std::endl
           << _T("  ProfileType ") << (uint32)m_profileType << std::endl;

    if (m_profileNameType == PROFILE_NAME_TYPE)
    {
        std::string profileName = m_profileName;
        if (profileName == "") { profileName = "NULL"; }
        stream << _T("  ProfileName ") << profileName << std::endl;
    }

    if (m_pdpTypeType == PDP_TYPE_TYPE)
    {
        stream << _T("  PdpType ") << (uint32)m_pdpType << std::endl;
    }

    if (m_apnNameType == APN_NAME_TYPE)
    {
        std::string apnName = m_apnName;
        if (apnName == "") { apnName = "NULL"; }
        stream << _T("  ApnName ") << apnName << std::endl;
    }

    if (m_primaryDnsIpv4AddrPrefType == PRIMARY_DNS_IPV4_ADDR_PREF_TYPE)
    {
        stream << _T("  PrimaryDnsIpv4AddrPref ");
        PrintIPv4Addr(m_primaryDnsIpv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_secondaryDnsIpv4AddrPrefType == SECONDARY_DNS_IPV4_ADDR_PREF_TYPE)
    {
        stream << _T("  SecondaryDnsIpv4AddrPref ");
        PrintIPv4Addr(m_secondaryDnsIpv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_umtsRequestedQosType == UMTS_REQUESTED_QOS_TYPE)
    {
        stream << _T("  ReqTrafficClass ") << (uint32)m_reqTrafficClass << std::endl
               << _T("  ReqMaxUplinkBitrate ") << (uint32)m_reqMaxUplinkBitrate << std::endl
               << _T("  ReqMaxDownlinkBitrate ") << (uint32)m_reqMaxDownlinkBitrate << std::endl
               << _T("  ReqGtdUplinkBitrate ") << (uint32)m_reqGtdUplinkBitrate << std::endl
               << _T("  ReqGtdDownlinkBitrate ") << (uint32)m_reqGtdDownlinkBitrate << std::endl
               << _T("  ReqQosDeliveryOrder ") << (uint32)m_reqQosDeliveryOrder << std::endl
               << _T("  ReqMaxSduSize ") << (uint32)m_reqMaxSduSize << std::endl
               << _T("  ReqSduErrorRatio ") << (uint32)m_reqSduErrorRatio << std::endl
               << _T("  ReqResidualBerRatio ") << (uint32)m_reqResidualBerRatio << std::endl
               << _T("  ReqDeliveryErroneousSdu ") << (uint32)m_reqDeliveryErroneousSdu << std::endl
               << _T("  ReqTransferDelay ") << (uint32)m_reqTransferDelay << std::endl
               << _T("  ReqTrafficHandlingPriority ") << (uint32)m_reqTrafficHandlingPriority << std::endl;
    }

    if (m_umtsMinimumQosType == UMTS_MINIMUM_QOS_TYPE)
    {
        stream << _T("  MinTrafficClass ") << (uint32)m_minTrafficClass << std::endl
               << _T("  MinMaxUplinkBitrate ") << (uint32)m_minMaxUplinkBitrate << std::endl
               << _T("  MinMaxDownlinkBitrate ") << (uint32)m_minMaxDownlinkBitrate << std::endl
               << _T("  MinGtdUplinkBitrate ") << (uint32)m_minGtdUplinkBitrate << std::endl
               << _T("  MinGtdDownlinkBitrate ") << (uint32)m_minGtdDownlinkBitrate << std::endl
               << _T("  MinQosDeliveryOrder ") << (uint32)m_minQosDeliveryOrder << std::endl
               << _T("  MinMaxSduSize ") << (uint32)m_minMaxSduSize << std::endl
               << _T("  MinSduErrorRatio ") << (uint32)m_minSduErrorRatio << std::endl
               << _T("  MinResidualBerRatio ") << (uint32)m_minResidualBerRatio << std::endl
               << _T("  MinDeliveryErroneousSdu ") << (uint32)m_minDeliveryErroneousSdu << std::endl
               << _T("  MinTransferDelay ") << (uint32)m_minTransferDelay << std::endl
               << _T("  MinTrafficHandlingPriority ") << (uint32)m_minTrafficHandlingPriority << std::endl;
    }

    if (m_gprsRequestedQosType == GPRS_REQUESTED_QOS_TYPE)
    {
        stream << _T("  ReqPrecedenceClass ") << (uint32)m_reqPrecedenceClass << std::endl
               << _T("  ReqDelayClass ") << (uint32)m_reqDelayClass << std::endl
               << _T("  ReqReliabilityClass ") << (uint32)m_reqReliabilityClass << std::endl
               << _T("  ReqPeakThroughputClass ") << (uint32)m_reqPeakThroughputClass << std::endl
               << _T("  ReqMeanThroughputClass ") << (uint32)m_reqMeanThroughputClass << std::endl;
    }

    if (m_gprsMinimumQosType == GPRS_MINIMUM_QOS_TYPE)
    {
        stream << _T("  MinPrecedenceClass ") << (uint32)m_minPrecedenceClass << std::endl
               << _T("  MinDelayClass ") << (uint32)m_minDelayClass << std::endl
               << _T("  MinReliabilityClass ") << (uint32)m_minReliabilityClass << std::endl
               << _T("  MinPeakThroughputClass ") << (uint32)m_minPeakThroughputClass << std::endl
               << _T("  MinMeanThroughputClass ") << (uint32)m_minMeanThroughputClass << std::endl;
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

    if (m_authPrefType == AUTH_PREF_TYPE)
    {
        stream << _T("  AuthPref ") << (uint32)m_authPref << std::endl;
    }

    if (m_ipv4AddrPrefType == IPV4_ADDR_PREF_TYPE)
    {
        stream << _T("  Ipv4AddrPref ");
        PrintIPv4Addr(m_ipv4AddrPref,stream);
        stream << std::endl;
    }
           
    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSCreateProfileRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSCreateProfileRsp.
// --------------------------------------------------------------------------
const uint8 WDSCreateProfileRsp::RESULT_CODE_TYPE = 0x02;
const uint8 WDSCreateProfileRsp::PROFILE_IDENTIFIER_TYPE = 0x01;
WDSCreateProfileRsp::WDSCreateProfileRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_CREATE_PROFILE_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_profileIdentifierType(TLV_TYPE_INVALID),
    m_profileIdentifierLen(0),
    m_profileType(-1),
    m_profileIndex(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSCreateProfileRsp.
// --------------------------------------------------------------------------
WDSCreateProfileRsp::~WDSCreateProfileRsp()
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
bool WDSCreateProfileRsp::Unpack(MsgBuf& msgBuf)
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
        if (m_length != 12) 
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("Expected message length is 12 bytes, unpacked length is ")
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
Message::Uint8UnpackerMap& WDSCreateProfileRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(PROFILE_IDENTIFIER_TYPE,(Unpacker)UnpackProfileIdentifier)).second;
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
bool WDSCreateProfileRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackProfileIdentifier
//
/// Unpack the profile identifier tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSCreateProfileRsp::UnpackProfileIdentifier(MsgBuf& msgBuf)
{
    m_profileIdentifierType = PROFILE_IDENTIFIER_TYPE;

    m_profileIdentifierLen = msgBuf.GetWord();
    if (m_profileIdentifierLen != 2) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Profile Identifier length is 2 bytes, unpacked length is ")
               << m_profileIdentifierLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_profileType = msgBuf.GetByte();
    m_profileIndex = msgBuf.GetByte();

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
void WDSCreateProfileRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_CREATE_PROFILE_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    if (m_profileIdentifierType == PROFILE_IDENTIFIER_TYPE)
    {
        stream << _T("  ProfileType ") << (int)m_profileType << std::endl
               << _T("  ProfileIndex ") << (int)m_profileIndex << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
