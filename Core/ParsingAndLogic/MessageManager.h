// --------------------------------------------------------------------------
//
// MessageManager.h
//
/// MessageManager interface.
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
#include <windows.h>
#include "Notice.h"
#include "TSQueue.h"
#include "CommonDefs.h"

class QMIService;
class Message;
class MsgBuf;

static const std::string SERVICE_TYPE_STRINGS[] = { "CTL","WDS","DMS","NAS" };

// --------------------------------------------------------------------------
// MessageManager
//
/// The MessageManager forms the core of the message parsing and logic layer.
/// The MessageManager is responsible for obtaining service handles from the
/// connection layer and managing them. The MessageManager receives messages
/// from the communication layer and publishes that message to its subscriber
/// list. The MessageManager provides an interface for sending messages to
/// the communication layer, loading messages from file and saving messages
/// to file. The MessageManager follows the singleton and publish/subscribe
/// patterns.
// --------------------------------------------------------------------------
class MessageManager : public NoticePublisher
{
public:
    static MessageManager& GetInstance();
    ~MessageManager();

    bool StartMessageManager();
    bool StopMessageManager();
    void EstablishDevice(std::string& deviceName);
    void TerminateDevice();
    void SendMessage(std::string& msgStr);
    void QueueMsgBuf(MsgBuf* pMsgBuf);
    void ReportStatus(std::string& status,uint8 severity);
    void ReportDevice
    (
        std::string& reason,
        uint8 type,
        std::vector<std::string>& networkAdapters
    );

    bool LoadFromFile(std::string filePath);
    bool SaveToFile(std::string filePath); // !!! need vector of msgs reference

private:
    MessageManager();
    static DWORD WINAPI StartProcessThread(LPVOID context);
    void ProcessThread();
    void CloseServices();

private:
    std::vector<QMIService*> m_services;
    TSQueue<MsgBuf*> m_rspQueue;
    HANDLE m_hProcessThread;
    HANDLE m_hProcessExitEvent;
    HANDLE m_hMsgEvent;
    CRITICAL_SECTION m_addServiceCS;
    bool m_isStarted;
};
