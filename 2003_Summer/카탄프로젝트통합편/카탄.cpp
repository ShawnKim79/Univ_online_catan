// Ä«Åº.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Ä«Åº.h"
#include "Ä«ÅºDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GAME_PATH "c:/catantest/"

/////////////////////////////////////////////////////////////////////////////
// CMyApp

BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
	//{{AFX_MSG_MAP(CMyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyApp construction

CMyApp::CMyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyApp object

CMyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyApp initialization

BOOL CMyApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.

	char str[500],space_which;
	strcpy(str,this->m_lpCmdLine);
	
	////////////////////////////////////////////////////////
	/*FILE *fp;
	char directory[256];
	
	fp=fopen("c:\\jun_catan.ini","r");
	fgets(directory,255,fp);
	fclose(fp);
	*/
	//SetCurrentDirectory("c:\\HGRGame\\catan");
	////////////////////////////////////////////////////////
	
	for(unsigned int i=0,n=0;i<strlen(str);i++) {
		if(str[i]==' ') {
			n++;
			space_which=i;
		}
	}
		
	if(n==0 && strlen(str)) {
		CKatanPlay game;
		
		CServerDlg *serverdlg;
		game.server = true;
		strcpy(game.name,str);
		
		game.ip_address=GetLocalIP();
		
		serverdlg = new (CServerDlg);
    
		serverdlg->start = &game.server_use;
		serverdlg->Create(IDD_SERVER_DLG);
//		serverdlg->ShowWindow(SW_SHOW);

		game.DoModal();

		serverdlg->DestroyWindow();

		game.server = false;
	} else if(n==1) {
		CKatanPlay game;

		game.server= false;
		strncpy(game.name,str,space_which);
		game.ip_address = str+space_which+1;

		game.DoModal();
	}

	return FALSE;
}

CString CMyApp::GetLocalIP()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	char name[255];

	CString ip; // ¿©±â¿¡ lcoal ip°¡ ÀúÀåµË´Ï´Ù.
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD( 2, 0 );

	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 ) {
		if( gethostname ( name, sizeof(name)) == 0) {
			if((hostinfo = gethostbyname(name)) != NULL) {
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}
	return ip;
}
