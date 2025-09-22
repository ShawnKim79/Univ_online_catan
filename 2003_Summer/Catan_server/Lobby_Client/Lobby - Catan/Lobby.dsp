# Microsoft Developer Studio Project File - Name="Lobby" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Lobby - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Lobby.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Lobby.mak" CFG="Lobby - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Lobby - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Lobby - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Lobby - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib Shell32.lib user32.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "Lobby - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib BingGoDll.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Lobby - Win32 Release"
# Name "Lobby - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cConsol.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameRoomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Lobby.cpp
# End Source File
# Begin Source File

SOURCE=.\Lobby.rc
# End Source File
# Begin Source File

SOURCE=.\LobbyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\MirListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NewRoomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UserInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserManager.cpp
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

SOURCE=.\ChannelDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameRoomDlg.h
# End Source File
# Begin Source File

SOURCE=.\header.h
# End Source File
# Begin Source File

SOURCE=.\ImgButton.h
# End Source File
# Begin Source File

SOURCE=.\Lobby.h
# End Source File
# Begin Source File

SOURCE=.\LobbyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Message.h
# End Source File
# Begin Source File

SOURCE=.\MirListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NewRoomDlg.h
# End Source File
# Begin Source File

SOURCE=.\PasswdDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoomManager.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\structs.h
# End Source File
# Begin Source File

SOURCE=.\UserInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserManager.h
# End Source File
# Begin Source File

SOURCE=.\WSASocket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\caption.bmp
# End Source File
# Begin Source File

SOURCE=.\res\channel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chgchannelclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chgchannelnor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chgchannelover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\closeclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\closenormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\closeover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\entchannelclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\entchannelnor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\entchannelover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enterroomclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enterroomnor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enterroomover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit2clk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit2nor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit2over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exitclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exitnor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exitover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gameroom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Lobby.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lobby.rc2
# End Source File
# Begin Source File

SOURCE=.\res\miniclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mininormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\miniover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\offergameclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\offergamenor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\offergameover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refresh2clk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refresh2nor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refresh2over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refreshclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refreshnor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refreshover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\userinfo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\userinfoclk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\userinfonor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\userinfoover.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Lobby : {524C8F31-1ADB-4BCA-804E-595C2053ADBC}
# 	2:11:ImgButton.h:ImgButton.h
# 	2:17:CLASS: CImgButton:CImgButton
# 	2:10:ENUM: enum:enum
# 	2:13:ImgButton.cpp:ImgButton.cpp
# 	2:19:Application Include:Lobby.h
# End Section
# Section Lobby : {5913A4D4-10DD-4129-906B-0B79138872AB}
# 	2:11:Message.cpp:Message.cpp
# 	2:9:Message.h:Message.h
# 	2:19:Application Include:Lobby.h
# 	2:15:CLASS: CMessage:CMessage
# End Section
# Section Lobby : {5D740746-7E98-4226-97BD-532E2933FEC9}
# 	2:13:WSASocket.cpp:WSASocket.cpp
# 	2:17:CLASS: CWSASocket:CWSASocket
# 	2:19:Application Include:Lobby.h
# 	2:11:WSASocket.h:WSASocket.h
# End Section
# Section Lobby : {EC8829CC-F187-4F6B-A806-DC0DA5841C3A}
# 	2:13:MirListCtrl.h:MirListCtrl.h
# 	2:21:CLASS: CMirHeaderCtrl:CMirHeaderCtrl
# 	2:19:CLASS: CMirListCtrl:CMirListCtrl
# 	2:15:MirListCtrl.cpp:MirListCtrl.cpp
# 	2:19:Application Include:Lobby.h
# End Section
