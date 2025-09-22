; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDummyWnd
LastTemplate=CFrameWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ChannelServer.h"

ClassCount=4
Class1=CChannelServerApp
Class2=CChannelServerDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CHANNELSERVER_DIALOG
Class4=CDummyWnd
Resource4=IDR_MENU

[CLS:CChannelServerApp]
Type=0
HeaderFile=ChannelServer.h
ImplementationFile=ChannelServer.cpp
Filter=N

[CLS:CChannelServerDlg]
Type=0
HeaderFile=ChannelServerDlg.h
ImplementationFile=ChannelServerDlg.cpp
Filter=D

[CLS:CAboutDlg]
Type=0
HeaderFile=ChannelServerDlg.h
ImplementationFile=ChannelServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHANNELSERVER_DIALOG]
Type=1
Class=CChannelServerDlg
ControlCount=0

[CLS:CDummyWnd]
Type=0
HeaderFile=DummyWnd.h
ImplementationFile=DummyWnd.cpp
BaseClass=CWnd
Filter=T
VirtualFilter=WC
LastObject=ID_EXIT

[MNU:IDR_MENU]
Type=1
Class=?
Command1=ID_STARTSERVER
Command2=ID_STOPSERVER
Command3=ID_CONFIG
Command4=ID_EXIT
CommandCount=4

