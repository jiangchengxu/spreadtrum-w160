// --------------------------------------------------------------------------
//
// DMSGetDeviceSerialNumbersMsg.cpp
//
/// DMSGetDeviceSerialNumbersReq/DMSGetDeviceSerialNumbersRsp implementation.
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
#include "DMSGetDeviceSerialNumbersMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<DMSGetDeviceSerialNumbersReq> ReqStringCreator("QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ");
static MessageCreator<DMSGetDeviceSerialNumbersReq> ReqUint32Creator(DMSGetDeviceSerialNumbersReqUID);

static MessageCreator<DMSGetDeviceSerialNumbersRsp> RspStringCreator("QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_RSP");
static MessageCreator<DMSGetDeviceSerialNumbersRsp> RspUint32Creator(DMSGetDeviceSerialNumbersRspUID);


// --------------------------------------------------------------------------
// DMSGetDeviceSerialNumbersReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceSerialNumbersReq.
// --------------------------------------------------------------------------
DMSGetDeviceSerialNumbersReq::DMSGetDeviceSerialNumbersReq() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceSerialNumbersReq.
// --------------------------------------------------------------------------
DMSGetDeviceSerialNumbersReq::~DMSGetDeviceSerialNumbersReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool DMSGetDeviceSerialNumbersReq::BuildMsgBuf()
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
void DMSGetDeviceSerialNumbersReq::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// DMSGetDeviceSerialNumbersRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceSerialNumbersRsp.
// --------------------------------------------------------------------------
const uint8 DMSGetDeviceSerialNumbersRsp::RESULT_CODE_TYPE = 0x02;
const uint8 DMSGetDeviceSerialNumbersRsp::ESN_TYPE = 0x10;
const uint8 DMSGetDeviceSerialNumbersRsp::IMEI_TYPE = 0x11;
const uint8 DMSGetDeviceSerialNumbersRsp::MEID_TYPE = 0x12;
DMSGetDeviceSerialNumbersRsp::DMSGetDeviceSerialNumbersRsp() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_esnType(TLV_TYPE_INVALID),
    m_esnLen(0),
    m_imeiType(TLV_TYPE_INVALID),
    m_imeiLen(0),
    m_meidType(TLV_TYPE_INVALID),
    m_meidLen(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceSerialNumbersRsp.
// --------------------------------------------------------------------------
DMSGetDeviceSerialNumbersRsp::~DMSGetDeviceSerialNumbersRsp()
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
bool DMSGetDeviceSerialNumbersRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // if successful at least one optional tlv must be present
    if (m_result == QMI_RESULT_SUCCESS &&
            m_esnType != ESN_TYPE &&
            m_imeiType != IMEI_TYPE &&
            m_meidType != MEID_TYPE) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("At least one of the following optional tlv's must be present:") << std::endl
               << _T("  ESN") << std::endl
               << _T("  IMEI") << std::endl
               << _T("  MEID") << std::endl
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
Message::Uint8UnpackerMap& DMSGetDeviceSerialNumbersRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(ESN_TYPE,(Unpacker)UnpackEsn)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(IMEI_TYPE,(Unpacker)UnpackImei)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(MEID_TYPE,(Unpacker)UnpackMeid)).second;
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
bool DMSGetDeviceSerialNumbersRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackEsn
//
/// Unpack the esn tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSGetDeviceSerialNumbersRsp::UnpackEsn(MsgBuf& msgBuf)
{
    m_esnType = ESN_TYPE;

    m_esnLen = msgBuf.GetWord();

    m_esn.resize(m_esnLen);
    msgBuf.GetCopy(&m_esn[0],m_esnLen);

    return true;
}

// --------------------------------------------------------------------------
// UnpackImei
//
/// Unpack the imei tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSGetDeviceSerialNumbersRsp::UnpackImei(MsgBuf& msgBuf)
{
    m_imeiType = IMEI_TYPE;

    m_imeiLen = msgBuf.GetWord();

    m_imei.resize(m_imeiLen);
    msgBuf.GetCopy(&m_imei[0],m_imeiLen);

    return true;
}

// --------------------------------------------------------------------------
// UnpackMeid
//
/// Unpack the meid tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSGetDeviceSerialNumbersRsp::UnpackMeid(MsgBuf& msgBuf)
{
    m_meidType = MEID_TYPE;

    m_meidLen = msgBuf.GetWord();

    m_meid.resize(m_meidLen);
    msgBuf.GetCopy(&m_meid[0],m_meidLen);

    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this DMSMessage to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void DMSGetDeviceSerialNumbersRsp::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    if (m_esnType == ESN_TYPE) {
        stream << _T("  Esn ") << m_esn << std::endl;
    }

    if (m_imeiType == IMEI_TYPE) {
        stream << _T("  Imei ") << m_imei << std::endl;
    }

    if (m_meidType == MEID_TYPE) {
        stream << _T("  Meid ") << m_meid << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
