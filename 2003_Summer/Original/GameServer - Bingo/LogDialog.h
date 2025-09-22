#if !defined(AFX_LOGDIALOG_H__A71D23D4_24E1_42DD_940C_E25FEB8C40B5__INCLUDED_)
#define AFX_LOGDIALOG_H__A71D23D4_24E1_42DD_940C_E25FEB8C40B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogDialog dialog

class CLogDialog : public CDialog
{
// Construction
public:
	void AddLogString(const char *format, ...);
	CLogDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogDialog)
	enum { IDD = IDD_LOG_DIALOG };
	CEdit	m_ctrlLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnShowall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGDIALOG_H__A71D23D4_24E1_42DD_940C_E25FEB8C40B5__INCLUDED_)
