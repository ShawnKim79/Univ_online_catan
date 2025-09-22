# Microsoft Developer Studio Project File - Name="GameServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GameServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak" CFG="GameServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameServer - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GameServer - Win32 Release"
# Name "GameServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cConsol.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DummyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.rc
# End Source File
# Begin Source File

SOURCE=.\GameServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\WSASocket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cConsol.h
# End Source File
# Begin Source File

SOURCE=.\ChannelManager.h
# End Source File
# Begin Source File

SOURCE=.\ClientManager.h
# End Source File
# Begin Source File

SOURCE=.\DataBase.h
# End Source File
# Begin Source File

SOURCE=.\DummyWnd.h
# End Source File
# Begin Source File

SOURCE=.\GameServer.h
# End Source File
# Begin Source File

SOURCE=.\GameServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\header.h
# End Source File
# Begin Source File

SOURCE=.\ImgButton.h
# End Source File
# Begin Source File

SOURCE=.\Message.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\structs.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\WSASocket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GameServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\GameServer.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section GameServer : {E97ED409-09B0-4F66-A40D-8D95D9153898}
# 	2:10:DataBase.h:DataBase.h
# 	2:16:CLASS: CDataBase:CDataBase
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:GameServer.h
# 	2:12:DataBase.cpp:DataBase.cpp
# End Section
# Section GameServer : {C5E64029-56F1-489B-A3E7-0EE65D8E4566}
# 	2:11:Message.cpp:Message.cpp
# 	2:9:Message.h:Message.h
# 	2:19:Application Include:GameServer.h
# 	2:15:CLASS: CMessage:CMessage
# End Section
# Section GameServer : {B55E18A9-D332-44CD-B567-14107B27C3DB}
# 	2:13:WSASocket.cpp:WSASocket.cpp
# 	2:17:CLASS: CWSASocket:CWSASocket
# 	2:19:Application Include:GameServer.h
# 	2:11:WSASocket.h:WSASocket.h
# End Section
# Section GameServer : {E86874C5-C0FF-4B60-8C74-90A3588D547F}
# 	2:10:TrayIcon.h:TrayIcon.h
# 	2:16:CLASS: CTrayIcon:CTrayIcon
# 	2:12:TrayIcon.cpp:TrayIcon.cpp
# 	2:19:Application Include:GameServer.h
# End Section
