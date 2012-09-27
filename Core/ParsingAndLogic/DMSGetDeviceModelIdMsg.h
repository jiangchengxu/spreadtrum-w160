// --------------------------------------------------------------------------
//
// DMSGetDeviceModelIdMsg.h
//
/// DMSGetDeviceModelIdReq/DMSGetDeviceModelIdRsp interface.
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

#pragma once

#include <string>
#include "Message.h"


// --------------------------------------------------------------------------
// Unique identifiers for DMSGetDeviceModelIdMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetDeviceModelIdReqUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_DMS_GET_DEVICE_MODEL_ID_MSG);
static const uint32 DMSGetDeviceModelIdRspUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_DMS_GET_DEVICE_MODEL_ID_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetDeviceModelIdReq;
class DMSGetDeviceModelIdRsp;
typedef TRCPointer<DMSGetDeviceModelIdReq> DMSGetDeviceModelIdReqRCP;
typedef TRCPointer<DMSGetDeviceModelIdRsp> DMSGetDeviceModelIdRspRCP;

// --------------------------------------------------------------------------
// DMSGetDeviceModelIdReq
//
/// This class represents a QMI_DMS_GET_DEVICE_MODEL_ID_REQ message which
/// requests the device model id.
// --------------------------------------------------------------------------
class DMSGetDeviceModelIdReq : public Message
{
public:
    ~DMSGetDeviceModelIdReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetDeviceModelIdReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// DMSGetDeviceModelIdRsp
//
/// This class represents a QMI_DMS_GET_DEVICE_MODEL_ID_RESP message which is the
/// response to a request for the the device model id.
// --------------------------------------------------------------------------

class DMSGetDeviceModelIdRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 DEVICE_MODEL_TYPE;

public:
    ~DMSGetDeviceModelIdRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

    std::string GetDeviceModelId() {
        return m_deviceModelId;
    }

protected:
    DMSGetDeviceModelIdRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackDeviceModel(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Device Model, mandatory tlv 0x01
    uint8 m_deviceModelType;
    uint16 m_deviceModelLen;
    std::string m_deviceModelId;
};
