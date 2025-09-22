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
// �ۼ���¥ : 2002�� 12�� 25��
//
// serverlist�� ���ο� ���� ���� ��带 �߰���
//
// params
// socket : ���� ������ ���� ���� ����

void CServerManager::AddServer(SOCKET socket)
{
	SERVER s;

	s.socket = socket;
	serverlist.push_back(s);
}


////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 25��
//
// serverlist�κ��� ���ڷ� �Ѿ�� socket��ȣ�� ������ ���Ӽ��� ��� �ּҸ� return
//
// params
// socket : ���� ���� ����

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
// �ۼ���¥ : 2002�� 12�� 25��
//
// serverlist�κ��� ���ڷ� �Ѿ�� id�� ������ ���Ӽ��� ��� �ּҸ� return
//
// params
// socket : ���� ���� ����

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
// �ۼ���¥ : 2002�� 12�� 25��
//
// serverlist�κ��� ���ڷ� �Ѿ�� socket��ȣ�� ������ ���Ӽ��� ��带 ����Ʈ���� ����
//
// params
// socket : ���� ���� ����

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
