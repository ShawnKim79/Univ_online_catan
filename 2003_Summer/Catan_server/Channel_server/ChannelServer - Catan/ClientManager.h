// ClientManager.h: interface for the CClientManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTMANAGER_H__DD350EC8_30A1_4B1D_B62F_E43969610A76__INCLUDED_)
#define AFX_CLIENTMANAGER_H__DD350EC8_30A1_4B1D_B62F_E43969610A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "structs.h"
#include <list>

using namespace std;

class CClientManager  
{
public:
	void AddClient(SOCKET socket);
	CClientManager();
	virtual ~CClientManager();

protected:
	list<SOCKET> clientlist;
};

#endif // !defined(AFX_CLIENTMANAGER_H__DD350EC8_30A1_4B1D_B62F_E43969610A76__INCLUDED_)
