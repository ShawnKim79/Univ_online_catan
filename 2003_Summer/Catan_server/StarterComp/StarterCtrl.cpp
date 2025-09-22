// StarterCtrl.cpp : Implementation of CStarterCtrl

#include "stdafx.h"
#include "StarterComp.h"
#include "StarterCtrl.h"
#include "winsock2.h"

#pragma comment(lib, "ws2_32")
//#include "Shlobj.h"

/////////////////////////////////////////////////////////////////////////////
// CStarterCtrl

BOOL CStarterCtrl::FtpConnect(LPCTSTR server, LPCTSTR userid, LPCTSTR passwd)
{
	hInternet = InternetOpen("FTPTEST", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	
	if(hInternet == NULL)
	{
		::MessageBox(NULL, "StartCom.cpp\nFtpConnect():Line 13\n���ͳ� ���� ���� ����", "Error !!", MB_OK);
		return FALSE;
	}
	
	::MessageBox(NULL, userid, passwd, MB_OK);

	hFtp = InternetConnect(hInternet, server, INTERNET_DEFAULT_FTP_PORT, userid, passwd, INTERNET_SERVICE_FTP, 0, 0);

	DWORD dwError = WSAGetLastError();

	char str[255];

	wsprintf(str, "%d", dwError);

	::MessageBox(NULL, str, NULL, MB_OK);

	if(hFtp == NULL)
	{
		InternetCloseHandle(hInternet);
		::MessageBox(NULL, "StartCom.cpp\nFtpConnect():Line 21\n���� ������ ������ �� �����ϴ�.", "Error !!", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CStarterCtrl::FtpDisconnect()
{
	InternetCloseHandle(hFtp);
	InternetCloseHandle(hInternet);
	hFtp = NULL;
	hInternet = NULL;
}

BOOL CStarterCtrl::DownloadUpdater()
{
	BOOL bContinue = TRUE;
	
	char szCacheFolder[MAX_PATH];
	
	if(hFtp == NULL)
		return FALSE;
	
	HINTERNET hRemote;
	HANDLE hLocal;
	DWORD dwRead, dwWritten, Prog;
	BOOL bResult;
				
	char NewFile[MAX_PATH];
	char buff[1000];
	
	CreateDirectory("c:\\temp", NULL);
	
	hRemote = FtpOpenFile(hFtp, "\\updater.exe", GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 0);
	
	if(hRemote == NULL)
		return FALSE;
	
	wsprintf(NewFile, "c:\\temp\\updater.exe", szCacheFolder);
	hLocal = CreateFile(NewFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	Prog = 0;
	
	while(1)
	{
		bResult = InternetReadFile(hRemote, buff, 1000, &dwRead);
		if((bResult == TRUE) && (dwRead == 0)) break;
		
		WriteFile(hLocal, buff, dwRead, &dwWritten, NULL);
		
		Prog += dwWritten;
	}
	
	CloseHandle(hLocal);
	InternetCloseHandle(hRemote);

	return TRUE;
}

// �ܺ� ���α׷����� �������͸� �����ϱ� ���� ���� �κ�.
// �̰��� Java Script�� �ҷ� ���� ���ư���.
STDMETHODIMP CStarterCtrl::StartUpdater(BSTR id, short type)
{
	char szparam[255] = {0, };
	char szid[32] = {0, };
	char sztype[32] = {0, };

	if(type == 0) strcpy(sztype, "catan");
	//else if(type == 1) strcpy(sztype, "bingo");

	USES_CONVERSION;

	wsprintf(szparam, "%s %s", W2A(id), sztype);

	ShellExecute(NULL, "open", "c:\\temp\\updater.exe", szparam, NULL, SW_SHOW);

	return S_OK;
}