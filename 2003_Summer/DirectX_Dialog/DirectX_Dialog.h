// DirectX_Dialog.h : main header file for the DIRECTX_DIALOG application
//

#if !defined(AFX_DIRECTX_DIALOG_H__B58EAA5B_88C1_4146_9896_EB20C1D9EBD3__INCLUDED_)
#define AFX_DIRECTX_DIALOG_H__B58EAA5B_88C1_4146_9896_EB20C1D9EBD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDirectX_DialogApp:
// See DirectX_Dialog.cpp for the implementation of this class
//

class CDirectX_DialogApp : public CWinApp
{
public:
	CDirectX_DialogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectX_DialogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDirectX_DialogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTX_DIALOG_H__B58EAA5B_88C1_4146_9896_EB20C1D9EBD3__INCLUDED_)
