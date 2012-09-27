// --------------------------------------------------------------------------
//
// WDSStopNetworkInterfaceMsg.h
//
/// WDSStopNetworkInterfaceReq/WDSStopNetworkInterfaceRsp interface.
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
// Unique identifiers for WDSStopNetworkInterfaceMsg types
// --------------------------------------------------------------------------
static const uint32 WDSStopNetworkInterfaceReqUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_WDS_STOP_NETWORK_INTERFACE_MSG);
static const uint32 WDSStopNetworkInterfaceRspUID =
    (QMUX_TYPE_WDS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_WDS_STOP_NETWORK_INTERFACE_MSG);

// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class WDSStopNetworkInterfaceReq;
class WDSStopNetworkInterfaceRsp;
typedef TRCPointer<WDSStopNetworkInterfaceReq> WDSStopNetworkInterfaceReqRCP;
typedef TRCPointer<WDSStopNetworkInterfaceRsp> WDSStopNetworkInterfaceRspRCP;


// --------------------------------------------------------------------------
// WDSStopNetworkInterfaceReq
//
/// This class represents a QMI_WDS_STOP_NETWORK_INTERFACE_REQ message which
/// releases the WWAN packet data connection unless other control points are
/// still using it (i.e. have previously called
/// QMI_WDS_START_NETWORK_INTERFACE).
///
/// Session will be closed (deactivating the packet data session on the air
/// interface) when all control points have called
/// QMI_WDS_STOP_NETWORK_INTERFACE on the interface.
// --------------------------------------------------------------------------

class WDSStopNetworkInterfaceReq : public Message
{
    // define tlv types
    static const uint8 PKT_DATA_HANDLE_TYPE;

public:
    ~WDSStopNetworkInterfaceReq();

    virtual void Print(std::ostream& stream);

protected:
    WDSStopNetworkInterfaceReq();
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildPktDataHandle(std::string& value);
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }

private:
    // Packet Data Handle, mandatory tlv 0x01
    uint8 m_pktDataHandleType;
    uint16 m_pktDataHandleLen;
    uint32 m_pktDataHandle;
};


// --------------------------------------------------------------------------
// WDSStopNetworkInterfaceRsp
//
/// This class represents a QMI_WDS_STOP_NETWORK_INTERFACE_RESP message
/// which is the response to the stop network interface request.
// --------------------------------------------------------------------------

class WDSStopNetworkInterfaceRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;

public:
    ~WDSStopNetworkInterfaceRsp();

    virtual void Print(std::ostream& stream);

    virtual uint16 GetResult() {
        return m_result;
    }
    virtual uint16 GetError() {
        return m_error;
    }

protected:
    WDSStopNetworkInterfaceRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;
};
