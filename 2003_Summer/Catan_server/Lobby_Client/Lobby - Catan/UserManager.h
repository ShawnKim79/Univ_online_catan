// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__F77129C7_7899_46FC_8482_A9DBAE85C22F__INCLUDED_)
#define AFX_USERMANAGER_H__F77129C7_7899_46FC_8482_A9DBAE85C22F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "structs.h"	// Added by ClassView

using namespace std;

class CUserManager  
{
public:
	void ClearUser();
	void UpdateUser(USERINFO userinfo);
	void DeleteUser(LPCTSTR id);
	LPUSERINFO GetUser(LPCTSTR id);
//	LPUSERINFO GetUserIp(short roomno);
	void AddUser(USERINFO user);
	CUserManager();
	virtual ~CUserManager();

protected:

	list<USERINFO> userlist;
};

#endif // !defined(AFX_USERMANAGER_H__F77129C7_7899_46FC_8482_A9DBAE85C22F__INCLUDED_)
