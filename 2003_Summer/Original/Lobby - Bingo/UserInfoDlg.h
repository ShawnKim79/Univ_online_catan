#if !defined(AFX_USERINFODLG_H__B5D2DBD2_00F3_4767_8815_F81F1B392D55__INCLUDED_)
#define AFX_USERINFODLG_H__B5D2DBD2_00F3_4767_8815_F81F1B392D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg dialog

#include "structs.h"
#include "header.h"

class CUserInfoDlg : public CDialog
{
// Construction
public:
	void SetUserInfo(USERINFO userinfo);
	CUserInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserInfoDlg)
	enum { IDD = IDD_USERINFO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	USERINFO m_Userinfo;

	// Generated message map functions
	//{{AFX_MSG(CUserInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERINFODLG_H__B5D2DBD2_00F3_4767_8815_F81F1B392D55__INCLUDED_)
