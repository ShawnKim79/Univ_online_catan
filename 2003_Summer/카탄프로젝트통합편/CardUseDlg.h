#if !defined(AFX_CARDUSEDLG_H__A5CF41E1_7179_4D5D_8270_24C1B4C03FA4__INCLUDED_)
#define AFX_CARDUSEDLG_H__A5CF41E1_7179_4D5D_8270_24C1B4C03FA4__INCLUDED_

#include "tile_inf.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardUseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCardUseDlg dialog

class CCardUseDlg : public CDialog
{
// Construction
public:
	int card[30];
	int cardn;
	char card_inf[27];
	int select;
	bool select_ok;

	CBitmap card_map;
	CCardUseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCardUseDlg)
	enum { IDD = IDD_CARD_USE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardUseDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCardUseDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDUSEDLG_H__A5CF41E1_7179_4D5D_8270_24C1B4C03FA4__INCLUDED_)
