// ClientManager.h: interface for the CClientManager class.
//
// 클라이언트들의 소켓 정보를 리스트에 저장 관리....
// 리스트는 STL로 만들어짐....
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
	list<USER> clientlist;	// 클라이언트 정보 저장 리스트
							// 클라이언트 정보 업데이트는 ServerWork클래스에서 List 내부의 
							// USER Struct pointer를 직접 얻어와 갱신하는 방법으로 해준다.
							// (CClientManager 클래스가 ServerWork클래스 내에 멤버로 들어가 있음)

};

#endif // !defined(AFX_CLIENTMANAGER_H__17D95AC2_8257_4087_B4E5_1C4E640BA17F__INCLUDED_)
