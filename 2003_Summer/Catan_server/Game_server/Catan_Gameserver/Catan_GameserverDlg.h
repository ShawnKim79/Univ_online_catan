// Catan_GameserverDlg.h : header file
//

#if !defined(AFX_CATAN_GAMESERVERDLG_H__A5894DC5_09B8_462C_9DC0_77AD24D05B08__INCLUDED_)
#define AFX_CATAN_GAMESERVERDLG_H__A5894DC5_09B8_462C_9DC0_77AD24D05B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverDlg dialog

class CCatan_GameserverDlg : public CDialog
{
// Construction
public:
	BOOL SaveSetting();
	BOOL LoadSetting();


	CCatan_GameserverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCatan_GameserverDlg)
	enum { IDD = IDD_CATAN_GAMESERVER_DIALOG };
	CString	m_strDBNAME;
	CString	m_strGameserverIP;
	UINT	m_nGameserverPORT;
	CString	m_strDBPASSWD;
	CString	m_strDBID;
	CString	m_strChannelserverIP;
	UINT	m_nChannelPORT;
	UINT	m_nChannel;
	UINT	m_nRoom;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatan_GameserverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCatan_GameserverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATAN_GAMESERVERDLG_H__A5894DC5_09B8_462C_9DC0_77AD24D05B08__INCLUDED_)
