#ifndef AT_CMD_DEV_H
#define AT_CMD_DEV_H

#include "winioctl.h"
#include "CommonStructs.h"
#include "SetupData.h"
#include "initguid.h"
#include <setupapi.h>

#define MAX__DESC            256

const GUID CLASS_GUID_PORT = {0x4D36E978L, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};
const GUID CLASS_GUID_CDROM = {0x4D36E965L, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};
const GUID CLASS_GUID_MODEM = {0x4D36E96DL, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
const GUID CLASS_GUID_DISK = {0x4D36E967L, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};

#define HARDWARE_ID_9501 _T("vid_1614&pid_6000")
#define HARDWARE_ID_9508 _T("vid_1614&pid_0800")

//设备的PID_VID标识
extern TCHAR gszHardwareID[MAX__DESC];

//USB Modem设备DeviceDesc,不含x#标识
extern TCHAR gszModemDeviceDesc[RAS_MaxDeviceName];

//USB Modem设备FriendlyName,可以含x#标识
extern TCHAR gszModemFriendlyName[RAS_MaxDeviceName];

//数据连接前查找USB Modem设备是否存在
extern BOOL FindUSBModem();
//added by wk end on 2007-1-23

//启动时查找PCUI口并获得其串口号
extern BOOL GetPCUIPortName(TCHAR* szDevName);

extern BOOL GetPCVOICEName(TCHAR* szDevName_voice);

//在9508 USB Modem项目中启动时查找CdRom是否存在
extern BOOL FindCdRom();

extern HANDLE g_hExitEvent;
extern DWORD WINAPI MonitorThread(LPVOID lpParameter);
extern BOOL g_bIsExist;
extern BOOL g_bSimReady;

#endif //#define AT_CMD_DEV_H
