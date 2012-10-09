# Microsoft Developer Studio Project File - Name="DataCrad_Chinese" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DataCrad_Chinese - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DataCrad_Chinese.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DataCrad_Chinese.mak" CFG="DataCrad_Chinese - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DataCrad_Chinese - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DataCrad_Chinese - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DataCrad_Chinese - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "DataCrad_Chinese - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "DataCrad_Chinese_EXPORTS" /D "_MBCS" /D "_AFXEXT" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../lang/DataCrad_Chinese.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DataCrad_Chinese - Win32 Release"
# Name "DataCrad_Chinese - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DataCrad_Chinese.cpp
# End Source File
# Begin Source File

SOURCE=.\DataCrad_Chinese.def
# End Source File
# Begin Source File

SOURCE=.\DataCrad_Chinese.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=..\res\archive.ico
# End Source File
# Begin Source File

SOURCE=..\res\back_default.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bk_con.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bk_con_default.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bk_page01.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bk_screen.bmp
# End Source File
# Begin Source File

SOURCE=..\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=..\res\browser1.ico
# End Source File
# Begin Source File

SOURCE=..\res\browser2.ico
# End Source File
# Begin Source File

SOURCE=..\res\browser3.ico
# End Source File
# Begin Source File

SOURCE=..\res\btn1.bmp
# End Source File
# Begin Source File

SOURCE=..\res\btn1_hot.bmp
# End Source File
# Begin Source File

SOURCE=..\res\call001.ico
# End Source File
# Begin Source File

SOURCE=..\res\call01.ico
# End Source File
# Begin Source File

SOURCE=..\res\call03.ico
# End Source File
# Begin Source File

SOURCE=..\res\checkno.bmp
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

SOURCE=.\DataCrad_Chinese.h
# End Source File
# Begin Source File

SOURCE=.\res\DataCrad_Chinese.rc2
# End Source File
# Begin Source File

SOURCE=..\res\delete.ico
# End Source File
# Begin Source File

SOURCE=..\res\dial.ico
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

SOURCE="..\res\Earphone-un.bmp"
# End Source File
# Begin Source File

SOURCE=..\res\Earphone.bmp
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

SOURCE=..\res\globe.bmp
# End Source File
# Begin Source File

SOURCE=..\res\GPRS.bmp
# End Source File
# Begin Source File

SOURCE=..\res\group.ico
# End Source File
# Begin Source File

SOURCE=..\res\gsm.bmp
# End Source File
# Begin Source File

SOURCE=..\res\HELP.ico
# End Source File
# Begin Source File

SOURCE=..\res\HELP2.ico
# End Source File
# Begin Source File

SOURCE=..\res\HSDPA.bmp
# End Source File
# Begin Source File

SOURCE=..\res\HSUPA.bmp
# End Source File
# Begin Source File

SOURCE=..\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=..\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=..\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_01.ico
# End Source File
# Begin Source File

SOURCE=..\res\Icon_02.ico
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

SOURCE=..\res\Icon_09.ico
# End Source File
# Begin Source File

SOURCE=..\res\icon_dow.ico
# End Source File
# Begin Source File

SOURCE=..\res\iconnull.bmp
# End Source File
# Begin Source File

SOURCE=..\res\IDB_CALLSTATE.bmp
# End Source File
# Begin Source File

SOURCE=..\res\IDB_PB_LOCSTATE.bmp
# End Source File
# Begin Source File

SOURCE=..\res\IDB_SMSSTATE.bmp
# End Source File
# Begin Source File

SOURCE=..\res\IDB_SMSTYPE.bmp
# End Source File
# Begin Source File

SOURCE=..\res\image.bmp
# End Source File
# Begin Source File

SOURCE=..\res\inbox.ico
# End Source File
# Begin Source File

SOURCE=..\res\LampOff.bmp
# End Source File
# Begin Source File

SOURCE=..\res\LampOn.bmp
# End Source File
# Begin Source File

SOURCE=..\res\longiconnul_speed.bmp
# End Source File
# Begin Source File

SOURCE=..\res\longiconnull.bmp
# End Source File
# Begin Source File

SOURCE=..\res\MAINFRAME.ico
# End Source File
# Begin Source File

SOURCE=..\res\menuimag.bmp
# End Source File
# Begin Source File

SOURCE=..\res\message001.ico
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

SOURCE=..\res\outbox.ico
# End Source File
# Begin Source File

SOURCE=..\res\padcall.ico
# End Source File
# Begin Source File

SOURCE=..\res\padhangup.ico
# End Source File
# Begin Source File

SOURCE=..\res\phonebook001.ico
# End Source File
# Begin Source File

SOURCE=..\res\phonebook01.ico
# End Source File
# Begin Source File

SOURCE=..\res\phonebook03.ico
# End Source File
# Begin Source File

SOURCE=..\res\play.bmp
# End Source File
# Begin Source File

SOURCE=..\res\play_hot.bmp
# End Source File
# Begin Source File

SOURCE=..\res\privacy_call.bmp
# End Source File
# Begin Source File

SOURCE=..\res\radio_of.bmp
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

SOURCE=..\res\roam_flash.bmp
# End Source File
# Begin Source File

SOURCE=..\res\roam_on.bmp
# End Source File
# Begin Source File

SOURCE=..\res\rssi0.bmp
# End Source File
# Begin Source File

SOURCE=..\res\rssi1.bmp
# End Source File
# Begin Source File

SOURCE=..\res\rssi2.bmp
# End Source File
# Begin Source File

SOURCE=..\res\rssi3.bmp
# End Source File
# Begin Source File

SOURCE=..\res\rssi4.bmp
# End Source File
# Begin Source File

SOURCE=..\res\rssi5.bmp
# End Source File
# Begin Source File

SOURCE=..\res\search.ico
# End Source File
# Begin Source File

SOURCE=..\res\setup001.ico
# End Source File
# Begin Source File

SOURCE=..\res\setup01.ico
# End Source File
# Begin Source File

SOURCE=..\res\setup03.ico
# End Source File
# Begin Source File

SOURCE=..\res\slider.bmp
# End Source File
# Begin Source File

SOURCE=..\res\slidertic.bmp
# End Source File
# Begin Source File

SOURCE=..\res\sms.ico
# End Source File
# Begin Source File

SOURCE=..\res\smsfull.bmp
# End Source File
# Begin Source File

SOURCE=..\res\smsmenu.bmp
# End Source File
# Begin Source File

SOURCE=..\res\status_bkground.bmp
# End Source File
# Begin Source File

SOURCE=..\res\stop.bmp
# End Source File
# Begin Source File

SOURCE=..\res\stop_hot.bmp
# End Source File
# Begin Source File

SOURCE=..\res\tabIcons_CallHistory.bmp
# End Source File
# Begin Source File

SOURCE=..\res\test_bit.bmp
# End Source File
# Begin Source File

SOURCE=..\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=..\res\Transfer.ico
# End Source File
# Begin Source File

SOURCE=..\res\up.bmp
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

SOURCE=..\res\voicemail.bmp
# End Source File
# Begin Source File

SOURCE=..\res\voicemailico.ico
# End Source File
# Begin Source File

SOURCE=..\res\volume.bmp
# End Source File
# Begin Source File

SOURCE=..\res\xsms.bmp
# End Source File
# Begin Source File

SOURCE=..\res\xsms.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\res\call.wav
# End Source File
# Begin Source File

SOURCE=..\res\sms.wav
# End Source File
# End Target
# End Project
