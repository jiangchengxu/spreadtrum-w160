// --------------------------------------------------------------------------
//
// NASGetServingSystemMsg.h
//
/// NASGetServingSystemReq/NASGetServingSystemRsp/NASServingSystemInd
/// interface.
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
static const uint32 NASGetServingSystemReqUID =
    (QMUX_TYPE_NAS << 24 |
     QMI_CTL_FLAG_TYPE_CMD << 16 |
     QMI_NAS_GET_SERVING_SYSTEM_MSG);
static const uint32 NASGetServingSystemRspUID =
    (QMUX_TYPE_NAS << 24 |
     QMI_CTL_FLAG_TYPE_RSP << 16 |
     QMI_NAS_GET_SERVING_SYSTEM_MSG);
static const uint32 NASServingSystemIndUID =
    (QMUX_TYPE_NAS << 24 |
     QMI_CTL_FLAG_TYPE_IND << 16 |
     QMI_NAS_GET_SERVING_SYSTEM_MSG);


// --------------------------------------------------------------------------
// Define reference counting pointer types
// --------------------------------------------------------------------------
class NASGetServingSystemReq;
class NASGetServingSystemRsp;
class NASServingSystemInd;
typedef TRCPointer<NASGetServingSystemReq> NASGetServingSystemReqRCP;
typedef TRCPointer<NASGetServingSystemRsp> NASGetServingSystemRspRCP;
typedef TRCPointer<NASServingSystemInd> NASServingSystemIndRCP;


// --------------------------------------------------------------------------
// NASGetServingSystemReq
//
/// This class represents a QMI_NAS_GET_SERVING_SYSTEM_REQ message which
/// queries information regarding the system that currently provides service.
// --------------------------------------------------------------------------
class NASGetServingSystemReq : public Message
{

public:
    ~NASGetServingSystemReq();

    virtual void Print(std::ostream& stream);

protected:
    NASGetServingSystemReq();
    virtual bool BuildMsgBuf();
    virtual bool Unpack(MsgBuf& msgBuf) {
        return false;
    }
};


// --------------------------------------------------------------------------
// NASGetServingSystemRsp
//
/// This class represents a QMI_NAS_GET_SERVING_SYSTEM_RESP message
/// which is the response to the get serving system request.
// --------------------------------------------------------------------------

class NASGetServingSystemRsp : public Message
{
    // define tlv types
    static const uint8 RESULT_CODE_TYPE;
    static const uint8 SERVING_SYSTEM_TYPE;
    static const uint8 ROAMING_INDICATOR_TYPE;
    static const uint8 CURRENT_PLMN_TYPE;

public:
    ~NASGetServingSystemRsp();

    virtual void Print(std::ostream& stream);

    uint16 GetResult() {
        return m_result;
    }
    uint16 GetError() {
        return m_error;
    }
    bool IsServingSystem() {
        return m_servingSystemType == SERVING_SYSTEM_TYPE;
    }
    int8 GetRegistrationState() {
        return m_registrationState;
    }
    uint8 GetCsAttachState() {
        return m_csAttachState;
    }
    uint8 GetPsAttachState() {
        return m_psAttachState;
    }
    uint8 GetRegisteredNetwork() {
        return m_registeredNetwork;
    }
    uint8 GetNumRadioIfs() {
        return m_numRadioIfs;
    }
    std::vector<uint8> GetRadioIfs() {
        return m_radioIfs;
    }
    bool IsRoamingIndicator() {
        return m_roamingIndicatorType == ROAMING_INDICATOR_TYPE;
    }
    uint8 GetRoamingIndicator() {
        return m_roamingIndicator;
    }
    bool IsCurrentPlmn() {
        return m_currentPlmnType == CURRENT_PLMN_TYPE;
    }
    uint16 GetMobileCountryCode() {
        return m_mobileCountryCode;
    }
    uint16 GetMobileNetworkCode() {
        return m_mobileNetworkCode;
    }
    uint8 GetNetworkDescLen() {
        return m_networkDescLen;
    }
    std::string GetNetworkDesc() {
        return m_networkDesc;
    }

protected:
    NASGetServingSystemRsp();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackResultCode(MsgBuf& msgBuf);
    virtual bool UnpackServingSystem(MsgBuf& msgBuf);
    virtual bool UnpackRoamingIndicator(MsgBuf& msgBuf);
    virtual bool UnpackCurrentPlmn(MsgBuf& msgBuf);

private:
    // Result Code, mandatory tlv 0x02
    uint8 m_resultCodeType;
    uint16 m_resultCodeLen;
    uint16 m_result;
    uint16 m_error;

    // Serving System, mandatory tlv 0x01
    uint8 m_servingSystemType;
    uint16 m_servingSystemLen;
    uint8 m_registrationState;
    uint8 m_csAttachState;
    uint8 m_psAttachState;
    uint8 m_registeredNetwork;
    uint8 m_numRadioIfs;
    std::vector<uint8> m_radioIfs;

    // Roaming Indicator, optional tlv 0x10
    uint8 m_roamingIndicatorType;
    uint16 m_roamingIndicatorLen;
    uint8 m_roamingIndicator;

    // Current PLMN, optional tlv 0x12
    uint8 m_currentPlmnType;
    uint16 m_currentPlmnLen;
    uint16 m_mobileCountryCode;
    uint16 m_mobileNetworkCode;
    uint8 m_networkDescLen;
    std::string m_networkDesc;
};

// --------------------------------------------------------------------------
// NASServingSystemInd
//
/// This class represents a QMI_NAS_GET_SERVING_SYSTEM_IND message.
/// Indication is sent when there is a change in the current serving system
/// registration state and/or radio technology.
// --------------------------------------------------------------------------

class NASServingSystemInd : public Message
{
    // define tlv types
    static const uint8 SERVING_SYSTEM_TYPE ;
    static const uint8 ROAMING_INDICATOR_TYPE ;
    static const uint8 CURRENT_PLMN_TYPE ;

public:
    ~NASServingSystemInd();

    virtual void Print(std::ostream& stream);

    bool IsServingSystem() {
        return m_servingSystemType == SERVING_SYSTEM_TYPE;
    }
    int8 GetRegistrationState() {
        return m_registrationState;
    }
    uint8 GetCsAttachState() {
        return m_csAttachState;
    }
    uint8 GetPsAttachState() {
        return m_psAttachState;
    }
    uint8 GetRegisteredNetwork() {
        return m_registeredNetwork;
    }
    uint8 GetNumRadioIfs() {
        return m_numRadioIfs;
    }
    std::vector<uint8> GetRadioIfs() {
        return m_radioIfs;
    }
    bool IsRoamingIndicator() {
        return m_roamingIndicatorType == ROAMING_INDICATOR_TYPE;
    }
    uint8 GetRoamingIndicator() {
        return m_roamingIndicator;
    }
    bool IsCurrentPlmn() {
        return m_currentPlmnType == CURRENT_PLMN_TYPE;
    }
    uint16 GetMobileCountryCode() {
        return m_mobileCountryCode;
    }
    uint16 GetMobileNetworkCode() {
        return m_mobileNetworkCode;
    }
    uint8 GetNetworkDescLen() {
        return m_networkDescLen;
    }
    std::string GetNetworkDesc() {
        return m_networkDesc;
    }

protected:
    NASServingSystemInd();
    virtual bool Build(std::string& nameValue) {
        return false;
    }
    virtual bool BuildMsgBuf() {
        return false;
    }
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();
    virtual bool UnpackServingSystem(MsgBuf& msgBuf);
    virtual bool UnpackRoamingIndicator(MsgBuf& msgBuf);
    virtual bool UnpackCurrentPlmn(MsgBuf& msgBuf);

private:
    // Serving System, mandatory tlv 0x01
    uint8 m_servingSystemType;
    uint16 m_servingSystemLen;
    uint8 m_registrationState;
    uint8 m_csAttachState;
    uint8 m_psAttachState;
    uint8 m_registeredNetwork;
    uint8 m_numRadioIfs;
    std::vector<uint8> m_radioIfs;

    // Roaming Indicator, optional tlv 0x10
    uint8 m_roamingIndicatorType;
    uint16 m_roamingIndicatorLen;
    uint8 m_roamingIndicator;

    // Current PLMN, optional tlv 0x12
    uint8 m_currentPlmnType;
    uint16 m_currentPlmnLen;
    uint16 m_mobileCountryCode;
    uint16 m_mobileNetworkCode;
    uint8 m_networkDescLen;
    std::string m_networkDesc;
};
