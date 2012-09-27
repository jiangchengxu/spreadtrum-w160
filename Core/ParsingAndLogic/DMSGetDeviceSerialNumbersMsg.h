// --------------------------------------------------------------------------
//
// DMSGetDeviceSerialNumbersMsg.h
//
/// DMSGetDeviceSerialNumbersReq/DMSGetDeviceSerialNumbersRsp interface.
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

#include "Message.h"


// --------------------------------------------------------------------------
// Unique identifiers for DMSGetDeviceSerialNumbersMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetDeviceSerialNumbersReqUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_MSG);
static const uint32 DMSGetDeviceSerialNumbersRspUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetDeviceSerialNumbersReq;
class DMSGetDeviceSerialNumbersRsp;
typedef TRCPointer<DMSGetDeviceSerialNumbersReq> DMSGetDeviceSerialNumbersReqRCP;
typedef TRCPointer<DMSGetDeviceSerialNumbersRsp> DMSGetDeviceSerialNumbersRspRCP;


// --------------------------------------------------------------------------
// DMSGetDeviceSerialNumbersReq
//
/// This class represents a QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ message which requests
/// the device serial number from the service.
// --------------------------------------------------------------------------
class DMSGetDeviceSerialNumbersReq : public Message
{
public:
    ~DMSGetDeviceSerialNumbersReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetDeviceSerialNumbersReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// DMSGetDeviceSerialNumbersRsp
//
/// This class represents a QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_RESP message which is the
/// response to a request for the device serial number from the service.
// --------------------------------------------------------------------------

class DMSGetDeviceSerialNumbersRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 ESN_TYPE;
    static const uint8 IMEI_TYPE;
    static const uint8 MEID_TYPE;

public:
    ~DMSGetDeviceSerialNumbersRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() {
        return m_result;
    }
    virtual uint16 GetError() {
        return m_error;
    }

    virtual bool IsEsn() {
        return m_esnType == ESN_TYPE;
    }
    virtual bool IsImei() {
        return m_imeiType == IMEI_TYPE;
    }
    virtual bool IsMeid() {
        return m_meidType == MEID_TYPE;
    }

    virtual const std::string& GetEsn() {
        return m_esn;
    }
    virtual const std::string& GetImei() {
        return m_imei;
    }
    virtual const std::string& GetMeid() {
        return m_meid;
    }

protected:
    DMSGetDeviceSerialNumbersRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackEsn(MsgBuf& msgBuf);
    virtual bool UnpackImei(MsgBuf& msgBuf);
    virtual bool UnpackMeid(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // ESN, optional tlv 0x10
    uint8 m_esnType;
    uint16 m_esnLen;
    std::string m_esn;

    // IMEI, optional tlv 0x11
    uint8 m_imeiType;
    uint16 m_imeiLen;
    std::string m_imei;

    // MEID, optional tlv 0x12
    uint8 m_meidType;
    uint16 m_meidLen;
    std::string m_meid;
};
