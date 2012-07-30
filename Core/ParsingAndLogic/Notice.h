// --------------------------------------------------------------------------
//
// Notice.h
//
/// Notice/MsgNotice/NoticePublisher/NoticeSubscriber interface.
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
#include <vector>
#include "PublishSubscribe.h"
#include "RefCountObject.h"
#include "Message.h"
#include "CommonDefs.h"

// --------------------------------------------------------------------------
// Define notice types
// --------------------------------------------------------------------------
enum 
{
    NT_MESSAGE = 0,
    NT_STATUS,
    NT_DEVICE
};

// --------------------------------------------------------------------------
// Define severity types
// --------------------------------------------------------------------------
enum 
{
    ST_ERROR = 0,
    ST_WARNING,
    ST_INFORMATION
};

// --------------------------------------------------------------------------
// Define device notice types
// --------------------------------------------------------------------------
enum 
{
    DT_AVAILABLE = 0,
    DT_CONNECT,
    DT_CONNECT_FAIL,
    DT_DISCONNECT,
    DT_DETACH
};

// --------------------------------------------------------------------------
// Define reference counting pointer notice types
// --------------------------------------------------------------------------
class Notice;
class MsgNotice;
class StatusNotice;
class DeviceNotice;
typedef TRCPointer<Notice> NoticeRCP;
typedef TRCPointer<MsgNotice> MsgNoticeRCP;
typedef TRCPointer<StatusNotice> StatusNoticeRCP;
typedef TRCPointer<DeviceNotice> DeviceNoticeRCP;


// --------------------------------------------------------------------------
// Notice
//
/// Notice is the base class for all notice types. It defines the basic 
/// interface that all notices must implement. The notice class works with
/// the Publish Subscribe classes to publish notices to subscribers. The 
/// Publish Subscribe classes are templated using the Notice class. 
/// Publishers can send, and Subscribers can receive, any subtype of Notice
/// allowing a single publisher to send different kinds of notices. 
/// Notice inherits from IRCObject which provides a reference counting 
/// interface to the class. Reference counting allows multiple references to 
/// the same heap allocated Notice without worring about orphaning or
/// premature deletion. Reference counting is necessary because a Notice can
/// be published to multiple subscribers.
// --------------------------------------------------------------------------
class Notice : public IRCObject
{
public:
    Notice(uint8 noticeType) : m_noticeType(noticeType)
    {
    }

    uint8 GetNoticeType() const { return m_noticeType; }

private:
    uint8 m_noticeType;
};

// --------------------------------------------------------------------------
// MsgNotice
//
/// A MsgNotice alerts a Subscriber that the Publisher has received a new 
/// Message. The MsgNotice holds a pointer to the new message as a data 
/// member.
// --------------------------------------------------------------------------
class MsgNotice : public Notice
{
public:
    MsgNotice(MessageRCP msgRCP) : Notice(NT_MESSAGE), m_msgRCP(msgRCP)
    {
    }

    MessageRCP GetMessageRCP() const { return m_msgRCP; }

private:
    MessageRCP m_msgRCP;
};

// --------------------------------------------------------------------------
// StatusNotice
//
/// A StatusNotice transmits status information to Subscribers. The event 
/// being reported is represented as a string and the severity of the event
/// (error, warning or information) is represented as a uint8.
// --------------------------------------------------------------------------
class StatusNotice : public Notice
{
public:
    StatusNotice(std::string event, uint8 severity) : 
        Notice(NT_STATUS), 
        m_event(event),
        m_severity(severity)
    {
    }

    std::string GetEvent() { return m_event; }
    uint8 GetSeverity() { return m_severity; }

private:
    std::string m_event;
    uint8 m_severity;
};

// --------------------------------------------------------------------------
// DeviceNotice
//
/// A DeviceNotice relays information about the attachment state of devices.
// --------------------------------------------------------------------------
class DeviceNotice : public Notice
{
public:
    DeviceNotice
    (
        std::string reason,
        uint8 type,
        const std::vector<std::string>& networkAdapters
    ) : 
        Notice(NT_DEVICE),
        m_reason(reason),
        m_type(type),
        m_networkAdapters(networkAdapters)
    {
    }

    std::string GetReason() { return m_reason; }
    uint8 GetType() { return m_type; }
    std::vector<std::string> GetNetworkAdapters() { return m_networkAdapters; }
    
private:
    std::string m_reason;
    uint8 m_type;
    std::vector<std::string> m_networkAdapters;
};

// !!! Any other Notice that needs to be Published should go here.


// --------------------------------------------------------------------------
// NoticePublisher
//
/// Publisher subclass for publishers that send Notices.
// --------------------------------------------------------------------------
class NoticePublisher : public Publisher<NoticeRCP>
{
public:
   NoticePublisher() {}
   virtual ~NoticePublisher() {}
};


// --------------------------------------------------------------------------
// NoticeSubscriber
//
/// Subscriber subclass for publishers that send Notices.
// --------------------------------------------------------------------------
class NoticeSubscriber : public Subscriber<NoticeRCP>
{
public:
   NoticeSubscriber() {}
   virtual ~NoticeSubscriber() {}
};
