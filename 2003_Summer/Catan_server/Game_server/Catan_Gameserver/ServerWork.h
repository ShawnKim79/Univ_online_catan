#if !defined(AFX_SERVERWORK_H__BDC56D7D_B4D2_4181_A305_150B86D7AF0D__INCLUDED_)
#define AFX_SERVERWORK_H__BDC56D7D_B4D2_4181_A305_150B86D7AF0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerWork.h : header file
//


#include "header.h"
#include "WSASocket.h"
#include "Message.h"
#include "structs.h"
#include "TrayIcon.h"
#include "ChannelManager.h"
#include "ClientManager.h"
#include "LogDialog.h"
#include "DataBase.h"
#include "cConsol.h"
#include <list>
#include <string>

using namespace std;

#define WM_TRAYNOTIFY			(WM_USER + 10)
#define WM_SOCKETMSGCHSERVER	(WM_USER + 30)
#define WM_SOCKETMSGCLIENT		(WM_USER + 40)

#define DBPOOLSIZE			20
#define SENDLENGTH			1024

#define BASERATIO			0.03f

class CServerWork;

typedef struct
{
	int send;
	int total;
	LPBYTE dataptr;
	CURSEND sendmode;
	SOCKET socket;
}SENDDATAINFO, FAR* LPSENDDATAINFO;

typedef struct
{
	CDataBase *pDb;
	BOOL bBusy;
	BOOL bAvailable;
	int id;
}DBCONNPOOL, FAR* LPDBCONNPOOL;

typedef struct
{
	CWnd *pWnd;
	SOCKET socket;

} THREADPARAM, FAR* LPTHREADPARAM;

typedef struct
{
	CServerWork *pWnd;
	USERINFO userinfo;
	int nCode;
}QUERYPARAM, FAR* LPQUERYPARAM;

/////////////////////////////////////////////////////////////////////////////
// CServerWork window

class CServerWork : public CWnd
{
	DECLARE_DYNCREATE(CServerWork)
// Construction
public:
	CServerWork();

// Attributes
// 소켓, 서버 설정 데이터 멤버(IP. Port, 최대 접속자, 현 접속자 등등...
public:
	CTrayIcon m_TrayIcon; // Application의 아이콘.
	CChannelManager m_ChannelManager;	// 채널 서버 연결 관리.
	CClientManager m_ClientManager;		// 접속한 클라이언트 연결 관리.
										// 클라이언트들의 정보 리스트가 데이터 멤버인 클래스임.

	CLogDialog *m_pLogDialog;

	// 통신용 소켓

	CWSASocket m_ServerSocket;	// Lobby Client 연결 소켓.
								// WSAAsycSelect로 대기 상태.
	CWSASocket m_ClientSocket;	// 채널 서버 연결 소켓. (채널에게 있어서 게임 서버는 클라이언트)

	// Dialog로 부터 받아올 정보.

	// 1. IP
	string m_strChannelIP;
	string m_strGameServerIP;
	
	// 2. Port
	UINT	m_nGameServerPort;	// GameServer가 쓰는 Port
	UINT	m_nChannelPort;		// Channel 연결 Port

	UINT	m_nClientPort;		// Lobby Client 연결 Port

	UINT	m_nMaxChannelCount;	// 생성 채널 한계
	UINT	m_nMaxRoomCount;	// 생성 방 한계

	// etc... : D/B Query
	string	m_strDBID;
	string	m_strDBName;
	string	m_strDBPassword;

	// 서버가 기억하고 있어야 할 정보.
	UINT	m_nMaxUser;			// 접속자 수.
	UINT	m_nRoom;			// 방 수.
	UINT	m_nServerID;		// 서버의 아이디.

	list<DBCONNPOOL> dbconnlist;

	//list<user> UserInfo;
	//lisr<room> RoomInfo;


// Operations
// 필요한 함수들
// 1. 메세지 함수 (윈도우 메세지)
// 2. 메세지 발생시 처리할 서버 작업 함수.
// 3. 서버 시작시 기초 설정 잡을 함수.
public:
	BOOL InitServer();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerWork)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateGameState(CMessage *pMsg);
	void CancelNewGame(CMessage *pMsg);
	void SendReqNewGame(CMessage *pMsg);
	void UpdateClientInfo(USERINFO userinfo);
	void ClientLogin(USERINFO userinfo);
	void NotifyRoomUpdate(LPROOM lproom);
	void NotifyClientUpdate(LPUSERINFO lpuserinfo);
	void ClientDisconnected(SOCKET socket);
	void SendCompetitorInfo(LPROOM lproom);
	void SendErrorMessage(SOCKET socket, FAILURE failure);
	void NotifyAllUpdate(LPUSERINFO lpuserinfo, LPROOM lproom);
	void ChatMessage(CMessage *pMsg);
	void OutRoom(CMessage *pMsg);
	void JoinRoom(CMessage *pMsg);
	void MakeRoom(CMessage *pMsg);
	void SendRoomInfo(CMessage *pMsg);
	void SendUserInfo(CMessage *pMsg);
	void SendServerInfo();

	LPDBCONNPOOL GetAvailableDbconn();
	
	void ProcessMessageClient(CMessage *pMsg);
	void ProcessMessageServer(CMessage *pMsg);
	virtual ~CServerWork();

	// Generated message map functions
protected:
	// 메세지가 발생할때 불러지는 함수들...
	//{{AFX_MSG(CServerWork)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnServerstart();
	afx_msg void OnServerexit();
	afx_msg void OnServerlog();
	afx_msg void OnServersetup();
	afx_msg void OnServerstop();
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);

	// 소켓 이벤트가 일어나면 발생하는 윈도우 메세지에 반응하는 함수들...
	afx_msg long OnSocketMsgChServer(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSocketMsgClient(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERWORK_H__BDC56D7D_B4D2_4181_A305_150B86D7AF0D__INCLUDED_)
