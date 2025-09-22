; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGameRoomDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Lobby.h"

ClassCount=12
Class1=CLobbyApp
Class2=CLobbyDlg
Class3=CAboutDlg

ResourceCount=10
Resource1=IDD_NEWROOM_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_GAMEROOM_DIALOG
Class4=CGameRoomDlg
Resource4=IDD_USERINFO_DIALOG
Class5=CChannelDlg
Class6=CMirHeaderCtrl
Class7=CMirListCtrl
Resource5=IDD_ABOUTBOX
Class8=CProgressDlg
Resource6=IDD_LOBBY_DIALOG
Class9=CNewRoomDlg
Resource7=IDD_CHAT_DIALOG
Class10=CUserInfoDlg
Resource8=IDD_CHANNEL_DIALOG
Class11=CChatDlg
Resource9=IDD_PROGRESS_DIALOG
Class12=CPasswdDlg
Resource10=IDD_PASSWD_DIALOG

[CLS:CLobbyApp]
Type=0
HeaderFile=Lobby.h
ImplementationFile=Lobby.cpp
Filter=N

[CLS:CLobbyDlg]
Type=0
HeaderFile=LobbyDlg.h
ImplementationFile=LobbyDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CLobbyDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=LobbyDlg.h
ImplementationFile=LobbyDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LOBBY_DIALOG]
Type=1
Class=CLobbyDlg
ControlCount=4
Control1=IDC_CAPTION,static,1342177294
Control2=IDC_MINIMIZE,button,1342242816
Control3=IDC_CLOSE,button,1342242816
Control4=IDC_PAGEHOLDER,static,1073741831

[DLG:IDD_GAMEROOM_DIALOG]
Type=1
Class=CGameRoomDlg
ControlCount=10
Control1=IDC_ROOMLIST,SysListView32,1350631437
Control2=IDC_USERLIST,SysListView32,1350631437
Control3=IDC_VIEWCHAT,RICHEDIT,1352728772
Control4=IDC_CHAT,edit,1350635652
Control5=IDC_OFFERGAME,button,1342242816
Control6=IDC_PERSONALINFO,button,1342242816
Control7=IDC_ENTERROOM,button,1342242816
Control8=IDC_CHANGECHANNEL,button,1342242816
Control9=IDC_EXIT,button,1342242816
Control10=IDC_REFRESH,button,1342242816

[CLS:CGameRoomDlg]
Type=0
HeaderFile=GameRoomDlg.h
ImplementationFile=GameRoomDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=IDC_CHANGECHANNEL
VirtualFilter=dWC

[DLG:IDD_CHANNEL_DIALOG]
Type=1
Class=CChannelDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177294
Control2=IDC_CHANNELLIST,SysListView32,1342242829
Control3=IDC_REFRESH,button,1342242816
Control4=IDC_ENTERCHANNEL,button,1342242816
Control5=IDC_EXIT,button,1342242816
Control6=IDC_STATIC,static,1342177297

[CLS:CChannelDlg]
Type=0
HeaderFile=ChannelDlg.h
ImplementationFile=ChannelDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EXIT
VirtualFilter=dWC

[CLS:CMirHeaderCtrl]
Type=0
HeaderFile=MirListCtrl.h
ImplementationFile=MirListCtrl.cpp
BaseClass=CHeaderCtrl

[CLS:CMirListCtrl]
Type=0
HeaderFile=MirListCtrl.h
ImplementationFile=MirListCtrl.cpp
BaseClass=CListCtrl

[DLG:IDD_PROGRESS_DIALOG]
Type=1
Class=CProgressDlg
ControlCount=2
Control1=IDC_STATUS,static,1342308353
Control2=IDC_STATIC,static,1342177294

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CProgressDlg
VirtualFilter=dWC

[DLG:IDD_NEWROOM_DIALOG]
Type=1
Class=CNewRoomDlg
ControlCount=7
Control1=IDC_TITLECOMBO,combobox,1344340226
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_LOCKED,button,1342246915
Control5=IDC_PASSWD,edit,1484849312
Control6=IDC_OK,button,1342242816
Control7=IDC_CANCEL,button,1342242816

[CLS:CNewRoomDlg]
Type=0
HeaderFile=NewRoomDlg.h
ImplementationFile=NewRoomDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TITLECOMBO
VirtualFilter=dWC

[DLG:IDD_USERINFO_DIALOG]
Type=1
Class=CUserInfoDlg
ControlCount=6
Control1=IDC_USERID,static,1342308352
Control2=IDC_USERNICK,static,1342308352
Control3=IDC_USERSCORE,static,1342308352
Control4=IDC_STATISTICS,static,1342308352
Control5=IDC_RATIO,static,1342308352
Control6=IDC_STATIC,static,1342177294

[CLS:CUserInfoDlg]
Type=0
HeaderFile=UserInfoDlg.h
ImplementationFile=UserInfoDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_CHAT_DIALOG]
Type=1
Class=CChatDlg
ControlCount=5
Control1=IDC_LITTLECHATVIEW,RICHEDIT,1350631552
Control2=IDC_LITTLECHAT,edit,1350631552
Control3=IDC_OFFERGAME,button,1476460544
Control4=IDC_EXIT,button,1342242816
Control5=IDC_CHGFONT,button,1342242816

[CLS:CChatDlg]
Type=0
HeaderFile=ChatDlg.h
ImplementationFile=ChatDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHGFONT

[DLG:IDD_PASSWD_DIALOG]
Type=1
Class=CPasswdDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PASSWD,edit,1350631584
Control3=IDC_OK,button,1342242816

[CLS:CPasswdDlg]
Type=0
HeaderFile=PasswdDlg.h
ImplementationFile=PasswdDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_OK
VirtualFilter=dWC

