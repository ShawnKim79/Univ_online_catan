// Catan_DummyDlg.h : header file
//

#if !defined(AFX_CATAN_DUMMYDLG_H__D97CE067_9580_4676_AA68_DEF87A6CF25D__INCLUDED_)
#define AFX_CATAN_DUMMYDLG_H__D97CE067_9580_4676_AA68_DEF87A6CF25D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CCatan_DummyDlg dialog

class CCatan_DummyDlg : public CDialog
{
// Construction
public:

	CCatan_DummyDlg(CWnd* pParent = NULL);	// standard constructor

	void AddLogString(const char *format, ...);

// Dialog Data
	//{{AFX_DATA(CCatan_DummyDlg)
	enum { IDD = IDD_CATAN_DUMMY_DIALOG };
	// NOTE: the ClassWizard will add data members here
	CEdit m_ctrlLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatan_DummyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
/*
	UINT nGAMESTART;
	UINT nLIVECHECK;

	UINT nINLIVE;
	UINT nGAMEEND;
	UINT nGAMESTATU;
*/
	WPARAM m_wParam;
	LPARAM m_lParam;

	CString m_strNICK;
	CString m_strIP;


protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCatan_DummyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnStartGame(WPARAM m_wParam, LPARAM m_lParam);
//	afx_msg void OnShowall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATAN_DUMMYDLG_H__D97CE067_9580_4676_AA68_DEF87A6CF25D__INCLUDED_)
