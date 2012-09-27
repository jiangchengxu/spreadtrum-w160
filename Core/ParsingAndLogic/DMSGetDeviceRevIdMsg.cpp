// --------------------------------------------------------------------------
//
// DMSGetDeviceRevIdMsg.cpp
//
/// DMSGetDeviceRevIdReq/DMSGetDeviceRevIdRsp implementation.
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
#include "DMSGetDeviceRevIdMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<DMSGetDeviceRevIdReq> ReqStringCreator("QMI_DMS_GET_DEVICE_REV_ID_REQ");
static MessageCreator<DMSGetDeviceRevIdReq> ReqUint32Creator(DMSGetDeviceRevIdReqUID);

static MessageCreator<DMSGetDeviceRevIdRsp> RspStringCreator("QMI_DMS_GET_DEVICE_REV_ID_RSP");
static MessageCreator<DMSGetDeviceRevIdRsp> RspUint32Creator(DMSGetDeviceRevIdRspUID);


// --------------------------------------------------------------------------
// DMSGetDeviceRevIdReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceRevIdReq.
// --------------------------------------------------------------------------
DMSGetDeviceRevIdReq::DMSGetDeviceRevIdReq() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_REV_ID_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceRevIdReq.
// --------------------------------------------------------------------------
DMSGetDeviceRevIdReq::~DMSGetDeviceRevIdReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool DMSGetDeviceRevIdReq::BuildMsgBuf()
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
void DMSGetDeviceRevIdReq::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_REV_ID_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// DMSGetDeviceRevIdRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceRevIdRsp.
// --------------------------------------------------------------------------
const uint8 DMSGetDeviceRevIdRsp::RESULT_CODE_TYPE = 0x02;
const uint8 DMSGetDeviceRevIdRsp::DEVICE_REVISION_ID_TYPE = 0x01;
DMSGetDeviceRevIdRsp::DMSGetDeviceRevIdRsp() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_REV_ID_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_deviceRevisionIdType(TLV_TYPE_INVALID),
    m_deviceRevisionIdLen(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceRevIdRsp.
// --------------------------------------------------------------------------
DMSGetDeviceRevIdRsp::~DMSGetDeviceRevIdRsp()
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
bool DMSGetDeviceRevIdRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length
    if (m_result == QMI_RESULT_SUCCESS) {
        // mandatory tlvs
        uint32 len = 10 + (uint32)m_deviceRevId.length();
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
Message::Uint8UnpackerMap& DMSGetDeviceRevIdRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(DEVICE_REVISION_ID_TYPE,(Unpacker)UnpackDeviceRevisionId)).second;
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
bool DMSGetDeviceRevIdRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackDeviceRevisionId
//
/// Unpack the device revision id tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSGetDeviceRevIdRsp::UnpackDeviceRevisionId(MsgBuf& msgBuf)
{
    m_deviceRevisionIdType = DEVICE_REVISION_ID_TYPE;

    m_deviceRevisionIdLen = msgBuf.GetWord();

    m_deviceRevId.resize(m_deviceRevisionIdLen);
    msgBuf.GetCopy(&m_deviceRevId[0],m_deviceRevisionIdLen);

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
void DMSGetDeviceRevIdRsp::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_REV_ID_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    // only print other mandatory tlvs if result code success
    if (m_result == QMI_RESULT_SUCCESS) {
        stream << "  DeviceRevId " << m_deviceRevId << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
