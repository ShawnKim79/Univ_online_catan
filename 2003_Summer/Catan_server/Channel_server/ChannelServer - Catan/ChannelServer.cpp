// ChannelServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "ChannelServerDlg.h"
#include "DummyWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp

BEGIN_MESSAGE_MAP(CChannelServerApp, CWinApp)
	//{{AFX_MSG_MAP(CChannelServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp construction

CChannelServerApp::CChannelServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChannelServerApp object

CChannelServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp initialization

BOOL CChannelServerApp::InitInstance()
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

	if(::FindWindow(NULL, "Catan Channel"))
	{
		AfxMessageBox("채널 서버가 이미 실행 중 입니다.");
		return FALSE;
	}

	CDummyWnd *pWnd = new CDummyWnd();
	CString strClassName = AfxRegisterWndClass(0, LoadCursor(IDC_ARROW), 0, 0);
	pWnd->CreateEx(0, strClassName, "Catan Channel", 0, CRect(), NULL, 0);

	/*
	if(!pWnd->CreateEx(0, strClassName, "Catan Channel Server", 0, CRect(), NULL, 0, 0))
	{
		AfxMessageBox("생성단계에서 문제 발생");
		return FALSE;

	}*/
	m_pMainWnd = pWnd;

	return TRUE;
}
