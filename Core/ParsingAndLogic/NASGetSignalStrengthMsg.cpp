// --------------------------------------------------------------------------
//
// NASGetSignalStrengthMsg.cpp
//
/// NASGetSignalStrengthReq/NASGetSignalStrengthRsp implementation.
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
#include "NASGetSignalStrengthMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<NASGetSignalStrengthReq> ReqStringCreator("QMI_NAS_GET_SIGNAL_STRENGTH_REQ");
static MessageCreator<NASGetSignalStrengthReq> ReqUint32Creator(NASGetSignalStrengthReqUID);

static MessageCreator<NASGetSignalStrengthRsp> RspStringCreator("QMI_NAS_GET_SIGNAL_STRENGTH_RSP");
static MessageCreator<NASGetSignalStrengthRsp> RspUint32Creator(NASGetSignalStrengthRspUID);


// --------------------------------------------------------------------------
// NASSetEventReportReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASGetSignalStrengthMsg.
// --------------------------------------------------------------------------
NASGetSignalStrengthReq::NASGetSignalStrengthReq() : 
    Message(QMUX_TYPE_NAS,QMI_NAS_GET_SIGNAL_STRENGTH_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportReq.
// --------------------------------------------------------------------------
NASGetSignalStrengthReq::~NASGetSignalStrengthReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool NASGetSignalStrengthReq::BuildMsgBuf()
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
void NASGetSignalStrengthReq::Print(std::ostream& stream)
{
    stream << "QMI_NAS_GET_SIGNAL_STRENGTH_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// NASGetSignalStrengthRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASGetSignalStrengthRsp.
// --------------------------------------------------------------------------
const uint8 NASGetSignalStrengthRsp::RESULT_CODE_TYPE = 0x02;
const uint8  NASGetSignalStrengthRsp::SIGNAL_STRENGTH_TYPE = 0x01;
NASGetSignalStrengthRsp::NASGetSignalStrengthRsp() :
    Message(QMUX_TYPE_NAS,QMI_NAS_GET_SIGNAL_STRENGTH_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_signalStrengthType(TLV_TYPE_INVALID),
    m_signalStrengthLen(0),
    m_signalStrength(-1),
    m_radioIf(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportRsp.
// --------------------------------------------------------------------------
NASGetSignalStrengthRsp::~NASGetSignalStrengthRsp()
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
bool NASGetSignalStrengthRsp::Unpack(MsgBuf& msgBuf)
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
Message::Uint8UnpackerMap& NASGetSignalStrengthRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(SIGNAL_STRENGTH_TYPE,(Unpacker)UnpackSignalStrength)).second;
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
bool NASGetSignalStrengthRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackSignalStrength
//
/// Unpack the signal strength tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASGetSignalStrengthRsp::UnpackSignalStrength(MsgBuf& msgBuf)
{
    m_signalStrengthType = SIGNAL_STRENGTH_TYPE;

    m_signalStrengthLen = msgBuf.GetWord();
    if (m_signalStrengthLen != 2) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Signal Strength length is 2 bytes, unpacked length is ")
               << m_signalStrengthLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_signalStrength = msgBuf.GetByte();

    m_radioIf = msgBuf.GetByte();
    if (m_radioIf > 5)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Radio IF values are 0 - 5 , unpacked value is ")
               << m_radioIf << _T(" .") << std::endl 
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
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void NASGetSignalStrengthRsp::Print(std::ostream& stream)
{
    stream << "QMI_NAS_GET_SIGNAL_STRENGTH_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    if (m_signalStrengthType == SIGNAL_STRENGTH_TYPE)
    {
        stream << _T("  SignalStrength ") << (int)m_signalStrength << std::endl
               << _T("  RadioIf ") << (int)m_radioIf << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
