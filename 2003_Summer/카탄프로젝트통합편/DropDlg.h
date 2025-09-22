#if !defined(AFX_DROPDLG_H__D2F783D9_E65B_4B84_8CD5_84B691ED80EA__INCLUDED_)
#define AFX_DROPDLG_H__D2F783D9_E65B_4B84_8CD5_84B691ED80EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDropDlg dialog

class CDropDlg : public CDialog
{
// Construction
public:
	void set_rgn();
	int resource[5];
	int target_resource[5];
	int drop_n;
	int drop_w;
	char caption[100];
	CDropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDropDlg)
	enum { IDD = IDD_DROP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDropDlg)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDropBtn();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPDLG_H__D2F783D9_E65B_4B84_8CD5_84B691ED80EA__INCLUDED_)
