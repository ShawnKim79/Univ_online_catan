; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUpdaterDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Updater.h"

ClassCount=3
Class1=CUpdaterApp
Class2=CUpdaterDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Class3=CImgButton
Resource3=IDD_UPDATER_DIALOG

[CLS:CUpdaterApp]
Type=0
HeaderFile=Updater.h
ImplementationFile=Updater.cpp
Filter=N

[CLS:CUpdaterDlg]
Type=0
HeaderFile=UpdaterDlg.h
ImplementationFile=UpdaterDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CURRENTFILE



[DLG:IDD_UPDATER_DIALOG]
Type=1
Class=CUpdaterDlg
ControlCount=15
Control1=IDC_STATIC11,static,1342177294
Control2=IDC_STATIC1,static,1342177294
Control3=IDC_CURRENTFILE,msctls_progress32,1082130432
Control4=IDC_ALLFILES,msctls_progress32,1082130432
Control5=IDC_STATIC12,button,1073741831
Control6=IDC_CURFILE,static,1073872896
Control7=IDC_STATIC13,button,1073741831
Control8=IDC_TIMEREMAINED,static,1073872896
Control9=IDC_PATH,edit,1350631552
Control10=IDC_STATIC3,static,1342308352
Control11=IDC_STATIC4,static,1342177296
Control12=IDC_INSTALL,button,1342242816
Control13=IDC_SELECTFOLDER,button,1342242816
Control14=IDC_STATIC2,static,1342308352
Control15=IDC_TRANSFERSPEED,static,1073872896

[CLS:CImgButton]
Type=0
HeaderFile=ImgButton.h
ImplementationFile=ImgButton.cpp
BaseClass=CButton

