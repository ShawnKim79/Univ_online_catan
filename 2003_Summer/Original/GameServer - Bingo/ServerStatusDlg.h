#if !defined(AFX_SERVERSTATUSDLG_H__3D068960_C37E_4EF4_A01A_7A03EFD51026__INCLUDED_)
#define AFX_SERVERSTATUSDLG_H__3D068960_C37E_4EF4_A01A_7A03EFD51026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerStatusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerStatusDlg dialog

class CServerStatusDlg : public CDialog
{
// Construction
public:
	CServerStatusDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerStatusDlg)
	enum { IDD = IDD_SERVERSTATUS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerStatusDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSTATUSDLG_H__3D068960_C37E_4EF4_A01A_7A03EFD51026__INCLUDED_)
