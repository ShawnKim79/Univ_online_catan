// ClientManager.cpp: implementation of the CClientManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelServer.h"
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

}

void CClientManager::AddClient(SOCKET socket)
{
	clientlist.push_back(socket);
}
