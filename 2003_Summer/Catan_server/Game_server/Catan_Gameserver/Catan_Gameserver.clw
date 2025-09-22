; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCatan_GameserverDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Catan_Gameserver.h"

ClassCount=4
Class1=CCatan_GameserverApp
Class2=CCatan_GameserverDlg

ResourceCount=4
Resource1=IDR_MAINFRAME
Class3=CServerWork
Resource2=IDD_LOGDIALOG
Resource3=IDD_CATAN_GAMESERVER_DIALOG
Class4=CLogDialog
Resource4=IDR_GSMENU

[CLS:CCatan_GameserverApp]
Type=0
HeaderFile=Catan_Gameserver.h
ImplementationFile=Catan_Gameserver.cpp
Filter=N

[CLS:CCatan_GameserverDlg]
Type=0
HeaderFile=Catan_GameserverDlg.h
ImplementationFile=Catan_GameserverDlg.cpp
Filter=D
LastObject=IDCANCEL
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_CATAN_GAMESERVER_DIALOG]
Type=1
Class=CCatan_GameserverDlg
ControlCount=25
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_GAMEIP,edit,1350631552
Control7=IDC_GAMEPORT,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_CHANNELIP,edit,1350631552
Control14=IDC_CHANNELPORT,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_DBNAME,edit,1350631552
Control20=IDC_DBID,edit,1350631552
Control21=IDC_DBPASSWORD,edit,1350631584
Control22=IDC_STATIC,static,1342177287
Control23=IDC_STATIC,static,1342177287
Control24=IDC_CHANNEL,edit,1350631552
Control25=IDC_ROOM,edit,1350631552

[CLS:CServerWork]
Type=0
HeaderFile=ServerWork.h
ImplementationFile=ServerWork.cpp
BaseClass=CWnd
Filter=W
LastObject=CServerWork
VirtualFilter=WC

[MNU:IDR_GSMENU]
Type=1
Class=CServerWork
Command1=ID_SERVERSTART
Command2=ID_SERVERSTOP
Command3=ID_SERVERSETUP
Command4=ID_SERVERLOG
Command5=ID_SERVEREXIT
CommandCount=5

[DLG:IDD_LOGDIALOG]
Type=1
Class=CLogDialog
ControlCount=2
Control1=IDC_LOGEDIT,edit,1352732804
Control2=IDC_SHOWALL,button,1342242816

[CLS:CLogDialog]
Type=0
HeaderFile=LogDialog.h
ImplementationFile=LogDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LOGEDIT
VirtualFilter=dWC

