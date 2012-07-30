#include "stdafx.h"
#include "Dev.h"
#include "Ds.h"
#include <atlbase.h>
#include "HSDPADlg.h"

#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")


HANDLE g_hExitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
static const TCHAR REG_KEY_STRING[] = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM");
BOOL g_bIsExist = FALSE;
BOOL g_bSimReady= FALSE;

DWORD WINAPI MonitorThread(LPVOID lpParameter)
{
	CHSDPADlg *pMainDlg = (CHSDPADlg*)lpParameter;
    // open the reg key to watch for changes
	CRegKey regKey;
    if (ERROR_SUCCESS != regKey.Open(HKEY_LOCAL_MACHINE, REG_KEY_STRING, KEY_READ))
    {
        return 0;
    }
	
    // create the event flag to be used in the watch
    HANDLE hRegChangedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (INVALID_HANDLE_VALUE == hRegChangedEvent)
    {
        return 0;
    }
	
	TCHAR szDevName[50] = _T("");
    // establish array of handles to wait on
    HANDLE waitHandles [] = {g_hExitEvent, hRegChangedEvent }; 
	
    do
    {
        if (ERROR_SUCCESS != RegNotifyChangeKeyValue((HKEY)regKey, TRUE,
			REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET, hRegChangedEvent, TRUE))
        {
            return 0;
        }
		
		if(GetPCUIPortName(szDevName))
		{
			Sleep(1000);
			if (!g_bIsExist)
			{
				pMainDlg->PostMessage(WM_INIT_SMSANDPB, 1, 0);
			}
			g_bIsExist = TRUE;
		}
		else
		{
			pMainDlg->PostMessage(WM_INIT_SMSANDPB, 0, 0);
			g_bIsExist = FALSE;
		}
		
    } while (::WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE) != WAIT_OBJECT_0);
	
    ::CloseHandle(hRegChangedEvent);
    regKey.Close();
	
	return 0;
}

//启动时查找PCUI口并获得其串口号
BOOL GetPCUIPortName(TCHAR* szDevName)
{ 
    int index = 0;
    BOOL match = FALSE;
    DWORD DataType, length;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA devInfoElem;
	TCHAR szDevDetail[SPDRP_MAXIMUM_PROPERTY][MAX__DESC];

    hDevInfo = SetupDiGetClassDevs((LPGUID)&CLASS_GUID_PORT, NULL, NULL, DIGCF_PRESENT);  

    if(INVALID_HANDLE_VALUE != hDevInfo)
    {
        devInfoElem.cbSize = sizeof(SP_DEVINFO_DATA);      
        while(SetupDiEnumDeviceInfo(hDevInfo, index++, &devInfoElem)) 
        {
            memset(szDevDetail, 0x00, sizeof(szDevDetail));

            SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &devInfoElem,
                SPDRP_DEVICEDESC,
                &DataType,
                (unsigned char *)szDevDetail[SPDRP_DEVICEDESC],
                MAX__DESC,
                (LPDWORD)&length);
            
            strrrc(szDevDetail[SPDRP_DEVICEDESC], ' ');
            
            SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &devInfoElem,
                SPDRP_HARDWAREID,
                &DataType,
                (unsigned char *)szDevDetail[SPDRP_HARDWAREID],
                MAX__DESC,
                (LPDWORD)&length);
            
    		wcslwr(szDevDetail[SPDRP_HARDWAREID]);
			wcslwr(gszHardwareID);

            if(!wcscmp(g_SetData.Internet_szPCUIName, szDevDetail[SPDRP_DEVICEDESC])
                && ( (wcsstr(szDevDetail[SPDRP_HARDWAREID], g_SetData.Main_szPID)) || (wcsstr(szDevDetail[SPDRP_HARDWAREID], _T("1001"))) ))	//wyw_0421 modify

            {                 
                HKEY hKeyDev = SetupDiOpenDevRegKey(hDevInfo, &devInfoElem, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ); 
                if(INVALID_HANDLE_VALUE != hKeyDev)
                {
                    WORD length = sizeof(szDevDetail[SPDRP_DEVICEDESC]); 
                    if(ERROR_SUCCESS == RegQueryValueEx(hKeyDev, _T("PortName"), NULL, NULL,
                        (unsigned char *)szDevDetail[SPDRP_DEVICEDESC], (LPDWORD)&length))
                    {
                        RegCloseKey(hKeyDev);
                        wcscpy(szDevName,_T("\\\\.\\"));
                        wcscat(szDevName,szDevDetail[SPDRP_DEVICEDESC]);       
                    }
                }
                match = TRUE;
                break;
            }
        } //while
        SetupDiDestroyDeviceInfoList(hDevInfo);
    }

    return match;
}

//在USB Modem项目中启动时查找CdRom是否存在
BOOL FindCdRom()
{
    int index = 0;
    BOOL match = FALSE;
    DWORD DataType,length;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA devInfoElem;
    char szFriendlyName[MAX__DESC];

    hDevInfo = SetupDiGetClassDevs((LPGUID)&CLASS_GUID_CDROM, NULL, NULL, DIGCF_PRESENT);

    if(INVALID_HANDLE_VALUE != hDevInfo)
    {
        devInfoElem.cbSize = sizeof(SP_DEVINFO_DATA);    
        while(SetupDiEnumDeviceInfo(hDevInfo, index++, &devInfoElem))
        {
            memset(szFriendlyName, 0x00, sizeof(szFriendlyName));

            SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &devInfoElem,
                SPDRP_FRIENDLYNAME,
                &DataType,
                (unsigned char *)szFriendlyName,
                MAX__DESC,
                (LPDWORD)&length);

            strrrcA(szFriendlyName, ' ');
            
            if(!strcmpi((char*)g_SetData.Main_szUsbDiskFriendlyName, (char* )szFriendlyName))
			{
				match = TRUE;
                break;
			}
        } //while
        SetupDiDestroyDeviceInfoList(hDevInfo);
	}
    
	return match;
}

//设备的PID_VID标识
TCHAR gszHardwareID[MAX__DESC];

//USB Modem设备DeviceDesc,不含x#标识
TCHAR gszModemDeviceDesc[RAS_MaxDeviceName];

//USB Modem设备FriendlyName,可以含x#标识
TCHAR gszModemFriendlyName[RAS_MaxDeviceName];

//数据连接前查找USB Modem设备是否存在
BOOL FindUSBModem()
{
	DWORD i;
    BOOL match = FALSE;
    DWORD DataType, length;
	HDEVINFO hDevInfo;   
	SP_DEVINFO_DATA DevInfoData;
	TCHAR szDevDetail[SPDRP_MAXIMUM_PROPERTY][MAX__DESC];

    hDevInfo = SetupDiGetClassDevs((LPGUID)&CLASS_GUID_MODEM, 0, 0, DIGCF_PRESENT);

    if(hDevInfo == INVALID_HANDLE_VALUE)
        return FALSE;

    memset(gszModemDeviceDesc, 0x00, RAS_MaxDeviceName);
    memset(gszModemFriendlyName, 0x00, RAS_MaxDeviceName);
    
    DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for(i=0; SetupDiEnumDeviceInfo(hDevInfo,i,&DevInfoData); i++)
    {
        memset(szDevDetail, 0x00, sizeof(szDevDetail));

		SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DevInfoData,
			SPDRP_DEVICEDESC,
			&DataType,
			(unsigned char *)szDevDetail[SPDRP_DEVICEDESC],
			MAX__DESC,
			(LPDWORD)&length);

        wcsncpy(gszModemDeviceDesc, szDevDetail[SPDRP_DEVICEDESC], RAS_MaxDeviceName);
        
//        strrrc(szDevDetail[SPDRP_DEVICEDESC], ' ');

		SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DevInfoData,
			SPDRP_HARDWAREID,
			&DataType,
			(unsigned char *)szDevDetail[SPDRP_HARDWAREID],
			MAX__DESC,
			(LPDWORD)&length);

		wcslwr(szDevDetail[SPDRP_HARDWAREID]);
        
        if(!wcscmp(szDevDetail[SPDRP_DEVICEDESC], g_SetData.Internet_szDeviceName)
				&& ( (wcsstr(szDevDetail[SPDRP_HARDWAREID], g_SetData.Main_szPID)) || (wcsstr(szDevDetail[SPDRP_HARDWAREID], _T("1001"))) ))	//wyw_0421 modify

        {            
            SetupDiGetDeviceRegistryProperty(
                hDevInfo,
                &DevInfoData,
                SPDRP_FRIENDLYNAME,
                &DataType,
                (unsigned char *)szDevDetail[SPDRP_FRIENDLYNAME],
                MAX__DESC,
                (LPDWORD)&length);

            wcsncpy(gszModemFriendlyName, szDevDetail[SPDRP_FRIENDLYNAME], RAS_MaxDeviceName);
            match = TRUE;
            break;
        }
    } //for

    SetupDiDestroyDeviceInfoList(hDevInfo);

    if(!match)
    {
        memset(gszModemDeviceDesc, 0x00, RAS_MaxDeviceName);
        memset(gszModemFriendlyName, 0x00, RAS_MaxDeviceName);
    }

    return match;
}

