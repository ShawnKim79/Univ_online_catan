// GameServerDlg.h : header file
//

#if !defined(AFX_GAMESERVERDLG_H__13E0C506_498A_44A9_9741_DC2906E769DF__INCLUDED_)
#define AFX_GAMESERVERDLG_H__13E0C506_498A_44A9_9741_DC2906E769DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgButton.h"

/////////////////////////////////////////////////////////////////////////////
// CGameServerDlg dialog

class CGameServerDlg : public CDialog
{
// Construction
public:
	BOOL SaveSettings();
	BOOL LoadSettings();
	CGameServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGameServerDlg)
	enum { IDD = IDD_GAMESERVER_DIALOG };
	CImgButton	m_ctrlCancel;
	CImgButton	m_ctrlOK;
	CString	m_strGSAddr;
	int		m_nServerBehavior;
	UINT	m_nChannel;
	CString	m_strCSAddr;
	UINT	m_nGSPortNo;
	CString	m_strDSN;
	CString	m_strDBID;
	CString	m_strDBPW;
	UINT	m_nRoom;
	UINT	m_nCSPortNo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameServerDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGameServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnChangeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMESERVERDLG_H__13E0C506_498A_44A9_9741_DC2906E769DF__INCLUDED_)
