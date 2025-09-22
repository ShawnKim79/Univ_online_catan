#if !defined(AFX_GAMEROOMDLG_H__3AADB0DB_5E1D_41D6_8427_7BEA76C49FB6__INCLUDED_)
#define AFX_GAMEROOMDLG_H__3AADB0DB_5E1D_41D6_8427_7BEA76C49FB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameRoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGameRoomDlg dialog

#include "structs.h"
#include "ImgButton.h"

class CLobbyDlg;

class CGameRoomDlg : public CDialog
{
// Construction
public:
	void SetMyInfo(USERINFO userinfo);
	void AddChatMessage(CString strChatMsg);
	void SendChatMessage();
	void UpdateClientList(USERINFO userinfo);
	void UpdateRoomList(ROOMINFO roominfo);
	void AddRoom(ROOMINFO roominfo);
	void AddUser(int nIndex, USERINFO userinfo);
	void InsertColumn();
	CGameRoomDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_strChat;

// Dialog Data
	//{{AFX_DATA(CGameRoomDlg)
	enum { IDD = IDD_GAMEROOM_DIALOG };
	CImageList	m_ImageList;
	CImgButton	m_ctrlRefresh;
	CImgButton	m_ctrlExit;
	CImgButton	m_ctrlChgChannel;
	CImgButton	m_ctrlEnterRoom;
	CImgButton	m_ctrlPersonalInfo;
	CImgButton	m_ctrlOfferGame;
	CListCtrl	m_ctrlRoomList;
	CListCtrl	m_ctrlUsersList;
	CRichEditCtrl	m_ctrlViewChat;
	CEdit	m_ctrlChat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameRoomDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CLobbyDlg *m_pParent;
	USERINFO userinfo;

	// Generated message map functions
	//{{AFX_MSG(CGameRoomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkRoomlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefresh();
	afx_msg void OnPersonalinfo();
	afx_msg void OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExit();
	afx_msg void OnOffergame();
	afx_msg void OnChangechannel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void EnterSelectedRoom();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEROOMDLG_H__3AADB0DB_5E1D_41D6_8427_7BEA76C49FB6__INCLUDED_)
