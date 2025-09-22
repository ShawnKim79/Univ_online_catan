#if !defined(AFX_CARDSTEALDLG_H__BF87EEEA_4134_4812_9E1E_306D9FFC044E__INCLUDED_)
#define AFX_CARDSTEALDLG_H__BF87EEEA_4134_4812_9E1E_306D9FFC044E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardStealDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCardStealDlg dialog

class CCardStealDlg : public CDialog
{
// Construction
public:
	char name[4][100];
	int p_n[4];
	int cardn[4];
	int players;

	int select;
	bool btn_click;

	CCardStealDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCardStealDlg)
	enum { IDD = IDD_CARDSTEAL_DLG };
	CComboBox	m_players;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardStealDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCardStealDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangePlayer();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDSTEALDLG_H__BF87EEEA_4134_4812_9E1E_306D9FFC044E__INCLUDED_)
