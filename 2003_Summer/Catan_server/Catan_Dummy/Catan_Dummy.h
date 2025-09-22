// Catan_Dummy.h : main header file for the CATAN_DUMMY application
//

#if !defined(AFX_CATAN_DUMMY_H__721C2142_AF31_41C7_8628_B388460AAEBF__INCLUDED_)
#define AFX_CATAN_DUMMY_H__721C2142_AF31_41C7_8628_B388460AAEBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCatan_DummyApp:
// See Catan_Dummy.cpp for the implementation of this class
//

class CCatan_DummyApp : public CWinApp
{
public:
	CCatan_DummyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatan_DummyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	CString m_strNICK;
	CString m_strIP;

	//{{AFX_MSG(CCatan_DummyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATAN_DUMMY_H__721C2142_AF31_41C7_8628_B388460AAEBF__INCLUDED_)
