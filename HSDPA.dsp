# Microsoft Developer Studio Project File - Name="HSDPA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HSDPA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HSDPA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HSDPA.mak" CFG="HSDPA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HSDPA - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HSDPA - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WMA9501", TYDFAAAA"
# PROP Scc_LocalPath "..\HSDPA-work"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HSDPA - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /I ".\\" /I ".\Core\ParsingAndLogic\\" /I ".\ConnectionManager\\" /I ".\Core\Communication\\" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 rasapi32.lib version.lib winmm.lib setupapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /machine:I386 /out:"Release/FlexiNet.exe" /mapinfo:lines
# SUBTRACT LINK32 /profile /pdb:none /debug

!ELSEIF  "$(CFG)" == "HSDPA - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\\" /I ".\Core\ParsingAndLogic\\" /I ".\ConnectionManager\\" /I ".\Core\Communication\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"StdAfx.h" /FD /GZ /Zm600 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Debug/PCCard.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 rasapi32.lib version.lib winmm.lib setupapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /debug /machine:I386 /out:"Debug/CM200 USB Modem.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HSDPA - Win32 Release"
# Name "HSDPA - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Sms Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\DlgPbImport.cpp
# End Source File
# Begin Source File

SOURCE=.\GetOutlookPBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\msoutl.cpp
# End Source File
# Begin Source File

SOURCE=.\PwdConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PwdModifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadFalshSMS.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsData.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsExportSel.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsExtractNumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsReadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsSendDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsTemplateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsTransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsWriteDlg.cpp
# End Source File
# End Group
# Begin Group "Call Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\CallDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CallHistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HistData.cpp
# End Source File
# Begin Source File

SOURCE=.\HistListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.cpp
# End Source File
# End Group
# Begin Group "PhoneBook Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\ContactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditGPMember.cpp
# End Source File
# Begin Source File

SOURCE=.\mylistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PbData.cpp
# End Source File
# Begin Source File

SOURCE=.\PBGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PBNewGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PbSelPositon.cpp
# End Source File
# Begin Source File

SOURCE=.\PhoneBookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchContactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelContactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelDialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitDlg.cpp
# End Source File
# End Group
# Begin Group "Internet Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\CopsInitTip.cpp
# End Source File
# Begin Source File

SOURCE=.\DataServiceHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInternetNewConn.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetProfileCDMA.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetProfileNewCDMA.cpp
# End Source File
# Begin Source File

SOURCE=.\InternetConnTip.cpp
# End Source File
# Begin Source File

SOURCE=.\InternetUsageLimitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogData.cpp
# End Source File
# Begin Source File

SOURCE=.\LogListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MFNetTraffic.cpp
# End Source File
# Begin Source File

SOURCE=.\mftrafficbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\NetLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NetStatistics.cpp
# End Source File
# End Group
# Begin Group "Setup Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\DiagnosticDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetSettingCDMA.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupNWSel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupNWSelMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupNWSelSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupPowerMng.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyPinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NWListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NWListCtrlSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\PinClckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PinEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupCallDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupData.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupPreferDlg.cpp
# End Source File
# End Group
# Begin Group "Common Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\BaseTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Dev.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSignIn.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubversion.cpp
# End Source File
# Begin Source File

SOURCE=.\Ds.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinWin\EnBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\EnTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExDataExch.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaderCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\HSDPA.cpp
# End Source File
# Begin Source File

SOURCE=.\HSDPA.rc
# End Source File
# Begin Source File

SOURCE=.\HSDPADlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MAPHYPERLINK.CPP
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinWin\MyBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\NumEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PopDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PreMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressExDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResDllMiss.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinWin\SkinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinWin\SkinResource.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinWin\SkinWin.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashScreenEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SkinWin\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TabedDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TransparentStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinWin\UtilsSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\VersionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\xDib.cpp
# End Source File
# Begin Source File

SOURCE=.\XInfoTip.cpp
# End Source File
# End Group
# Begin Group "Serial Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SerialPort\SerialBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort\SerialBufQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort\SerialProt.cpp
# End Source File
# End Group
# Begin Group "NDIS COMMON Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\ConnectionManager\ConnectionManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ConnectionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileGeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileGprsMinPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileGprsReqPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfilesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileUmtsMinPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileUmtsReqPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\StatisticsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\StatusPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\TextProgressCtrl.cpp
# End Source File
# End Group
# Begin Group "NDIS CORE Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DeviceMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceCapMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceMfrMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceModelIdMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceRevIdMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceSerialNumbersMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetMsisdnMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetPowerStateMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSSetEventReportMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\MessageFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\MessageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\MsgBuf.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\NASGetServingSystemMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\NASGetSignalStrengthMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\NASSetEventReportMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\QMIService.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\StringUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSAbortMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSCreateProfileMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSDeleteProfileMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetCurrentChannelRateMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetDefaultSettingsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetPktSrvcStatusMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetProfileListMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetProfileSettingsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSModifyProfileSettingsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSResetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSSetEventReportMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSStartNetworkInterfaceMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSStopNetworkInterfaceMsg.cpp
# End Source File
# End Group
# Begin Group "NDIS API Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Core\Communication\api.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\Communication\reg.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\Communication\utils.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Sms Header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DlgPbImport.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupNWSelMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupNWSelSearch.h
# End Source File
# Begin Source File

SOURCE=.\DlgSMSSetupCDMA.h
# End Source File
# Begin Source File

SOURCE=.\GetOutlookPBDlg.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\MenuButtonST.h
# End Source File
# Begin Source File

SOURCE=.\msoutl.h
# End Source File
# Begin Source File

SOURCE=.\NWListCtrlSearch.h
# End Source File
# Begin Source File

SOURCE=.\PwdConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\PwdModifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReadFalshSMS.h
# End Source File
# Begin Source File

SOURCE=.\SmsData.h
# End Source File
# Begin Source File

SOURCE=.\SMSDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsEdit.h
# End Source File
# Begin Source File

SOURCE=.\SmsExportSel.h
# End Source File
# Begin Source File

SOURCE=.\SmsExtractNumDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SmsReadDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsSendDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsTemplateDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsTransferDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsWriteDlg.h
# End Source File
# Begin Source File

SOURCE=.\UndefUnicode.h
# End Source File
# End Group
# Begin Group "Call Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CallDlg.h
# End Source File
# Begin Source File

SOURCE=.\CallHistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistData.h
# End Source File
# Begin Source File

SOURCE=.\HistListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.h
# End Source File
# End Group
# Begin Group "PhoneBook Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ContactDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditGPMember.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetProfileCDMA.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetProfileNewCDMA.h
# End Source File
# Begin Source File

SOURCE=.\mylistctrl.h
# End Source File
# Begin Source File

SOURCE=.\PbData.h
# End Source File
# Begin Source File

SOURCE=.\PBGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\PBNewGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\PbSelPositon.h
# End Source File
# Begin Source File

SOURCE=.\PhoneBookDlg.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\SearchContactDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelContactDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelDialDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelPosition.h
# End Source File
# Begin Source File

SOURCE=.\TransferDlg.h
# End Source File
# Begin Source File

SOURCE=.\WaitDlg.h
# End Source File
# End Group
# Begin Group "Internet Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CopsInitTip.h
# End Source File
# Begin Source File

SOURCE=.\DataServiceHistory.h
# End Source File
# Begin Source File

SOURCE=.\dlgconnect.h
# End Source File
# Begin Source File

SOURCE=.\DlgInternetNewConn.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetProfile.h
# End Source File
# Begin Source File

SOURCE=.\InternetConnTip.h
# End Source File
# Begin Source File

SOURCE=.\InternetUsageLimitDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogData.h
# End Source File
# Begin Source File

SOURCE=.\LogListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MFNetTraffic.h
# End Source File
# Begin Source File

SOURCE=.\mftrafficbutton.h
# End Source File
# Begin Source File

SOURCE=.\NetLog.h
# End Source File
# Begin Source File

SOURCE=.\NetStatistics.h
# End Source File
# End Group
# Begin Group "Setup Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DiagnosticDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetSettingCDMA.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupGeneral.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupNWSel.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupPowerMng.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupSkin.h
# End Source File
# Begin Source File

SOURCE=.\ModifyPinDlg.h
# End Source File
# Begin Source File

SOURCE=.\NWListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PinClckDlg.h
# End Source File
# Begin Source File

SOURCE=.\PinEx.h
# End Source File
# Begin Source File

SOURCE=.\SetupCallDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetupData.h
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetupPreferDlg.h
# End Source File
# End Group
# Begin Group "Common Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\BaseDialog.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\BaseTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CommonStructs.h
# End Source File
# Begin Source File

SOURCE=.\ConnProfile.h
# End Source File
# Begin Source File

SOURCE=.\Constants.h
# End Source File
# Begin Source File

SOURCE=.\Dev.h
# End Source File
# Begin Source File

SOURCE=.\DlgSignIn.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubversion.h
# End Source File
# Begin Source File

SOURCE=.\Ds.h
# End Source File
# Begin Source File

SOURCE=.\SkinWin\EnBitmap.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrl\EnTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ExDataExch.h
# End Source File
# Begin Source File

SOURCE=.\FeatureCustom.h
# End Source File
# Begin Source File

SOURCE=.\HeaderCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\HSDPA.h
# End Source File
# Begin Source File

SOURCE=.\HSDPADlg.h
# End Source File
# Begin Source File

SOURCE=.\MAPHYPERLINK.H
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=.\MsgThread.h
# End Source File
# Begin Source File

SOURCE=.\SkinWin\MyBitmap.h
# End Source File
# Begin Source File

SOURCE=.\NumEdit.h
# End Source File
# Begin Source File

SOURCE=.\PopDlg.h
# End Source File
# Begin Source File

SOURCE=.\PreMsg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressExDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResDllMiss.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenStatic.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.h
# End Source File
# Begin Source File

SOURCE=.\SkinWin\SkinButton.h
# End Source File
# Begin Source File

SOURCE=.\SkinWin\SkinResource.h
# End Source File
# Begin Source File

SOURCE=.\SkinWin\SkinWin.h
# End Source File
# Begin Source File

SOURCE=.\SplashScreenEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SkinWin\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\TabDialog.h
# End Source File
# Begin Source File

SOURCE=.\TabedDialog.h
# End Source File
# Begin Source File

SOURCE=.\TransparentStatic.h
# End Source File
# Begin Source File

SOURCE=.\UserMsgIdDef.h
# End Source File
# Begin Source File

SOURCE=.\VersionDlg.h
# End Source File
# Begin Source File

SOURCE=.\xDib.h
# End Source File
# Begin Source File

SOURCE=.\XInfoTip.h
# End Source File
# End Group
# Begin Group "Serial Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SerialPort\SerialBuffer.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort\SerialBufQueue.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort\SerialPort.h
# End Source File
# End Group
# Begin Group "NDIS COMMON Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConnectionManager\ConnectionManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ConnectionPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileGeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileGprsMinPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileGprsReqPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileSheet.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileSheetPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfilesPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileUmtsMinPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\ProfileUmtsReqPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\resource.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\StatisticsPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\StatusPage.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionManager\TextProgressCtrl.h
# End Source File
# End Group
# Begin Group "NDIS CORE Header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\CommonDefs.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DeviceMonitor.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceCapMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceMfrMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceModelIdMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceRevIdMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetDeviceSerialNumbersMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetMsisdnMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSGetPowerStateMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\DMSSetEventReportMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\Message.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\MessageFactory.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\MessageManager.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\MsgBuf.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\NASGetServingSystemMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\NASGetSignalStrengthMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\NASSetEventReportMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\Notice.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\PublishSubscribe.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\QMIService.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\RefCountObject.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\StringUtils.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\TSQueue.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSAbortMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSCreateProfileMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSDeleteProfileMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetCurrentChannelRateMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetDefaultSettingsMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetPktSrvcStatusMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetProfileListMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSGetProfileSettingsMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSModifyProfileSettingsMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSResetMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSSetEventReportMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSStartNetworkInterfaceMsg.h
# End Source File
# Begin Source File

SOURCE=.\Core\ParsingAndLogic\WDSStopNetworkInterfaceMsg.h
# End Source File
# End Group
# Begin Group "NDIS API Header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Core\Communication\api.h
# End Source File
# Begin Source File

SOURCE=.\Core\Communication\reg.h
# End Source File
# Begin Source File

SOURCE=.\Core\Communication\utils.h
# End Source File
# End Group
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxDlg.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\bcgcontrolbar.hm
# End Source File
# Begin Source File

SOURCE=.\hlp\HSDPA.cnt

!IF  "$(CFG)" == "HSDPA - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\HSDPA.cnt
InputName=HSDPA

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "HSDPA - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\HSDPA.cnt
InputName=HSDPA

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\MAINFRAME.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\HM11_ndis\ConnectionManager\ConnectionManager.dsw
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
