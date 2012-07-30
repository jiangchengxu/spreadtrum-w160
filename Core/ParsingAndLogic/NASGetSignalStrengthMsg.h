// --------------------------------------------------------------------------
//
// NASGetSignalStrengthMsg.h
//
/// NASGetSignalStrengthReq/NASGetSignalStrengthRsp interface.
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
#include <vector>


// --------------------------------------------------------------------------
// Unique identifiers for NASSetEventReportMsg types
// --------------------------------------------------------------------------
static const uint32 NASGetSignalStrengthReqUID = 
    (QMUX_TYPE_NAS << 24 | 
     QMI_CTL_FLAG_TYPE_CMD << 16 | 
     QMI_NAS_GET_SIGNAL_STRENGTH_MSG);
static const uint32 NASGetSignalStrengthRspUID = 
    (QMUX_TYPE_NAS << 24 | 
     QMI_CTL_FLAG_TYPE_RSP << 16 | 
     QMI_NAS_GET_SIGNAL_STRENGTH_MSG);


// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class NASGetSignalStrengthReq;
class NASGetSignalStrengthRsp;
typedef TRCPointer<NASGetSignalStrengthReq> NASGetSignalStrengthReqRCP;
typedef TRCPointer<NASGetSignalStrengthRsp> NASGetSignalStrengthRspRCP;


// --------------------------------------------------------------------------
// NASGetSignalStrengthReq
//
/// This class represents a QMI_NAS_GET_SIGNAL_STRENGTH_REQ message which
/// queries the current signal strength as measured by the device.
// --------------------------------------------------------------------------
class NASGetSignalStrengthReq : public Message
{

public:
    ~NASGetSignalStrengthReq();

    virtual void Print(std::ostream& stream);

protected:
    NASGetSignalStrengthReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) { return false; }
};


// --------------------------------------------------------------------------
// NASGetSignalStrengthRsp
//
/// This class represents a QMI_NAS_GET_SIGNAL_STRENGTH_RESP message 
/// which is the response to the get signal strength request.
// --------------------------------------------------------------------------

class NASGetSignalStrengthRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE ;
    static const uint8 SIGNAL_STRENGTH_TYPE;

public:
    ~NASGetSignalStrengthRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() { return m_result; }
    uint16 GetError() { return m_error; }
    int8 GetSignalStrength() { return m_signalStrength; }
    uint8 GetRadioIf() { return m_radioIf; }

protected:
    NASGetSignalStrengthRsp();
    virtual bool Build(std::string& nameValue) { return false; }
    virtual bool BuildMsgBuf() { return false; }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackSignalStrength(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Signal Strength, mandatory tlv 0x01
    uint8 m_signalStrengthType;
    uint16 m_signalStrengthLen;
    int8 m_signalStrength;
    uint8 m_radioIf;
};
