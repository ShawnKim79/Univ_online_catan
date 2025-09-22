// DirectX_DialogDlg.h : header file
//

#if !defined(AFX_DIRECTX_DIALOGDLG_H__B15C9B08_B917_43BA_BCE3_E3B47A2E7AF4__INCLUDED_)
#define AFX_DIRECTX_DIALOGDLG_H__B15C9B08_B917_43BA_BCE3_E3B47A2E7AF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDirectX_DialogDlg dialog

class CDirectX_DialogDlg : public CDialog
{
// Construction
public:
	CDirectX_DialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDirectX_DialogDlg)
	enum { IDD = IDD_DIRECTX_DIALOG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectX_DialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDirectX_DialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTX_DIALOGDLG_H__B15C9B08_B917_43BA_BCE3_E3B47A2E7AF4__INCLUDED_)
