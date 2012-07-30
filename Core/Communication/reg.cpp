// --------------------------------------------------------------------------
//
// reg.c
//
/// reg implementation.
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
#include "reg.h"

BOOL QueryKey
(
   HKEY  hKey,
   PCHAR DeviceFriendlyName,
   PCHAR ControlFileName,
   PCHAR FullKeyName
)
{ 
    CHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    CHAR    achClass[MAX_PATH] = "";  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
    char     fullKeyName[MAX_KEY_LENGTH];
 
    DWORD i, retCode; 
 
    DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKeyA
              (
                 hKey,                    // key handle 
                 achClass,                // buffer for class name 
                 &cchClassName,           // size of class string 
                 NULL,                    // reserved 
                 &cSubKeys,               // number of subkeys 
                 &cbMaxSubKey,            // longest subkey size 
                 &cchMaxClass,            // longest class string 
                 &cValues,                // number of values for this key 
                 &cchMaxValue,            // longest value name 
                 &cbMaxValueData,         // longest value data 
                 &cbSecurityDescriptor,   // security descriptor 
                 &ftLastWriteTime         // last write time 
              );
 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    
    if (cSubKeys)
    {
        for (i=0; i<cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyExA
                      (
                         hKey, i,
                         achKey, 
                         &cbName, 
                         NULL, 
                         NULL, 
                         NULL, 
                         &ftLastWriteTime
                      ); 
            if (retCode == ERROR_SUCCESS) 
            {
                BOOL result;

                // _tprintf(TEXT("C-(%d) %s\n"), i+1, achKey);
                sprintf(fullKeyName, "%s\\%s", FullKeyName, achKey);
                result = FindDeviceInstance(fullKeyName, DeviceFriendlyName, ControlFileName);
                if (result == TRUE)
                {
                   // printf("QueryKey: TRUE\n");
                   return TRUE;
                }
            }
        }
    }

    // printf("QueryKey: FALSE\n");

    return FALSE;
}  // QueryKey

BOOL FindDeviceInstance
(
   PCHAR InstanceKey,
   PCHAR  DeviceFriendlyName,
   PCHAR  ControlFileName
)
{
   HKEY hTestKey;
   BOOL ret = FALSE;

   if (RegOpenKeyExA
       (
          HKEY_LOCAL_MACHINE,
          InstanceKey,
          0,
          KEY_READ,
          &hTestKey
       ) == ERROR_SUCCESS
      )
   {
      ret = QCWWAN_GetEntryValue
            (
               hTestKey,
               DeviceFriendlyName,
               "QCDeviceControlFile",
               ControlFileName
            );

      RegCloseKey(hTestKey);
   }

   return ret;
}

BOOL QCWWAN_GetEntryValue
(
   HKEY  hKey,
   PCHAR DeviceFriendlyName,
   PCHAR EntryName,         // "QCDeviceControlFile"
   PCHAR ControlFileName
)
{
    DWORD retCode = ERROR_SUCCESS;
    unsigned char achValue[MAX_VALUE_NAME];
    CHAR achSwKey[MAX_VALUE_NAME];
    DWORD cchValue = MAX_VALUE_NAME;
    BOOL  instanceFound = FALSE;
    HKEY  hSwKey;

    achValue[0] = 0;

    // first get device friendly name
    retCode = RegQueryValueExA
              (
                 hKey,
                 "FriendlyName", // "DriverDesc", // value name
                 NULL,         // reserved
                 NULL,         // returned type
                 achValue,
                 &cchValue
              );

    if (retCode == ERROR_SUCCESS )
    {
       achValue[cchValue] = 0;

		if (strcmp((char *)achValue, DeviceFriendlyName) == 0)
       {
          instanceFound = TRUE;
       }
       // printf("FriendlyName <%s>\n", achValue);
    }
    else
    {
       // no FriendlyName, get DeviceDesc
       achValue[0] = 0;
       cchValue = MAX_VALUE_NAME;
       retCode = RegQueryValueExA
                 (
                    hKey,
                    "DeviceDesc", // value name
                    NULL,         // reserved
                    NULL,         // returned type
                    achValue,
                    &cchValue
                 );
       if (retCode == ERROR_SUCCESS )
       {
          achValue[cchValue] = 0;

// @@

// 		CString strTemp =(char *) achValue;
// 		if ((strTemp.Find(DeviceFriendlyName))+1)
		  if (strcmp((char *)achValue, DeviceFriendlyName) == 0)
          {
             instanceFound = TRUE;
          }

          // printf("DeviceDesc <%s>\n", achValue);

       }
    }


    if (instanceFound == TRUE)
    {
       // Get "Driver" instance path
       achValue[0] = 0;
       cchValue = MAX_VALUE_NAME;
       retCode = RegQueryValueExA
                 (
                    hKey,
                    "Driver",     // value name
                    NULL,         // reserved
                    NULL,         // returned type
                    achValue,
                    &cchValue
                 );
       if (retCode == ERROR_SUCCESS )
       {
          // Construct device software key
          achValue[cchValue] = 0;
          sprintf(achSwKey, "%s\\%s", QCNET_REG_SW_KEY, achValue);

          // printf("Got SW Key <%s>\n", achSwKey);

          // Open device software key
          retCode = RegOpenKeyExA
                    (
                       HKEY_LOCAL_MACHINE,
                       achSwKey,
                       0,
                       KEY_READ,
                       &hSwKey
                    );


          if (retCode == ERROR_SUCCESS)
          {
             // Retrieve the control file name
             achValue[0] = 0;
             cchValue = MAX_VALUE_NAME;
             retCode = RegQueryValueExA
                       (
                          hSwKey,
                          EntryName,    // value name
                          NULL,         // reserved
                          NULL,         // returned type
                          achValue,
                          &cchValue
                       );

             if (retCode == ERROR_SUCCESS )
             {
                PCHAR p = (PCHAR)achValue + cchValue;

                achValue[cchValue] = 0;

				// @@

                while ((p > ((char *)achValue)) && (*p != '\\'))
                {
                   p--;
                }
                if (*p == '\\') p++;
                strcpy(ControlFileName, p);
                // _tprintf(TEXT("QCDeviceControlFile: %s\n"), achValue);
                return TRUE;
             }

             RegCloseKey(hSwKey);
          }  // if (retCode == ERROR_SUCCESS)
       }  // if (retCode == ERROR_SUCCESS)
       else
       {
          printf("Error: cannot get device software key, retCode %u\n", retCode);
       }
    }  // if (instanceFound == TRUE)

    return FALSE;
}

BOOL QCWWAN_GetControlFileName
(
   PCHAR DeviceFriendlyName,
   PCHAR ControlFileName
)
{
   HKEY hTestKey;


   if (RegOpenKeyExA
       (
          HKEY_LOCAL_MACHINE,
          QCNET_REG_HW_KEY,
          0,
          KEY_READ,
          &hTestKey
       ) == ERROR_SUCCESS
      )
   {
       CHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
       DWORD    cbName;                   // size of name string
       CHAR    achClass[MAX_PATH] = "";  // buffer for class name
       DWORD    cchClassName = MAX_PATH;  // size of class string
       DWORD    cSubKeys=0;               // number of subkeys
       DWORD    cbMaxSubKey;              // longest subkey size
       DWORD    cchMaxClass;              // longest class string
       DWORD    cValues;              // number of values for key
       DWORD    cchMaxValue;          // longest value name
       DWORD    cbMaxValueData;       // longest value data
       DWORD    cbSecurityDescriptor; // size of security descriptor
       FILETIME ftLastWriteTime;      // last write time
       DWORD    i, retCode;
       DWORD    cchValue = MAX_VALUE_NAME;

       // Get the class name and the value count.
       retCode = RegQueryInfoKeyA
                 (
                    hTestKey,                    // key handle
                    achClass,                // buffer for class name
                    &cchClassName,           // size of class string
                    NULL,                    // reserved
                    &cSubKeys,               // number of subkeys
                    &cbMaxSubKey,            // longest subkey size
                    &cchMaxClass,            // longest class string
                    &cValues,                // number of values for this key
                    &cchMaxValue,            // longest value name
                    &cbMaxValueData,         // longest value data
                    &cbSecurityDescriptor,   // security descriptor
                    &ftLastWriteTime         // last write time
                 );

       // Enumerate the subkeys, until RegEnumKeyEx fails.
       if (cSubKeys)
       {
           for (i=0; i<cSubKeys; i++)
           {
               cbName = MAX_KEY_LENGTH;
               retCode = RegEnumKeyExA
                         (
                            hTestKey, i,
                            achKey,
                            &cbName,
                            NULL,
                            NULL,
                            NULL,
                            &ftLastWriteTime
                                  );
               if (retCode == ERROR_SUCCESS)
               {
                   BOOL result;

                   // _tprintf(TEXT("A-(%d) %s\n"), i+1, achKey);
                   result = QueryUSBDeviceKeys(achKey, DeviceFriendlyName, ControlFileName);
                   if (result == TRUE)
                   {
                      // printf("QueryKey: TRUE\n");
                      return TRUE;
                   }
               }
           }
       }

      RegCloseKey(hTestKey);

      return retCode;
   }

   return FALSE;
}

BOOL QueryUSBDeviceKeys
(
   PCHAR InstanceKey,
   PCHAR  DeviceFriendlyName,
   PCHAR  ControlFileName
)
{
   HKEY hTestKey;
   char fullKeyName[MAX_KEY_LENGTH];
   BOOL ret = FALSE;

   sprintf(fullKeyName, "%s\\%s", QCNET_REG_HW_KEY, InstanceKey);

   // printf("B-full key name: [%s]\n", fullKeyName);

   if (RegOpenKeyExA
       (
          HKEY_LOCAL_MACHINE,
          fullKeyName,
          0,
          KEY_READ,
          &hTestKey
       ) == ERROR_SUCCESS
      )
   {
      ret = QueryKey(hTestKey, DeviceFriendlyName, ControlFileName, fullKeyName);
      RegCloseKey(hTestKey);

      return ret;
   }

   return FALSE;
}  // QueryUSBDeviceKeys
