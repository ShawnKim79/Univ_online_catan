#if !defined(AFX_ITEMINPUTDLG_H__EA953A74_D5BC_11D1_8EDE_006097283F10__INCLUDED_)
#define AFX_ITEMINPUTDLG_H__EA953A74_D5BC_11D1_8EDE_006097283F10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemInputDlg dialog

class CItemInputDlg : public CDialog
{
// Construction
public:
	CItemInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemInputDlg)
	enum { IDD = IDD_ITEMINPUT };
	CString	m_strName;
	CString	m_strPhone;
	int		m_nAge;
	int		m_nSex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CItemInputDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMINPUTDLG_H__EA953A74_D5BC_11D1_8EDE_006097283F10__INCLUDED_)
