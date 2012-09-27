// --------------------------------------------------------------------------
//
// DMSGetDeviceCapMsg.h
//
/// DMSGetDeviceCapReq/DMSGetDeviceCapRsp interface.
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

#include <vector>
#include "Message.h"


// --------------------------------------------------------------------------
// Unique identifiers for DMSGetDeviceCapMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetDeviceCapReqUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_DMS_GET_DEVICE_CAP_MSG);
static const uint32 DMSGetDeviceCapRspUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_DMS_GET_DEVICE_CAP_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetDeviceCapReq;
class DMSGetDeviceCapRsp;
typedef TRCPointer<DMSGetDeviceCapReq> DMSGetDeviceCapReqRCP;
typedef TRCPointer<DMSGetDeviceCapRsp> DMSGetDeviceCapRspRCP;


// --------------------------------------------------------------------------
// DMSGetDeviceCapReq
//
/// This class represents a QMI_DMS_GET_DEVICE_CAP_REQ message which requests that the
/// the static capabilities of the device across all services.
// --------------------------------------------------------------------------
class DMSGetDeviceCapReq : public Message
{
public:
    ~DMSGetDeviceCapReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetDeviceCapReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// DMSGetDeviceCapRsp
//
/// This class represents a QMI_DMS_GET_DEVICE_CAP_RESP message which is the
/// response to a request for the static device capabilities across all
/// services.
// --------------------------------------------------------------------------

class DMSGetDeviceCapRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 DEVICE_CAPABILITIES_TYPE;

public:
    ~DMSGetDeviceCapRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

    uint32 GetMaxTxChannelRate() {
        return m_maxTxChannelRate;
    }
    uint32 GetMaxRxChannelRate() {
        return m_maxRxChannelRate;
    }
    uint8 GetServiceCapability() {
        return m_serviceCapability;
    }
    uint8 GetSimCapability() {
        return m_simCapability;
    }

protected:
    DMSGetDeviceCapRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackDeviceCapabilities(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Device Capabilities, mandatory tlv 0x01
    uint8 m_deviceCapabilitiesType;
    uint16 m_deviceCapabilitiesLen;
    uint32 m_maxTxChannelRate;
    uint32 m_maxRxChannelRate;
    uint8 m_serviceCapability;
    uint8 m_simCapability;
    uint8 m_radioIfListCnt;
    std::vector<uint8> m_radioIfList;
};
