// ChannelServerDlg.h : header file
//

#if !defined(AFX_CHANNELSERVERDLG_H__43D70B4F_5546_4154_8DF4_953533D8250B__INCLUDED_)
#define AFX_CHANNELSERVERDLG_H__43D70B4F_5546_4154_8DF4_953533D8250B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChannelServerDlg dialog

class CChannelServerDlg : public CDialog
{
// Construction
public:
	CChannelServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChannelServerDlg)
	enum { IDD = IDD_CHANNELSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChannelServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELSERVERDLG_H__43D70B4F_5546_4154_8DF4_953533D8250B__INCLUDED_)
