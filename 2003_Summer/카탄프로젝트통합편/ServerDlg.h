#if !defined(AFX_SERVERDLG_H__474BC821_15B8_48FD_9475_A2E59B04B5AB__INCLUDED_)
#define AFX_SERVERDLG_H__474BC821_15B8_48FD_9475_A2E59B04B5AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

#include "ListenSocket.h"

class CServerDlg : public CDialog
{
// Construction
public:
	bool *start;
	CListenSocket server_s;				// 辑滚包府 家南
	CServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__474BC821_15B8_48FD_9475_A2E59B04B5AB__INCLUDED_)
