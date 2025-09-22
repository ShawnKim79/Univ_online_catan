#if !defined(AFX_KATANPLAY_H__DCE0EDAA_ECC2_4201_A3B5_8A2E920DEE2F__INCLUDED_)
#define AFX_KATANPLAY_H__DCE0EDAA_ECC2_4201_A3B5_8A2E920DEE2F__INCLUDED_

#include "block_inf.h"
#include "player_inf.h"
#include "tile_inf.h"
#include "ListenSocket.h"
#include "Client.h"
#include "NoEditView.h"
#include "WaitRoomDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KatanPlay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKatanPlay dialog

class CKatanPlay : public CDialog
{
// Construction
public:
	void set_rgn();
	int randdice();
	void set_longest_road(int new_road,int old_road);
	void draw_bmp(CDC *pDC, CBitmap *bitmap,int x,int y,int sizex,int sizey,int m_r);
	void load_bmp(CBitmap *bitmap, char *filename);
	void call_dice();
	void add_chat(int n,char *message);
	void player_check();
	int player_add(CString name,bool pn=true);
	void con_enable(bool e);
	void load_bone_map();
	void draw_map();
	void load_map();

	CString ip_address;
	CNoEditView viewer[2];

	char name[100];
	bool server;
	bool server_use;
	bool trade;
	bool connect_ok;

	CWinThread* bgThread;				// 배경음 쓰레드
	int bgindex;						// 배경음 flag
	int wait_picture;					// 게임시작 기다림 그림
	
	CBitmap view_image;
	CBitmap bone_image;

	CPlayer_Inf player;
	
	CClient Clientx;					// 클라이언트관리 소켓

	CKatanPlay(CWnd* pParent = NULL);   // standard constructor
	Cblock_inf g_map;

// Dialog Data
	//{{AFX_DATA(CKatanPlay)
	enum { IDD = IDD_KATAN_PLAY };
	CEdit	m_chat_input;
	CListCtrl	m_playerlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKatanPlay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKatanPlay)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnRoadBtn();
	afx_msg void OnSettBtn();
	afx_msg void OnCityBtn();
	afx_msg void OnCardBtn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChatBtn();
	afx_msg void OnTurnBtn();
	afx_msg void OnCardUseBtn();
	afx_msg void OnTradeBtn();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnQuitBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KATANPLAY_H__DCE0EDAA_ECC2_4201_A3B5_8A2E920DEE2F__INCLUDED_)
