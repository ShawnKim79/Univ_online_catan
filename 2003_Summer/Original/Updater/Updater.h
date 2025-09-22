// Updater.h : main header file for the UPDATER application
//

#if !defined(AFX_UPDATER_H__22C46EEE_E5C5_442C_94FE_A14424401302__INCLUDED_)
#define AFX_UPDATER_H__22C46EEE_E5C5_442C_94FE_A14424401302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUpdaterApp:
// See Updater.cpp for the implementation of this class
//

class CUpdaterApp : public CWinApp
{
public:
	CUpdaterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdaterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

public:
	BOOL FindInstalledGame();
	CString m_strPath;
	CString m_strID;
	CString m_strGameType;

// Implementation

	//{{AFX_MSG(CUpdaterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATER_H__22C46EEE_E5C5_442C_94FE_A14424401302__INCLUDED_)
