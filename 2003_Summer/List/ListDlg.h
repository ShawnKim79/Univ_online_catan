#if !defined(AFX_LISTDLG_H__EA953A73_D5BC_11D1_8EDE_006097283F10__INCLUDED_)
#define AFX_LISTDLG_H__EA953A73_D5BC_11D1_8EDE_006097283F10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog

class CListDlg : public CDialog
{
// Construction
public:
	CListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListDlg)
	enum { IDD = IDD_LIST };
	CListCtrl	m_ctrlList;
	int		m_nStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddSampleData();
	void AddItem();
	void List();
	void Detail();
	void SmallIconStyle();
	void LargeIconStyle();
	void InsertColumn();
	void LoadImageList();

	CImageList m_LargeImageList;
	CImageList m_SmallImageList;
	CString m_strName, m_strAge, m_strSex, m_strPhone;
	int m_nImage;

	// Generated message map functions
	//{{AFX_MSG(CListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnDeleteitem();
	afx_msg void OnAdditem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDLG_H__EA953A73_D5BC_11D1_8EDE_006097283F10__INCLUDED_)
