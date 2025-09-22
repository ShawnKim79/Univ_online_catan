// NoEditView.cpp : implementation file
//

#include "stdafx.h"
#include "īź.h"
#include "NoEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoEditView

CNoEditView::CNoEditView()
{
}

CNoEditView::~CNoEditView()
{
}


BEGIN_MESSAGE_MAP(CNoEditView, CEdit)
	//{{AFX_MSG_MAP(CNoEditView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoEditView message handlers

void CNoEditView::OnSetFocus(CWnd* pOldWnd) 
{
//	CEdit::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}
