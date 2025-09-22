// Catan_Gameserver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Catan_Gameserver.h"
#include "Catan_GameserverDlg.h"
#include "ServerWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverApp

BEGIN_MESSAGE_MAP(CCatan_GameserverApp, CWinApp)
	//{{AFX_MSG_MAP(CCatan_GameserverApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverApp construction

CCatan_GameserverApp::CCatan_GameserverApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCatan_GameserverApp object

CCatan_GameserverApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverApp initialization

BOOL CCatan_GameserverApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	CServerWork *pWnd = new CServerWork();	// CServerWork Ŭ������ Wnd Ŭ���� ���...
											// Application Initialze(����)�κп��� Dialog�� �ƴ� ServerWork ��ü ����
											// Dialog�� ServerWork�� initialize �κп��� ����.


	CString strClassName = AfxRegisterWndClass(0, LoadCursor(IDC_ARROW), 0, 0);
											// Standard Window�� ������� ���.
	pWnd->CreateEx(0, strClassName, "Game Server", 0, CRect(), NULL, 0);
											// CreatEx �Լ��� �̿��� pWnd ��ü �ν��Ͻ��� ���� ������� ����� �ش�.
											// CServerWork�� OnCreat�Լ��� ȣ��ȴ�.
											// OnCreat�Լ� ���ο� �⺻���� Server Initializing �ڵ带 �־��ָ� �ȴ�.

	m_pMainWnd = pWnd;						// Application MainWnd�� ServerWork�� ���
											// Application Thread�� ServerWork�� ������ �ϱ� ����.

	return TRUE;
	
	
}
