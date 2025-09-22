// UserManager.cpp: implementation of the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lobby.h"
#include "UserManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserManager::CUserManager()
{

}

CUserManager::~CUserManager()
{
	userlist.clear();
}

void CUserManager::AddUser(USERINFO user)
{
	LPUSERINFO lpuser = GetUser((LPCTSTR)user.id);
	if(lpuser == NULL) userlist.push_back(user);
}

LPUSERINFO CUserManager::GetUser(LPCTSTR id)
{
	list<USERINFO>::iterator iter = userlist.begin();

	for(; iter != userlist.end() ; iter++)
	{
		LPUSERINFO lpuser = &(*iter);

		if(!stricmp((char *)lpuser->id, id)) return lpuser;
	}

	return NULL;
}

void CUserManager::DeleteUser(LPCTSTR id)
{
	list<USERINFO>::iterator iter = userlist.begin();

	for(; iter != userlist.end() ; iter++)
	{
		LPUSERINFO lpuser = &(*iter);

		if(!stricmp((char *)lpuser->id, id)) 
		{
			userlist.erase(iter);
			return;
		}
	}
}

void CUserManager::UpdateUser(USERINFO userinfo)
{
	list<USERINFO>::iterator iter = userlist.begin();

	for(; iter != userlist.end() ; iter++)
	{
		LPUSERINFO lpuser = &(*iter);

		if(!stricmp((char *)lpuser->id, (char *)userinfo.id)) 
		{
			*lpuser = userinfo;
			return;
		}
	}
}
/*
unsigned char* GetUserIp(short roomno);
{
	list<USERINFO>::iterator iter = userlist.begin();

	for(; iter != userlist.end() ; iter++)
	{
		LPUSERINFO lpuser = &(*iter);

		if(lpuser->roomno == roomno && lpuser->)) 
		{
			*lpuser = userinfo;
			return;
		}
	}
	
}
*/
void CUserManager::ClearUser()
{
	userlist.clear();
}
