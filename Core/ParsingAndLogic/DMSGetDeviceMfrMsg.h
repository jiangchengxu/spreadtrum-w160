// --------------------------------------------------------------------------
//
// DMSGetDeviceMfrMsg.h
//
/// DMSGetDeviceMfrReq/DMSGetDeviceMfrRsp interface.
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
// Unique identifiers for DMSGetDeviceMfrMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetDeviceMfrReqUID = 
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_DMS_GET_DEVICE_MFR_MSG);
static const uint32 DMSGetDeviceMfrRspUID = 
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_DMS_GET_DEVICE_MFR_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetDeviceMfrReq;
class DMSGetDeviceMfrRsp;
typedef TRCPointer<DMSGetDeviceMfrReq> DMSGetDeviceMfrReqRCP;
typedef TRCPointer<DMSGetDeviceMfrRsp> DMSGetDeviceMfrRspRCP;


// --------------------------------------------------------------------------
// DMSGetDeviceMfrReq
//
/// This class represents a QMI_DMS_GET_DEVICE_MFR_REQ message which requests
/// the device manufacturer of the service.
// --------------------------------------------------------------------------
class DMSGetDeviceMfrReq : public Message
{
public:
    ~DMSGetDeviceMfrReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetDeviceMfrReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }
};


// --------------------------------------------------------------------------
// DMSGetDeviceMfrRsp
//
/// This class represents a QMI_DMS_GET_DEVICE_MFR_RESP message which is the
/// response to a request for the device manufacturer of the service. 
// --------------------------------------------------------------------------

class DMSGetDeviceMfrRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 DEVICE_MANUFACTURER_TYPE;

public:
    ~DMSGetDeviceMfrRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() { return m_result; }
    uint16 GetError() { return m_error; }

    std::string GetDeviceManufacturer() { return m_deviceManufacturer; }

protected:
    DMSGetDeviceMfrRsp();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackDeviceManufacturer(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Device Manufacturer, mandatory tlv 0x01
    uint8 m_deviceManufacturerType;
    uint16 m_deviceManufacturerLen;
    std::string m_deviceManufacturer;
};
