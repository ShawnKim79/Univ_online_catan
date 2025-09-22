#if !defined(AFX_WAITROOMDLG_H__F4B80813_3317_4C8C_BB69_4A20C1E89E52__INCLUDED_)
#define AFX_WAITROOMDLG_H__F4B80813_3317_4C8C_BB69_4A20C1E89E52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WaitRoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitRoomDlg dialog

class CWaitRoomDlg : public CDialog
{
// Construction
public:
	void EyeMove();
	double angle(int x1,int y1, int x2, int y2);
	void set_rgn();

	CPoint m_p;
	bool set;

	CWaitRoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaitRoomDlg)
	enum { IDD = IDD_WAITROOM_DLG };
	CEdit	m_input_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitRoomDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitRoomDlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExitBtn();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITROOMDLG_H__F4B80813_3317_4C8C_BB69_4A20C1E89E52__INCLUDED_)
