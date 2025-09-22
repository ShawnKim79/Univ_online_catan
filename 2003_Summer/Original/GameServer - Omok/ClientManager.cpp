// ClientManager.cpp: implementation of the CClientManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameServer.h"
#include "ClientManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientManager::CClientManager()
{

}

CClientManager::~CClientManager()
{
	clientlist.clear();
}

void CClientManager::AddClient(DWORD socket)
{
	USER user;

	memset(&user, 0, sizeof(user));

	user.socket = socket;

	clientlist.push_back(user);
}

LPUSER CClientManager::GetClient(DWORD socket)
{
	list<USER>::iterator iter = clientlist.begin();

	for(; iter != clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(lpuser->socket == socket) return lpuser;
	}

	return NULL;
}

LPUSER CClientManager::GetClient(LPCTSTR userid)
{
	list<USER>::iterator iter = clientlist.begin();

	for(; iter != clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(!stricmp((char *)lpuser->id, userid)) return lpuser;
	}

	return NULL;
}

int CClientManager::GetLength()
{
	return clientlist.size();
}

void CClientManager::DeleteClient(SOCKET socket)
{
	list<USER>::iterator i = clientlist.begin();

	for(; i != clientlist.end() ; i++)
	{
		LPUSER lpuser = &(*i);

		if(lpuser->socket == socket) 
		{
			clientlist.erase(i);
			return;
		}
	}
}
