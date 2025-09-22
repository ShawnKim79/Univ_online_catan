// catan_design.h : main header file for the CATAN_DESIGN application
//

#if !defined(AFX_CATAN_DESIGN_H__A1D23BD4_F826_425C_A199_54A503893FFD__INCLUDED_)
#define AFX_CATAN_DESIGN_H__A1D23BD4_F826_425C_A199_54A503893FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCatan_designApp:
// See catan_design.cpp for the implementation of this class
//

class CCatan_designApp : public CWinApp
{
public:
	CCatan_designApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatan_designApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCatan_designApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATAN_DESIGN_H__A1D23BD4_F826_425C_A199_54A503893FFD__INCLUDED_)
