// --------------------------------------------------------------------------
//
// WDSSetEventReportMsg.cpp
//
/// WDSSetEventReportReq/WDSSetEventReportRsp/WDSEventReportInd
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
#include "WDSSetEventReportMsg.h"
#include "MessageFactory.h"
#include "MessageManager.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// Instantiate MessageCreator objects (adds to MessageFactory)
// --------------------------------------------------------------------------
static MessageCreator<WDSSetEventReportReq> ReqStringCreator("QMI_WDS_SET_EVENT_REPORT_REQ");
static MessageCreator<WDSSetEventReportReq> ReqUint32Creator(WDSSetEventReportReqUID);

static MessageCreator<WDSSetEventReportRsp> RspStringCreator("QMI_WDS_SET_EVENT_REPORT_RSP");
static MessageCreator<WDSSetEventReportRsp> RspUint32Creator(WDSSetEventReportRspUID);

static MessageCreator<WDSEventReportInd> IndStringCreator("QMI_WDS_EVENT_REPORT_IND");
static MessageCreator<WDSEventReportInd> IndUint32Creator(WDSEventReportIndUID);

// --------------------------------------------------------------------------
// WDSSetEventReportReq
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSSetEventReportReq.
// --------------------------------------------------------------------------
const uint8 WDSSetEventReportReq::CURRENT_CHANNEL_RATE_INDICATOR_TYPE = 0x10;
const uint8 WDSSetEventReportReq::TRANSFER_STATISTICS_INDICATOR_TYPE = 0x11;
const uint8 WDSSetEventReportReq::CURRENT_DATA_BEARER_TECH_INDICATOR_TYPE = 0x12;
WDSSetEventReportReq::WDSSetEventReportReq() : 
    Message(QMUX_TYPE_WDS,QMI_WDS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_CMD),
    m_currentChannelRateIndicatorType(TLV_TYPE_INVALID),
    m_currentChannelRateIndicatorLen(0),
    m_reportChannelRate(-1),
    m_transferStatisticsIndicatorType(TLV_TYPE_INVALID),
    m_transferStatisticsIndicatorLen(0),
    m_statsPeriod(-1),
    m_statsMask(-1),
    m_currentDataBearerTechIndicatorType(TLV_TYPE_INVALID),
    m_currentDataBearerTechIndicatorLen(0),
    m_reportDataBearerTech(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSSetEventReportReq.
// --------------------------------------------------------------------------
WDSSetEventReportReq::~WDSSetEventReportReq()
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
bool WDSSetEventReportReq::Build(std::string& nameValue)
{
    // call the base build function
    if (Message::Build(nameValue))
    {
        // at least one optional tlv must be present
        if (m_currentChannelRateIndicatorType != CURRENT_CHANNEL_RATE_INDICATOR_TYPE &&
            m_transferStatisticsIndicatorType != TRANSFER_STATISTICS_INDICATOR_TYPE &&
            m_currentDataBearerTechIndicatorType != CURRENT_DATA_BEARER_TECH_INDICATOR_TYPE)
        {
            std::stringstream stream;
            stream << _T("Warning: unable to unpack message:") << std::endl
                << _T("At least one of the following optional tlv's must be present:") << std::endl 
                << _T("  Current Channel Rate Indicator") << std::endl
                << _T("  Transfer Statistics Indicator") << std::endl
                << _T("  Current Data Bearer Technology Indicator") << std::endl
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
Message::StringBuilderMap& WDSSetEventReportReq::GetBuilderMap()
{
    static StringBuilderMap SBMap;
    if (SBMap.empty())
    {
        bool bSuccess = SBMap.insert(SBPair("ReportChannelRate",(Builder)BuildReportChannelRate)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("StatsPeriod",(Builder)BuildStatsPeriod)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("StatsMask",(Builder)BuildStatsMask)).second;
        assert(bSuccess);
        bSuccess = SBMap.insert(SBPair("ReportDataBearerTech",(Builder)BuildReportDataBearerTech)).second;
        assert(bSuccess);
    }
    return SBMap;
}

// --------------------------------------------------------------------------
// BuildReportChannelRate
//
/// Build the report channel rate attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSSetEventReportReq::BuildReportChannelRate(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num != 0 && num != 1)
    {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Channel Rate Indicator Mode '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid mode values are 0 and 1.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_currentChannelRateIndicatorType = CURRENT_CHANNEL_RATE_INDICATOR_TYPE;
    m_currentChannelRateIndicatorLen = 1;
    m_reportChannelRate = (uint8)num;
    m_length += (m_currentChannelRateIndicatorLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildStatsPeriod
//
/// Build the stats mask attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSSetEventReportReq::BuildStatsPeriod(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 256)
    {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Stats Period value '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid period values are 0 - 256.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_transferStatisticsIndicatorType = 0x11;
    m_transferStatisticsIndicatorLen = 5;
    m_statsPeriod = (uint8)num;
    m_length += (m_transferStatisticsIndicatorLen + QMI_TLV_HEADER_BYTES);

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName = "StatsMask";
    return true;
}

// --------------------------------------------------------------------------
// BuildStatsMask
//
/// Build the battery level upper limit attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSSetEventReportReq::BuildStatsMask(std::string& value)
{
    // verify expected previous attribute
    if (m_prevName.compare("StatsPeriod") != 0)
    {
        // report invalid attribute sequence
        ReportInvalidSequence("StatsPeriod");
        return false;
    }

    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num > 63)
    {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Stats Mask '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid mask values are 0 - 63.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    // no type or length, part of m_transferStatisticsIndicatorType tlv
    m_statsMask = (uint32)num;

    // set previous name to current name and set expectations for next name.
    m_prevName = m_name;
    m_nextName.erase(m_nextName.begin(), m_nextName.end());
    return true;
}

// --------------------------------------------------------------------------
// BuildReportDataBearerTech
//
/// Build the report data bearer technology attribute for this Message.
///
/// @param value - string representation of attribute value.
///
/// @returns bool - true if build successful.
// --------------------------------------------------------------------------
bool WDSSetEventReportReq::BuildReportDataBearerTech(std::string& value)
{
    uint32 num;

    sscanf(value.c_str(), "%i", &num);

    // validate entry
    if (num != 0 && num != 1)
    {
        // report invalid profile type
        std::stringstream stream;
        stream << _T("Warning: unable to build message:") << std::endl
               << _T("Invalid Data Bearer Technology Indicator Mode '") << (int)num
               << _T("'.") << std::endl
               << _T("Valid mode values are 0 and 1.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    // set member data
    m_currentDataBearerTechIndicatorType = CURRENT_DATA_BEARER_TECH_INDICATOR_TYPE;
    m_currentDataBearerTechIndicatorLen = 1;
    m_reportDataBearerTech = (uint8)num;
    m_length += (m_currentChannelRateIndicatorLen + QMI_TLV_HEADER_BYTES);

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
bool WDSSetEventReportReq::BuildMsgBuf()
{
    m_pMsgBuf = new MsgBuf(m_length + QMI_SDU_HEADER_BYTES);

    // Compose QMI header
    m_pMsgBuf->PutByte(QMI_CTL_FLAG_SINGLE_MSG | QMI_CTL_FLAG_TYPE_CMD);
    m_pMsgBuf->PutWord(m_txId);

    // Compose QMI message body
    m_pMsgBuf->PutWord(m_msgType);
    m_pMsgBuf->PutWord(m_length);

    // optional tlv, current channel rate indicator
    if (m_currentChannelRateIndicatorType == CURRENT_CHANNEL_RATE_INDICATOR_TYPE)
    {
        m_pMsgBuf->PutByte(m_currentChannelRateIndicatorType);
        m_pMsgBuf->PutWord(m_currentChannelRateIndicatorLen);
        m_pMsgBuf->PutByte(m_reportChannelRate);
    }

    // optional tlv, transfer statistics indicator
    if (m_transferStatisticsIndicatorType == TRANSFER_STATISTICS_INDICATOR_TYPE)
    {
        m_pMsgBuf->PutByte(m_transferStatisticsIndicatorType);
        m_pMsgBuf->PutWord(m_transferStatisticsIndicatorLen);
        m_pMsgBuf->PutByte(m_statsPeriod);
        m_pMsgBuf->PutDWord(m_statsMask);
    }

    // optional tlv, current data bearer technology indicator
    if (m_currentDataBearerTechIndicatorType == CURRENT_DATA_BEARER_TECH_INDICATOR_TYPE)
    {
        m_pMsgBuf->PutByte(m_currentDataBearerTechIndicatorType);
        m_pMsgBuf->PutWord(m_currentDataBearerTechIndicatorLen);
        m_pMsgBuf->PutByte(m_reportDataBearerTech);
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
void WDSSetEventReportReq::Print(std::ostream& stream)
{
    stream << "QMI_WDS_SET_EVENT_REPORT_REQ" << std::endl
           << _T("{") << std::endl;
    
    if (m_currentChannelRateIndicatorType == CURRENT_CHANNEL_RATE_INDICATOR_TYPE)
    {
        stream << _T("  ReportChannelRate ") << (int)m_reportChannelRate << std::endl;
    }

    if (m_transferStatisticsIndicatorType == TRANSFER_STATISTICS_INDICATOR_TYPE)
    {
        stream << _T("  StatsPeriod ") << (int)m_statsPeriod << std::endl
               << _T("  StatsMask ") << (int)m_statsMask << std::endl;
    }

    if (m_currentDataBearerTechIndicatorType == CURRENT_DATA_BEARER_TECH_INDICATOR_TYPE)
    {
        stream << _T("  ReportDataBearerTech ") << (int)m_reportDataBearerTech << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSSetEventReportRsp
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSSetEventReportRsp.
// --------------------------------------------------------------------------
const uint8 WDSSetEventReportRsp::RESULT_CODE_TYPE = 0x02;
WDSSetEventReportRsp::WDSSetEventReportRsp() :
    Message(QMUX_TYPE_WDS,QMI_WDS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_RSP),
    m_resultCodeType(TLV_TYPE_INVALID),
    m_resultCodeLen(0),
    m_result(-1),
    m_error(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSSetEventReportRsp.
// --------------------------------------------------------------------------
WDSSetEventReportRsp::~WDSSetEventReportRsp()
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
bool WDSSetEventReportRsp::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }
    
    // validate message length
    if (m_length != 7) 
    {
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
Message::Uint8UnpackerMap& WDSSetEventReportRsp::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
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
bool WDSSetEventReportRsp::UnpackResultCode(MsgBuf& msgBuf)
{
    m_resultCodeType = RESULT_CODE_TYPE;

    m_resultCodeLen = msgBuf.GetWord();
    if (m_resultCodeLen != 4) 
    {
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

    if (!msgBuf.EOB())
    {
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
void WDSSetEventReportRsp::Print(std::ostream& stream)
{
    stream << "QMI_WDS_SET_EVENT_REPORT_RESP" << std::endl
           << _T("{") << std::endl
           << _T("  ResultCode ") << (int)m_result << std::endl
           << _T("  ErrorCode ") << (int)m_error << std::endl
           << _T("}") << std::endl << std::endl;
}


// --------------------------------------------------------------------------
// WDSEventReportInd
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for WDSEventReportInd.
// --------------------------------------------------------------------------
const uint8 WDSEventReportInd::TX_PACKETS_OK_TYPE = 0x10;
const uint8 WDSEventReportInd::RX_PACKETS_OK_TYPE = 0x11;
const uint8 WDSEventReportInd::TX_PACKET_ERRORS_TYPE = 0x12;
const uint8 WDSEventReportInd::RX_PACKET_ERRORS_TYPE = 0x13;
const uint8 WDSEventReportInd::TX_OVERFOLWS_TYPE = 0x14;
const uint8 WDSEventReportInd::RX_OVERFOLWS_TYPE = 0x15;
const uint8 WDSEventReportInd::CHANNEL_RATE_TYPE = 0x16;
const uint8 WDSEventReportInd::DATA_BEARER_TECH_TYPE = 0x17;
WDSEventReportInd::WDSEventReportInd() :
    Message(QMUX_TYPE_WDS,QMI_WDS_SET_EVENT_REPORT_MSG,QMI_CTL_FLAG_TYPE_IND),
    m_txPacketsOkType(TLV_TYPE_INVALID),
    m_txPacketsOkLen(0),
    m_txOkCount(-1),
    m_rxPacketsOkType(TLV_TYPE_INVALID),
    m_rxPacketsOkLen(0),
    m_rxOkCount(-1),
    m_txPacketErrorsType(TLV_TYPE_INVALID),
    m_txPacketErrorsLen(0),
    m_txErrCount(-1),
    m_rxPacketErrorsType(TLV_TYPE_INVALID),
    m_rxPacketErrorsLen(0),
    m_rxErrCount(-1),
    m_txOverflowsType(TLV_TYPE_INVALID),
    m_txOverflowsLen(0),
    m_txOflCount(-1),
    m_rxOverflowsType(TLV_TYPE_INVALID),
    m_rxOverflowsLen(0),
    m_rxOflCount(-1),
    m_channelRateType(TLV_TYPE_INVALID),
    m_channelRateLen(0),
    m_currentChannelTxRate(-1),
    m_currentChannelRxRate(-1),
    m_dataBearerTechType(TLV_TYPE_INVALID),
    m_dataBearerTechLen(0),
    m_dataBearerTech(-1)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for WDSEventReportInd.
// --------------------------------------------------------------------------
WDSEventReportInd::~WDSEventReportInd()
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
bool WDSEventReportInd::Unpack(MsgBuf& msgBuf)
{
    // call the base unpack
    if (!Message::Unpack(msgBuf))
    {
        return false;
    }

    // at least one optional tlv must be present
    if (m_txPacketsOkType != TX_PACKETS_OK_TYPE &&
        m_rxPacketsOkType != RX_PACKETS_OK_TYPE &&
        m_txPacketErrorsType != TX_PACKET_ERRORS_TYPE &&
        m_rxPacketErrorsType != RX_PACKET_ERRORS_TYPE &&
        m_rxOverflowsType != TX_OVERFOLWS_TYPE &&
        m_txOverflowsType != RX_OVERFOLWS_TYPE &&
        m_channelRateType != CHANNEL_RATE_TYPE &&
        m_dataBearerTechType != DATA_BEARER_TECH_TYPE)
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("At least one of the following optional tlv's must be present:") << std::endl 
               << _T("  TX Packets OK") << std::endl
               << _T("  RX Packets OK") << std::endl
               << _T("  TX Packet Errors") << std::endl
               << _T("  RX Packet Errors") << std::endl
               << _T("  TX Overflows") << std::endl
               << _T("  RX Overflows") << std::endl
               << _T("  Channel Rate") << std::endl
               << _T("  Data Bearer Technology") << std::endl
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
Message::Uint8UnpackerMap& WDSEventReportInd::GetUnpackerMap()
{
    static Uint8UnpackerMap UUMap;
    if (UUMap.empty())
    {
        bool bSuccess = UUMap.insert(UUPair(TX_PACKETS_OK_TYPE,(Unpacker)UnpackTxPacketsOk)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(RX_PACKETS_OK_TYPE,(Unpacker)UnpackRxPacketsOk)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(TX_PACKET_ERRORS_TYPE,(Unpacker)UnpackTxPacketErrors)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(RX_PACKET_ERRORS_TYPE,(Unpacker)UnpackRxPacketErrors)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(TX_OVERFOLWS_TYPE,(Unpacker)UnpackTxOverflows)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(RX_OVERFOLWS_TYPE,(Unpacker)UnpackRxOverflows)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(CHANNEL_RATE_TYPE,(Unpacker)UnpackChannelRate)).second;
        assert(bSuccess);
        bSuccess = UUMap.insert(UUPair(DATA_BEARER_TECH_TYPE,(Unpacker)UnpackDataBearerTech)).second;
        assert(bSuccess);
    }
    return UUMap;
}

// --------------------------------------------------------------------------
// UnpackTxPacketsOk
//
/// Unpack the tx packets ok tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackTxPacketsOk(MsgBuf& msgBuf)
{
    m_txPacketsOkType = TX_PACKETS_OK_TYPE;

    m_txPacketsOkLen = msgBuf.GetWord();
    if (m_txPacketsOkLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected TX Packets OK length is 4 bytes, unpacked length is ")
               << m_txPacketsOkLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_txOkCount = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackRxPacketsOk
//
/// Unpack the rx packets ok tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackRxPacketsOk(MsgBuf& msgBuf)
{
    m_rxPacketsOkType = RX_PACKETS_OK_TYPE;

    m_rxPacketsOkLen = msgBuf.GetWord();
    if (m_rxPacketsOkLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected RX Packets OK length is 4 bytes, unpacked length is ")
               << m_rxPacketsOkLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_rxOkCount = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackTxPacketErrors
//
/// Unpack the tx packet errors tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackTxPacketErrors(MsgBuf& msgBuf)
{
    m_txPacketErrorsType = TX_PACKET_ERRORS_TYPE;

    m_txPacketErrorsLen = msgBuf.GetWord();
    if (m_txPacketErrorsLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected TX Packet Errors length is 4 bytes, unpacked length is ")
               << m_txPacketErrorsLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_txErrCount = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackRxPacketErrors
//
/// Unpack the rx packet errors tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackRxPacketErrors(MsgBuf& msgBuf)
{
    m_rxPacketErrorsType = RX_PACKET_ERRORS_TYPE;

    m_rxPacketErrorsLen = msgBuf.GetWord();
    if (m_rxPacketErrorsLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected RX Packet Errors length is 4 bytes, unpacked length is ")
               << m_rxPacketErrorsLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_rxErrCount = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackTxOverflows
//
/// Unpack the tx overflows tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackTxOverflows(MsgBuf& msgBuf)
{
    m_txOverflowsType = TX_OVERFOLWS_TYPE;

    m_txOverflowsLen = msgBuf.GetWord();
    if (m_txOverflowsLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected TX Overflows length is 4 bytes, unpacked length is ")
               << m_txOverflowsLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_txOflCount = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackRxOverflows
//
/// Unpack the rx overflows tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackRxOverflows(MsgBuf& msgBuf)
{
    m_rxOverflowsType = RX_OVERFOLWS_TYPE;

    m_rxOverflowsLen = msgBuf.GetWord();
    if (m_rxOverflowsLen != 4) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected RX Overflows length is 4 bytes, unpacked length is ")
               << m_rxOverflowsLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_rxOflCount = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackChannelRate
//
/// Unpack the channel rate tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackChannelRate(MsgBuf& msgBuf)
{
    m_channelRateType = CHANNEL_RATE_TYPE;

    m_channelRateLen = msgBuf.GetWord();
    if (m_channelRateLen != 8) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Channel Rate length is 8 bytes, unpacked length is ")
               << m_channelRateLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_currentChannelTxRate = msgBuf.GetDWord();
    m_currentChannelRxRate = msgBuf.GetDWord();

    return true;
}

// --------------------------------------------------------------------------
// UnpackDataBearerTech
//
/// Unpack the data bearer technology tlv for this Message.
///
/// @param msgBuf - the message buffer containg the tlv values.
///
/// @returns bool - true if unpack successful.
// --------------------------------------------------------------------------
bool WDSEventReportInd::UnpackDataBearerTech(MsgBuf& msgBuf)
{
    m_dataBearerTechType = DATA_BEARER_TECH_TYPE;

    m_dataBearerTechLen = msgBuf.GetWord();
    if (m_dataBearerTechLen != 1) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Expected Data Bearer Technology length is 1 bytes, unpacked length is ")
               << m_dataBearerTechLen << _T(" bytes.") << std::endl 
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        return false;
    }

    m_dataBearerTech = msgBuf.GetByte();
    if (m_dataBearerTech < 1 || m_dataBearerTech > 5) 
    {
        std::stringstream stream;
        stream << _T("Warning: unable to unpack message:") << std::endl
               << _T("Valid Data Bearer Technology values are 1 - 5 , unpacked value is ")
               << m_dataBearerTech << _T(" .") << std::endl 
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
void WDSEventReportInd::Print(std::ostream& stream)
{
    stream << "QMI_WDS_EVENT_REPORT_IND" << std::endl
           << _T("{") << std::endl;

    if (m_txPacketsOkType == TX_PACKETS_OK_TYPE)
    {
        stream << _T("  TxOkCount ") << (int)m_txOkCount << std::endl;
    }

    if (m_rxPacketsOkType == RX_PACKETS_OK_TYPE)
    {
        stream << _T("  RxOkCount ") << (int)m_rxOkCount << std::endl;
    }

    if (m_txPacketErrorsType == TX_PACKET_ERRORS_TYPE)
    {
        stream << _T("  TxErrCount ") << (int)m_txErrCount << std::endl;
    }

    if (m_rxPacketErrorsType == RX_PACKET_ERRORS_TYPE)
    {
        stream << _T("  RxErrCount ") << (int)m_rxErrCount << std::endl;
    }

    if (m_txOverflowsType == TX_OVERFOLWS_TYPE)
    {
        stream << _T("  TxOflCount ") << (int)m_txOflCount << std::endl;
    }

    if (m_rxOverflowsType == RX_OVERFOLWS_TYPE)
    {
        stream << _T("  RxOflCount ") << (int)m_txOflCount << std::endl;
    }

    if (m_channelRateType == CHANNEL_RATE_TYPE)
    {
        stream << _T("  CurrentChannelTXRate ") << (int)m_currentChannelTxRate << std::endl
               << _T("  CurrentChannelRXRate ") << (int)m_currentChannelRxRate << std::endl;
    }

    if (m_dataBearerTechType == DATA_BEARER_TECH_TYPE)
    {
        stream << _T("  DataBearerTech ") << (int)m_dataBearerTech << std::endl;
    }

    stream << _T("}") << std::endl << std::endl;
}
