// --------------------------------------------------------------------------
//
// QMIService.h
//
/// QMIService interface.
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



#include <windows.h>
#include <string>
#include "CommonDefs.h"

class MsgBuf;


// !!! you may want to make this a MsgBufPublisher, the MM a MsgBufSubscriber
// !!! and remove the MM::QueueMsgBuf method.
// --------------------------------------------------------------------------
// QMIService
//
/// The QMIService class facilitates sending and receiving message buffers of
/// a QMI service type. The class is responsible for opening and closing WDS,
/// DMS and NAS services with the api, and sending message buffers to and
/// receiving message buffers from the api for its service type.
// --------------------------------------------------------------------------
class QMIService
{
public:
    QMIService(uint8 svcType);
    ~QMIService();

    bool OpenService(std::string& deviceName);
    bool CloseService();
    bool IsOpen() {
        return m_isOpen;
    }
    bool SendMsgBuf(MsgBuf& msgBuf);

private:
    static DWORD WINAPI StartServiceThread(LPVOID context);
    void ServiceThread();
    void ProcessBuffer(UCHAR* rspBuf, ULONG bytesRead);
    void CancelService();

private:
    uint8 m_svcType;
    bool m_isOpen;
    HANDLE m_hServiceHandle;
    HANDLE m_hServiceThread;
    HANDLE m_hReadExitEvent;
};
