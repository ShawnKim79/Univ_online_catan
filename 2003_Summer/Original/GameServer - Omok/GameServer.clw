; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServerStatusDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GameServer.h"

ClassCount=6
Class1=CGameServerApp
Class2=CGameServerDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_SERVERSTATUS_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_GAMESERVER_DIALOG
Class4=CDummyWnd
Class5=CImgButton
Resource4=IDD_ABOUTBOX
Class6=CServerStatusDlg
Resource5=IDR_MENU

[CLS:CGameServerApp]
Type=0
HeaderFile=GameServer.h
ImplementationFile=GameServer.cpp
Filter=N

[CLS:CGameServerDlg]
Type=0
HeaderFile=GameServerDlg.h
ImplementationFile=GameServerDlg.cpp
Filter=D
LastObject=CGameServerDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=GameServerDlg.h
ImplementationFile=GameServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GAMESERVER_DIALOG]
Type=1
Class=CGameServerDlg
ControlCount=32
Control1=IDC_GSADDR,edit,1350631552
Control2=IDC_GSPORT,edit,1350631552
Control3=IDC_ASOMOK,button,1073938441
Control4=IDC_ASBINGO,button,1073741833
Control5=IDC_CHANNEL,edit,1350631552
Control6=IDC_SPIN1,msctls_updown32,1342177462
Control7=IDC_ROOM,edit,1350631552
Control8=IDC_SPIN2,msctls_updown32,1342177462
Control9=IDC_CSADDR,edit,1350631552
Control10=IDC_CSPORT,edit,1350631552
Control11=IDC_DSN,edit,1350631552
Control12=IDC_DBID,edit,1350631552
Control13=IDC_DBPW,edit,1350631584
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1073872896
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342177296
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342177296
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_OK,button,1342242816
Control31=IDC_STATIC,static,1342177296
Control32=IDC_CANCEL,button,1342242816

[CLS:CDummyWnd]
Type=0
HeaderFile=DummyWnd.h
ImplementationFile=DummyWnd.cpp
BaseClass=CWnd
Filter=T
VirtualFilter=WC
LastObject=ID_STARTSERVER

[MNU:IDR_MENU]
Type=1
Class=?
Command1=ID_STARTSERVER
Command2=ID_STOPSERVER
Command3=ID_SERVERCONFIG
Command4=ID_STATISTICS
Command5=ID_EXIT
CommandCount=5

[CLS:CImgButton]
Type=0
HeaderFile=ImgButton.h
ImplementationFile=ImgButton.cpp
BaseClass=CButton
LastObject=CImgButton

[DLG:IDD_SERVERSTATUS_DIALOG]
Type=1
Class=CServerStatusDlg
ControlCount=0

[CLS:CServerStatusDlg]
Type=0
HeaderFile=ServerStatusDlg.h
ImplementationFile=ServerStatusDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CServerStatusDlg

