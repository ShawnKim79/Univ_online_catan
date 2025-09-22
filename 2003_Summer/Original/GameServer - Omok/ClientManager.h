// ClientManager.h: interface for the CClientManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTMANAGER_H__F42E69EC_023E_4212_92E5_0D24692F2D7F__INCLUDED_)
#define AFX_CLIENTMANAGER_H__F42E69EC_023E_4212_92E5_0D24692F2D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"
#include "structs.h"
#include <list>

using namespace std;

class CClientManager
{
public:
	void DeleteClient(SOCKET socket);
	int GetLength();
	LPUSER GetClient(LPCTSTR userid);
	LPUSER GetClient(DWORD socket);
	void AddClient(DWORD socket);
	CClientManager();
	virtual ~CClientManager();

public:
	list<USER> clientlist;
};

#endif // !defined(AFX_CLIENTMANAGER_H__F42E69EC_023E_4212_92E5_0D24692F2D7F__INCLUDED_)
