// GameServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"
#include "DummyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameServerApp

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
	//{{AFX_MSG_MAP(CGameServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameServerApp construction

CGameServerApp::CGameServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGameServerApp object

CGameServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGameServerApp initialization

BOOL CGameServerApp::InitInstance()
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

	CDummyWnd *pWnd = new CDummyWnd();

	CString strClassName = AfxRegisterWndClass(0, LoadCursor(IDC_ARROW), 0, 0);
	pWnd->CreateEx(0, strClassName, "Game Server", 0, CRect(), NULL, 0);

	m_pMainWnd = pWnd;

	return TRUE;
}
