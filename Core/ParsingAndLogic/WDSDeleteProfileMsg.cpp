// --------------------------------------------------------------------------
//
// WDSDeleteProfileMsg.cpp
//
/// WDSDeleteProfileReq/WDSDeleteProfileRsp implementation.
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
#include "WDSDeleteProfileMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSDeleteProfileReq> ReqStringCreator("QMI_WDS_DELETE_PROFILE_REQ");
static MessageCreator<WDSDeleteProfileReq> ReqUint32Creator(WDSDeleteProfileReqUID);

static MessageCreator<WDSDeleteProfileRsp> RspStringCreator("QMI_WDS_DELETE_PROFILE_RSP");
static MessageCreator<WDSDeleteProfileRsp> RspUint32Creator(WDSDeleteProfileRspUID);

// --------------------------------------------------------------------------
// WDSDeleteProfileReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSDeleteProfileReq.
// --------------------------------------------------------------------------
const uint8 WDSDeleteProfileReq::PROFILE_IDENTIFIER_TYPE = 0x01;
WDSDeleteProfileReq::WDSDeleteProfileReq() : 
    Message(QMUX_TYPE_WDS,QMI_WDS_DELETE_PROFILE_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_profileIdentifierType(TLV_TYPE_INVALID),
    m_profileIdentifierLen(0),
    m_profileType(-1),
    m_profileIndex(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSDeleteProfileReq.
// --------------------------------------------------------------------------
WDSDeleteProfileReq::~WDSDeleteProfileReq()
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
bool WDSDeleteProfileReq::Build(std::string& nameValue)
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
Message::StringBuilderMap& WDSDeleteProfileReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty())
    {
        bool bSuccess = SBMap.insert(SBPair("ProfileType",(Builder)BuildProfileType)).second;
        assert(bSuccess);
        SBMap.insert(SBPair("ProfileIndex",(Builder)BuildProfileIndex)).second;
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
bool WDSDeleteProfileReq::BuildProfileType(std::string& value)
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
bool WDSDeleteProfileReq::BuildProfileIndex(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("ProfileType") != 0)
    {
        ReportInvalidSequence("ProfileType");
        return false;
    }

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
bool WDSDeleteProfileReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // Mandatory tlv, profile identifier
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
void WDSDeleteProfileReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_DELETE_PROFILE_REQ" << std::endl
           << _T("{") << std::endl
           << _T("  ProfileType ") << (uint32)m_profileType << std::endl
           << _T("  ProfileIndex ") << (uint32)m_profileIndex << std::endl
           << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSDeleteProfileRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSDeleteProfileRsp.
// --------------------------------------------------------------------------
const uint8 WDSDeleteProfileRsp::RESULT_CODE_TYPE = 0x02;
WDSDeleteProfileRsp::WDSDeleteProfileRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_DELETE_PROFILE_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSDeleteProfileRsp.
// --------------------------------------------------------------------------
WDSDeleteProfileRsp::~WDSDeleteProfileRsp()
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
bool WDSDeleteProfileRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }
    
    // validate message length
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
Message::Uint8UnpackerMap& WDSDeleteProfileRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
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
bool WDSDeleteProfileRsp::UnpackResultCode(MsgBuf& msgBuf)
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
void WDSDeleteProfileRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_DELETE_PROFILE_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl
           << _T("}") << std::endl << std::endl;
}
