#if !defined(AFX_NOEDITVIEW_H__7B95DEFC_547C_4144_ABBD_DB7CF7F7D15B__INCLUDED_)
#define AFX_NOEDITVIEW_H__7B95DEFC_547C_4144_ABBD_DB7CF7F7D15B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoEditView window

class CNoEditView : public CEdit
{
// Construction
public:
	CNoEditView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoEditView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNoEditView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNoEditView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOEDITVIEW_H__7B95DEFC_547C_4144_ABBD_DB7CF7F7D15B__INCLUDED_)
