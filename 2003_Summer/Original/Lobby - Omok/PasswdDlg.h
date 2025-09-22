#if !defined(AFX_PASSWDDLG_H__00E049C9_77DB_4C43_9C9E_AC1E01B29CE4__INCLUDED_)
#define AFX_PASSWDDLG_H__00E049C9_77DB_4C43_9C9E_AC1E01B29CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswdDlg.h : header file
//

#include "imgbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CPasswdDlg dialog

class CPasswdDlg : public CDialog
{
// Construction
public:
	CPasswdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasswdDlg)
	enum { IDD = IDD_PASSWD_DIALOG };
	CImgButton	m_ctrlOk;
	CString	m_strPasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswdDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswdDlg)
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWDDLG_H__00E049C9_77DB_4C43_9C9E_AC1E01B29CE4__INCLUDED_)
