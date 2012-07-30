// --------------------------------------------------------------------------
//
// DeviceMonitor.cpp
//
/// DeviceMonitor implementation.
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
#include <sstream>
#include <atlbase.h>
#include "DeviceMonitor.h"
#include "MessageManager.h"
#include "tchar.h"
#include "SetupAPI.h"
#include <devguid.h>

// CRM does not use the current PlatformSDK thus the following #define.
//#include <cfgmgr32.h>
#define CM_DEVCAP_REMOVABLE (0x00000084)

const std::wstring REG_KEY_STRING = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM");

// --------------------------------------------------------------------------
// GetInstance
//
/// Get the one and only DeviceMonitor.
///
/// @returns DeviceMonitor& - reference to the DeviceMonitor singleton.
// --------------------------------------------------------------------------
DeviceMonitor& DeviceMonitor::GetInstance()
{
    static DeviceMonitor DeviceMonitor;
    return DeviceMonitor;
}

// --------------------------------------------------------------------------
// ctor
//
/// Constructor for MessageManager. Initialize data members and start the 
/// message manager (MM) thread.
// --------------------------------------------------------------------------
DeviceMonitor::DeviceMonitor() : 
    m_hMonitorThread(INVALID_HANDLE_VALUE),
    m_hExitEvent(::CreateEvent(NULL, TRUE, FALSE, NULL)),
    m_isStarted(false),
    m_hDetachThread(INVALID_HANDLE_VALUE),
    m_hDetachHandle(INVALID_HANDLE_VALUE),
    m_hDetachExitEvent(::CreateEvent(NULL, TRUE, FALSE, NULL)),
    m_hIsConnectedMutex(::CreateMutex(NULL,FALSE,NULL)),
    m_isConnected(false)
{}

// --------------------------------------------------------------------------
// dtor
//
/// Destructor for DeviceMonitor.
// --------------------------------------------------------------------------
DeviceMonitor::~DeviceMonitor()
{
    if (m_isStarted)
    {
        StopDeviceMonitor();
    }

    ::CloseHandle(m_hMonitorThread); // ???
    ::CloseHandle(m_hExitEvent);

	//wyw_0104
	::CloseHandle(m_hDetachExitEvent);

    // close the connection flag mutex
    ::CloseHandle(m_hIsConnectedMutex);
}

// --------------------------------------------------------------------------
// StartDeviceMonitor
//
/// Start monitoring for changes in removable network adapters.
///
/// @returns bool - true if monitor started successfully.
// --------------------------------------------------------------------------
bool DeviceMonitor::StartDeviceMonitor()
{
    // start the monitor thread
    m_hMonitorThread = ::CreateThread(NULL,0,StartMonitorThread,NULL,0,NULL);

    // report if error starting the monitor thread
    if (m_hMonitorThread == NULL)
    {
        m_hMonitorThread = INVALID_HANDLE_VALUE;

        std::stringstream stream;
        stream << _T("Error: Unable to start device monitor thread.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        return false;
    }

    // indicate that the monitor has started
    m_isStarted = true;

    return true;
}

// --------------------------------------------------------------------------
// StopDeviceMonitor
//
/// Stop monitoring for changes in removable network adapters.
///
/// @returns bool - true if monitor stopped successfully.
// --------------------------------------------------------------------------
bool DeviceMonitor::StopDeviceMonitor()
{
    // stop the monitor thread
    if(m_isStarted)
    {
        // trigger the exit event and wait for the thread to exit
        ::SetEvent(m_hExitEvent);
        ::WaitForSingleObject(m_hMonitorThread,INFINITE);
    }

    // indicate that the monitor has stopped
    m_isStarted = false;
    return true;
}

// --------------------------------------------------------------------------
// StartMonitorThread
//
/// Start the device monitor thread. This method must be static to work with
/// ::CreateThread. DeviceMonitor is a singleton, so get its instance to call
/// a non-static method and use non-static data members.
/// 
/// @param context - not used. 
// --------------------------------------------------------------------------
DWORD WINAPI DeviceMonitor::StartMonitorThread(LPVOID context)
{
    DeviceMonitor::GetInstance().MonitorThread();
    return 0;
}

// --------------------------------------------------------------------------
// MonitorThread
//
/// The monitor thread keeps track of removable network adapters attached to 
/// computer and reports changes to the MessageManager.
// --------------------------------------------------------------------------
void DeviceMonitor::MonitorThread()
{
    // open the reg key to watch for changes

    CRegKey regKey;
    if (ERROR_SUCCESS != 
        regKey.Open(HKEY_LOCAL_MACHINE,REG_KEY_STRING.c_str(),KEY_READ))
    {
        std::stringstream stream;
        stream << _T("Error: MonitorThread:") << std::endl
               << _T("Unable to open registry key.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        m_isStarted = false;
        return;
    }

    // create the event flag to be used in the watch
    HANDLE hRegChangedEvent = CreateEvent(NULL, FALSE, FALSE, NULL) ;
    if (INVALID_HANDLE_VALUE == hRegChangedEvent)
    {
        std::stringstream stream;
        stream << _T("Error: MonitorThread:") << std::endl
               << _T("Unable to create event flag.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
        m_isStarted = false;

        return;
    }



    // establish array of handles to wait on
    HANDLE waitHandles [] = { m_hExitEvent,hRegChangedEvent }; 

    // loop until stop is requested
    do
    {
        // attaching/detaching a phone causes changes in the serialcomm key
        // and triggers a chack of removable network adapters on the system.
        // it would be better to be notified when there is a change to the 
        // GUID_DEVCLASS_NET but including files for IORegisterPlugPlayNotify
        // causes namespace conflicts with other Windows include files.
        // request to be notified when the reg key/sub key changes
        if (ERROR_SUCCESS != 
            RegNotifyChangeKeyValue(
                (HKEY)regKey,
                TRUE,
                REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET,
                hRegChangedEvent,
                TRUE))
        {
            std::stringstream stream;
            stream << _T("Error: MonitorThread:") << std::endl
                   << _T("Unable to establish registry key notification.") << std::endl
                   << std::endl;
            MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
            m_isStarted = false;
            return;
        }

        // check for and report changes in removable network adapters
        InspectNetAdapters();        
    }
    while (::WaitForMultipleObjects(2,waitHandles,FALSE,INFINITE) != WAIT_OBJECT_0);

    // cleanup
    ::CloseHandle(hRegChangedEvent);
    regKey.Close();
}

// --------------------------------------------------------------------------
// InspectNetAdapters
//
/// Inspect the system for removable network adapters. If available devices
/// have changed, report to MessageManager.
// --------------------------------------------------------------------------
const GUID GUID_DEVCLASS_NET_EX = {0x4D36E972L, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};
void DeviceMonitor::InspectNetAdapters()
{
    // local collection of network adapters
    std::vector<std::string> netAdapters;

    // get the set of network adapters on the system
#if 0
    HDEVINFO hDevInfo = 
        SetupDiGetClassDevs(&GUID_DEVCLASS_NET,NULL,NULL,DIGCF_PRESENT);
		//SetupDiGetClassDevs(&GUID_DEVCLASS_NET,NULL,NULL,DIGCF_ALLCLASSES);
#else
	// @@
	GUID guid = GUID_DEVCLASS_NET_EX;
    HDEVINFO hDevInfo = 
        SetupDiGetClassDevs(&guid,NULL,NULL,DIGCF_PRESENT);
#endif

    // Enumerate through all devices in the set
    SP_DEVINFO_DATA DeviceInfoData;
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//DeviceInfoSet Supplies a handle to the device information set. 

   //MemberIndex Supplies the 0-based index of the device information member to retrieve. 

//DeviceInfoData Supplies a pointer to an SP_DEVINFO_DATA structure to receive information about this member. 
 //GL Start

          



    for (DWORD i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)
    {    
	     DWORD regDataType = 0;

		DWORD DataT = 0;
		
		CHAR hwi[256] = {0};

        DWORD MAX__DESC = 256;

		DWORD hwisize = 0; 

		char* p = NULL;

		//std::vector<std::string> strHardwareId;
	    
		
        //strHardwareId = "USB\Vid_05c6&Pid_7001&MI_01";	

        SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
            SPDRP_HARDWAREID,
            &DataT,
            (PBYTE)hwi,
            MAX__DESC,
            &hwisize);               
        for (p=hwi;*p&&(p<&hwi[hwisize]);p+=strlen(p)+sizeof(char))
        {	
			//if(!_stricmp(p, "USB\Vid_05c6&Pid_7001&Rev_0000&Mi_01"))     

			if(!_stricmp(p, "USB\\Vid_1614&Pid_7002&Mi_01"))    
			
			//if(!_stricmp(p, strHardwareId))   

            {					   
			            CHAR buf[256] = {0};
            DWORD bufSize = 256;

            // get the device friendly name
            SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DeviceInfoData,
                SPDRP_FRIENDLYNAME,
                &regDataType,
                (PBYTE)buf,
                bufSize,
                &bufSize);

            // if no device friendly name, get the device description
            if (buf[0] == '\0')
            {
                // get the device friendly name
                SetupDiGetDeviceRegistryProperty(
                    hDevInfo,
                    &DeviceInfoData,
                    SPDRP_DEVICEDESC,
                    &regDataType,
                    (PBYTE)buf,
                    bufSize,
                    &bufSize);
            }

            // add to available network adapters
            netAdapters.push_back(buf);										
            }
        }		

	}
	
//end	

	
	/*for (DWORD i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)

 
 {
        DWORD regDataType = 0;

        DWORD cap = 0;
		 
        DWORD capSize = sizeof(cap);

        // get the device capabilities
       buffer(
            hDevInfo,
            &DeviceInfoData,
            SPDRP_CAPABILITIES,
            &regDataType,
            (PBYTE)&cap,
            capSize,
            &capSize);
// ADD function by WY
	//	if ( i == 5 ) {
	//		 cap = 4;}
       //   cap = 4; 
        // we are only interested in removable devices
        if (cap & CM_DEVCAP_REMOVABLE)
        {
            CHAR buf[256] = {0};
            DWORD bufSize = 256;

            // get the device friendly name
            SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DeviceInfoData,
                SPDRP_FRIENDLYNAME,
                &regDataType,
                (PBYTE)buf,
                bufSize,
                &bufSize);

            // if no device friendly name, get the device description
            if (buf[0] == '\0')
            {
                // get the device friendly name
                SetupDiGetDeviceRegistryProperty(
                    hDevInfo,
                    &DeviceInfoData,
                    SPDRP_DEVICEDESC,
                    &regDataType,
                    (PBYTE)buf,
                    bufSize,
                    &bufSize);
            }

            // add to available network adapters
            netAdapters.push_back(buf);
        }
    }
*/
    // report to MessageManager if collection of network adapters has changed
    if (m_netAdapters != netAdapters)
    {
        m_netAdapters = netAdapters;

        std::string reason = "Attached Network Adapters.";
        MessageManager::GetInstance().ReportDevice(reason,DT_AVAILABLE,m_netAdapters);
    }

    //  Cleanup
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

// --------------------------------------------------------------------------
// SetDevice
//
/// Set validate the device, start the detach notification thread and set the
/// device name.
///
/// @param deviceName - string name of the device to be used.
// --------------------------------------------------------------------------
void DeviceMonitor::SetDevice(std::string& deviceName)
{
    // check connection status
    if (!m_deviceName.empty() && m_isConnected)
    {
        // check if device name is already set to submitted name
        if (m_deviceName.compare(deviceName) == 0) 
        {
            return;
        }

        // fail if already connected to another device
        std::stringstream stream;
        stream << _T("Unable to establish a connection with '") << deviceName
               << _T("'.") << std::endl << _T("A connection is currently established with, '")
               << m_deviceName << _T("'.") << std::endl << std::endl;
        MessageManager::GetInstance().ReportDevice(stream.str(),DT_CONNECT_FAIL, m_netAdapters);
        return;  
    }

    // verify the device name by attempting to open a service
    m_hDetachHandle = QCWWAN_OpenService((PCHAR)deviceName.c_str(),QMUX_TYPE_WDS);
    if (m_hDetachHandle == INVALID_HANDLE_VALUE)
    {
        // fail if unable to open service on device
        std::stringstream stream;
        stream << _T("Unable to establish a connection to the device, '") << deviceName
               << _T("'.") << std::endl
               << _T("Make sure the device name is correct and the device is attched properly.") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportDevice(stream.str(),DT_CONNECT_FAIL, m_netAdapters);
        return;
    }

    // get the connected flag mutex
    ::WaitForSingleObject(m_hIsConnectedMutex,INFINITE);

    // start the device detach notification thread
    m_hDetachThread = ::CreateThread(NULL,0,StartDetachThread,NULL,0,NULL);

    // report if error starting the detach notification thread
    if (m_hDetachThread == NULL)
    {
        m_hDetachThread = INVALID_HANDLE_VALUE;
        std::stringstream stream;
        stream << _T("Unable to start the device detach notification thread.")
                << std::endl << std::endl;
        MessageManager::GetInstance().ReportDevice(stream.str(),DT_CONNECT_FAIL, m_netAdapters);

        // release the connected flag mutex
        ::ReleaseMutex(m_hIsConnectedMutex);
        return;
    }

    // set the device name and connected flag
    m_deviceName = deviceName;
    m_isConnected = true;

    // report successful connection
    std::stringstream stream;
    stream << _T("Connection established with ") << deviceName
           << _T(".") << std::endl << std::endl;
    MessageManager::GetInstance().ReportDevice(stream.str(),DT_CONNECT, m_netAdapters);

    // release the connected flag mutex
    ::ReleaseMutex(m_hIsConnectedMutex);
}

// --------------------------------------------------------------------------
// ClearDevice
//
/// Clear the connected device name, close the detach notification handle and
/// set the connected flag to false.
// --------------------------------------------------------------------------
void DeviceMonitor::ClearDevice()
{
    // exit the detach notification thread if active
    if (m_hDetachThread != INVALID_HANDLE_VALUE)
    {
        ::SetEvent(m_hDetachExitEvent);
        ::WaitForSingleObject(m_hDetachThread,INFINITE);
        ::ResetEvent(m_hDetachExitEvent);
    }
}

// --------------------------------------------------------------------------
// StartDetachThread
//
/// Start the device detach notification thread. This method must be static
/// to work with ::CreateThread. MessageManager is a singleton, so get its 
/// instance to call a non-static method and use non-static data members.
/// 
/// @param context - not used. 
// --------------------------------------------------------------------------
DWORD WINAPI DeviceMonitor::StartDetachThread(LPVOID context)
{
    DeviceMonitor::GetInstance().DetachThread();
    return 0;
}

// --------------------------------------------------------------------------
// DetachThread
//
/// The detach thread registers with the device driver to be notified if the
/// device is detached.
// --------------------------------------------------------------------------
void DeviceMonitor::DetachThread()
{
    // set up the device io control parameters
    DWORD event = 0;
    DWORD bytesReturned = 0;
    OVERLAPPED ov;
    std::stringstream stream;

    ::memset(&ov,0,sizeof(ov));
    ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
 
    // report create event failure  
    if (ov.hEvent == NULL)
    {
        stream << _T("Unable to create overlapped event for detach notification.") << std::endl
               << _T("Disconnecting from device, ") << m_deviceName << _T(".")
               << std::endl << std::endl;
        MessageManager::GetInstance().ReportDevice(stream.str(),DT_DISCONNECT,m_netAdapters);
        CancelDevice();
        return;
    }

    // request detach notification
    BOOL result = DeviceIoControl
                  (
                      m_hDetachHandle,
                      IOCTL_QCDEV_WAIT_NOTIFY,
                      NULL,
                      0,
                      &event,
                      sizeof(event),
                      &bytesReturned,
                      &ov
                  );

    // preset the default type of exit
    uint8 type = DT_CONNECT_FAIL;

    // check for immediate response or unexpected error
    if (result == TRUE)
    {
        ProcessDeviceEvent(event,stream,type);
    }
    else
    {
        int status = ::GetLastError();
        if (ERROR_IO_PENDING != status)
        {
            // report device IO control error
            stream << _T("Unexpected error in detach notification.") << std::endl
                   << _T("Last error code: ") << status << std::endl
                   << _T("Disconnecting from device, ") << m_deviceName << _T(".")
                   << std::endl << std::endl;
        }
        else
        {
            // establish array of handles to wait on
            HANDLE waitHandles [] = { m_hDetachExitEvent,ov.hEvent };

            if (::WaitForMultipleObjects(2,waitHandles,FALSE,INFINITE) == WAIT_OBJECT_0)
            {
                // exit event signaled, cancel any pending IO
                if (!::CancelIo(m_hDetachHandle))
                {
                    // report cancel IO failure
                    status = ::GetLastError();
                    stream << _T("Error cancelling IO on detach notification exit.") << std::endl
                           << _T("Last error code: ") << status << std::endl
                           << _T("Disconnecting from device, ") << m_deviceName << _T(".")
                           << std::endl << std::endl;
                }

                // successful termination on exit event
                stream << _T("Successful disconnection from ") << m_deviceName
                       << _T(".") << std::endl << std::endl;
                type = DT_DISCONNECT;
            }
            else
            {
                // overlapped event signaled, get result
                if (GetOverlappedResult(m_hDetachHandle,&ov,&bytesReturned,TRUE))
                {
                    ProcessDeviceEvent(event,stream,type);
                }
                else
                {
                    // report get overlapped error
                    status = ::GetLastError();
                    stream << _T("Error: Unexpected error in get overlapped of detach notification.") << std::endl
                           << _T("Last error code: ") << status << std::endl
                           << _T("Disconnecting from device, ") << m_deviceName << _T(".")
                           << std::endl << std::endl;
                }
            }
        }
    }

    // notify subscribers of disconnect
    MessageManager::GetInstance().ReportDevice(stream.str(),type,m_netAdapters);

    // disconnect from the device
    CancelDevice();

    // cleanup overlap handle
    ::CloseHandle(ov.hEvent);

    // !!! refresh net adapters collection
    m_netAdapters.clear();
    InspectNetAdapters();
}

// --------------------------------------------------------------------------
// ProcessDeviceEvent
//
/// Respond to the event code returned by a call to DeviceIOControl.
///
/// @param event - the device event to be processed.
///
/// @param stream - the stream to insert the response into.
///
/// @param type - a reference to the device notice type that should be
///     reported.
// --------------------------------------------------------------------------
void DeviceMonitor::ProcessDeviceEvent(DWORD event,std::stringstream& stream, uint8& type)
{
    if (event & 0x00000001)
    { 
        // detach notification
        stream << _T("The device, ") << m_deviceName
               << _T(", has been detached.") << std::endl
               << _T("Closing service handles...") << std::endl
               << std::endl;

        // remove the detached device from the collection of network adapters
        std::vector<std::string>::iterator iter = 
            std::find(m_netAdapters.begin(),m_netAdapters.end(),m_deviceName);

        if (iter != m_netAdapters.end())
        {
            m_netAdapters.erase(iter);
        }

        // set type
        type = DT_DETACH;
    }
    else
    {
        // device notification, but not a detach notice
        stream << _T("Unknown device notification received: Mask:  ") 
               << std::showbase << std::hex << event << std::endl
               << _T("Disconnecting from device, ") << m_deviceName << _T(".")
               << std::endl << std::endl;

        // set type
        type = DT_CONNECT_FAIL;
    }
}

// --------------------------------------------------------------------------
// CancelDevice
//
/// Clear the device name, close the device notification handle and set the
/// connection flag to false.
// --------------------------------------------------------------------------
void DeviceMonitor::CancelDevice()
{
    // check if service already closed
    if (!m_isConnected) { return; }

    std::stringstream stream;

    // get the connected flag mutex
    ::WaitForSingleObject(m_hIsConnectedMutex,INFINITE);

    // clear the device name
	// @@
    //m_deviceName.clear();
	m_deviceName.erase(m_deviceName.begin(), m_deviceName.end());

    // close the detach notification handle
    if (!QCWWAN_CloseService(m_hDetachHandle))
    {
        // report error closing handle
        stream << _T("Error: Detach notification handle did not close successfully:") << std::endl
               << std::endl;
        MessageManager::GetInstance().ReportStatus(stream.str(),ST_ERROR);
    }

    m_hDetachHandle = INVALID_HANDLE_VALUE;

    // set the connected flag
    m_isConnected = false;

    // release the connected flag mutex
    ::ReleaseMutex(m_hIsConnectedMutex);
}
