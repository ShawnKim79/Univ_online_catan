// RoomManager.h: interface for the CRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROOMMANAGER_H__BAB0C4D7_A778_4C1E_8295_402D50120AFB__INCLUDED_)
#define AFX_ROOMMANAGER_H__BAB0C4D7_A778_4C1E_8295_402D50120AFB__INCLUDED_

#include "structs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

class CRoomManager  
{
public:
	void ClearRoom();
	LPROOMINFO GetRoom(int roomno);
	void UpdateRoom(ROOMINFO roominfo);
	void AddRoom(ROOMINFO roominfo);
	CRoomManager();
	virtual ~CRoomManager();

private:

	list<ROOMINFO> roomlist;
};

#endif // !defined(AFX_ROOMMANAGER_H__BAB0C4D7_A778_4C1E_8295_402D50120AFB__INCLUDED_)
