// ClientManager.h: interface for the CClientManager class.
//
// Ŭ���̾�Ʈ���� ���� ������ ����Ʈ�� ���� ����....
// ����Ʈ�� STL�� �������....
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTMANAGER_H__17D95AC2_8257_4087_B4E5_1C4E640BA17F__INCLUDED_)
#define AFX_CLIENTMANAGER_H__17D95AC2_8257_4087_B4E5_1C4E640BA17F__INCLUDED_

#include "structs.h"	// Added by ClassView
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
	void AddClient(DWORD socket);
	LPUSER GetClient(DWORD socket);
	LPUSER GetClient(LPCTSTR userid);
	int GetLength();
	void DeleteClient(SOCKET socket);
	CClientManager();
	virtual ~CClientManager();

public:
	list<USER> clientlist;	// Ŭ���̾�Ʈ ���� ���� ����Ʈ
							// Ŭ���̾�Ʈ ���� ������Ʈ�� ServerWorkŬ�������� List ������ 
							// USER Struct pointer�� ���� ���� �����ϴ� ������� ���ش�.
							// (CClientManager Ŭ������ ServerWorkŬ���� ���� ����� �� ����)

};

#endif // !defined(AFX_CLIENTMANAGER_H__17D95AC2_8257_4087_B4E5_1C4E640BA17F__INCLUDED_)
