// --------------------------------------------------------------------------
//
// StringUtils.cpp
//
/// StringUtils implementation.
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
#include "StringUtils.h"

// remove space, newline and tab chars from the left of a string
std::string& TrimLeft(std::string& str)
{
    return str.erase(0,str.find_first_not_of(" \t\r\n\b\v"));
}

// remove space, newline and tab chars from the right of a string
std::string& TrimRight(std::string& str)
{
    return str.erase(str.find_last_not_of(" \t\r\n\b\v")+1);
}

// remove space, newline and tab chars from the left and right of a string
std::string& Trim(std::string& str)
{
    return TrimLeft(TrimRight(str));
}