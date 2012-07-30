// --------------------------------------------------------------------------
//
// DMSGetPowerStateMsg.h
//
/// DMSGetPowerStateReq/DMSGetPowerStateRsp interface.
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
// Unique identifiers for DMSGetPowerStateMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetPowerStateReqUID = 
    (QMUX_TYPE_DMS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_DMS_GET_POWER_STATE_MSG);
static const uint32 DMSGetPowerStateRspUID = 
    (QMUX_TYPE_DMS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_DMS_GET_POWER_STATE_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetPowerStateReq;
class DMSGetPowerStateRsp;
typedef TRCPointer<DMSGetPowerStateReq> DMSGetPowerStateReqRCP;
typedef TRCPointer<DMSGetPowerStateRsp> DMSGetPowerStateRspRCP;


// --------------------------------------------------------------------------
// DMSGetPowerStateReq
//
/// This class represents a QMI_DMS_GET_POWER_STATE_REQ message which
/// queries the power status of the device.
// --------------------------------------------------------------------------
class DMSGetPowerStateReq : public Message
{
public:
    ~DMSGetPowerStateReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetPowerStateReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }
};


// --------------------------------------------------------------------------
// DMSGetPowerStateRsp
//
/// This class represents a QMI_DMS_GET_POWER_STATE_RESP message 
/// which is the response to the get power state request.
// --------------------------------------------------------------------------
class DMSGetPowerStateRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 POWER_STATE_TYPE;

public:
    ~DMSGetPowerStateRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() { return m_result; }
    uint16 GetError() { return m_error; }
    virtual uint8 GetPowerStatus() { return m_powerStatus; }
    virtual uint8 GetBatteryLvl() { return m_batteryLvl; }

protected:
    DMSGetPowerStateRsp();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackPowerState(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Power State, mandatory tlv 0x01
    uint8 m_powerStateType;
    uint16 m_powerStateLen;
    uint8 m_powerStatus;
    uint8 m_batteryLvl;
};
