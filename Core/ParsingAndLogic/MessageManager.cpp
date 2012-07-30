// --------------------------------------------------------------------------
//
// MessageManager.cpp
//
/// MessageManager implementation.
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
#include <algorithm>
#include <sstream>
#include "MessageManager.h"
#include "QMIService.h"
#include "DeviceMonitor.h"
#include "MsgBuf.h"
#include "Message.h"
#include "MessageFactory.h"
#include "tchar.h"


// --------------------------------------------------------------------------
// ctor
//
/// Constructor for MessageManager. Initialize data members and start the 
/// message manager (MM) thread.
// --------------------------------------------------------------------------
MessageManager::MessageManager() : 
    m_hProcessThread(INVALID_HANDLE_VALUE),
    m_hProcessExitEvent(::CreateEvent(NULL, TRUE, FALSE, NULL)),
    m_hMsgEvent(::CreateEvent(NULL, TRUE, FALSE, NULL)), //??? close in dtor?
    m_isStarted(false)
{
    // initialize api resources
    QCWWAN_InitializeResources();

    // initialize add service critical section
    ::InitializeCriticalSection(&m_addServiceCS);

    // initialize service handles
    for (int i = 0; i < QMUX_TYPE_MAX; i++)
    {
        m_services.push_back(new QMIService(i));
    }
}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for MessageManager.
// --------------------------------------------------------------------------
MessageManager::~MessageManager()
{
    // if the message manager is still in the started state, stop it
    if (m_isStarted)
    {
        StopMessageManager();
    }

    // delete service objects
    for (int i = 0; i < QMUX_TYPE_MAX; i++)
    {
        delete m_services[i];
    }

    // delete add service critical section
    ::DeleteCriticalSection(&m_addServiceCS);

    // release api resources
    QCWWAN_ReleaseResources();
	
	//wyw_0104
	::CloseHandle(m_hProcessExitEvent);
	::CloseHandle(m_hMsgEvent);
}

// --------------------------------------------------------------------------
// GetInstance
//
/// Get the one and only MessageManager.
///
/// @returns MessageManager& - reference to the MessageManager singleton.
// --------------------------------------------------------------------------
MessageManager& MessageManager::GetInstance()
{
    static MessageManager messageManager;
    return messageManager;
}

// --------------------------------------------------------------------------
// StartMessageManager
//
/// Start the received message processing thread and the device monitoring 
/// thread used by the message manager to send messages to and recieve
/// messages from a device.
///
/// @returns bool - true if the MessageManager started successfully.
// --------------------------------------------------------------------------
bool MessageManager::StartMessageManager()
{
    // start the received message process thread
    m_hProcessThread = 
        ::CreateThread(NULL,0,StartProcessThread,NULL,0,NULL);

    // report if error starting the monitor thread
    if (m_hProcessThread == NULL)
    {
        m_hProcessThread = INVALID_HANDLE_VALUE;
        std::stringstream stream;
        stream << _T("Error: Unable to start received message process thread.") << std::endl
               << std::endl;
        ReportStatus(stream.str(),ST_ERROR);
        return false;
    }

    // start monitoring for removable network adapters
    if(!DeviceMonitor::GetInstance().StartDeviceMonitor())
    {
        return false;
    }

    m_isStarted = true;
    return true;
}

// --------------------------------------------------------------------------
// StopMessageManager
//
/// Terminate any active device, stop the device monitoring thread and stop
/// the received message processing thread.
///
/// @returns bool - true if the MessageManager stopped successfully.
// --------------------------------------------------------------------------
bool MessageManager::StopMessageManager()
{
    // close the services
    TerminateDevice();

    // stop the device monitoring thread
    DeviceMonitor::GetInstance().StopDeviceMonitor();

    // stop the received message process thread
    if (m_isStarted)
    {
        // trigger the exit event and wait for the thread to exit
        ::SetEvent(m_hProcessExitEvent);
        ::WaitForSingleObject(m_hProcessThread,INFINITE);

		//wyw_0104
		::CloseHandle(m_hProcessThread);
		m_hProcessThread = INVALID_HANDLE_VALUE;
    }

    return true;
}

// --------------------------------------------------------------------------
// StartProcessThread
//
/// Start the recieved messsage processing thread. This method must be static
/// to work with ::CreateThread. MessageManager is a singleton, so get its 
/// instance to call a non-static method and use non-static data members.
/// 
/// @param context - not used. 
// --------------------------------------------------------------------------
DWORD WINAPI MessageManager::StartProcessThread(LPVOID context)
{
    MessageManager::GetInstance().ProcessThread();
    return 0;
}

// --------------------------------------------------------------------------
// ProcessThread
//
/// The process thread repeatedly takes a buffer from the received queue,
/// creates the appropriate Message and broadcasts the Message to all objects
/// that have regestered to be notified.
// --------------------------------------------------------------------------
void MessageManager::ProcessThread()
{
    HANDLE waitHandles[2] = {m_hProcessExitEvent,m_hMsgEvent};

    // process messages when available until exit is triggered

	
	
	while (::WaitForMultipleObjects(2,waitHandles,FALSE,INFINITE) != WAIT_OBJECT_0)
    {

		// get a message off the queue

		m_rspQueue.lock();

        MsgBuf* pMsgBuf = m_rspQueue.front();
        m_rspQueue.pop();
        m_rspQueue.unlock();

        // build a message reference counted pointer from the message buffer
        MessageRCP msgRCP = MessageFactory::GetInstance().CreateMessage(*pMsgBuf);

        if (!msgRCP.IsNull())
        {
            // notify subscribers message received
            NoticeRCP noticeRCP = THeapObject<MsgNotice>::Create(msgRCP);
            NotifySubscribers(noticeRCP);
        }
        
        // delete the used message buffer
        delete pMsgBuf;

        // if no more messages to process, reset msg event
        TSQCriticalSection<MsgBuf*> localLock(m_rspQueue);
        if (m_rspQueue.empty())
        {
            ::ResetEvent(m_hMsgEvent);
        }
    }
}

// --------------------------------------------------------------------------
// EstablishDevice
//
/// Coordinate with the DeviceMonitor to establish the device to use.
///
/// @param deviceName - string name of the device to be used.
// --------------------------------------------------------------------------
void MessageManager::EstablishDevice(std::string& deviceName)
{
    DeviceMonitor::GetInstance().SetDevice(deviceName);
}

// --------------------------------------------------------------------------
// TerminateDevice
//
/// Coordinate with the DeviceMonitor to terminate a device in use.
// --------------------------------------------------------------------------
void MessageManager::TerminateDevice()
{
    DeviceMonitor::GetInstance().ClearDevice();
}

// --------------------------------------------------------------------------
// CloseServices
//
/// Close any open QMIServices
// --------------------------------------------------------------------------
void MessageManager::CloseServices()
{
    // !!! make a map of uint16 to service, change close iteration
    for (int i = 0; i < QMUX_TYPE_MAX; i++)
    {
        m_services[i]->CloseService();
    }
}

// --------------------------------------------------------------------------
// SendMessage
//
/// Send a message to the target.
///
/// @param msgStr - string representation of the message to be sent.
///
/// @returns bool - true if message sent successfully.
// --------------------------------------------------------------------------
void MessageManager::SendMessage(std::string& msgStr)
{
    // build a message reference counted pointer from the message string
    MessageRCP msgRCP = MessageFactory::GetInstance().CreateMessage(msgStr);
    // return if message is not created
    if (msgRCP.IsNull()) { return; }

    // get the message service type
    int svcType = msgRCP->GetSvcType();

    // verify connection to a device has been established
    if (!DeviceMonitor::GetInstance().IsConnected())
    {
        std::stringstream stream;
        stream << _T("Error: Unable to send message:  ") << std::endl
               << _T("No device connection has been established.")
               << std::endl << std::endl;
        ReportStatus(stream.str(),ST_ERROR);
        return;
    }

    // open service if necessary
    ::EnterCriticalSection(&m_addServiceCS);
    if (!m_services[svcType]->IsOpen())
    {
        if (!m_services[svcType]->OpenService(DeviceMonitor::GetInstance().GetDeviceName()))
        {
            // ??? TerminateDevice();
            ::LeaveCriticalSection(&m_addServiceCS);
            return;
        }
    }
    ::LeaveCriticalSection(&m_addServiceCS);

    // send the message buffer
    if (m_services[svcType]->SendMsgBuf(msgRCP->GetMsgBuf()))
    {
        // notify subscribers message sent
        NoticeRCP noticeRCP = THeapObject<MsgNotice>::Create(msgRCP);
        NotifySubscribers(noticeRCP);
        return;
    }

    // close the service if send fails
    m_services[svcType]->CloseService();
}

// --------------------------------------------------------------------------
// QueueMsgBuf
//
/// Push the provided MsgBuf onto the received message queue.
///
/// @param pMsgBuf - the MsgBuf to be added to the queue.
// --------------------------------------------------------------------------
void MessageManager::QueueMsgBuf(MsgBuf* pMsgBuf)
{
    // lock the queue during this block of code
    TSQCriticalSection<MsgBuf*> localLock(m_rspQueue);

    // push the message buffer on the response queue
    m_rspQueue.push(pMsgBuf);

    // signal that a message has arrived
    ::SetEvent(m_hMsgEvent);
}

// --------------------------------------------------------------------------
// ReportStatus
//
/// Publish reported status to subscribers.
///
/// @param status - string representation of the status to report.
// --------------------------------------------------------------------------
void MessageManager::ReportStatus(std::string& status,uint8 severity)
{
    // publish status
    NoticeRCP noticeRCP = 
        THeapObject<StatusNotice>::Create(status, severity);
    NotifySubscribers(noticeRCP);
}

// --------------------------------------------------------------------------
// ReportDevice
//
/// Respond to and publish reported device events
///
/// @param reason - string description of the device event.
///
/// @param type - uint8 type of device event.
///
/// @param networkAdapters - vector of string representations of network
///     adapters.
// --------------------------------------------------------------------------
void MessageManager::ReportDevice
(
    std::string& reason,
    uint8 type,
    std::vector<std::string>& networkAdapters
)
{
    // publish device notice
    NoticeRCP noticeRCP = 
        THeapObject<DeviceNotice>::Create(reason,type,networkAdapters);
    NotifySubscribers(noticeRCP);

    // if the connection to a device is lost, close all open services
    if (type == DT_CONNECT_FAIL ||type == DT_DISCONNECT || type == DT_DETACH)
    {
        CloseServices();
    }
}
