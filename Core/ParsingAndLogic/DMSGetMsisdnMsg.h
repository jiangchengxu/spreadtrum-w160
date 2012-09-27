// --------------------------------------------------------------------------
//
// DMSGetMsisdnMsg.h
//
/// DMSGetMsisdnReq/DMSGetMsisdnRsp interface.
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
// Unique identifiers for DMSGetMsisdnMsg types
// --------------------------------------------------------------------------
static const uint32 DMSGetMsisdnReqUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_CMD << 16 | QMI_DMS_GET_MSISDN_MSG);
static const uint32 DMSGetMsisdnRspUID =
    (QMUX_TYPE_DMS << 24 | QMI_CTL_FLAG_TYPE_RSP << 16 | QMI_DMS_GET_MSISDN_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class DMSGetMsisdnReq;
class DMSGetMsisdnRsp;
typedef TRCPointer<DMSGetMsisdnReq> DMSGetMsisdnReqRCP;
typedef TRCPointer<DMSGetMsisdnRsp> DMSGetMsisdnRspRCP;


// --------------------------------------------------------------------------
// DMSGetMsisdnReq
//
/// This class represents a QMI_DMS_GET_MSISDN_REQ message which requests
/// the voice number of the service.
// --------------------------------------------------------------------------
class DMSGetMsisdnReq : public Message
{
public:
    ~DMSGetMsisdnReq();

    virtual void Print(std::ostream& stream);

protected:
    DMSGetMsisdnReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// DMSGetMsisdnRsp
//
/// This class represents a QMI_DMS_GET_MSISDN_RESP message which is the
/// response to a request for the voice number of the service.
// --------------------------------------------------------------------------

class DMSGetMsisdnRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 DEVICE_VOICE_NUMBER_TYPE;

public:
    ~DMSGetMsisdnRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

    std::string GetVoiceNumber() {
        return m_voiceNumber;
    }

protected:
    DMSGetMsisdnRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackVoiceNumber(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Voice Number, mandatory tlv 0x01
    uint8 m_voiceNumberType;
    uint16 m_voiceNumberLen;
    std::string m_voiceNumber;
};
