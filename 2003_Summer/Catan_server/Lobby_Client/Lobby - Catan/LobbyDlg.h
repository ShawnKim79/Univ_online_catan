// LobbyDlg.h : header file
//

#if !defined(AFX_LOBBYDLG_H__8E6D7510_8989_4497_BFB7_2904ECB40E60__INCLUDED_)
#define AFX_LOBBYDLG_H__8E6D7510_8989_4497_BFB7_2904ECB40E60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgButton.h"
#include "ChannelDlg.h"
#include "GameRoomDlg.h"
#include "ProgressDlg.h"
#include "ChatDlg.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "WSASocket.h"
#include "Message.h"
#include "structs.h"
#include "header.h"
#include "afxwin.h"


//#include "BingGomanage.h"	// 이거 어딨는 거냐? ㅡㅡ;

typedef struct
{
	int nTotal;
	int nRead;
	SOCKET socket;

} RECVINFO, FAR* LPRECVINFO;



/////////////////////////////////////////////////////////////////////////////
// CLobbyDlg dialog

class CLobbyDlg : public CDialog
{
// Construction
public:
	void RecvAllUsersInfo(CMessage *pMsg);
	void ProcessErrorMessage(CMessage *pMsg);
	void RequestAllUsers(BOOL bSplash = TRUE);
	void SwitchToChannelPage();
	void SwitchToGamePage();
	void GetMyData(CMessage *pMsg);
	void RequestLogin();
	void ConnectToGameServer(CMessage *pMsg);
	void EnterChannel(unsigned short channelno);
	void UpdateChannelInfo(CMessage *pMsg);
	void RequestChInfo();

	// 메세지 처리
	void ProcessMessage(CMessage *pMsg);	
	
	void RemovePages();
	CString GetMyIP();

	void RecvAllRoomsInfo(CMessage *pMsg);
	//  모든 방 정보 받음 
	void RequestAllRooms();
	// 모든 방 정보 요구

	void UpdateRoomInfo(CMessage *pMsg);
	CLobbyDlg(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	//{{AFX_DATA(CLobbyDlg)
	enum { IDD = IDD_LOBBY_DIALOG };
	CImgButton	m_ctrlMinimize;
	CImgButton	m_ctrlClose;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLobbyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	enum CURPAGE { PAGE_CHANNEL, PAGE_GAMEROOM };

	CChannelDlg* m_pChannelDlg;		// 채널 선택 페이지
	CGameRoomDlg* m_pGameRoomDlg;	// 게임 로비 페이지
	CProgressDlg* m_pProgressDlg;	// 현재 상태 표시 페이지
	CChatDlg* m_pChatDlg;			// 채팅 대화상자

	CURPAGE m_nCurPage;				// 현재 페이지

	USHORT m_nChannelNo;

	LPBYTE m_pReadBuff;
	RECVINFO m_RecvInfo;

	BOOL m_bOwner;
	BOOL m_bReadyGame;
	BOOL m_bOfferGame;
	
public:
	void test(int i);
	
	void ChangeChannel();
	void DetermineToPlay(CMessage *pMsg);
	void OfferGame(LPUSERINFO lpuserinfo);

	void NewChatRejected();
	void StartNewChat();

	void NewChat(LPUSERINFO lpuserinfo);
	void UpdateAllInfo(CMessage *pMsg);
	void Exit();
	void StartGame(BOOL bServer, CMessage *pMsg);
	void GetCompetitorData(CMessage *pMsg);
	void RequestGetoutRoom();
	void UpdateChatMessage(CMessage *pMsg);
	void UpdateClientInfo(CMessage *pMsg);
	BOOL ConnectToChannelServer();

// 다른 게이머의 정보 저장.
//	void SetMyInfo(USERINFO& user);				
//	void SetcompetitorInfo(USERINFO& cuser);	
// 종희코드...ㅡㅡ;

	CWSASocket m_ClientSocket;		// 서버 접속용 소켓
	CUserManager m_UserManager;
	CRoomManager m_RoomManager;

	USERINFO m_MyInfo;
	USERINFO m_CompetInfo;

	USERINFOEX		m_myinfo;
	USERINFOEX		m_competitorinfo;			// 상대방의 정보

	CString m_strID;
	CString m_strIP;

//  빙고 게임을 위한 부분...(과감히 떼자!)

	//BingGoManage *m_pBingGo;
//	HWND m_hBingGoWnd;

// 카탄 게임을 위한 부분.
//	HINSTANCE hinst_CatanGame;	// Return Value to shellexcute
//	PROCESS_INFORMATION pi;
//	HWND m_hWndcatan;

	BOOL m_bExitRoom;

// 메세지용 변수 
	/*
	UINT nGAMESTART;
	UINT nLIVECHECK;

	UINT nINLIVE;
	UINT nGAMEEND;		
	UINT nGAMESTATU;

	WPARAM m_wParam;
	LPARAM m_lParam;
*/
	// Thread 용
	
//	CWinThread *m_pThread;

protected:
	// Generated message map functions
	//{{AFX_MSG(CLobbyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnClose();
	afx_msg void OnMinimize();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnSocketMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGameResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGameState(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadStart();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOBBYDLG_H__8E6D7510_8989_4497_BFB7_2904ECB40E60__INCLUDED_)
