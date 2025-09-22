// UpdaterDlg.h : header file
//

#if !defined(AFX_UPDATERDLG_H__D32B0CF8_BA6B_4DD1_90CF_7881CE69C9C0__INCLUDED_)
#define AFX_UPDATERDLG_H__D32B0CF8_BA6B_4DD1_90CF_7881CE69C9C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PathDialog.h"
#include "imgbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CUpdaterDlg dialog

class CUpdaterDlg : public CDialog
{
// Construction
public:
	CUpdaterDlg(CWnd* pParent = NULL);	// standard constructor

	CString m_strID;
	CString m_strGameType;

// Dialog Data
	//{{AFX_DATA(CUpdaterDlg)
	enum { IDD = IDD_UPDATER_DIALOG };
	CProgressCtrl	m_ctrlAllFiles;
	CProgressCtrl	m_ctrlCurFile;
	CImgButton	m_ctrlFolder;
	CImgButton	m_ctrlInstall;
	CString	m_strPath;
	CString m_strTitle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdaterDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static UINT threadDownload(LPVOID arg);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUpdaterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInstall();
	afx_msg void OnSelectfolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATERDLG_H__D32B0CF8_BA6B_4DD1_90CF_7881CE69C9C0__INCLUDED_)
