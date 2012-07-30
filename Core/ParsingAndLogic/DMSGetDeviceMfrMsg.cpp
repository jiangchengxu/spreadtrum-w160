// --------------------------------------------------------------------------
//
// DMSGetDeviceMfrMsg.cpp
//
/// DMSGetDeviceMfrReq/DMSGetDeviceMfrRsp implementation.
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
#include "DMSGetDeviceMfrMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<DMSGetDeviceMfrReq> ReqStringCreator("QMI_DMS_GET_DEVICE_MFR_REQ");
static MessageCreator<DMSGetDeviceMfrReq> ReqUint32Creator(DMSGetDeviceMfrReqUID);

static MessageCreator<DMSGetDeviceMfrRsp> RspStringCreator("QMI_DMS_GET_DEVICE_MFR_RSP");
static MessageCreator<DMSGetDeviceMfrRsp> RspUint32Creator(DMSGetDeviceMfrRspUID);


// --------------------------------------------------------------------------
// DMSGetDeviceMfrReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceMfrReq.
// --------------------------------------------------------------------------
DMSGetDeviceMfrReq::DMSGetDeviceMfrReq() : 
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_MFR_MSG,QMI_CTL_FLAG_TYPE_CMD)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceMfrReq.
// --------------------------------------------------------------------------
DMSGetDeviceMfrReq::~DMSGetDeviceMfrReq()
{}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool DMSGetDeviceMfrReq::BuildMsgBuf()
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
void DMSGetDeviceMfrReq::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_MFR_REQ" << std::endl
           << _T("{}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// DMSGetDeviceMfrRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for DMSGetDeviceMfrRsp.
// --------------------------------------------------------------------------
const uint8 DMSGetDeviceMfrRsp::RESULT_CODE_TYPE = 0x02;
const uint8 DMSGetDeviceMfrRsp::DEVICE_MANUFACTURER_TYPE = 0x01;
DMSGetDeviceMfrRsp::DMSGetDeviceMfrRsp() :
    Message(QMUX_TYPE_DMS,QMI_DMS_GET_DEVICE_MFR_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1),
    m_deviceManufacturerType(TLV_TYPE_INVALID),
    m_deviceManufacturerLen(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DMSGetDeviceMfrRsp.
// --------------------------------------------------------------------------
DMSGetDeviceMfrRsp::~DMSGetDeviceMfrRsp()
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
bool DMSGetDeviceMfrRsp::Unpack(MsgBuf& msgBuf)
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
        uint32 len = 10 + (uint32)m_deviceManufacturer.length();
        if (m_length != len) 
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("Expected message length is ") << (int)len
                << _T(" bytes, unpacked length is ")
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
Message::Uint8UnpackerMap& DMSGetDeviceMfrRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(DEVICE_MANUFACTURER_TYPE,(Unpacker)UnpackDeviceManufacturer)).second;
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
bool DMSGetDeviceMfrRsp::UnpackResultCode(MsgBuf& msgBuf)
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
// UnpackDeviceManufacturer
//
/// Unpack the device manufacturer tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool DMSGetDeviceMfrRsp::UnpackDeviceManufacturer(MsgBuf& msgBuf)
{
    m_deviceManufacturerType = DEVICE_MANUFACTURER_TYPE;

    m_deviceManufacturerLen = msgBuf.GetWord();

    m_deviceManufacturer.resize(m_deviceManufacturerLen);
    msgBuf.GetCopy(&m_deviceManufacturer[0],m_deviceManufacturerLen);

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
/// Print this DMSMessage to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void DMSGetDeviceMfrRsp::Print(std::ostream& stream)
{
    stream << "QMI_DMS_GET_DEVICE_MFR_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl;

    // only print other mandatory tlvs if result code success
    if (m_result == QMI_RESULT_SUCCESS)
    {
        stream << _T("  DeviceManufacturer ") << m_deviceManufacturer << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
