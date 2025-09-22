// GameServer.h : main header file for the GAMESERVER application
//

#if !defined(AFX_GAMESERVER_H__8EFB99DE_97C7_46A7_9FCC_C7597DC3AD54__INCLUDED_)
#define AFX_GAMESERVER_H__8EFB99DE_97C7_46A7_9FCC_C7597DC3AD54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGameServerApp:
// See GameServer.cpp for the implementation of this class
//

class CGameServerApp : public CWinApp
{
public:
	CGameServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGameServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMESERVER_H__8EFB99DE_97C7_46A7_9FCC_C7597DC3AD54__INCLUDED_)
