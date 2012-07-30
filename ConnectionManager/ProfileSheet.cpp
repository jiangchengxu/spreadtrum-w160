// --------------------------------------------------------------------------
//
// ProfileSheet.cpp
//
/// ProfileSheet implementation.
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
#include <sstream>
#include <assert.h>


#include "ProfileSheet.h"
#include "ConnectionManagerDlg.h"
#include "ProfileUmtsReqPage.h"
#include "ProfileUmtsMinPage.h"
#include "ProfileGprsReqPage.h"
#include "ProfileGprsMinPage.h"
#include "ProfileSheetPage.h"

BEGIN_MESSAGE_MAP(ProfileSheet, CPropertySheet)
    ON_COMMAND(IDOK,OnOk)
    ON_COMMAND(IDCANCEL,OnCancel)
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// ctor - for create profile
// --------------------------------------------------------------------------
ProfileSheet::ProfileSheet(ConnectionManagerDlg* pCMDlg,uint8 profileType) :
    CPropertySheet(_T("Create Profile")),
    m_pCMDlg(pCMDlg),
    m_profileType(profileType),
    m_profileIndex(-1),
    m_profileSheetType(PST_CREATE)
    
{}

// --------------------------------------------------------------------------
// ctor - for view/modify profile
// --------------------------------------------------------------------------
ProfileSheet::ProfileSheet(
    ConnectionManagerDlg* pCMDlg,
    uint8 profileType,
    uint8 profileIndex
    ) :
    CPropertySheet(_T("View/Modify Profile")),
    m_pCMDlg(pCMDlg),
    m_profileType(profileType),
    m_profileIndex(profileIndex),
    m_profileSheetType(PST_MODIFY)
    
{}

// --------------------------------------------------------------------------
// OnInitDialog
// --------------------------------------------------------------------------
BOOL ProfileSheet::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

    // causes dodataexchange call, linking controls to data members
    int numPages = GetPageCount();
    while (--numPages >= 0)
    {
        SetActivePage(numPages);
    }
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// --------------------------------------------------------------------------
// OnOk
//
/// Actions to take when the  button is pressed.
// --------------------------------------------------------------------------
void ProfileSheet::OnOk()
{
    std::string msgString = BuildCreateModifyProfileString();
    if (!msgString.empty())
    {
        m_pCMDlg->SendRequest(msgString);
    }
    else
    {
        // no message to send, same effect as cancel
        EndDialog(IDCANCEL);
    }

    // when sending a create/modify request, closing the dialog is handled
    // in the create/modify response
}

// --------------------------------------------------------------------------
// OnCancel
//
/// Actions to take when the cancel button is pressed
// --------------------------------------------------------------------------
void ProfileSheet::OnCancel()
{
    std::string msgString = BuildCreateModifyProfileString();
    std::string tlvString = BuildTlvString();

    // prompt to save profile if changes have been made
    if (!tlvString.empty())
    {
        int result = IDNO;
        std::stringstream stream;

        switch (m_profileSheetType)
        {
            case PST_CREATE:
                result = AfxMessageBox((PTCHAR)"Changes have been made to the "
                                   "default profile settings.\n"
                                   "Do you want to create this profile?",
                                   MB_YESNOCANCEL | MB_ICONEXCLAMATION);
                break;

            case PST_MODIFY:
                result = AfxMessageBox((PTCHAR)"Changes have been made to this"
                                   " profile's settings.\n"
                                   "Do you want to modify this profile?",
                                   MB_YESNOCANCEL | MB_ICONEXCLAMATION);
                break;

            default:
                stream << _T("Error: Unknown ProfileSheet type:") 
                       << (int)m_profileSheetType;
                AfxMessageBox((PTCHAR)stream.str().c_str());
        }

        // save changes
        if (result == IDYES)
        {
            m_pCMDlg->SendRequest(msgString);
            EndDialog(IDOK);
        }

        // continue dialog
        if (result == IDCANCEL)
        {
            return;
        }
    }

    // cancel dialog without saving
    EndDialog(IDCANCEL);
}

// --------------------------------------------------------------------------
// BuildCreateModifyProfileString
//
/// Build a string of message type and name/value pairs representing a
/// WDSCreateProfileReq or WDSModifyProfileSettingsReq message.
///
/// @returns std::string - string representation of message.
// --------------------------------------------------------------------------
std::string ProfileSheet::BuildCreateModifyProfileString()
{
    std::stringstream stream;
    std::string tlvString;

    // collect tlv's from profile sheet pages
    tlvString = BuildTlvString();

    switch (m_profileSheetType)
    {
        case PST_CREATE:
            stream << _T("QMI_WDS_CREATE_PROFILE_REQ") << std::endl 
                   << _T("{") << std::endl
                   << _T("  ProfileType ") << (uint32)m_profileType << std::endl;
            break;

        case PST_MODIFY:
            // if no tlv's return empty string (nothing to modify)
            if (tlvString.empty()) { return tlvString; }

            // msg type
            stream << _T("QMI_WDS_MODIFY_PROFILE_SETTINGS_REQ") << std::endl 
                   << _T("{") << std::endl
                   << _T("  ProfileType ") << (uint32)m_profileType << std::endl
                   << _T("  ProfileIndex ") << (uint32)m_profileIndex << std::endl;
            break;

        default:
            stream << _T("Error: Unknown ProfileSheet type:") 
                   << (int)m_profileSheetType;
            AfxMessageBox((PTCHAR)stream.str().c_str());
            return "";
    }

    stream << tlvString << _T("}");
    return stream.str();
}

// --------------------------------------------------------------------------
// BuildTlvString
//
/// Build a string of message type and name/value pairs representing the 
/// tlv's reported by the ProfileSheet pages.
///
/// @returns std::string - string representation of the tlv's.
// --------------------------------------------------------------------------
std::string ProfileSheet::BuildTlvString()
{
    std::stringstream stream;

    // add tlv's from profile sheet pages
    int numPages = GetPageCount();
    for (int i = 0; i < numPages; i++)
    {
		// @@
#if 0
        ProfileSheetPage* pPage = dynamic_cast<ProfileSheetPage*>(GetPage(i));
#else
        ProfileSheetPage* pPage = (ProfileSheetPage *)(GetPage(i));
#endif
        assert(pPage);
        stream << pPage->BuildTlvString();
    }
    
    return stream.str();
}
