// --------------------------------------------------------------------------
//
// StringUtils.h
//
/// StringUtils interface.
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

#include <string>

//namespace StringUtils
//{
    // remove space, newline and tab chars from the left of a string
    std::string& TrimLeft(std::string& str);

    // remove space, newline and tab chars from the right of a string
    std::string& TrimRight(std::string& str);

    // remove space, newline and tab chars from the left and right of a string
    std::string& Trim(std::string& str);
//}