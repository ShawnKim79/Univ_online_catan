// Updater.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Updater.h"
#include "UpdaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdaterApp

BEGIN_MESSAGE_MAP(CUpdaterApp, CWinApp)
	//{{AFX_MSG_MAP(CUpdaterApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdaterApp construction

CUpdaterApp::CUpdaterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUpdaterApp object

CUpdaterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUpdaterApp initialization

BOOL CUpdaterApp::InitInstance()
{
	AfxEnableControlContainer();

	if(__argc < 2)
	{
		AfxMessageBox("이 프로그램은 단독으로 실행될 수 없습니다.\n웹페이지에 접속하여 실행하여 주십시오");
		return FALSE;
	}

	else if(__argc == 2)
	{
		AfxMessageBox("실행에 필요한 파라메터가 부족합니다.");
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


	// 게임이 깔려 있는것이 확인되어야 Lobby를 실행 시킨다.
	if(FindInstalledGame())
	{
		strParam.Format("%s %s", m_strID, m_strGameType);

		// 로비가 실행되면 코드를 나간다.
		ShellExecute(NULL, "open", m_strPath + "\\Lobby.exe", strParam, m_strPath, SW_SHOW);
		return FALSE;
	}



	/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/

	// 게임이 실행되지 않을때....InitInstance()는 다운로드까지 마치고 나간다.
	CUpdaterDlg dlg;
	dlg.m_strID = m_strID;
	dlg.m_strGameType = m_strGameType;

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

BOOL CUpdaterApp::FindInstalledGame()
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwSize;
	BOOL bSucceeded = TRUE;

	char szGamePath[255] = {0, };

	// 파일이 인스톨 되어 있는지 검사.
	CString strRootKey = "SOFTWARE\\HGRGame\\" + m_strGameType + "\\Setting";

	RegCreateKeyEx(HKEY_CURRENT_USER,
		strRootKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hKey, &dwDisp);

	dwSize = sizeof(szGamePath);
	RegQueryValueEx(hKey, "installedpath", 0, NULL, (LPBYTE)szGamePath, &dwSize);

	// 레지스트리에 저장된 게임 저장 폴더 패스정보가 없음
	if(!strlen((LPTSTR)szGamePath))
		return FALSE;

	m_strPath = szGamePath;

	FILE *fp = NULL;
	char filename[255] = {0, };
	char version[255] = {0, };
	int nNumofFile = 0;
	int nFileSize = 0;
	int nTotalSize = 0;
	

	if((fp = fopen(m_strPath + "\\filelist.log", "rt")) == NULL)
		return FALSE;

	fscanf(fp, "%d", &nNumofFile);
	fscanf(fp, "%d", &nTotalSize);

	for(int i = 0 ; i < nNumofFile ; i++)
	{
		fscanf(fp, "%s", filename);
		fscanf(fp, "%d", &nFileSize);
//		fscanf(fp, "%s", version);

		

		CFile file;
		CString strFilename = filename;

		strFilename.Remove('*');

		if(!file.Open(m_strPath + "\\" + strFilename, CFile::modeRead)) // 해당 파일이 존재하지 않음, 무조건 다시 받음
			return FALSE;

		file.Close();
	}

	fclose(fp);

	return TRUE; // 게임이 이미 설치되어있음
}
