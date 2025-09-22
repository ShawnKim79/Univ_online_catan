// īźDlg.h : header file
//

#if !defined(AFX_DLG_H__9184B2AC_3D79_4FF1_94AB_DB770B03CFF1__INCLUDED_)
#define AFX_DLG_H__9184B2AC_3D79_4FF1_94AB_DB770B03CFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KatanPlay.h"
#include "ServerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	~CMyDlg();
	CString GetLocalIP();
	int SCREENY;
	int SCREENX;

	CServerDlg *serverdlg;
	CMyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
	CString	m_name;
	CString	m_ip;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnServerBtn();
	afx_msg void OnClientBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__9184B2AC_3D79_4FF1_94AB_DB770B03CFF1__INCLUDED_)
