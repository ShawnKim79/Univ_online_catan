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
	
	CServerWork *pWnd = new CServerWork();	// CServerWork 클래스는 Wnd 클래스 상속...
											// Application Initialze(시작)부분에서 Dialog가 아닌 ServerWork 객체 생성
											// Dialog는 ServerWork의 initialize 부분에서 생성.


	CString strClassName = AfxRegisterWndClass(0, LoadCursor(IDC_ARROW), 0, 0);
											// Standard Window로 생성토록 등록.
	pWnd->CreateEx(0, strClassName, "Game Server", 0, CRect(), NULL, 0);
											// CreatEx 함수를 이용해 pWnd 객체 인스턴스를 실제 윈도우로 만들어 준다.
											// CServerWork의 OnCreat함수가 호출된다.
											// OnCreat함수 내부에 기본적인 Server Initializing 코드를 넣어주면 된다.

	m_pMainWnd = pWnd;						// Application MainWnd를 ServerWork로 등록
											// Application Thread에 ServerWork가 돌도록 하기 위함.

	return TRUE;
	
	
}
