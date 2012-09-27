// --------------------------------------------------------------------------
//
// ProfileSheetPage.h
//
/// ProfileSheetPage interface.
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


// --------------------------------------------------------------------------
// ProfileSheetPage
//
/// The ProfileSheetPage class represents the kind of CPropertyPage that can
/// be added as tabs in a ProfileSheet. This is an abstract base class that
/// provides an interface for all pages that reside in a ProfileSheet.
// --------------------------------------------------------------------------

class ProfileSheetPage : public CPropertyPage
{
public:
    ProfileSheetPage(UINT nIDTemplate) : CPropertyPage(nIDTemplate) {}
    virtual ~ProfileSheetPage() {}
    virtual std::string BuildTlvString() = 0;
};
