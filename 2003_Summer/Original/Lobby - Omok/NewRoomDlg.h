#if !defined(AFX_NEWROOMDLG_H__079D4490_3B53_474C_92A5_0E7DD82E5FA7__INCLUDED_)
#define AFX_NEWROOMDLG_H__079D4490_3B53_474C_92A5_0E7DD82E5FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewRoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewRoomDlg dialog

#include "ImgButton.h"

class CNewRoomDlg : public CDialog
{
// Construction
public:
	CNewRoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewRoomDlg)
	enum { IDD = IDD_NEWROOM_DIALOG };
	CImgButton	m_ctrlCancel;
	CImgButton	m_ctrlOK;
	BOOL	m_bLocked;
	CString	m_strPasswd;
	CString	m_strRoomTitle;
	//}}AFX_DATA

public:
	CString m_strTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewRoomDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewRoomDlg)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnLocked();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWROOMDLG_H__079D4490_3B53_474C_92A5_0E7DD82E5FA7__INCLUDED_)
