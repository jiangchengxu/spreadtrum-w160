# Microsoft Developer Studio Project File - Name="DataCrad_Arabic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DataCrad_Arabic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DataCrad_Arabic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DataCrad_Arabic.mak" CFG="DataCrad_Arabic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DataCrad_Arabic - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DataCrad_Arabic - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WMA9501(UI)", SOTXAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DataCrad_Arabic - Win32 Release"

# PROP BASE Use_MFC 0
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DataCrad_Arabic_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /D "_MBCS" /D "_USRDLL" /D "DataCrad_Arabic_EXPORTS" /D "_WINDLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x401 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /out:"../lang/DataCrad_Arabic.dll"
# SUBTRACT LINK32 /profile /debug

!ELSEIF  "$(CFG)" == "DataCrad_Arabic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DataCrad_Arabic_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DataCrad_Arabic_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x401 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../lang/DataCrad_Arabic.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DataCrad_Arabic - Win32 Release"
# Name "DataCrad_Arabic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DataCrad_Arabic.cpp
# End Source File
# Begin Source File

SOURCE=.\DataCrad_Arabic.def
# End Source File
# Begin Source File

SOURCE=.\DataCrad_Arabic.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DataCrad_Arabic.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\amazonia.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\amazonia.bmp
# End Source File
# Begin Source File

SOURCE=..\res\archive.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\back.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\back.bmp
# End Source File
# Begin Source File

SOURCE=..\res\back1.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\back_default.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\back_default.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\bitmap1.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\bk_con.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\bk_con.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\bk_con_default.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\bk_con_default.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bk_ground_bot.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\bk_page01.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\bk_page01.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\bk_screen.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\bk_screen.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\bmp00001.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=..\res\browser.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\browser001.ico"
# End Source File
# Begin Source File

SOURCE=..\res\browser001.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\browser01.ico"
# End Source File
# Begin Source File

SOURCE=..\res\browser01.ico
# End Source File
# Begin Source File

SOURCE=..\res\browser03.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\browser1.ico"
# End Source File
# Begin Source File

SOURCE=..\res\browser1.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\browser2.ico"
# End Source File
# Begin Source File

SOURCE=..\res\browser2.ico
# End Source File
# Begin Source File

SOURCE=..\res\browser3.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\btn1.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\btn1.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\btn1_hot.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\btn1_hot.bmp
# End Source File
# Begin Source File

SOURCE=..\res\call.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\call001.ico"
# End Source File
# Begin Source File

SOURCE=..\res\call001.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\call01.ico"
# End Source File
# Begin Source File

SOURCE=..\res\call01.ico
# End Source File
# Begin Source File

SOURCE=..\res\call03.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum00.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum000.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum002.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum003.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum004.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum005.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum006.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum007.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum008.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum009.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum02.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum03.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum04.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum05.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum06.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum07.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum08.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnum09.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumclea01.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumclear001.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumcontacts001.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumcontacts01.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumdial001.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumdial01.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumend001.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumend01.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumhash001.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumhash01.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumstar001.ico
# End Source File
# Begin Source File

SOURCE=..\res\callnumstar01.ico
# End Source File
# Begin Source File

SOURCE=..\res\cdma.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\checkno.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\checkno.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\claro.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\claro.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\close.ico"
# End Source File
# Begin Source File

SOURCE=..\res\close.ico
# End Source File
# Begin Source File

SOURCE=..\res\connect001.ico
# End Source File
# Begin Source File

SOURCE=..\res\connect01.ico
# End Source File
# Begin Source File

SOURCE=..\res\connect03.ico
# End Source File
# Begin Source File

SOURCE=..\res\delete.ico
# End Source File
# Begin Source File

SOURCE=..\res\dial.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\down.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\down.bmp
# End Source File
# Begin Source File

SOURCE=..\res\downspeedicon.bmp
# End Source File
# Begin Source File

SOURCE=..\res\draft.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\Earphone-un.bmp"
# End Source File
# Begin Source File

SOURCE="..\res\Earphone-un.bmp"
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\Earphone.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\Earphone.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\EDGE.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\EDGE.bmp
# End Source File
# Begin Source File

SOURCE=..\res\edit.ico
# End Source File
# Begin Source File

SOURCE=..\res\extract.ico
# End Source File
# Begin Source File

SOURCE=..\res\file.ico
# End Source File
# Begin Source File

SOURCE=..\res\forward.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\globe.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\globe.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\GPRS.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\GPRS.bmp
# End Source File
# Begin Source File

SOURCE=..\res\group.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\gsm.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\gsm.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\HELP.ico"
# End Source File
# Begin Source File

SOURCE=..\res\HELP.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\HELP2.ico"
# End Source File
# Begin Source File

SOURCE=..\res\HELP2.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\HSDPA.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\HSDPA.bmp
# End Source File
# Begin Source File

SOURCE=..\res\HSDPA.rc2
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\HSUPA.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\HSUPA.bmp
# End Source File
# Begin Source File

SOURCE=..\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\icon1.ico"
# End Source File
# Begin Source File

SOURCE=..\res\icon1.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\icon2.ico"
# End Source File
# Begin Source File

SOURCE=..\res\icon2.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\icon3.ico"
# End Source File
# Begin Source File

SOURCE=..\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=..\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_01.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_02.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_03.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\Icon_04.ico"
# End Source File
# Begin Source File

SOURCE=..\res\Icon_04.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_05.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_06.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_07.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\Icon_09.ico"
# End Source File
# Begin Source File

SOURCE=..\res\Icon_09.ico
# End Source File
# Begin Source File

SOURCE=..\res\icon_cal.ico
# End Source File
# Begin Source File

SOURCE=..\res\icon_dow.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\iconnull.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\iconnull.bmp
# End Source File
# Begin Source File

SOURCE=..\res\icons.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\IDB_CALLSTATE.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\IDB_CALLSTATE.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\IDB_PB_LOCSTATE.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\IDB_PB_LOCSTATE.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\IDB_SMSSTATE.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\IDB_SMSSTATE.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\IDB_SMSTYPE.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\IDB_SMSTYPE.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\image.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\image.bmp
# End Source File
# Begin Source File

SOURCE=..\res\inbox.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\LampOff.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\LampOff.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\LampOn.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\LampOn.bmp
# End Source File
# Begin Source File

SOURCE=..\res\LimitSrv.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\logo.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=..\res\longiconnul_speed.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\longiconnull.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\longiconnull.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\MAINFRAME.ICO"
# End Source File
# Begin Source File

SOURCE=..\res\MAINFRAME.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\menuimag.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\menuimag.bmp
# End Source File
# Begin Source File

SOURCE=..\res\message.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\message001.ico"
# End Source File
# Begin Source File

SOURCE=..\res\message001.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\message01.ico"
# End Source File
# Begin Source File

SOURCE=..\res\message01.ico
# End Source File
# Begin Source File

SOURCE=..\res\message03.ico
# End Source File
# Begin Source File

SOURCE=..\res\middle02.bmp
# End Source File
# Begin Source File

SOURCE=..\res\new.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\oi.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\oi.bmp
# End Source File
# Begin Source File

SOURCE=..\res\Ondalogo.bmp
# End Source File
# Begin Source File

SOURCE=..\res\Ondasplash.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\open.ico"
# End Source File
# Begin Source File

SOURCE=..\res\open.ico
# End Source File
# Begin Source File

SOURCE=..\res\outbox.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\padcall.ico"
# End Source File
# Begin Source File

SOURCE=..\res\padcall.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\padhangup.ico"
# End Source File
# Begin Source File

SOURCE=..\res\padhangup.ico
# End Source File
# Begin Source File

SOURCE=..\res\phonebook.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\phonebook001.ico"
# End Source File
# Begin Source File

SOURCE=..\res\phonebook001.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\phonebook01.ico"
# End Source File
# Begin Source File

SOURCE=..\res\phonebook01.ico
# End Source File
# Begin Source File

SOURCE=..\res\phonebook03.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\play.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\play.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\play_hot.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\play_hot.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\PS.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\PS.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\radio_of.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\radio_of.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\radio_on.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\radio_on.bmp
# End Source File
# Begin Source File

SOURCE=..\res\reply.ico
# End Source File
# Begin Source File

SOURCE=..\res\resume.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\roam_flash.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\roam_flash.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\roam_on.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\roam_on.bmp
# End Source File
# Begin Source File

SOURCE=..\res\roam_on1.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\rssi0.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\rssi0.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\rssi1.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\rssi1.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\rssi2.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\rssi2.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\rssi3.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\rssi3.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\rssi4.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\rssi4.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\rssi5.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\rssi5.bmp
# End Source File
# Begin Source File

SOURCE=..\res\search.ico
# End Source File
# Begin Source File

SOURCE=..\res\setup.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\setup001.ico"
# End Source File
# Begin Source File

SOURCE=..\res\setup001.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\setup01.ico"
# End Source File
# Begin Source File

SOURCE=..\res\setup01.ico
# End Source File
# Begin Source File

SOURCE=..\res\setup03.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\slider.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\slider.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\slidertic.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\slidertic.bmp
# End Source File
# Begin Source File

SOURCE=..\res\sms.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\smsfull.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\smsfull.bmp
# End Source File
# Begin Source File

SOURCE=..\res\smsmenu.bmp
# End Source File
# Begin Source File

SOURCE=..\res\splash.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\splashex.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\splashex.bmp
# End Source File
# Begin Source File

SOURCE=..\res\status_bkground.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\stop.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\stop.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\stop_hot.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\stop_hot.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\tabIcons_CallHistory.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\tabIcons_CallHistory.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\telemig.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\telemig.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\test_bit.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\test_bit.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\toolbar1.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=..\res\Transfer.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\up.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\up.bmp
# End Source File
# Begin Source File

SOURCE=..\res\up.ico
# End Source File
# Begin Source File

SOURCE=..\res\up1.bmp
# End Source File
# Begin Source File

SOURCE=..\res\upspeedicon.bmp
# End Source File
# Begin Source File

SOURCE=..\res\usage001.ico
# End Source File
# Begin Source File

SOURCE=..\res\usage01.ico
# End Source File
# Begin Source File

SOURCE=..\res\usage03.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\USSD1.ico"
# End Source File
# Begin Source File

SOURCE=..\res\ussd1.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\USSD2.ico"
# End Source File
# Begin Source File

SOURCE=..\res\ussd2.ico
# End Source File
# Begin Source File

SOURCE=..\res\ussddown.ico
# End Source File
# Begin Source File

SOURCE=..\res\ussdnormal.ico
# End Source File
# Begin Source File

SOURCE=..\res\ussdpressed.ico
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\vivo.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\vivo.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\volume.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\volume.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\wcdma.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\wcdma.bmp
# End Source File
# Begin Source File

SOURCE=..\res\wcdma1.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\xsms.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\xsms.bmp
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\xsms.ico"
# End Source File
# Begin Source File

SOURCE=..\res\xsms.ico
# End Source File
# End Group
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\call.wav"
# End Source File
# Begin Source File

SOURCE=..\res\call.wav
# End Source File
# Begin Source File

SOURCE="C:\Documents and Settings\周斌\res\sms.wav"
# End Source File
# Begin Source File

SOURCE=..\res\sms.wav
# End Source File
# End Target
# End Project
