// --------------------------------------------------------------------------
//
// WDSGetPktSrvcStatusMsg.h
//
/// WDSGetPktSrvcStatusReq/WDSGetPktSrvcStatusRsp/WDSPktSrvcStatusInd interface.
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
// Unique identifiers for WDSGetPktSrvcStatusMsg types
// --------------------------------------------------------------------------
static const uint32 WDSGetPktSrvcStatusReqUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_WDS_GET_PKT_SRVC_STATUS_MSG);
static const uint32 WDSGetPktSrvcStatusRspUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_WDS_GET_PKT_SRVC_STATUS_MSG);
static const uint32 WDSPktSrvcStatusIndUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_IND << 16 |
     QMI_WDS_GET_PKT_SRVC_STATUS_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSGetPktSrvcStatusReq;
class WDSGetPktSrvcStatusRsp;
class WDSPktSrvcStatusInd;
typedef TRCPointer<WDSGetPktSrvcStatusReq> WDSGetPktSrvcStatusReqRCP;
typedef TRCPointer<WDSGetPktSrvcStatusRsp> WDSGetPktSrvcStatusRspRCP;
typedef TRCPointer<WDSPktSrvcStatusInd> WDSPktSrvcStatusIndRCP;


// --------------------------------------------------------------------------
// WDSGetPktSrvcStatusReq
//
/// This class represents a QMI_WDS_GET_PKT_SRVC_STATUS_REQ message which
/// queries the current packet data connection status.
// --------------------------------------------------------------------------
class WDSGetPktSrvcStatusReq : public Message
{
public:
    ~WDSGetPktSrvcStatusReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSGetPktSrvcStatusReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// WDSGetPktSrvcStatusRsp
//
/// This class represents a QMI_WDS_GET_PKT_SRVC_STATUS_RESP message
/// which is the response to the get packet service status request.
// --------------------------------------------------------------------------
// define tlv types

class WDSGetPktSrvcStatusRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 REQUIRED_PARAMETERS_TYPE;

public:
    ~WDSGetPktSrvcStatusRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }

    uint32 GetConnectionStatus() {
        return m_connectionStatus;
    }

protected:
    WDSGetPktSrvcStatusRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackRequiredParameters(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Required Parameters, mandatory tlv 0x01
    uint8 m_requiredParametersType;
    uint16 m_requiredParametersLen;
    uint8 m_connectionStatus;
};


// --------------------------------------------------------------------------
// WDSPktSrvcStatusInd
//
/// This class represents a QMI_WDS_GET_PKT_SRVC_STATUS_IND message which
/// indicates the state of the network connection.
// --------------------------------------------------------------------------

class WDSPktSrvcStatusInd : public Message
{
    // define tlv types
    static const uint8 PACKET_SERVICE_STATUS_TYPE ;

public:
    ~WDSPktSrvcStatusInd();

    virtual void Print(std::ostream& stream);

    virtual uint8 GetConnectionStatus() {
        return m_connectionStatus;
    }
    virtual uint8 GetReconfigurationRequired() {
        return m_reconfigurationRequired;
    }

protected:
    WDSPktSrvcStatusInd();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackPacketServiceStatus(MsgBuf& msgBuf);

private:
    // Packet Service Status, mandatory tlv 0x10
    uint8 m_packetServiceStatusType;
    uint16 m_packetServiceStatusLen;
    uint8 m_connectionStatus;
    uint8 m_reconfigurationRequired;
};
