// --------------------------------------------------------------------------
//
// NASSetEventReportMsg.cpp
//
/// NASSetEventReportReq/NASSetEventReportRsp/NASEventReportInd
/// implementation.
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
#include "stdafx.h"
#include <assert.h>
#include <sstream>
#include "NASSetEventReportMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<NASSetEventReportReq> ReqStringCreator("QMI_NAS_SET_EVENT_REPORT_REQ");
static MessageCreator<NASSetEventReportReq> ReqUint32Creator(NASSetEventReportReqUID);

static MessageCreator<NASSetEventReportRsp> RspStringCreator("QMI_NAS_SET_EVENT_REPORT_RSP");
static MessageCreator<NASSetEventReportRsp> RspUint32Creator(NASSetEventReportRspUID);

static MessageCreator<NASEventReportInd> IndStringCreator("QMI_NAS_EVENT_REPORT_IND");
static MessageCreator<NASEventReportInd> IndUint32Creator(NASEventReportIndUID);

// --------------------------------------------------------------------------
// NASSetEventReportReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASSetEventReportReq.
// --------------------------------------------------------------------------
const uint8 NASSetEventReportReq::SIGNAL_STRENGTH_INDICATOR_TYPE = 0x10;
NASSetEventReportReq::NASSetEventReportReq() :
    Message(QMUX_TYPE_NAS,QMI_NAS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_signalStrengthIndicatorType(TLV_TYPE_INVALID),
    m_signalStrengthIndicatorLen(0),
    m_reportSigStrength(-1),
    m_numRangeLimits(0)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportReq.
// --------------------------------------------------------------------------
NASSetEventReportReq::~NASSetEventReportReq()
{}

// --------------------------------------------------------------------------
// Build
//
/// Parse a string into this Message.
///
/// @param nameValue - string of name value pairs.
///
/// @returns bool - true if build successful
// --------------------------------------------------------------------------
bool NASSetEventReportReq::Build(std::string& nameValue)
{
    // call the base build function
    if (Message::Build(nameValue)) {
        // at least one optional tlv must be present
        if (m_signalStrengthIndicatorType != SIGNAL_STRENGTH_INDICATOR_TYPE) {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                   << _T("At least one of the following optional tlv's must be present:") << std::endl
                   << _T("  Signal Strength Indicator") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
            return false;
        }
    }
    return true;
}

// --------------------------------------------------------------------------
// GetBuilderMap
//
/// Return a reference to the string to builder map for this Message.
///
/// @returns StringBuilderMap& - reference to the string to builder map for
///     this Message type.
// --------------------------------------------------------------------------
Message::StringBuilderMap& NASSetEventReportReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty()) {
        bool bSuccess = SBMap.insert(SBPair("ReportSigStrength",(Builder)BuildReportSigStrength)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("RangeLimit",(Builder)BuildRangeLimit)).second;
        assert(bSuccess);

    }
    return SBMap;
}

// --------------------------------------------------------------------------
// BuildReportSigStrength
//
/// Build the report signal strength attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool NASSetEventReportReq::BuildReportSigStrength(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num != 0 && num != 1) {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Report Signal Strength value '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid values are 0 and 1.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_signalStrengthIndicatorType = SIGNAL_STRENGTH_INDICATOR_TYPE;
    m_signalStrengthIndicatorLen = 2;
    m_reportSigStrength = (uint8)num;
    m_length += (m_signalStrengthIndicatorLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "RangeLimit";
    return true;
}

// --------------------------------------------------------------------------
// BuildRangeLimit
//
/// Build the range limit attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool NASSetEventReportReq::BuildRangeLimit(std::string& value)
{
    int32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num < -128 || num > 127) {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Range Limit '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid limit values are -127 to 128, inclusive.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_rangeLimits.push_back(num);
    m_numRangeLimits++;
    m_signalStrengthIndicatorLen++;
    m_length++;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildMsgBuf
//
/// Build a MsgBuf representation of this Message.
///
/// @returns bool - true if the MsgBuf was built successfully.
// --------------------------------------------------------------------------
bool NASSetEventReportReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // optional tlv, power state reporting mode
    if (m_signalStrengthIndicatorType == SIGNAL_STRENGTH_INDICATOR_TYPE) {
        m_pMsgBuf->PutByte(m_signalStrengthIndicatorType);
        m_pMsgBuf->PutWord(m_signalStrengthIndicatorLen);
        m_pMsgBuf->PutByte(m_reportSigStrength);
        m_pMsgBuf->PutByte(m_numRangeLimits);
        for (int i = 0; i < m_numRangeLimits; i++) {
            m_pMsgBuf->PutByte(m_rangeLimits[i]);
        }
    }

    assert(m_length == m_pMsgBuf->GetIndex() - QMI_SDU_HEADER_BYTES);
    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void NASSetEventReportReq::Print(std::ostream& stream)
{
    stream << "QMI_NAS_SET_EVENT_REPORT_REQ" << std::endl
           << _T("{") << std::endl;

    if (m_signalStrengthIndicatorType == SIGNAL_STRENGTH_INDICATOR_TYPE) {
        stream << _T("  ReportSigStrength ") << (int)m_reportSigStrength << std::endl;

        for (int i = 0; i < m_numRangeLimits; i++) {
            stream << _T("  RangeLimit ") << (int)m_rangeLimits[i] << std::endl;
        }
    }

    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// NASSetEventReportRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASSetEventReportRsp.
// --------------------------------------------------------------------------
const uint8 NASSetEventReportRsp::RESULT_CODE_TYPE = 0x02;
NASSetEventReportRsp::NASSetEventReportRsp() :
    Message(QMUX_TYPE_NAS,QMI_NAS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASSetEventReportRsp.
// --------------------------------------------------------------------------
NASSetEventReportRsp::~NASSetEventReportRsp()
{}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into this Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASSetEventReportRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // validate message length
    if (m_length != 7) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected message length is 7 bytes, unpacked length is ")
               << m_length << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// GetUnpackerMap
//
/// Return a reference to the uint8 to unpacker map for this Message.
///
/// @returns Uint*UnpackerMap& - reference to the uint8 to unpacker map for
///     this Message type.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& NASSetEventReportRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(RESULT_CODE_TYPE,(Unpacker)UnpackResultCode)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackResultCode
//
/// Unpack the result code tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASSetEventReportRsp::UnpackResultCode(MsgBuf& msgBuf)
{
    m_resultCodeType = RESULT_CODE_TYPE;

    m_resultCodeLen = msgBuf.GetWord();
    if (m_resultCodeLen != 4) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Result Code length is 4 bytes, unpacked length is ")
               << m_resultCodeLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_result = msgBuf.GetWord();
    m_error = msgBuf.GetWord();

    if (!msgBuf.EOB()) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Finished unpacking message but end of buffer not reached")
               << std::endl << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    return true;
}
// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void NASSetEventReportRsp::Print(std::ostream& stream)
{
    stream << "QMI_NAS_SET_EVENT_REPORT_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl
           << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// NASEventReportInd
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for NASEventReportInd.
// --------------------------------------------------------------------------
const uint8 NASEventReportInd::SIGNAL_STRENGTH_TYPE = 0x10;
NASEventReportInd::NASEventReportInd() :
    Message(QMUX_TYPE_NAS,QMI_NAS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_IND),
    m_signalStrengthType(TLV_TYPE_INVALID),
    m_signalStrengthLen(0),
    m_signalStrength(-1),
    m_radioIf(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for NASEventReportInd.
// --------------------------------------------------------------------------
NASEventReportInd::~NASEventReportInd()
{}

// --------------------------------------------------------------------------
// Unpack
//
/// Unpack a MsgBuf into this Message.
///
/// @param msgBuf - the MsgBuf to unpack.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASEventReportInd::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf)) {
        return false;
    }

    // at least one optional tlv must be present
    if (m_signalStrengthType != SIGNAL_STRENGTH_TYPE) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("At least one of the following optional tlv's must be present:") << std::endl
               << _T("  Signal Strength") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // validate message length
    if (m_length != 5) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected message length is 5 bytes, unpacked length is ")
               << m_length << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// GetUnpackerMap
//
/// Return a reference to the uint8 to unpacker map for this Message.
///
/// @returns Uint*UnpackerMap& - reference to the uint8 to unpacker map for
///     this Message type.
// --------------------------------------------------------------------------
Message::Uint8UnpackerMap& NASEventReportInd::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty()) {
        bool bSuccess = UUMap.insert(UUPair(SIGNAL_STRENGTH_TYPE,(Unpacker)UnpackSignalStrength)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackSignalStrength
//
/// Unpack the signal strength tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool NASEventReportInd::UnpackSignalStrength(MsgBuf& msgBuf)
{
    m_signalStrengthType = SIGNAL_STRENGTH_TYPE;

    m_signalStrengthLen = msgBuf.GetWord();
    if (m_signalStrengthLen != 2) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Signal Strength length is 2 bytes, unpacked length is ")
               << m_signalStrengthLen << _T(" bytes.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_signalStrength = msgBuf.GetByte();

    m_radioIf = msgBuf.GetByte();
    if (m_radioIf > 5) {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Radio IF values are 0 - 5 , unpacked value is ")
               << m_radioIf << _T(" .") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// Print
//
/// Print this Message to a stream.
///
/// @param stream - the ostream to print to.
// --------------------------------------------------------------------------
void NASEventReportInd::Print(std::ostream& stream)
{
    stream << "QMI_NAS_EVENT_REPORT_IND" << std::endl
           << _T("{") << std::endl;

    if (m_signalStrengthType == SIGNAL_STRENGTH_TYPE) {
        stream << _T("  SignalStrength ") << (int)m_signalStrength << std::endl
               << _T("  RadioIf ") << (int)m_radioIf << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
