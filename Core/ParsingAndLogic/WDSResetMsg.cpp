// --------------------------------------------------------------------------
//
// WDSResetMsg.cpp
//
/// WDSResetReq/WDSResetRsp implementation.
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
#include "WDSResetMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSResetReq> ReqStringCreator("QMI_WDS_RESET_REQ");
static MessageCreator<WDSResetReq> ReqUint32Creator(WDSResetReqUID);

static MessageCreator<WDSResetRsp> RspStringCreator("QMI_WDS_RESET_RSP");
static MessageCreator<WDSResetRsp> RspUint32Creator(WDSResetRspUID);


// --------------------------------------------------------------------------
// WDSResetReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSResetReq.
// --------------------------------------------------------------------------
WDSResetReq::WDSResetReq() :
    Message(QMUX_TYPE_WDS,QMI_WDS_RESET_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSResetReq.
// --------------------------------------------------------------------------
WDSResetReq::~WDSResetReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool WDSResetReq::BuildMsgBuf()
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
/// Print this WDSMessage to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSResetReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_RESET_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSResetRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSResetRsp.
// --------------------------------------------------------------------------
WDSResetRsp::WDSResetRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_RESET_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    RESULT_CODE_TYPE(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSResetRsp.
// --------------------------------------------------------------------------
WDSResetRsp::~WDSResetRsp()
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
bool WDSResetRsp::Unpack(MsgBuf& msgBuf)
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
Message::Uint8UnpackerMap& WDSResetRsp::GetUnpackerMap()
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
bool WDSResetRsp::UnpackResultCode(MsgBuf& msgBuf)
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
/// Print this WDSMessage to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void WDSResetRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_RESET_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl
           << _T("}") << std::endl << std::endl;
}
