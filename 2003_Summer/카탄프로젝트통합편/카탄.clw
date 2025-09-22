; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKatanPlay
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "īź.h"

ClassCount=11
Class1=CMyApp
Class2=CMyDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_CARD_USE_DLG
Resource2=IDR_MAINFRAME
Resource3=IDD_SERVER_DLG
Class4=CKatanPlay
Resource4=IDD_CARDSTEAL_DLG
Class5=CDropDlg
Resource5=IDD_KATAN_PLAY
Class6=CCardStealDlg
Resource6=IDD_ABOUTBOX
Class7=CServerDlg
Resource7=IDD_MY_DIALOG
Class8=CCardUseDlg
Resource8=IDD_DROP_DIALOG
Class9=CTrainDlg
Class10=CNoEditView
Class11=CWaitRoomDlg
Resource9=IDD_TRAIN_DLG

[CLS:CMyApp]
Type=0
HeaderFile=īź.h
ImplementationFile=īź.cpp
Filter=N
LastObject=CMyApp

[CLS:CMyDlg]
Type=0
HeaderFile=īźDlg.h
ImplementationFile=īźDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMyDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=īźDlg.h
ImplementationFile=īźDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MY_DIALOG]
Type=1
Class=CMyDlg
ControlCount=6
Control1=IDC_SERVER_BTN,button,1342242816
Control2=IDC_CLIENT_BTN,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_IP_EDIT,edit,1350631552
Control5=IDC_NAME_EDIT,edit,1350631552
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_KATAN_PLAY]
Type=1
Class=CKatanPlay
ControlCount=26
Control1=IDC_CHAT_INPUT,edit,1350631552
Control2=IDC_CHAT_BTN,button,1342242816
Control3=IDC_PLAYER_INFORMATION,SysListView32,1350631425
Control4=IDC_ROAD_BTN,button,1342242816
Control5=IDC_SETT_BTN,button,1342242816
Control6=IDC_CITY_BTN,button,1342242816
Control7=IDC_CARD_BTN,button,1342242816
Control8=IDC_TRADE_BTN,button,1342242816
Control9=IDC_CARD_USE_BTN,button,1342242816
Control10=IDC_TURN_BTN,button,1342242816
Control11=IDC_QUIT_BTN,button,1342177280
Control12=IDC_VIEWER,static,1476395015
Control13=IDC_CARD1,static,1350565902
Control14=IDC_CARD2,static,1350565902
Control15=IDC_CARD3,static,1350565902
Control16=IDC_CARD4,static,1350565902
Control17=IDC_CARD5,static,1350565902
Control18=IDC_CARD1_SU,static,1342308352
Control19=IDC_CARD2_SU,static,1342308352
Control20=IDC_CARD3_SU,static,1342308352
Control21=IDC_CARD4_SU,static,1342308352
Control22=IDC_CARD5_SU,static,1342308352
Control23=IDC_STATIC,static,1342177285
Control24=IDC_EVENT_RECT,static,1342177287
Control25=IDC_CHAT_RECT,static,1342177287
Control26=IDC_STATIC,static,1350696961

[CLS:CKatanPlay]
Type=0
HeaderFile=KatanPlay.h
ImplementationFile=KatanPlay.cpp
BaseClass=CDialog
Filter=W
LastObject=IDC_VIEWER
VirtualFilter=dWC

[DLG:IDD_DROP_DIALOG]
Type=1
Class=CDropDlg
ControlCount=3
Control1=IDC_DROP_BTN,button,1342242816
Control2=IDC_CARD_N,static,1342308352
Control3=IDC_CAPTION_STATIC,static,1342308353

[CLS:CDropDlg]
Type=0
HeaderFile=DropDlg.h
ImplementationFile=DropDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDropDlg

[CLS:CCardStealDlg]
Type=0
HeaderFile=CardStealDlg.h
ImplementationFile=CardStealDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CCardStealDlg
VirtualFilter=dWC

[DLG:IDD_CARDSTEAL_DLG]
Type=1
Class=CCardStealDlg
ControlCount=2
Control1=IDC_PLAYER,combobox,1344340227
Control2=IDC_STATIC,static,1342308352

[DLG:IDD_SERVER_DLG]
Type=1
Class=CServerDlg
ControlCount=0

[CLS:CServerDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=CServerDlg

[DLG:IDD_CARD_USE_DLG]
Type=1
Class=CCardUseDlg
ControlCount=2
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CANCEL,button,1342242816

[CLS:CCardUseDlg]
Type=0
HeaderFile=CardUseDlg.h
ImplementationFile=CardUseDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CCardUseDlg
VirtualFilter=dWC

[DLG:IDD_TRAIN_DLG]
Type=1
Class=CTrainDlg
ControlCount=8
Control1=IDC_SELECT,combobox,1344339971
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_SOURCE_BTN,button,1342242816
Control6=IDC_TARGET_BTN,button,1342242816
Control7=IDC_CANCEL_BTN,button,1342242816
Control8=IDC_NO_BTN,button,1342242816

[CLS:CTrainDlg]
Type=0
HeaderFile=TrainDlg.h
ImplementationFile=TrainDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CTrainDlg
VirtualFilter=dWC

[CLS:CNoEditView]
Type=0
HeaderFile=NoEditView.h
ImplementationFile=NoEditView.cpp
BaseClass=CEdit
Filter=W
LastObject=CNoEditView
VirtualFilter=WC

[CLS:CWaitRoomDlg]
Type=0
HeaderFile=WaitRoomDlg.h
ImplementationFile=WaitRoomDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CWaitRoomDlg
VirtualFilter=dWC

