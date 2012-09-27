// --------------------------------------------------------------------------
//
// CommonDefs.h
//
/// Typedefs for primitive types used throughout WCEditor code
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
#include "UndefUnicode.h"

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef  __int64 int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;

typedef uint8 byte;
typedef uint16 word;
typedef uint32 dword;


#ifdef QWORD_IS_STRUCT
//For clients who prefer to think of qwords
// as a struct of 2 integers
typedef struct tagQword {
    dword lo;
    dword hi;
} qword;

#else

//For clients who prefer to think of qwords
// as a 64-bit integer
typedef uint64 qword;

#endif

typedef qword time_type;
typedef unsigned char boolean;

// --------------------------------------------------------------------------
// DeleteObjectFunctor - a function object used for deleting a container of
// newed up pointers (e.g. via std::for_each).
// --------------------------------------------------------------------------
struct DeleteObjectFunctor {
    template<typename T>
    void operator()(const T* ptr) const {
        delete ptr;
    }
};
