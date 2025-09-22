// Catan_Gameserver.h : main header file for the CATAN_GAMESERVER application
//

#if !defined(AFX_CATAN_GAMESERVER_H__FA3948EF_B528_4A74_88F9_F6B50DB4753A__INCLUDED_)
#define AFX_CATAN_GAMESERVER_H__FA3948EF_B528_4A74_88F9_F6B50DB4753A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverApp:
// See Catan_Gameserver.cpp for the implementation of this class
//

class CCatan_GameserverApp : public CWinApp
{
public:
	CCatan_GameserverApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatan_GameserverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCatan_GameserverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATAN_GAMESERVER_H__FA3948EF_B528_4A74_88F9_F6B50DB4753A__INCLUDED_)
