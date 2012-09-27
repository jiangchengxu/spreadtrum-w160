// --------------------------------------------------------------------------
//
// QMIService.cpp
//
/// QMIService implementation.
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

#include "api.h"

#include <sstream>
#include "QMIService.h"
#include "MessageManager.h"
#include "MsgBuf.h"
#include "tchar.h"

static uint32 THREAD_EXIT_TIMEOUT = 1000;


// --------------------------------------------------------------------------
// ctor
//
/// Constructor for QMIService. Initialize data members.
// --------------------------------------------------------------------------
QMIService::QMIService(uint8 svcType) :
    m_svcType(svcType),
    m_isOpen(false),
    m_hServiceHandle(INVALID_HANDLE_VALUE),
    m_hServiceThread(INVALID_HANDLE_VALUE),
    m_hReadExitEvent(::CreateEvent(NULL,FALSE,FALSE,NULL))
{
}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for QMIService.
// --------------------------------------------------------------------------
QMIService::~QMIService()
{
    if (m_isOpen) {
        CloseService();
    }

    ::CloseHandle(m_hReadExitEvent);
}

// --------------------------------------------------------------------------
// OpenService
//
/// Get the service handle from the api and begin the service thread that
/// listens for messages to be received.
///
/// @params deviceName - name of the device on which to open services.
///
/// @returns bool - true if service opened successfully
// --------------------------------------------------------------------------
bool QMIService::OpenService(std::string& deviceName)
{
    // create service handle
    m_hServiceHandle =
        QCWWAN_OpenService((PCHAR)deviceName.c_str(),m_svcType);

    if (m_hServiceHandle == INVALID_HANDLE_VALUE) {
        // report unable to open service
        std::stringstream stream;
        stream << _T("Error: Unable to open service:") << std::endl
               << _T("  Device Name: ") << deviceName << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[m_svcType]
               << std::endl << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        return false;
    }

    // start the service thread
    m_hServiceThread =
        ::CreateThread(NULL,0,StartServiceThread,(LPVOID)this,0,NULL);

    if (m_hServiceThread == NULL) {
        QCWWAN_CloseService(m_hServiceHandle);
        m_hServiceHandle = INVALID_HANDLE_VALUE;

        // report unable to start service thread
        std::stringstream stream;
        stream << _T("Error: Unable to start service thread:") << std::endl
               << _T("  Device Name: ") << deviceName << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[m_svcType] << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        return false;
    }

    // indicate that the service is open
    m_isOpen = true;

    return true;
}

// --------------------------------------------------------------------------
// CloseService
//
/// Stop the service thread, close the service handle and mark the service as
/// closed.
///
/// @returns bool - true if service closed successfully
// --------------------------------------------------------------------------
bool QMIService::CloseService()
{
    // check if service already closed
    if (!m_isOpen) {
        return true;
    }

    bool result = true;
    std::stringstream stream;

    // trigger the thread exit event
    ::SetEvent(m_hReadExitEvent);

    // wait for thread to exit
    DWORD exitVal = ::WaitForSingleObject(m_hServiceThread,THREAD_EXIT_TIMEOUT);

    // check for abnormal exit
    if (exitVal != 0) {
        // report abnormal exit
        stream << _T("Error: Abnormal exit from service thread:") << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[m_svcType] << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);
        result = false;
    }

    m_hServiceThread = INVALID_HANDLE_VALUE;

    // close the service handle
    if (!QCWWAN_CloseService(m_hServiceHandle)) {
        // report error closing service handle
        stream << _T("Error: Service did not close successfully:") << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[m_svcType] << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        result = false;
    }

    m_hServiceHandle = INVALID_HANDLE_VALUE;

    // indicate that the service is closed
    m_isOpen = false;

    return result;
}

// --------------------------------------------------------------------------
// SendMsgBuf
//
/// Send a message buffer to the target.
///
/// @param msgBuf - the message buffer to be sent.
///
/// @returns bool - true if message buffer sent successfully.
// --------------------------------------------------------------------------
bool QMIService::SendMsgBuf(MsgBuf& msgBuf)
{
    bool result = false;
    uint32 bytesWritten = 0;

    // send the packed buffer
    result = !!QCWWAN_SendRaw(
                 m_hServiceHandle,
                 (PCHAR)msgBuf.GetBuffer(),
                 (ULONG)msgBuf.GetSize(),
                 (PULONG)&bytesWritten
             );

    if (false == result) {
        TRACE(_T("error #1 !\n"));
        if (false == !!QCWWAN_SendRaw(m_hServiceHandle, (PCHAR)msgBuf.GetBuffer(), (ULONG)msgBuf.GetSize(), (PULONG)&bytesWritten)) {
            TRACE(_T("error #2 !\n"));
            result = !!QCWWAN_SendRaw(m_hServiceHandle, (PCHAR)msgBuf.GetBuffer(), (ULONG)msgBuf.GetSize(), (PULONG)&bytesWritten);
        }
    }

    // check for error sending buffer
    if (!result) {
        TRACE(_T("error #3 !\n"));
        std::stringstream stream;
        stream << _T("Error: Message buffer was not sent successfully:") << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[m_svcType] << std::endl;
        msgBuf.Print(stream);
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
    }

    return result;
}

// --------------------------------------------------------------------------
// StartServiceThread
//
/// Start a service thread. This method must be static to work with
/// ::CreateThread. By passing the QMIService pointer we can call a
/// non-static method and use non-static data members.
///
/// @param context - pointer to the MessageManager.
// --------------------------------------------------------------------------
DWORD WINAPI QMIService::StartServiceThread(LPVOID context)
{
    ((QMIService*)context)->ServiceThread();
    return 0;
}

// --------------------------------------------------------------------------
// ServiceThread
//
/// Service threads "listen" to an overlapped I/O file for messages being
/// sent from the target. Each service type has its own overlapped file and
/// thread.
// --------------------------------------------------------------------------
void QMIService::ServiceThread()
{
    UCHAR rspBuf[QMUX_MAX_DATA_LEN] = {0};
    ULONG bytesRead = 0;
    OVERLAPPED ov;
    std::stringstream stream;

    ::memset(&ov,0,sizeof(ov));
    ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // report create event failure
    if (ov.hEvent == NULL) {
        stream << _T("Unable to create overlapped event for service file.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        CancelService();
        return;
    }

    // do asyncronous file read until exit triggered
    while (true) {
        while (::ReadFile(m_hServiceHandle,rspBuf,QMUX_MAX_DATA_LEN,&bytesRead,&ov)) {
            // process the raw buffer from the service file read
            ProcessBuffer(rspBuf,bytesRead);

            // reset the buffer and number of bytes returned
            ::memset(rspBuf,0,QMUX_MAX_DATA_LEN);
            bytesRead = 0;
        }

        uint32 status = GetLastError();
        if (ERROR_IO_PENDING != status) {
            stream << _T("Service file read failure.") << std::endl
                   << _T("Last error code: ") << status << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);

            // cleanup
            ::CloseHandle(ov.hEvent);
            CancelService();
            return;
        } else {
            // establish array of handles to wait on
            HANDLE waitHandles [] = { m_hReadExitEvent,ov.hEvent };

            if (::WaitForMultipleObjects(2,waitHandles,FALSE,INFINITE) == WAIT_OBJECT_0) {
                // exit event signaled, cancel any pending IO
                if (!::CancelIo(m_hServiceHandle)) {
                    // report cancel IO failure
                    status = ::GetLastError();
                    stream << _T("Error cancelling IO on service read exit.") << std::endl
                           << _T("Last error code: ") << status << std::endl
                           << std::endl;
                    MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
                }

                // cleanup (no need to cancel service, already closing service)
                ::CloseHandle(ov.hEvent);

                return;
            } else {
                // overlapped event signaled, get result
                if (GetOverlappedResult(m_hServiceHandle,&ov,&bytesRead,TRUE)) {
                    // process the raw buffer from the service file read
                    ProcessBuffer(rspBuf,bytesRead);

                    // reset the buffer and number of bytes returned
                    ::memset(rspBuf,0,QMUX_MAX_DATA_LEN);
                    bytesRead = 0;
                } else {
                    // report get overlapped error
                    status = ::GetLastError();
                    stream << _T("Error: Unexpected error in get overlapped of service read.") << std::endl
                           << _T("Last error code: ") << status << std::endl
                           << std::endl;
                    MessageManager::GetInstance().ReportStatus(stream.str(),ST_WARNING);

                    // cleanup
                    ::CloseHandle(ov.hEvent);
                    CancelService();
                    return;
                }
            }
        }
    }
}

// --------------------------------------------------------------------------
// ProcessBuffer
//
/// Convert a raw buffer from a service file read to a MsgBuf and put it on
/// the MessageManager's message response queue.
///
/// @param rspBuf - the response buffer to process.
// --------------------------------------------------------------------------
void QMIService::ProcessBuffer(UCHAR* rspBuf, ULONG bytesRead)
{
    // create a MsgBuf with the message
    MsgBuf* pMsgBuf = new MsgBuf(rspBuf,bytesRead);

    // indicate the service type
    pMsgBuf->SetSvcType(m_svcType);

    // push the target response buffer on the response queue
    MessageManager::GetInstance().QueueMsgBuf(pMsgBuf);
}

// --------------------------------------------------------------------------
// CancelService
//
/// Close the service handle and mark the service as closed.
// --------------------------------------------------------------------------
void QMIService::CancelService()
{
    // check if service already closed
    if (!m_isOpen) {
        return;
    }

    std::stringstream stream;

    // close the service handle
    if (!QCWWAN_CloseService(m_hServiceHandle)) {
        // report error closing service handle
        stream << _T("Error: Service did not close successfully:") << std::endl
               << _T("  Service Type: ") << SERVICE_TYPE_STRINGS[m_svcType] << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
    }

    m_hServiceHandle = INVALID_HANDLE_VALUE;

    // indicate that the service is closed
    m_isOpen = false;
}
