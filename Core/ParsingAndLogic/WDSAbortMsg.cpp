// --------------------------------------------------------------------------
//
// WDSAbortMsg.cpp
//
/// WDSAbortReq/WDSAbortRsp implementation.
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
#include "WDSAbortMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSAbortReq> ReqStringCreator("QMI_WDS_ABORT_REQ");
static MessageCreator<WDSAbortReq> ReqUint32Creator(WDSAbortReqUID);

static MessageCreator<WDSAbortRsp> RspStringCreator("QMI_WDS_ABORT_RSP");
static MessageCreator<WDSAbortRsp> RspUint32Creator(WDSAbortRspUID);

// --------------------------------------------------------------------------
// WDSAbortReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSAbortReq.
// --------------------------------------------------------------------------
const uint8 WDSAbortReq::TX_ID_TYPE = 0x01;
WDSAbortReq::WDSAbortReq() :
    Message(QMUX_TYPE_WDS,QMI_WDS_ABORT_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_txIdType(TLV_TYPE_INVALID),
    m_txIdLen(0),
    m_txIdValue(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSAbortReq.
// --------------------------------------------------------------------------
WDSAbortReq::~WDSAbortReq()
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
bool WDSAbortReq::Build(std::string& nameValue)
{
    // mandatory tlv PktDataHandle is expected
    m_nextName = "TxId";

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
Message::StringBuilderMap& WDSAbortReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty()) {
        bool bSuccess = SBMap.insert(SBPair("TxId",(Builder)BuildTxId)).second;
        assert(bSuccess);
    }
    return SBMap;
}

// --------------------------------------------------------------------------
// BuildTxId
//
/// Build the tx id attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSAbortReq::BuildTxId(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // set member data
    m_txIdType = TX_ID_TYPE;
    m_txIdLen = sizeof(m_txIdValue);
    m_txIdValue = (uint16)num;
    m_length += (m_txIdLen + QMI_TLV_HEADER_BYTES);

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
bool WDSAbortReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // Mandatory tlv, tx_id
    m_pMsgBuf->PutByte(m_txIdType);
    m_pMsgBuf->PutWord(m_txIdLen);
    m_pMsgBuf->PutWord(m_txIdValue);

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
void WDSAbortReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_ABORT_REQ" << std::endl
           << _T("{") << std::endl
           << _T("  TxId ") << (int)m_txIdValue << std::endl
           << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSAbortRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSAbortRsp.
// --------------------------------------------------------------------------
const uint8 WDSAbortRsp::RESULT_CODE_TYPE = 0x02;
WDSAbortRsp::WDSAbortRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_ABORT_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSAbortRsp.
// --------------------------------------------------------------------------
WDSAbortRsp::~WDSAbortRsp()
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
bool WDSAbortRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length
    if (m_length != 7) {
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
Message::Uint8UnpackerMap& WDSAbortRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
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
bool WDSAbortRsp::UnpackResultCode(MsgBuf& msgBuf)
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

    if (!msgBuf.EOB()) {
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
void WDSAbortRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_ABORT_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl
           << _T("}") << std::endl << std::endl;
}
