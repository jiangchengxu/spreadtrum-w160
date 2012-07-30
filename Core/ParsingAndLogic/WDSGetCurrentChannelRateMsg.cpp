// --------------------------------------------------------------------------
//
// WDSGetCurrentChannelRateMsg.cpp
//
/// WDSGetCurrentChannelRateReq/WDSGetCurrentChannelRateRsp
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
#include "WDSGetCurrentChannelRateMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSGetCurrentChannelRateReq> ReqStringCreator("QMI_WDS_GET_CURRENT_CHANNEL_RATE_REQ");
static MessageCreator<WDSGetCurrentChannelRateReq> ReqUint32Creator(WDSGetCurrentChannelRateReqUID);

static MessageCreator<WDSGetCurrentChannelRateRsp> RspStringCreator("QMI_WDS_GET_CURRENT_CHANNEL_RATE_RSP");
static MessageCreator<WDSGetCurrentChannelRateRsp> RspUint32Creator(WDSGetCurrentChannelRateRspUID);

// --------------------------------------------------------------------------
// WDSGetCurrentChannelRateReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetCurrentChannelRateReq.
// --------------------------------------------------------------------------
WDSGetCurrentChannelRateReq::WDSGetCurrentChannelRateReq() : 
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_CURRENT_CHANNEL_RATE_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetCurrentChannelRateReq.
// --------------------------------------------------------------------------
WDSGetCurrentChannelRateReq::~WDSGetCurrentChannelRateReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool WDSGetCurrentChannelRateReq::BuildMsgBuf()
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
void WDSGetCurrentChannelRateReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_CURRENT_CHANNEL_RATE_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSGetCurrentChannelRateRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetCurrentChannelRateRsp.
// --------------------------------------------------------------------------
const uint8 WDSGetCurrentChannelRateRsp::RESULT_CODE_TYPE = 0x02;
const uint8 WDSGetCurrentChannelRateRsp::CHANNEL_RATE_TYPE = 0x01;
WDSGetCurrentChannelRateRsp::WDSGetCurrentChannelRateRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_CURRENT_CHANNEL_RATE_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_channelRateType(TLV_TYPE_INVALID),
    m_channelRateLen(0),
    m_currentChannelTxRate(0),
    m_currentChannelRxRate(0),
    m_maxChannelTxRate(0),
    m_maxChannelRxRate(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetCurrentChannelRateRsp.
// --------------------------------------------------------------------------
WDSGetCurrentChannelRateRsp::~WDSGetCurrentChannelRateRsp()
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
bool WDSGetCurrentChannelRateRsp::Unpack(MsgBuf& msgBuf)
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
        if (m_length != 26)
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("Expected message length is 26 bytes, unpacked length is ")
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
Message::Uint8UnpackerMap& WDSGetCurrentChannelRateRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(CHANNEL_RATE_TYPE,(Unpacker)UnpackChannelRate)).second;
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
bool WDSGetCurrentChannelRateRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackChannelRate
//
/// Unpack the channel rate tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetCurrentChannelRateRsp::UnpackChannelRate(MsgBuf& msgBuf)
{
    m_channelRateType = CHANNEL_RATE_TYPE;

    m_channelRateLen = msgBuf.GetWord();
    if (m_channelRateLen != 16)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Channel Rate length is 16 bytes, unpacked length is ")
               << m_channelRateLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_currentChannelTxRate = msgBuf.GetDWord();
    m_currentChannelRxRate = msgBuf.GetDWord();
    m_maxChannelTxRate = msgBuf.GetDWord();
    m_maxChannelRxRate = msgBuf.GetDWord();

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
void WDSGetCurrentChannelRateRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_CURRENT_CHANNEL_RATE_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    // only print other mandatory tlvs if result code success
    if (m_result == QMI_RESULT_SUCCESS)
    {
        stream << _T("  CurrentChannelTXRate ") << (uint32)m_currentChannelTxRate << std::endl
               << _T("  CurrentChannelRXRate ") << (uint32)m_currentChannelRxRate << std::endl
               << _T("  MaxChannelTXRate ") << (uint32)m_maxChannelTxRate << std::endl
               << _T("  MaxChannelRXRate ") << (uint32)m_maxChannelRxRate << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
