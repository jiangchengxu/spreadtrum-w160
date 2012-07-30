// --------------------------------------------------------------------------
//
// Message.h
//
/// Message interface.
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

#include "api.h"

#include <iosfwd>
#include <map>
#include "RefCountObject.h"
#include "CommonDefs.h"

//#define OPEN_PAGE_UI

class MsgBuf;

// --------------------------------------------------------------------------
// Message structure constants
// --------------------------------------------------------------------------
static const uint8 QMI_CTL_FLAG_BYTES = 1;
static const uint8 QMI_TX_ID_BYTES = 2;
static const uint8 QMI_MSG_HEADER_BYTES = 4;
static const uint8 QMI_SDU_HEADER_BYTES = 7;
static const uint8 QMI_TLV_HEADER_BYTES = 3;
static const uint8 TLV_TYPE_INVALID = 0xFF;

// --------------------------------------------------------------------------
// QMI_CTL_FLAGS - Message Type of the bundled message element(s)
// --------------------------------------------------------------------------
static const uint8 QMI_CTL_FLAG_SINGLE_MSG =                            0x00;
static const uint8 QMI_CTL_FLAG_COMPOUND_MSG =                          0x01;
static const uint8 QMI_CTL_FLAG_TYPE_CMD =                              0x00;
static const uint8 QMI_CTL_FLAG_TYPE_RSP =                              0x02;
static const uint8 QMI_CTL_FLAG_TYPE_IND =                              0x04;
//static const uint8 QMI_CTL_FLAG_MASK_COMPOUND =                         0x01;
//static const uint8 QMI_CTL_FLAG_MASK_TYPE =                             0x06; 

// --------------------------------------------------------------------------
// QMI_WDS - Wireless Data Service Messages
// --------------------------------------------------------------------------
static const uint16 QMI_WDS_RESET_MSG =                               0x0000;
static const uint16 QMI_WDS_SET_EVENT_REPORT_MSG =                    0x0001;
static const uint16 QMI_WDS_ABORT_MSG =                               0x0002;
static const uint16 QMI_WDS_START_NETWORK_INTERFACE_MSG =             0x0020;
static const uint16 QMI_WDS_STOP_NETWORK_INTERFACE_MSG =              0x0021;
static const uint16 QMI_WDS_GET_PKT_SRVC_STATUS_MSG =                 0x0022;
static const uint16 QMI_WDS_GET_CURRENT_CHANNEL_RATE_MSG =            0x0023;
static const uint16 QMI_WDS_GET_PKT_STATISTICS_MSG =                  0x0024;
static const uint16 QMI_WDS_CREATE_PROFILE_MSG =                      0x0027;
static const uint16 QMI_WDS_MODIFY_PROFILE_SETTINGS_MSG =             0x0028;
static const uint16 QMI_WDS_DELETE_PROFILE_MSG =                      0x0029;
static const uint16 QMI_WDS_GET_PROFILE_LIST_MSG =                    0x002A;
static const uint16 QMI_WDS_GET_PROFILE_SETTINGS_MSG =                0x002B;
static const uint16 QMI_WDS_GET_DEFAULT_SETTINGS_MSG =                0x002C;
static const uint16 QMI_WDS_GET_GATEWAY_ADDR_MSG =                    0xF222;

// --------------------------------------------------------------------------
// QMI_DMS - Device Management Service Messages
// --------------------------------------------------------------------------
static const uint16 QMI_DMS_RESET_MSG =                               0x0000;
static const uint16 QMI_DMS_SET_EVENT_REPORT_MSG =                    0x0001;
static const uint16 QMI_DMS_GET_DEVICE_CAP_MSG =                      0x0020;
static const uint16 QMI_DMS_GET_DEVICE_MFR_MSG =                      0x0021;
static const uint16 QMI_DMS_GET_DEVICE_MODEL_ID_MSG =                 0x0022;
static const uint16 QMI_DMS_GET_DEVICE_REV_ID_MSG =                   0x0023;
static const uint16 QMI_DMS_GET_MSISDN_MSG =                          0x0024;
static const uint16 QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_MSG =           0x0025;
static const uint16 QMI_DMS_GET_POWER_STATE_MSG =                     0x0026;

// --------------------------------------------------------------------------
// QMI_NAS - Network Access Service Messages
// --------------------------------------------------------------------------
static const uint16 QMI_NAS_RESET_MSG =                               0x0000;
static const uint16 QMI_NAS_ABORT_MSG =                               0x0001;
static const uint16 QMI_NAS_SET_EVENT_REPORT_MSG =                    0x0002;
static const uint16 QMI_NAS_GET_SIGNAL_STRENGTH_MSG =                 0x0020;
static const uint16 QMI_NAS_PERFORM_NETWORK_SCAN_MSG =                0x0021;
static const uint16 QMI_NAS_PERFORM_NETWORK_REGISTER_MSG =            0x0022;
static const uint16 QMI_NAS_PERFORM_ATTACH_MSG =                      0x0023;
static const uint16 QMI_NAS_GET_SERVING_SYSTEM_MSG =                  0x0024;
static const uint16 QMI_NAS_GET_HOME_NETWORK_MSG =                    0x0025;
static const uint16 QMI_NAS_GET_PREFERRED_NETWORKS_MSG =              0x0026;
static const uint16 QMI_NAS_SET_PREFERRED_NETWORKS_MSG =              0x0027;
static const uint16 QMI_NAS_GET_FORBIDDEN_NETWORKS_MSG =              0x0028;
static const uint16 QMI_NAS_SET_FORBIDDEN_NETWORKS_MSG =              0x0029;

// --------------------------------------------------------------------------
// Result Codes
// --------------------------------------------------------------------------
static const uint16 QMI_RESULT_SUCCESS =                              0x0000;
static const uint16 QMI_RESULT_FAILURE =                              0x0001;

// --------------------------------------------------------------------------
// Error Codes
// --------------------------------------------------------------------------
static const uint16 QMI_ERR_NONE =                                    0x0000;
static const uint16 QMI_ERR_MALFORMED_MSG =                           0x0001;
static const uint16 QMI_ERR_NO_MEMORY =                               0x0002;
static const uint16 QMI_ERR_INTERNAL =                                0x0003;
static const uint16 QMI_ERR_ABORTED =                                 0x0004;
static const uint16 QMI_ERR_CLIENT_IDS_EXHAUSTED =                    0x0005;
static const uint16 QMI_ERR_UNABORTABLE_TRANSACTION =                 0x0006;
static const uint16 QMI_ERR_INVALID_CLIENT_ID =                       0x0007;
static const uint16 QMI_ERR_NO_THRESHOLDS =                           0x0008;
static const uint16 QMI_ERR_INVALID_HANDLE =                          0x0009;
static const uint16 QMI_ERR_INVALID_PROFILE =                         0x000A;
static const uint16 QMI_ERR_PIN_LOCKED =                              0x000E;
static const uint16 QMI_ERR_OUT_OF_CALL =                             0x000F;
static const uint16 QMI_ERR_NOT_PROVISIONED =                         0x0010;
static const uint16 QMI_ERR_MISSING_ARG =                             0x0011;
static const uint16 QMI_ERR_ARG_TOO_LONG =                            0x0013;
static const uint16 QMI_ERR_INVALID_TX_ID =                           0x0016;
static const uint16 QMI_ERR_DEVICE_IN_USE =                           0x0017;
static const uint16 QMI_ERR_OP_NETWORK_UNSUPPORTED =                  0x0018;
static const uint16 QMI_ERR_OP_DEVICE_UNSUPPORTED =                   0x0019;
static const uint16 QMI_ERR_NO_EFFECT =                               0x001A;
static const uint16 QMI_ERR_NO_FREE_PROFILE =                         0x001B;
static const uint16 QMI_ERR_INVALID_PDP_TYPE =                        0x001C;
static const uint16 QMI_ERR_INVALID_TECH_PREF =                       0x001D;
static const uint16 QMI_ERR_INVALID_PROFILE_TYPE =                    0x001E;
static const uint16 QMI_ERR_INVALID_SERVICE_TYPE =                    0x001F;
static const uint16 QMI_ERR_INVALID_REGISTER_ACTION =                 0x0020;
static const uint16 QMI_ERR_INVALID_PS_ATTACH_ACTION =                0x0021;
static const uint16 QMI_ERR_AUTHENTICATION_FAILED =                   0x0022;
static const uint16 QMI_ERR_MAX =                                     0x0023;
static const uint16 QMI_ERR_WIDTH =                                   0xFFFF;

// --------------------------------------------------------------------------
// Error Strings
// --------------------------------------------------------------------------
static const std::string ERROR_STRINGS[] = 
{
    "None",
    "Malformed Message",
    "No Memmory",
    "Internal",
    "Aborted",
    "Client IDs Exausted",
    "Unabortable Transaction",
    "Invalid Client ID",
    "No Thresholds",
    "Invalid Handle",
    "Invalid Profile",
    "UNSUPPORTED ERROR CODE",
    "UNSUPPORTED ERROR CODE",
    "UNSUPPORTED ERROR CODE",
    "PIN Locked",
    "Out Of Call",
    "Not Provisioned",
    "Missing Arguement",
    "UNSUPPORTED ERROR CODE",
    "Arguement Too Long",
    "UNSUPPORTED ERROR CODE",
    "UNSUPPORTED ERROR CODE",
    "Invalid Transaction ID",
    "Device In Use",
    "OP Network Unsupported",
    "OP Device Unsupported",
    "No Effect",
    "No Free Profile",
    "Invalid PDP Type",
    "Invalid Technology Preference",
    "Invalid Profile Type",
    "Invalid Service Type",
    "Invalid Register Action",
    "Invalid PS Attach Action",
    "Authentication Failed"
};

// --------------------------------------------------------------------------
// Data Bearer Technology Strings
// --------------------------------------------------------------------------
static const std::string DATA_BEARER_TECH_STRINGS[] = 
{
    "INVALID DATA BEARER TECHNOLOGY",
    "CDMA2000 1x",
    "CDMA2000 HRPD (1xEV-DO)",
    "GSM",
    "UMTS",
    "IEEE 802.11 (WiFi)"
};

// --------------------------------------------------------------------------
// Radio Interface Technology Strings
// --------------------------------------------------------------------------
static const std::string RADIO_IF_TECH_STRINGS[] = 
{
    "No Service",
    "CDMA2000 1x",
    "CDMA2000 HRPD",
    "AMPS",
    "GSM",
    "UMTS"
};

// --------------------------------------------------------------------------
// Profile Type Strings
// --------------------------------------------------------------------------
static const std::string PROFILE_TYPE_STRINGS[] = 
{
    "3GPP"
};

// --------------------------------------------------------------------------
// Define reference counting pointer message type
// --------------------------------------------------------------------------
class Message;
typedef TRCPointer<Message> MessageRCP;

// --------------------------------------------------------------------------
// Message
//
/// Message is the base class for all message types. It is an abstract class
/// that defines the basic interface that all messages must implement.
// --------------------------------------------------------------------------
class Message : public IRCObject
{
public:
    typedef bool (Message::*Builder)(std::string&);
    typedef std::map<std::string,Builder> StringBuilderMap;
    typedef StringBuilderMap::value_type SBPair;

    typedef bool (Message::*Unpacker)(MsgBuf&);
    typedef std::map<uint8,Unpacker> Uint8UnpackerMap;
    typedef Uint8UnpackerMap::value_type UUPair;

public:
    // !!! make protected
    Message(uint8 svcType,uint16 msgType,uint8 ctlType);
    virtual ~Message();

    template<typename T>
    static Message* Create(std::string& nameValue, T* dummy)
	{
		// new up the message
		Message* pMsg = THeapObject<T>::Create();

		// initialize the message with the string
		if (pMsg->Build(nameValue))
		{
			// build successful, return the message pointer
			return pMsg;
		}

		// build not successful, add/release to delete the message and return null.
		pMsg->AddRef();
		pMsg->Release();
		return NULL;
	}

    template<typename T>
    static Message* Create(MsgBuf& msgBuf, T* dummy)
	{
		// new up the message
		Message* pMsg = THeapObject<T>::Create();

		// initialize the message with the buffer
		if (pMsg->Unpack(msgBuf))
		{
			// unpack successful, return the message pointer
			return pMsg;
		}

		// unpack not successful, add/release to delete the message and return null.
		pMsg->AddRef();
		pMsg->Release();
		return NULL;
	}

    virtual uint8 GetSvcType() { return m_svcType; }
    virtual uint16 GetMsgType() { return m_msgType; }
    virtual uint8 GetCtlType() { return m_ctlType; }
    virtual uint16 GetTxId() { return m_txId; }
    virtual uint16 GetLength() { return m_length; }
    virtual MsgBuf& GetMsgBuf() { return *m_pMsgBuf; }

    virtual void Print(std::ostream& stream) = 0;
    virtual void PrintMsgBuf(std::ostream& stream);
    virtual bool ExtractIpv4Addr(std::string& value,uint32& addr);
    virtual void PrintIPv4Addr(uint32 addr,std::ostream& stream);

protected:
    virtual bool Build(std::string& nameValue);
    virtual StringBuilderMap& GetBuilderMap();
    virtual bool BuildMsgBuf() = 0;
    virtual bool Unpack(MsgBuf& msgBuf);
    virtual Uint8UnpackerMap& GetUnpackerMap();

    virtual void ReportInvalidSequence(std::string expectedPrev);

protected:
    uint8 m_svcType;
    uint16 m_msgType;
    uint8 m_ctlType;
    uint16 m_txId;
    uint16 m_length;

    MsgBuf* m_pMsgBuf;

    std::string m_prevName;
    std::string m_name;
    std::string m_nextName;

private:
    Message(const Message&);
    Message& operator=(const Message&);
};

#if 0
// --------------------------------------------------------------------------
// Create
//
/// Create a new Message by parsing a string. By making the ctor protected
/// and using a static create function, initialization of the Message using
/// the MsgBuf can be validated before returning a Message pointer.
///
/// @param nameValue - string used to initialize the Message.
///
/// @returns T* - pointer to the created Message if successful,
///     NULL otherwise.
// --------------------------------------------------------------------------
template<typename T>
Message* Message::Create(std::string& nameValue)
{
    // new up the message
    Message* pMsg = THeapObject<T>::Create();

    // initialize the message with the string
    if (pMsg->Build(nameValue))
    {
        // build successful, return the message pointer
        return pMsg;
    }

    // build not successful, add/release to delete the message and return null.
    pMsg->AddRef();
    pMsg->Release();
    return NULL;
}

// --------------------------------------------------------------------------
// Create
//
/// Create a new Message by unpacking a MsgBuf. By making the ctor protected
/// and using a static create function, initialization of the Message using
/// the MsgBuf can be validated before returning a Message pointer.
///
/// @param msgBuf - MsgBuf used to initialize the Message.
///
/// @returns Message* - pointer to the created Message if successful,
///     NULL otherwise.
// --------------------------------------------------------------------------
template<typename T>
Message* Message::Create(MsgBuf& msgBuf)
{
    // new up the message
    Message* pMsg = THeapObject<T>::Create();

    // initialize the message with the buffer
    if (pMsg->Unpack(msgBuf))
    {
        // unpack successful, return the message pointer
        return pMsg;
    }

    // unpack not successful, add/release to delete the message and return null.
    pMsg->AddRef();
    pMsg->Release();
    return NULL;
}
#endif