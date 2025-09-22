#if !defined(AFX_CHANNELDLG_H__84C1B52C_9B8D_4D3A_AE10_87C4805240E0__INCLUDED_)
#define AFX_CHANNELDLG_H__84C1B52C_9B8D_4D3A_AE10_87C4805240E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelDlg.h : header file
//

#include "MirListCtrl.h"
#include "ImgButton.h"
#include "structs.h"
#include <list>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CChannelDlg dialog

class CLobbyDlg;

class CChannelDlg : public CDialog
{
// Construction
public:
	void EnterSelectedChannel();
	void ClearChannelInfo();
	void AddChannelInfo(CHANNEL channel);
	void InsertColumn();
	CChannelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChannelDlg)
	enum { IDD = IDD_CHANNEL_DIALOG };
	CImgButton	m_ctrlExit;
	CImgButton	m_ctrlEnterChannel;
	CImgButton	m_ctrlRefresh;
	CListCtrl	m_ctrlChannelList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:

	int m_nTotalConnection;

protected:

	CLobbyDlg *m_pParent;
	list<CHANNEL> channellist;

	// Generated message map functions
	//{{AFX_MSG(CChannelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	afx_msg void OnDblclkChannellist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterchannel();
	afx_msg void OnExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELDLG_H__84C1B52C_9B8D_4D3A_AE10_87C4805240E0__INCLUDED_)
