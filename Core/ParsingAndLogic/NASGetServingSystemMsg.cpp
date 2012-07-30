// --------------------------------------------------------------------------
//
// NASGetServingSystemMsg.cpp
//
/// NASGetServingSystemReq/NASGetServingSystemRsp/NASServingSystemInd
/// implementation.
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
#include "NASGetServingSystemMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<NASGetServingSystemReq> ReqStringCreator("QMI_NAS_GET_SERVING_SYSTEM_REQ");
static MessageCreator<NASGetServingSystemReq> ReqUint32Creator(NASGetServingSystemReqUID);

static MessageCreator<NASGetServingSystemRsp> RspStringCreator("QMI_NAS_GET_SERVING_SYSTEM_RSP");
static MessageCreator<NASGetServingSystemRsp> RspUint32Creator(NASGetServingSystemRspUID);

static MessageCreator<NASServingSystemInd> IndStringCreator("QMI_NAS_GET_SERVING_SYSTEM_IND");
static MessageCreator<NASServingSystemInd> IndUint32Creator(NASServingSystemIndUID);

// --------------------------------------------------------------------------
// NASSetEventReportReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASGetServingSystemMsg.
// --------------------------------------------------------------------------
NASGetServingSystemReq::NASGetServingSystemReq() : 
    Message(QMUX_TYPE_NAS,QMI_NAS_GET_SERVING_SYSTEM_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportReq.
// --------------------------------------------------------------------------
NASGetServingSystemReq::~NASGetServingSystemReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool NASGetServingSystemReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

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
void NASGetServingSystemReq::Print(std::ostream& stream)
{
    stream << "QMI_NAS_GET_SERVING_SYSTEM_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// NASGetServingSystemRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASGetServingSystemRsp.
// --------------------------------------------------------------------------
const uint8 NASGetServingSystemRsp::RESULT_CODE_TYPE = 0x02;
const uint8 NASGetServingSystemRsp::SERVING_SYSTEM_TYPE = 0x01;
const uint8 NASGetServingSystemRsp::ROAMING_INDICATOR_TYPE = 0x10;
const uint8 NASGetServingSystemRsp::CURRENT_PLMN_TYPE = 0x12;
NASGetServingSystemRsp::NASGetServingSystemRsp() :
    Message(QMUX_TYPE_NAS,QMI_NAS_GET_SERVING_SYSTEM_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_servingSystemType(TLV_TYPE_INVALID),
    m_servingSystemLen(-1),
    m_registrationState(-1),
    m_csAttachState(-1),
    m_psAttachState(-1),
    m_registeredNetwork(-1),
    m_numRadioIfs(0),
    m_roamingIndicatorType(TLV_TYPE_INVALID),
    m_roamingIndicatorLen(0),
    m_roamingIndicator(-1),
    m_currentPlmnType(TLV_TYPE_INVALID),
    m_currentPlmnLen(0),
    m_mobileCountryCode(-1),
    m_mobileNetworkCode(-1),
    m_networkDescLen(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportRsp.
// --------------------------------------------------------------------------
NASGetServingSystemRsp::~NASGetServingSystemRsp()
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
bool NASGetServingSystemRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }
    
    // validate message length
    if (m_result != QMI_RESULT_SUCCESS)
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

    // should be at the end of buffer
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
// GetUnpackerMap
//
/// Return a reference to the uint8 to unpacker map for this Message.
///
/// @returns Uint*UnpackerMap& - reference to the uint8 to unpacker map for
///     this Message type.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& NASGetServingSystemRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(SERVING_SYSTEM_TYPE,(Unpacker)UnpackServingSystem)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(ROAMING_INDICATOR_TYPE,(Unpacker)UnpackRoamingIndicator)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(CURRENT_PLMN_TYPE,(Unpacker)UnpackCurrentPlmn)).second;
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
bool NASGetServingSystemRsp::UnpackResultCode(MsgBuf& msgBuf)
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

    // should be at the end of buffer on failure
    if (m_result == QMI_RESULT_FAILURE && !msgBuf.EOB())
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
// UnpackServingSystem
//
/// Unpack the serving system tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASGetServingSystemRsp::UnpackServingSystem(MsgBuf& msgBuf)
{
    m_servingSystemType = SERVING_SYSTEM_TYPE;
    m_servingSystemLen = msgBuf.GetWord();

    m_registrationState = msgBuf.GetByte();
    // !!! allow 6, 802.11 (QMI_RADIO_IF_WLAN), not advertised as supported by RmNet yet
    if (m_registrationState > 4 && m_registrationState != 6)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Registration State values are 0 - 4 , unpacked value is ")
               << (uint32)m_registrationState << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_csAttachState = msgBuf.GetByte();
    if (m_csAttachState > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid CS Attach State values are 0 - 2 , unpacked value is ")
               << (uint32)m_csAttachState << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_psAttachState = msgBuf.GetByte();
    if (m_psAttachState > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid PS Attach State values are 0 - 2 , unpacked value is ")
               << (uint32)m_psAttachState << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_registeredNetwork = msgBuf.GetByte();
    if (m_registeredNetwork > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Registered Network values are 0 - 2 , unpacked value is ")
               << (uint32)m_registeredNetwork << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_numRadioIfs = msgBuf.GetByte();
    for (int i = 0; i < m_numRadioIfs; i++)
    {
        m_radioIfs.push_back(msgBuf.GetByte());
        if (m_radioIfs[i] > 5)
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("Valid Radio IF values are 0 - 5 , unpacked value is ")
                   << (uint32)m_radioIfs[i] << _T(".") << std::endl 
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    }

    return true;
}

// --------------------------------------------------------------------------
// UnpackRoamingIndicator
//
/// Unpack the roaming indicator tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASGetServingSystemRsp::UnpackRoamingIndicator(MsgBuf& msgBuf)
{
    m_roamingIndicatorType = ROAMING_INDICATOR_TYPE;
    m_roamingIndicatorLen = msgBuf.GetWord();
    if (m_roamingIndicatorLen != 1) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Roaming Indicator length is 1 bytes, unpacked length is ")
               << (uint32)m_roamingIndicatorLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_roamingIndicator = msgBuf.GetByte();
    return true;
}

// --------------------------------------------------------------------------
// UnpackCurrentPlmn
//
/// Unpack the current plmn tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASGetServingSystemRsp::UnpackCurrentPlmn(MsgBuf& msgBuf)
{
    m_currentPlmnType = CURRENT_PLMN_TYPE;
    m_currentPlmnLen = msgBuf.GetWord();

    m_mobileCountryCode = msgBuf.GetWord();
    if (m_mobileCountryCode > 999)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Mobile Country Code values are 0 - 999 , unpacked value is ")
               << (uint32)m_mobileCountryCode << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_mobileNetworkCode = msgBuf.GetWord();
    if (m_mobileNetworkCode > 999)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Mobile Network Code values are 0 - 999 , unpacked value is ")
               << (uint32)m_mobileNetworkCode << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_networkDescLen = msgBuf.GetByte();

    m_networkDesc.resize(m_networkDescLen);
    msgBuf.GetCopy(&m_networkDesc[0],m_networkDescLen);
    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void NASGetServingSystemRsp::Print(std::ostream& stream)
{
    stream << "QMI_NAS_GET_SERVING_SYSTEM_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    if (m_servingSystemType == SERVING_SYSTEM_TYPE)
    {
        stream << _T("  RegistrationState ") << (uint32)m_registrationState << std::endl
               << _T("  CsAttachState ") << (uint32)m_csAttachState << std::endl
               << _T("  PsAttachState ") << (uint32)m_psAttachState << std::endl
               << _T("  RegisteredNetwork ") << (uint32)m_registeredNetwork << std::endl;

        for (int i = 0; i < m_numRadioIfs; i++)
        {
            stream  << _T("  RadioIf ") << (uint32)m_radioIfs[i] << std::endl;
        }
    }

    if (m_roamingIndicatorType == ROAMING_INDICATOR_TYPE)
    {
        stream << _T("  RoamingIndicator ") << (uint32)m_roamingIndicator << std::endl;
    }

    if (m_currentPlmnType == CURRENT_PLMN_TYPE)
    {
        stream << _T("  MobileCountryCode ") << (uint32)m_mobileCountryCode << std::endl
               << _T("  MobileNetworkCode ") << (uint32)m_mobileNetworkCode << std::endl
               << _T("  NetworkDesc ") << m_networkDesc << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// NASServingSystemInd
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASServingSystemInd.
// --------------------------------------------------------------------------
const uint8 NASServingSystemInd::SERVING_SYSTEM_TYPE = 0x01;
const uint8 NASServingSystemInd::ROAMING_INDICATOR_TYPE = 0x10;
const uint8 NASServingSystemInd::CURRENT_PLMN_TYPE = 0x12;
NASServingSystemInd::NASServingSystemInd() :
    Message(QMUX_TYPE_NAS,QMI_NAS_GET_SERVING_SYSTEM_MSG,QMI_CTL_FLAG_TYPE_IND),
    m_servingSystemType(TLV_TYPE_INVALID),
    m_servingSystemLen(-1),
    m_registrationState(-1),
    m_csAttachState(-1),
    m_psAttachState(-1),
    m_registeredNetwork(-1),
    m_numRadioIfs(0),
    m_roamingIndicatorType(TLV_TYPE_INVALID),
    m_roamingIndicatorLen(0),
    m_roamingIndicator(-1),
    m_currentPlmnType(TLV_TYPE_INVALID),
    m_currentPlmnLen(0),
    m_mobileCountryCode(-1),
    m_mobileNetworkCode(-1),
    m_networkDescLen(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportRsp.
// --------------------------------------------------------------------------
NASServingSystemInd::~NASServingSystemInd()
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
bool NASServingSystemInd::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }

    // mandatory tlv must be present
    if (m_servingSystemType != SERVING_SYSTEM_TYPE)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("The following mandatory tlv must be present:") << std::endl 
               << _T("  Serving System") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // should be at the end of buffer
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
// GetUnpackerMap
//
/// Return a reference to the uint8 to unpacker map for this Message.
///
/// @returns Uint*UnpackerMap& - reference to the uint8 to unpacker map for
///     this Message type.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& NASServingSystemInd::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(SERVING_SYSTEM_TYPE,(Unpacker)UnpackServingSystem)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(ROAMING_INDICATOR_TYPE,(Unpacker)UnpackRoamingIndicator)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(CURRENT_PLMN_TYPE,(Unpacker)UnpackCurrentPlmn)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackServingSystem
//
/// Unpack the serving system tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASServingSystemInd::UnpackServingSystem(MsgBuf& msgBuf)
{
    m_servingSystemType = SERVING_SYSTEM_TYPE;
    m_servingSystemLen = msgBuf.GetWord();

    m_registrationState = msgBuf.GetByte();
    if (m_registrationState > 4)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Registration State values are 0 - 4 , unpacked value is ")
               << (uint32)m_registrationState << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_csAttachState = msgBuf.GetByte();
    if (m_csAttachState > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid CS Attach State values are 0 - 2 , unpacked value is ")
               << (uint32)m_csAttachState << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_psAttachState = msgBuf.GetByte();
    if (m_psAttachState > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid PS Attach State values are 0 - 2 , unpacked value is ")
               << (uint32)m_psAttachState << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_registeredNetwork = msgBuf.GetByte();
    if (m_registeredNetwork > 2)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Registered Network values are 0 - 2 , unpacked value is ")
               << (uint32)m_registeredNetwork << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_numRadioIfs = msgBuf.GetByte();
    for (int i = 0; i < m_numRadioIfs; i++)
    {
        m_radioIfs.push_back(msgBuf.GetByte());
        if (m_radioIfs[i] > 5)
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("Valid Radio IF values are 0 - 5 , unpacked value is ")
                   << (uint32)m_radioIfs[i] << _T(".") << std::endl 
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    }

    return true;
}

// --------------------------------------------------------------------------
// UnpackRoamingIndicator
//
/// Unpack the roaming indicator tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASServingSystemInd::UnpackRoamingIndicator(MsgBuf& msgBuf)
{
    m_roamingIndicatorType = ROAMING_INDICATOR_TYPE;
    m_roamingIndicatorLen = msgBuf.GetWord();
    if (m_roamingIndicatorLen != 1) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Roaming Indicator length is 1 bytes, unpacked length is ")
               << (uint32)m_roamingIndicatorLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_roamingIndicator = msgBuf.GetByte();
    return true;
}

// --------------------------------------------------------------------------
// UnpackCurrentPlmn
//
/// Unpack the current plmn tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASServingSystemInd::UnpackCurrentPlmn(MsgBuf& msgBuf)
{
    m_currentPlmnType = CURRENT_PLMN_TYPE;
    m_currentPlmnLen = msgBuf.GetWord();

    m_mobileCountryCode = msgBuf.GetWord();
    if (m_mobileCountryCode > 999)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Mobile Country Code values are 0 - 999 , unpacked value is ")
               << (uint32)m_mobileCountryCode << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_mobileNetworkCode = msgBuf.GetWord();
    if (m_mobileNetworkCode > 999)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Mobile Network Code values are 0 - 999 , unpacked value is ")
               << (uint32)m_mobileNetworkCode << _T(".") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_networkDescLen = msgBuf.GetByte();

    m_networkDesc.resize(m_networkDescLen);
    msgBuf.GetCopy(&m_networkDesc[0],m_networkDescLen);
    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void NASServingSystemInd::Print(std::ostream& stream)
{
    stream << "QMI_NAS_SERVING_SYSTEM_IND" << std::endl
           << _T("{") << std::endl;

    if (m_servingSystemType == SERVING_SYSTEM_TYPE)
    {
        stream << _T("  RegistrationState ") << (uint32)m_registrationState << std::endl
               << _T("  CsAttachState ") << (uint32)m_csAttachState << std::endl
               << _T("  PsAttachState ") << (uint32)m_psAttachState << std::endl
               << _T("  RegisteredNetwork ") << (uint32)m_registeredNetwork << std::endl;

        for (int i = 0; i < m_numRadioIfs; i++)
        {
            stream  << _T("  RadioIf ") << (uint32)m_radioIfs[i] << std::endl;
        }
    }

    if (m_roamingIndicatorType == ROAMING_INDICATOR_TYPE)
    {
        stream << _T("  RoamingIndicator ") << (uint32)m_roamingIndicator << std::endl;
    }

    if (m_currentPlmnType == CURRENT_PLMN_TYPE)
    {
        stream << _T("  MobileCountryCode ") << (uint32)m_mobileCountryCode << std::endl
               << _T("  MobileNetworkCode ") << (uint32)m_mobileNetworkCode << std::endl
               << _T("  NetworkDesc ") << m_networkDesc << std::endl;
    }

    stream << std::endl;
}
