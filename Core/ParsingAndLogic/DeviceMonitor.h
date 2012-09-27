// --------------------------------------------------------------------------
//
// DeviceMonitor.h
//
/// DeviceMonitor interface.
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
#include <vector>
#include "CommonDefs.h"


// --------------------------------------------------------------------------
// DeviceMonitor
//
/// The DeviceMonitor class keeps track of removable network adapters
/// attached to the computer and reports changes to the MessageManager. There
/// should only ever be one instance of this class monitoring network
/// adapters, so it is implemented as a singleton.
// --------------------------------------------------------------------------
class DeviceMonitor
{
public:
    static DeviceMonitor& GetInstance();
    ~DeviceMonitor();

    bool StartDeviceMonitor();
    bool StopDeviceMonitor();
    void SetDevice(std::string& deviceName);
    void ClearDevice();
    bool IsConnected() {
        return m_isConnected;
    }
    std::string GetDeviceName() {
        return m_deviceName;
    }

private:
    DeviceMonitor();
    static DWORD WINAPI StartMonitorThread(LPVOID context);
    void MonitorThread();
    void InspectNetAdapters();
    static DWORD WINAPI StartDetachThread(LPVOID context);
    void DetachThread();
    void ProcessDeviceEvent(DWORD event,std::stringstream& stream,uint8& type);
    void CancelDevice();

private:
    std::vector<std::string> m_netAdapters;
    HANDLE m_hMonitorThread;
    HANDLE m_hExitEvent;
    bool m_isStarted;
    std::string m_deviceName;
    HANDLE m_hDetachThread;
    HANDLE m_hDetachHandle;
    HANDLE m_hDetachExitEvent;
    HANDLE m_hIsConnectedMutex;
    bool m_isConnected;
};
