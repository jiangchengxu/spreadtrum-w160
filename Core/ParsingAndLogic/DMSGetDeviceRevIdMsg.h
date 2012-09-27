// --------------------------------------------------------------------------
//
// DMSGetDeviceRevIdMsg.h
//
/// DMSGetDeviceRevIdReq/DMSGetDeviceRevIdRsp interface.
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
// Unique identifiers for DMSGetDeviceRevIdMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetDeviceRevIdReqUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_DMS_GET_DEVICE_REV_ID_MSG);
static const uint32 DMSGetDeviceRevIdRspUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_DMS_GET_DEVICE_REV_ID_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetDeviceRevIdReq;
class DMSGetDeviceRevIdRsp;
typedef TRCPointer<DMSGetDeviceRevIdReq> DMSGetDeviceRevIdReqRCP;
typedef TRCPointer<DMSGetDeviceRevIdRsp> DMSGetDeviceRevIdRspRCP;


// --------------------------------------------------------------------------
// DMSGetDeviceRevIdReq
//
/// This class represents a QMI_DMS_GET_DEVICE_REV_ID_REQ message which requests
/// the device firmware revision ID.
// --------------------------------------------------------------------------
class DMSGetDeviceRevIdReq : public Message
{
public:
    ~DMSGetDeviceRevIdReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetDeviceRevIdReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// DMSGetDeviceRevIdRsp
//
/// This class represents a QMI_DMS_GET_DEVICE_REV_ID_RESP message which is the
/// response to a request for the the device firmware revision ID.
// --------------------------------------------------------------------------

class DMSGetDeviceRevIdRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 DEVICE_REVISION_ID_TYPE;

public:
    ~DMSGetDeviceRevIdRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

    std::string GetDeviceRevId() {
        return m_deviceRevId;
    }

protected:
    DMSGetDeviceRevIdRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackDeviceRevisionId(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Device Manufacturer, mandatory tlv 0x01
    uint8 m_deviceRevisionIdType;
    uint16 m_deviceRevisionIdLen;
    std::string m_deviceRevId;
};
