// --------------------------------------------------------------------------
//
// reg.h
//
/// Interface for registry functions.
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

#ifndef REG_H
#define REG_H

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "UndefUnicode.h"

#define QCNET_REG_HW_KEY "SYSTEM\\CurrentControlSet\\Enum\\USB"
#define QCNET_REG_SW_KEY "SYSTEM\\CurrentControlSet\\Control\\Class"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

BOOL QueryKey
(
    HKEY  hKey,
    PCHAR DeviceFriendlyName,
    PCHAR ControlFileName,
    PCHAR FullKeyName
);

BOOL FindDeviceInstance
(
    PCHAR InstanceKey,
    PCHAR  DeviceFriendlyName,
    PCHAR  ControlFileName
);

BOOL QCWWAN_GetEntryValue
(
    HKEY  hKey,
    PCHAR DeviceFriendlyName,
    PCHAR EntryName,
    PCHAR ControlFileName
);

BOOL QCWWAN_GetControlFileName
(
    PCHAR DeviceFriendlyName,
    PCHAR ControlFileName
);

BOOL QueryUSBDeviceKeys
(
    PCHAR InstanceKey,
    PCHAR  DeviceFriendlyName,
    PCHAR  ControlFileName
);

#endif  // REG_H
