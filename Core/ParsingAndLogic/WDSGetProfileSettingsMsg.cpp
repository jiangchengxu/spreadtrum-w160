// --------------------------------------------------------------------------
//
// WDSGetProfileSettingsMsg.cpp
//
/// WDSGetProfileSettingsReq/WDSGetProfileSettingsRsp implementation.
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
#include "WDSGetProfileSettingsMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSGetProfileSettingsReq> ReqStringCreator("QMI_WDS_GET_PROFILE_SETTINGS_REQ");
static MessageCreator<WDSGetProfileSettingsReq> ReqUint32Creator(WDSGetProfileSettingsReqUID);

static MessageCreator<WDSGetProfileSettingsRsp> RspStringCreator("QMI_WDS_GET_PROFILE_SETTINGS_RSP");
static MessageCreator<WDSGetProfileSettingsRsp> RspUint32Creator(WDSGetProfileSettingsRspUID);

// --------------------------------------------------------------------------
// WDSGetProfileSettingsReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetProfileSettingsReq.
// --------------------------------------------------------------------------
const uint8 WDSGetProfileSettingsReq::PROFILE_IDENTIFIER_TYPE = 0x01;
WDSGetProfileSettingsReq::WDSGetProfileSettingsReq() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PROFILE_SETTINGS_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_profileIdentifierType(TLV_TYPE_INVALID),
    m_profileIdentifierLen(0),
    m_profileType(-1),
    m_profileIndex(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetProfileSettingsReq.
// --------------------------------------------------------------------------
WDSGetProfileSettingsReq::~WDSGetProfileSettingsReq()
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
bool WDSGetProfileSettingsReq::Build(std::string& nameValue)
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
Message::StringBuilderMap& WDSGetProfileSettingsReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty()) {
        bool bSuccess = SBMap.insert(SBPair("ProfileType",(Builder)BuildProfileType)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("ProfileIndex",(Builder)BuildProfileIndex)).second;
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
bool WDSGetProfileSettingsReq::BuildProfileType(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_profileIdentifierType = PROFILE_IDENTIFIER_TYPE;
    m_profileIdentifierLen += sizeof(m_profileType);
    m_profileType = (uint8)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "ProfileIndex";
    return true;
}

// --------------------------------------------------------------------------
// BuildProfileIndex
//
/// Build the profile index attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsReq::BuildProfileIndex(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_profileIdentifierLen += sizeof(m_profileIndex);
    m_profileIndex = (uint8)num;
    m_length += (m_profileIdentifierLen + QMI_TLV_HEADER_BYTES);

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
bool WDSGetProfileSettingsReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // Mandatory tlv, tx_id
    m_pMsgBuf->PutByte(m_profileIdentifierType);
    m_pMsgBuf->PutWord(m_profileIdentifierLen);
    m_pMsgBuf->PutByte(m_profileType);
    m_pMsgBuf->PutByte(m_profileIndex);

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
void WDSGetProfileSettingsReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_PROFILE_SETTINGS_REQ" << std::endl
           << _T("{") << std::endl
           << _T("  ProfileType ") << (int)m_profileType << std::endl
           << _T("  ProfileIndex ") << (int)m_profileIndex << std::endl
           << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSGetProfileSettingsRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetProfileSettingsRsp.
// --------------------------------------------------------------------------
const uint8 WDSGetProfileSettingsRsp::RESULT_CODE_TYPE = 0x02;
const uint8 WDSGetProfileSettingsRsp::PROFILE_NAME_TYPE = 0x10;
const uint8 WDSGetProfileSettingsRsp::PDP_TYPE_TYPE = 0x11;
const uint8 WDSGetProfileSettingsRsp::APN_NAME_TYPE = 0x14;
const uint8 WDSGetProfileSettingsRsp::PRIMARY_DNS_IPV4_ADDR_PREF_TYPE = 0x15;
const uint8 WDSGetProfileSettingsRsp::SECONDARY_DNS_IPV4_ADDR_PREF_TYPE = 0x16;
const uint8 WDSGetProfileSettingsRsp::UMTS_REQUESTED_QOS_TYPE = 0x17;
const uint8 WDSGetProfileSettingsRsp::UMTS_MINIMUM_QOS_TYPE = 0x18;
const uint8 WDSGetProfileSettingsRsp::GPRS_REQUESTED_QOS_TYPE = 0x19;
const uint8 WDSGetProfileSettingsRsp::GPRS_MINIMUM_QOS_TYPE = 0x1A;
const uint8 WDSGetProfileSettingsRsp::USERNAME_TYPE = 0x1B;
const uint8 WDSGetProfileSettingsRsp::AUTH_PREF_TYPE = 0x1D;
const uint8 WDSGetProfileSettingsRsp::IPV4_ADDR_PREF_TYPE = 0x1E;
WDSGetProfileSettingsRsp::WDSGetProfileSettingsRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PROFILE_SETTINGS_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
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
/// Destructor for WDSGetProfileSettingsRsp.
// --------------------------------------------------------------------------
WDSGetProfileSettingsRsp::~WDSGetProfileSettingsRsp()
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
bool WDSGetProfileSettingsRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length on failure (variable on success)
    if (m_result != QMI_RESULT_SUCCESS && m_length != 7) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected message length is 7 bytes, unpacked length is ")
               << m_length << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
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
Message::Uint8UnpackerMap& WDSGetProfileSettingsRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(PROFILE_NAME_TYPE,(Unpacker)UnpackProfileName)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(PDP_TYPE_TYPE,(Unpacker)UnpackPdpType)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(APN_NAME_TYPE,(Unpacker)UnpackApnName)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(PRIMARY_DNS_IPV4_ADDR_PREF_TYPE,(Unpacker)UnpackPrimaryDnsIpv4AddrPref)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(SECONDARY_DNS_IPV4_ADDR_PREF_TYPE,(Unpacker)UnpackSecondaryDnsIpv4AddrPref)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(UMTS_REQUESTED_QOS_TYPE,(Unpacker)UnpackUmtsRequestedQos)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(UMTS_MINIMUM_QOS_TYPE,(Unpacker)UnpackUmtsMinimumQos)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(GPRS_REQUESTED_QOS_TYPE,(Unpacker)UnpackGprsRequestedQos)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(GPRS_MINIMUM_QOS_TYPE,(Unpacker)UnpackGprsMinimumQos)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(USERNAME_TYPE,(Unpacker)UnpackUsername)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(AUTH_PREF_TYPE,(Unpacker)UnpackAuthPref)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(IPV4_ADDR_PREF_TYPE,(Unpacker)UnpackIpv4AddrPref)).second;
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
bool WDSGetProfileSettingsRsp::UnpackResultCode(MsgBuf& msgBuf)
{
    m_resultCodeType = RESULT_CODE_TYPE;

    m_resultCodeLen = msgBuf.GetWord();
    if (m_resultCodeLen != 4) {
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
// UnpackProfileName
//
/// Unpack the profile name tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackProfileName(MsgBuf& msgBuf)
{
    m_profileNameType = PROFILE_NAME_TYPE;
    m_profileNameLen = msgBuf.GetWord();
    m_profileName.resize(m_profileNameLen);
    msgBuf.GetCopy(&m_profileName[0],m_profileNameLen);

    return true;
}

// --------------------------------------------------------------------------
// UnpackPdpType
//
/// Unpack the pdp type tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackPdpType(MsgBuf& msgBuf)
{
    m_pdpTypeType = PDP_TYPE_TYPE;

    m_pdpTypeLen = msgBuf.GetWord();
    if (m_pdpTypeLen != 1) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected PDP Type length is 1 byte, unpacked length is ")
               << m_pdpTypeLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_pdpType = msgBuf.GetByte();

    return true;
}

// --------------------------------------------------------------------------
// UnpackApnName
//
/// Unpack the apn name tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackApnName(MsgBuf& msgBuf)
{
    m_apnNameType = APN_NAME_TYPE;
    m_apnNameLen = msgBuf.GetWord();
    m_apnName.resize(m_apnNameLen);
    msgBuf.GetCopy(&m_apnName[0],m_apnNameLen);

    return true;
}

// --------------------------------------------------------------------------
// UnpackPrimaryDnsIpv4AddrPref
//
/// Unpack the primary dns ipv4 address preference tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackPrimaryDnsIpv4AddrPref(MsgBuf& msgBuf)
{
    m_primaryDnsIpv4AddrPrefType = PRIMARY_DNS_IPV4_ADDR_PREF_TYPE;

    m_primaryDnsIpv4AddrPrefLen = msgBuf.GetWord();
    if (m_primaryDnsIpv4AddrPrefLen != 4) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Primary Dns IPv4 Address Preference length is 4 bytes, unpacked length is ")
               << m_primaryDnsIpv4AddrPrefLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_primaryDnsIpv4AddrPref = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackSecondaryDnsIpv4AddrPref
//
/// Unpack the secondary dns ipv4 address preference tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackSecondaryDnsIpv4AddrPref(MsgBuf& msgBuf)
{
    m_secondaryDnsIpv4AddrPrefType = SECONDARY_DNS_IPV4_ADDR_PREF_TYPE;

    m_secondaryDnsIpv4AddrPrefLen = msgBuf.GetWord();
    if (m_secondaryDnsIpv4AddrPrefLen != 4) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Secondary Dns IPv4 Address Preference length is 4 bytes, unpacked length is ")
               << m_secondaryDnsIpv4AddrPrefLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_secondaryDnsIpv4AddrPref = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackUmtsRequestedQos
//
/// Unpack the umts requested qos tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackUmtsRequestedQos(MsgBuf& msgBuf)
{
    m_umtsRequestedQosType = UMTS_REQUESTED_QOS_TYPE;

    m_umtsRequestedQosLen = msgBuf.GetWord();
    if (m_umtsRequestedQosLen != 33) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected UMTS Requested QoS length is 33 bytes, unpacked length is ")
               << m_umtsRequestedQosLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_reqTrafficClass = msgBuf.GetByte();
    m_reqMaxUplinkBitrate = msgBuf.GetDWord();
    m_reqMaxDownlinkBitrate = msgBuf.GetDWord();
    m_reqGtdUplinkBitrate = msgBuf.GetDWord();
    m_reqGtdDownlinkBitrate = msgBuf.GetDWord();
    m_reqQosDeliveryOrder = msgBuf.GetByte();
    m_reqMaxSduSize = msgBuf.GetDWord();
    m_reqSduErrorRatio = msgBuf.GetByte();
    m_reqResidualBerRatio = msgBuf.GetByte();
    m_reqDeliveryErroneousSdu = msgBuf.GetByte();
    m_reqTransferDelay = msgBuf.GetDWord();
    m_reqTrafficHandlingPriority = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackUmtsMinimumQos
//
/// Unpack the umts minimum qos tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackUmtsMinimumQos(MsgBuf& msgBuf)
{
    m_umtsMinimumQosType = UMTS_MINIMUM_QOS_TYPE;

    m_umtsMinimumQosLen = msgBuf.GetWord();
    if (m_umtsMinimumQosLen != 33) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected UMTS Minimum QoS length is 33 bytes, unpacked length is ")
               << m_umtsMinimumQosLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_minTrafficClass = msgBuf.GetByte();
    m_minMaxUplinkBitrate = msgBuf.GetDWord();
    m_minMaxDownlinkBitrate = msgBuf.GetDWord();
    m_minGtdUplinkBitrate = msgBuf.GetDWord();
    m_minGtdDownlinkBitrate = msgBuf.GetDWord();
    m_minQosDeliveryOrder = msgBuf.GetByte();
    m_minMaxSduSize = msgBuf.GetDWord();
    m_minSduErrorRatio = msgBuf.GetByte();
    m_minResidualBerRatio = msgBuf.GetByte();
    m_minDeliveryErroneousSdu = msgBuf.GetByte();
    m_minTransferDelay = msgBuf.GetDWord();
    m_minTrafficHandlingPriority = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackGprsRequestedQos
//
/// Unpack the gprs requested qos tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackGprsRequestedQos(MsgBuf& msgBuf)
{
    m_gprsRequestedQosType = GPRS_REQUESTED_QOS_TYPE;

    m_gprsRequestedQosLen = msgBuf.GetWord();
    if (m_gprsRequestedQosLen != 20) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected GPRS Requested QoS length is 20 bytes, unpacked length is ")
               << m_gprsRequestedQosLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_reqPrecedenceClass = msgBuf.GetDWord();
    m_reqDelayClass = msgBuf.GetDWord();
    m_reqReliabilityClass = msgBuf.GetDWord();
    m_reqPeakThroughputClass = msgBuf.GetDWord();
    m_reqMeanThroughputClass = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackGprsMinimumQos
//
/// Unpack the gprs minimum qos tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackGprsMinimumQos(MsgBuf& msgBuf)
{
    m_gprsMinimumQosType = GPRS_MINIMUM_QOS_TYPE;

    m_gprsMinimumQosLen = msgBuf.GetWord();
    if (m_gprsMinimumQosLen != 20) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected GPRS Minimum QoS length is 20 bytes, unpacked length is ")
               << m_gprsMinimumQosLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_minPrecedenceClass = msgBuf.GetDWord();
    m_minDelayClass = msgBuf.GetDWord();
    m_minReliabilityClass = msgBuf.GetDWord();
    m_minPeakThroughputClass = msgBuf.GetDWord();
    m_minMeanThroughputClass = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackUsername
//
/// Unpack the username tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackUsername(MsgBuf& msgBuf)
{
    m_usernameType = USERNAME_TYPE;
    m_usernameLen = msgBuf.GetWord();
    m_username.resize(m_usernameLen);
    msgBuf.GetCopy(&m_username[0],m_usernameLen);

    return true;
}

// --------------------------------------------------------------------------
// UnpackAuthPref
//
/// Unpack the authentication preference tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackAuthPref(MsgBuf& msgBuf)
{
    m_authPrefType = AUTH_PREF_TYPE;

    m_authPrefLen = msgBuf.GetWord();
    if (m_authPrefLen != 1) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Authentication Preference length is 1 byte, unpacked length is ")
               << m_authPrefLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_authPref = msgBuf.GetByte();

    return true;
}

// --------------------------------------------------------------------------
// UnpackIpv4AddrPref
//
/// Unpack the ipv4 address preference tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetProfileSettingsRsp::UnpackIpv4AddrPref(MsgBuf& msgBuf)
{
    m_ipv4AddrPrefType = IPV4_ADDR_PREF_TYPE;

    m_ipv4AddrPrefLen = msgBuf.GetWord();
    if (m_ipv4AddrPrefLen != 4) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected IPv4 Address Preference length is 4 bytes, unpacked length is ")
               << m_ipv4AddrPrefLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_ipv4AddrPref = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSGetProfileSettingsRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_PROFILE_SETTINGS_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (uint32)m_result << std::endl
           << _T("  ErrorCode ") << (uint32)m_error << std::endl;

    if (m_profileNameType == PROFILE_NAME_TYPE) {
        std::string profileName = m_profileName;
        if (profileName.empty()) {
            profileName = "NULL";
        }
        stream << _T("  ProfileName ") << profileName << std::endl;
    }

    if (m_pdpTypeType == PDP_TYPE_TYPE) {
        stream << _T("  PdpType ") << (uint32)m_pdpType << std::endl;
    }

    if (m_apnNameType == APN_NAME_TYPE) {
        std::string apnName = m_apnName;
        if (apnName.empty()) {
            apnName = "NULL";
        }
        stream << _T("  ApnName ") << apnName << std::endl;
    }

    if (m_primaryDnsIpv4AddrPrefType == PRIMARY_DNS_IPV4_ADDR_PREF_TYPE) {
        stream << _T("  PrimaryDnsIpv4AddrPref ");
        PrintIPv4Addr(m_primaryDnsIpv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_secondaryDnsIpv4AddrPrefType == SECONDARY_DNS_IPV4_ADDR_PREF_TYPE) {
        stream << _T("  SecondaryDnsIpv4AddrPref ");
        PrintIPv4Addr(m_secondaryDnsIpv4AddrPref,stream);
        stream << std::endl;
    }

    if (m_umtsRequestedQosType == UMTS_REQUESTED_QOS_TYPE) {
        stream << _T("  ReqTrafficClass ") << (uint32)m_reqTrafficClass << std::endl;
        stream << _T("  ReqMaxUplinkBitrate ") << (uint32)m_reqMaxUplinkBitrate << std::endl;
        stream << _T("  ReqMaxDownlinkBitrate ") << (uint32)m_reqMaxDownlinkBitrate << std::endl;
        stream << _T("  ReqGtdUplinkBitrate ") << (uint32)m_reqGtdUplinkBitrate << std::endl;
        stream << _T("  ReqGtdDownlinkBitrate ") << (uint32)m_reqGtdDownlinkBitrate << std::endl;
        stream << _T("  ReqQosDeliveryOrder ") << (uint32)m_reqQosDeliveryOrder << std::endl;
        stream << _T("  ReqMaxSduSize ") << (uint32)m_reqMaxSduSize << std::endl;
        stream << _T("  ReqSduErrorRatio ") << (uint32)m_reqSduErrorRatio << std::endl;
        stream << _T("  ReqResidualBerRatio ") << (uint32)m_reqResidualBerRatio << std::endl;
        stream << _T("  ReqDeliveryErroneousSdu ") << (uint32)m_reqDeliveryErroneousSdu << std::endl;
        stream << _T("  ReqTransferDelay ") << (uint32)m_reqTransferDelay << std::endl;
        stream << _T("  ReqTrafficHandlingPriority ") << (uint32)m_reqTrafficHandlingPriority << std::endl;
    }

    if (m_umtsMinimumQosType == UMTS_MINIMUM_QOS_TYPE) {
        stream << _T("  MinTrafficClass ") << (uint32)m_minTrafficClass << std::endl;
        stream << _T("  MinMaxUplinkBitrate ") << (uint32)m_minMaxUplinkBitrate << std::endl;
        stream << _T("  MinMaxDownlinkBitrate ") << (uint32)m_minMaxDownlinkBitrate << std::endl;
        stream << _T("  MinGtdUplinkBitrate ") << (uint32)m_minGtdUplinkBitrate << std::endl;
        stream << _T("  MinGtdDownlinkBitrate ") << (uint32)m_minGtdDownlinkBitrate << std::endl;
        stream << _T("  MinQosDeliveryOrder ") << (uint32)m_minQosDeliveryOrder << std::endl;
        stream << _T("  MinMaxSduSize ") << (uint32)m_minMaxSduSize << std::endl;
        stream << _T("  MinSduErrorRatio ") << (uint32)m_minSduErrorRatio << std::endl;
        stream << _T("  MinResidualBerRatio ") << (uint32)m_minResidualBerRatio << std::endl;
        stream << _T("  MinDeliveryErroneousSdu ") << (uint32)m_minDeliveryErroneousSdu << std::endl;
        stream << _T("  MinTransferDelay ") << (uint32)m_minTransferDelay << std::endl;
        stream << _T("  MinTrafficHandlingPriority ") << (uint32)m_minTrafficHandlingPriority << std::endl;
    }

    if (m_gprsRequestedQosType == GPRS_REQUESTED_QOS_TYPE) {
        stream << _T("  ReqPrecedenceClass ") << (uint32)m_reqPrecedenceClass << std::endl;
        stream << _T("  ReqDelayClass ") << (uint32)m_reqDelayClass << std::endl;
        stream << _T("  ReqReliabilityClass ") << (uint32)m_reqReliabilityClass << std::endl;
        stream << _T("  ReqPeakThroughputClass ") << (uint32)m_reqPeakThroughputClass << std::endl;
        stream << _T("  ReqMeanThroughputClass ") << (uint32)m_reqMeanThroughputClass << std::endl;
    }

    if (m_gprsMinimumQosType == GPRS_MINIMUM_QOS_TYPE) {
        stream << _T("  MinPrecedenceClass ") << (uint32)m_minPrecedenceClass << std::endl;
        stream << _T("  MinDelayClass ") << (uint32)m_minDelayClass << std::endl;
        stream << _T("  MinReliabilityClass ") << (uint32)m_minReliabilityClass << std::endl;
        stream << _T("  MinPeakThroughputClass ") << (uint32)m_minPeakThroughputClass << std::endl;
        stream << _T("  MinMeanThroughputClass ") << (uint32)m_minMeanThroughputClass << std::endl;
    }

    if (m_usernameType == USERNAME_TYPE) {
        std::string username = m_username;
        if (username.empty()) {
            username = "NULL";
        }
        stream << _T("  Username ") << username << std::endl;
    }

    if (m_authPref == AUTH_PREF_TYPE) {
        stream << _T("  AuthPref ") << (uint32)m_authPref << std::endl;
    }

    if (m_ipv4AddrPrefType == IPV4_ADDR_PREF_TYPE) {
        stream << _T("  Ipv4AddrPref ");
        PrintIPv4Addr(m_ipv4AddrPref,stream);
        stream << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
