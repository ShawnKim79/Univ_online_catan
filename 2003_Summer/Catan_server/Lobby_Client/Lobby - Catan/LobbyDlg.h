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


//#include "BingGomanage.h"	// �̰� ����� �ų�? �Ѥ�;

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

	// �޼��� ó��
	void ProcessMessage(CMessage *pMsg);	
	
	void RemovePages();
	CString GetMyIP();

	void RecvAllRoomsInfo(CMessage *pMsg);
	//  ��� �� ���� ���� 
	void RequestAllRooms();
	// ��� �� ���� �䱸

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

	CChannelDlg* m_pChannelDlg;		// ä�� ���� ������
	CGameRoomDlg* m_pGameRoomDlg;	// ���� �κ� ������
	CProgressDlg* m_pProgressDlg;	// ���� ���� ǥ�� ������
	CChatDlg* m_pChatDlg;			// ä�� ��ȭ����

	CURPAGE m_nCurPage;				// ���� ������

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

// �ٸ� ���̸��� ���� ����.
//	void SetMyInfo(USERINFO& user);				
//	void SetcompetitorInfo(USERINFO& cuser);	
// �����ڵ�...�Ѥ�;

	CWSASocket m_ClientSocket;		// ���� ���ӿ� ����
	CUserManager m_UserManager;
	CRoomManager m_RoomManager;

	USERINFO m_MyInfo;
	USERINFO m_CompetInfo;

	USERINFOEX		m_myinfo;
	USERINFOEX		m_competitorinfo;			// ������ ����

	CString m_strID;
	CString m_strIP;

//  ���� ������ ���� �κ�...(������ ����!)

	//BingGoManage *m_pBingGo;
//	HWND m_hBingGoWnd;

// īź ������ ���� �κ�.
//	HINSTANCE hinst_CatanGame;	// Return Value to shellexcute
//	PROCESS_INFORMATION pi;
//	HWND m_hWndcatan;

	BOOL m_bExitRoom;

// �޼����� ���� 
	/*
	UINT nGAMESTART;
	UINT nLIVECHECK;

	UINT nINLIVE;
	UINT nGAMEEND;		
	UINT nGAMESTATU;

	WPARAM m_wParam;
	LPARAM m_lParam;
*/
	// Thread ��
	
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
