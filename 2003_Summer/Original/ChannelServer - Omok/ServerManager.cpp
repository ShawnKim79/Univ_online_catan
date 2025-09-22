// ServerManager.cpp: implementation of the CServerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelServer.h"
#include "ServerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerManager::CServerManager()
{

}

CServerManager::~CServerManager()
{
	serverlist.clear();
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 25일
//
// serverlist에 새로운 게임 서버 노드를 추가함
//
// params
// socket : 새로 접속한 게임 서버 소켓

void CServerManager::AddServer(SOCKET socket)
{
	SERVER s;

	s.socket = socket;
	serverlist.push_back(s);
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 25일
//
// serverlist로부터 인자로 넘어온 socket번호를 가지는 게임서버 노드 주소를 return
//
// params
// socket : 게임 서버 소켓

LPSERVER CServerManager::GetServer(SOCKET socket)
{
	list<SERVER>::iterator i = serverlist.begin();

	for(; i != serverlist.end() ; i++)
	{
		LPSERVER lpserver = &(*i);

		if(lpserver->socket == socket) return lpserver;
	}

	return NULL;
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 25일
//
// serverlist로부터 인자로 넘어온 id를 가지는 게임서버 노드 주소를 return
//
// params
// socket : 게임 서버 소켓

LPSERVER CServerManager::GetServer(int id)
{
	list<SERVER>::iterator i = serverlist.begin();

	for(; i != serverlist.end() ; i++)
	{
		LPSERVER lpserver = &(*i);

		if(lpserver->id == id) return lpserver;
	}

	return NULL;
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 25일
//
// serverlist로부터 인자로 넘어온 socket번호를 가지는 게임서버 노드를 리스트에서 삭제
//
// params
// socket : 게임 서버 소켓

void CServerManager::DeleteServer(SOCKET socket)
{
	list<SERVER>::iterator i = serverlist.begin();

	for(; i != serverlist.end() ; i++)
	{
		LPSERVER lpserver = &(*i);

		if(lpserver->socket == socket) 
		{
			serverlist.erase(i);
			return;
		}
	}
}

int CServerManager::GetLength()
{
	return serverlist.size();
}

void CServerManager::GetServerSockets(DWORD **sockets, int *nLen)
{
	*nLen = serverlist.size();

	*sockets = new DWORD[*nLen];

	list<SERVER>::iterator iter = serverlist.begin();

	int cnt = 0;

	for(; iter != serverlist.end() ; iter++)
	{
		LPSERVER lpserver = &(*iter);

		*sockets[cnt++] = lpserver->socket;
	}
}
