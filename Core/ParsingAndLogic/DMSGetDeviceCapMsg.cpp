// --------------------------------------------------------------------------
//
// DMSGetDeviceCapMsg.cpp
//
/// DMSGetDeviceCapReq/DMSGetDeviceCapRsp implementation.
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
#include "DMSGetDeviceCapMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<DMSGetDeviceCapReq> ReqStringCreator("QMI_DMS_GET_DEVICE_CAP_REQ");
static MessageCreator<DMSGetDeviceCapReq> ReqUint32Creator(DMSGetDeviceCapReqUID);

static MessageCreator<DMSGetDeviceCapRsp> RspStringCreator("QMI_DMS_GET_DEVICE_CAP_RSP");
static MessageCreator<DMSGetDeviceCapRsp> RspUint32Creator(DMSGetDeviceCapRspUID);


// --------------------------------------------------------------------------
// DMSGetDeviceCapReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceCapReq.
// --------------------------------------------------------------------------
DMSGetDeviceCapReq::DMSGetDeviceCapReq() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_CAP_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceCapReq.
// --------------------------------------------------------------------------
DMSGetDeviceCapReq::~DMSGetDeviceCapReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool DMSGetDeviceCapReq::BuildMsgBuf()
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
/// Print this DMSMessage to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void DMSGetDeviceCapReq::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_CAP_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// DMSGetDeviceCapRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceCapRsp.
// --------------------------------------------------------------------------
const uint8 DMSGetDeviceCapRsp::RESULT_CODE_TYPE = 0x02;
const uint8 DMSGetDeviceCapRsp::DEVICE_CAPABILITIES_TYPE = 0x01;
DMSGetDeviceCapRsp::DMSGetDeviceCapRsp() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_CAP_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_deviceCapabilitiesType(TLV_TYPE_INVALID),
    m_deviceCapabilitiesLen(0),
    m_maxTxChannelRate(0),
    m_maxRxChannelRate(0),
    m_serviceCapability(-1),
    m_simCapability(-1),
    m_radioIfListCnt(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceCapRsp.
// --------------------------------------------------------------------------
DMSGetDeviceCapRsp::~DMSGetDeviceCapRsp()
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
bool DMSGetDeviceCapRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length
    if (m_result == QMI_RESULT_SUCCESS) {
        // mandatory tlvs
        uint32 len = 21 + m_radioIfListCnt;
        if (m_length != len) {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("Expected message length is ") << (int)len
                   << _T(" bytes, unpacked length is ")
                   << m_length << _T(" bytes.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    } else {
        // only result code tlv on failure
        if (m_length != 7) {
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
Message::Uint8UnpackerMap& DMSGetDeviceCapRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(DEVICE_CAPABILITIES_TYPE,(Unpacker)UnpackDeviceCapabilities)).second;
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
bool DMSGetDeviceCapRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackDeviceCapabilities
//
/// Unpack the device capabilities tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSGetDeviceCapRsp::UnpackDeviceCapabilities(MsgBuf& msgBuf)
{
    m_deviceCapabilitiesType = DEVICE_CAPABILITIES_TYPE;

    m_deviceCapabilitiesLen = msgBuf.GetWord();

    m_maxTxChannelRate = msgBuf.GetDWord();
    m_maxRxChannelRate = msgBuf.GetDWord();

    m_serviceCapability = msgBuf.GetByte();
    if (m_serviceCapability < 1 || m_serviceCapability > 4) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Service Capability values are 1 - 4 , unpacked value is ")
               << m_serviceCapability << _T(" .") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_simCapability = msgBuf.GetByte();
    if (m_simCapability < 1 || m_simCapability > 2) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Sim Capability values are 1 and 2, unpacked value is ")
               << m_simCapability << _T(" .") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_radioIfListCnt = msgBuf.GetByte();
    for (int i = 0; i < m_radioIfListCnt; i++) {
        m_radioIfList.push_back(msgBuf.GetByte());
    }

    uint32 len = 11 + m_radioIfListCnt;
    if (m_deviceCapabilitiesLen != len) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Device Capabilities length is 4 ")
               << (int)len << _T("bytes, unpacked length is ")
               << m_deviceCapabilitiesLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // all tlvs are mandatory, so we should be at end of buffer
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
/// Print this DMSMessage to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void DMSGetDeviceCapRsp::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_CAP_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    // only print other mandatory tlvs if result code success
    if (m_result == QMI_RESULT_SUCCESS) {
        stream << _T("  MaxTxChannelRate ") << (int)m_maxTxChannelRate << std::endl
               << _T("  MaxRxChannelRate ") << (int)m_maxRxChannelRate << std::endl
               << _T("  ServiceCapability ") << (int)m_serviceCapability << std::endl
               << _T("  SimCapability ") << (int)m_simCapability << std::endl;

        for (int i = 0; i < m_radioIfListCnt; i++) {
            stream << _T("  RadioIf ") << (int)m_radioIfList[i] << std::endl;
        }
    }

    stream << _T("}") << std::endl << std::endl;
}
