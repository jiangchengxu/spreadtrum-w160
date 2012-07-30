// --------------------------------------------------------------------------
//
// utils.h
//
/// Interface for utilities functions.
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

#ifndef UTILS_H
#define UTILS_H

#include "api.h"
#include "UndefUnicode.h"

#define QCSER_DbgPrint(x,y,z) printf("%s", z);

// Function prototypes
BOOL WINAPI GetServiceFile
(
   HANDLE hd,
   PCHAR  ServiceFileName,
   UCHAR  ServiceType
);

DWORD WINAPI RegisterNotification(PVOID Context);

VOID USBUTL_PrintBytes(PVOID Buf, ULONG len, ULONG PktLen, char *info);

PCHAR GetCharArray
(
   PCHAR  InBuffer,
   PCHAR  CharArray,
   UCHAR  MaxElements,
   PUCHAR ActualElements
);

#endif // UTILS_H
