// Lobby.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Lobby.h"
#include "LobbyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLobbyApp

BEGIN_MESSAGE_MAP(CLobbyApp, CWinApp)
	//{{AFX_MSG_MAP(CLobbyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLobbyApp construction

CLobbyApp::CLobbyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLobbyApp object

CLobbyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLobbyApp initialization

BOOL CLobbyApp::InitInstance()
{
	if(::FindWindow(NULL, "HGR Catan..."))
	{
		AfxMessageBox("�̹� ���� ���Դϴ�.");
		return FALSE;
	}


	if(__argc < 2)
	{
		AfxMessageBox("�� ���α׷��� �ܵ����� ����� �� �����ϴ�.\n���������� �����Ͽ� �����Ͽ� �ֽʽÿ�");
		return FALSE;
	}

	else if(__argc == 2)
	{
		CString error;
		error.Format("���࿡ �ʿ��� �Ķ���Ͱ� �����մϴ�.\n%s", m_lpCmdLine);

		AfxMessageBox(error);
		return FALSE;
	}

	CString strParam = m_lpCmdLine;

	m_strID = strParam.Left(strParam.Find(' '));
	m_strGameType = strParam.Right(strParam.GetLength()-m_strID.GetLength()-1);

	if(m_strGameType.CompareNoCase("catan"))
	{
		AfxMessageBox("Invalid Game type.");
		return FALSE;
	}

	AfxEnableControlContainer();
	AfxInitRichEdit();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CLobbyDlg dlg;
	dlg.m_strID = m_strID;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
