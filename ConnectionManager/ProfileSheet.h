// --------------------------------------------------------------------------
//
// ProfileSheet.h
//
/// ProfileSheet interface.
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

#include "CommonDefs.h"


class ConnectionManagerDlg;


// --------------------------------------------------------------------------
// ProfilesSheet
//
/// The ProfilesSheet class is a tabbed control that contains the settings
/// of a selected profile. This control provides view/modify/add profile
/// functionality.
// --------------------------------------------------------------------------

class ProfileSheet : public CPropertySheet
{
public:
    ProfileSheet(ConnectionManagerDlg* pCMDlg,uint8 profileType);

    ProfileSheet(
        ConnectionManagerDlg* pCMDlg,
        uint8 profileType,
        uint8 profileIndex);

protected:
    // ProfileSheet types
    enum {
        PST_CREATE,
        PST_MODIFY
    };

    virtual BOOL OnInitDialog();
    afx_msg void OnOk();
    afx_msg void OnCancel();
    virtual std::string BuildCreateModifyProfileString();
    virtual std::string BuildTlvString();

    DECLARE_MESSAGE_MAP()

private:
    ConnectionManagerDlg* m_pCMDlg;
    uint8 m_profileSheetType;
    uint8 m_profileType;
    uint8 m_profileIndex;
};