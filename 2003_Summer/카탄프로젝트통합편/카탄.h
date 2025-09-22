// īź.h : main header file for the īź application
//

#if !defined(AFX__H__44808298_45B6_4CED_8B5F_048467601520__INCLUDED_)
#define AFX__H__44808298_45B6_4CED_8B5F_048467601520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "KatanPlay.h"
#include "ServerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyApp:
// See īź.cpp for the implementation of this class
//

class CMyApp : public CWinApp
{
public:
	CString GetLocalIP();
	CMyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__H__44808298_45B6_4CED_8B5F_048467601520__INCLUDED_)
