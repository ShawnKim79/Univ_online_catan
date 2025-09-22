#if !defined(AFX_TRAINDLG_H__EE8EE7DD_D350_4C30_83AE_B55A42A16E64__INCLUDED_)
#define AFX_TRAINDLG_H__EE8EE7DD_D350_4C30_83AE_B55A42A16E64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg dialog

class CTrainDlg : public CDialog
{
// Construction
public:
	int resource[5];
	int select;
	int con;
	int turn;
	
	int source[20];
	int target[20];
	int target_select;		// 항구와 트레이드해서 받을 카드
	
	int sn;
	int tn;
	char player_name[4][100];
	char player_pn[4];
	int playern;
	bool trade_ok;

	bool hangu[6];
	CTrainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrainDlg)
	enum { IDD = IDD_TRAIN_DLG };
	CComboBox	m_select;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrainDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSourceBtn();
	afx_msg void OnTargetBtn();
	afx_msg void OnCancelBtn();
	afx_msg void OnNoBtn();
	afx_msg void OnSelchangeSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINDLG_H__EE8EE7DD_D350_4C30_83AE_B55A42A16E64__INCLUDED_)
