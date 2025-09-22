// ServerManager.h: interface for the CServerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERMANAGER_H__6A8492DD_2E9B_458A_B501_4F2D3A8A548E__INCLUDED_)
#define AFX_SERVERMANAGER_H__6A8492DD_2E9B_458A_B501_4F2D3A8A548E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "structs.h"
#include <list>
#include <string>

using namespace std;

class CServerManager
{
public:
	void GetServerSockets(DWORD **sockets, int *nLen);
	int GetLength();
	void DeleteServer(SOCKET socket);
	LPSERVER GetServer(SOCKET socket);
	LPSERVER GetServer(int id);
	void AddServer(SOCKET socket);
	CServerManager();
	virtual ~CServerManager();

private:
	list<SERVER> serverlist;

};

#endif // !defined(AFX_SERVERMANAGER_H__6A8492DD_2E9B_458A_B501_4F2D3A8A548E__INCLUDED_)
