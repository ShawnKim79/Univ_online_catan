// catan_designDlg.h : header file
//

#if !defined(AFX_CATAN_DESIGNDLG_H__7B5F3E8D_7688_451B_83D4_AEF3FDDE9F75__INCLUDED_)
#define AFX_CATAN_DESIGNDLG_H__7B5F3E8D_7688_451B_83D4_AEF3FDDE9F75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCatan_designDlg dialog

class CCatan_designDlg : public CDialog
{
// Construction
public:

	CCatan_designDlg(CWnd* pParent = NULL);	// standard constructor

	void LoadImageList();
	void List();
	void InsertColumn();
	

// Dialog Data
	//{{AFX_DATA(CCatan_designDlg)
	enum { IDD = IDD_CATAN_DESIGN_DIALOG };

	CListCtrl m_ctrlList;
	int m_nStyle;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatan_designDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CString m_strPlayer, m_strCardcount, m_strLongestRoad, m_strLargeArmy, m_strScore; // 리스트 내 구성원들 정보 받을 변수
	// 플레이어 이름, 카드 보유 수, 롱기스트 웨이, 라지스트 아미, 총점

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCatan_designDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATAN_DESIGNDLG_H__7B5F3E8D_7688_451B_83D4_AEF3FDDE9F75__INCLUDED_)
