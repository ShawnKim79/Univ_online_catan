// RoomManager.cpp: implementation of the CRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lobby.h"
#include "RoomManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoomManager::CRoomManager()
{

}

CRoomManager::~CRoomManager()
{
	roomlist.clear();
}

void CRoomManager::AddRoom(ROOMINFO roominfo)
{
	roomlist.push_back(roominfo);
}

void CRoomManager::UpdateRoom(ROOMINFO roominfo)
{
	list<ROOMINFO>::iterator iter = roomlist.begin();

	for(; iter != roomlist.end() ; iter++)
	{
		LPROOMINFO lproominfo = &(*iter);

		if(lproominfo->id == roominfo.id) 
		{
			*lproominfo = roominfo;
		}
	}
}

LPROOMINFO CRoomManager::GetRoom(int roomno)
{
	list<ROOMINFO>::iterator iter = roomlist.begin();

	for(; iter != roomlist.end() ; iter++)
	{
		LPROOMINFO lproominfo = &(*iter);

		if(lproominfo->id == roomno) return lproominfo;
	}

	return NULL;
}

void CRoomManager::ClearRoom()
{
	roomlist.clear();
}
