// --------------------------------------------------------------------------
//
// WDSGetPktSrvcStatusMsg.cpp
//
/// WDSGetPktSrvcStatusReq/WDSGetPktSrvcStatusRsp/WDSPktSrvcStatusInd
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
#include "WDSGetPktSrvcStatusMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSGetPktSrvcStatusReq> ReqStringCreator("QMI_WDS_GET_PKT_SRVC_STATUS_REQ");
static MessageCreator<WDSGetPktSrvcStatusReq> ReqUint32Creator(WDSGetPktSrvcStatusReqUID);

static MessageCreator<WDSGetPktSrvcStatusRsp> RspStringCreator("QMI_WDS_GET_PKT_SRVC_STATUS_RSP");
static MessageCreator<WDSGetPktSrvcStatusRsp> RspUint32Creator(WDSGetPktSrvcStatusRspUID);

static MessageCreator<WDSPktSrvcStatusInd> IndStringCreator("QMI_WDS_PKT_SRVC_STATUS_IND");
static MessageCreator<WDSPktSrvcStatusInd> IndUint32Creator(WDSPktSrvcStatusIndUID);

// --------------------------------------------------------------------------
// WDSGetPktSrvcStatusReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetPktSrvcStatusReq.
// --------------------------------------------------------------------------
WDSGetPktSrvcStatusReq::WDSGetPktSrvcStatusReq() : 
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PKT_SRVC_STATUS_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetPktSrvcStatusReq.
// --------------------------------------------------------------------------
WDSGetPktSrvcStatusReq::~WDSGetPktSrvcStatusReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool WDSGetPktSrvcStatusReq::BuildMsgBuf()
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
void WDSGetPktSrvcStatusReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_PKT_SRVC_STATUS_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSGetPktSrvcStatusRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSGetPktSrvcStatusRsp.
// --------------------------------------------------------------------------
const uint8 WDSGetPktSrvcStatusRsp::RESULT_CODE_TYPE = 0x02;
const uint8 WDSGetPktSrvcStatusRsp::REQUIRED_PARAMETERS_TYPE = 0x01;
WDSGetPktSrvcStatusRsp::WDSGetPktSrvcStatusRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PKT_SRVC_STATUS_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_requiredParametersType(TLV_TYPE_INVALID),
    m_requiredParametersLen(0),
    m_connectionStatus(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSGetPktSrvcStatusRsp.
// --------------------------------------------------------------------------
WDSGetPktSrvcStatusRsp::~WDSGetPktSrvcStatusRsp()
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
bool WDSGetPktSrvcStatusRsp::Unpack(MsgBuf& msgBuf)
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
        if (m_length != 11)
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("Expected message length is 11 bytes, unpacked length is ")
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
Message::Uint8UnpackerMap& WDSGetPktSrvcStatusRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(REQUIRED_PARAMETERS_TYPE,(Unpacker)UnpackRequiredParameters)).second;
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
bool WDSGetPktSrvcStatusRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackRequiredParameters
//
/// Unpack the required parameters tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSGetPktSrvcStatusRsp::UnpackRequiredParameters(MsgBuf& msgBuf)
{
    m_requiredParametersType = REQUIRED_PARAMETERS_TYPE;

    m_requiredParametersLen = msgBuf.GetWord();
    if (m_requiredParametersLen != 1)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Required Parameter length is 1 bytes, unpacked length is ")
               << m_requiredParametersLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_connectionStatus = msgBuf.GetByte();
    if (m_connectionStatus < 1 || m_connectionStatus > 3) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Connection Status values are 1 - 3 , unpacked value is ")
               << m_connectionStatus << _T(" .") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

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
void WDSGetPktSrvcStatusRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_GET_PKT_SRVC_STATUS_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    // only print other mandatory tlvs if result code success
    if (m_result == QMI_RESULT_SUCCESS)
    {
        stream << _T("  ConnectionStatus ") << (int)m_connectionStatus << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSPktSrvcStatusInd
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSPktSrvcStatusInd.
// --------------------------------------------------------------------------
const uint8 WDSPktSrvcStatusInd ::PACKET_SERVICE_STATUS_TYPE = 0x01;
WDSPktSrvcStatusInd::WDSPktSrvcStatusInd() :
    Message(QMUX_TYPE_WDS,QMI_WDS_GET_PKT_SRVC_STATUS_MSG,QMI_CTL_FLAG_TYPE_IND),
    m_packetServiceStatusType(TLV_TYPE_INVALID),
    m_packetServiceStatusLen(0),
    m_connectionStatus(-1),
    m_reconfigurationRequired(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSPktSrvcStatusInd.
// --------------------------------------------------------------------------
WDSPktSrvcStatusInd::~WDSPktSrvcStatusInd()
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
bool WDSPktSrvcStatusInd::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }

    // validate message length
    if (m_length != 5) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected message length is 5 bytes, unpacked length is ")
               << m_length << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false; 
    }

    // verify mandatory tlvs are present
    if (m_packetServiceStatusType != PACKET_SERVICE_STATUS_TYPE) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Mandatory TLV Packet Service Status is not present.") << std::endl 
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
Message::Uint8UnpackerMap& WDSPktSrvcStatusInd::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(PACKET_SERVICE_STATUS_TYPE,(Unpacker)UnpackPacketServiceStatus)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackPacketServiceStatus
//
/// Unpack the packet service status tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSPktSrvcStatusInd::UnpackPacketServiceStatus(MsgBuf& msgBuf)
{
    m_packetServiceStatusType = PACKET_SERVICE_STATUS_TYPE;

    m_packetServiceStatusLen = msgBuf.GetWord();
    if (m_packetServiceStatusLen != 2) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Poacket Service Status length is 2 bytes, unpacked length is ")
               << m_packetServiceStatusLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_connectionStatus = msgBuf.GetByte();
    if (m_connectionStatus < 1 || m_connectionStatus > 3) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Connection Status values are 1 - 3 , unpacked value is ")
               << m_connectionStatus << _T(" .") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_reconfigurationRequired = msgBuf.GetByte();
    if (m_reconfigurationRequired > 1)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Reconfiguration Required values are 0 and 1, unpacked value is ")
               << m_reconfigurationRequired << _T(" .") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

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
void WDSPktSrvcStatusInd::Print(std::ostream& stream)
{
    stream << "QMI_WDS_PKT_SRVC_STATUS_IND" << std::endl
           << _T("{") << std::endl
           << _T("  ConnectionStatus ") << (int)m_connectionStatus << std::endl
           << _T("  ReconfigurationRequired ") << (int)m_reconfigurationRequired << std::endl
           << _T("}") << std::endl << std::endl;
}
