#if !defined(AFX_DUMMYWND_H__43A11F23_8673_4DE4_9E9C_F1E788C84CDC__INCLUDED_)
#define AFX_DUMMYWND_H__43A11F23_8673_4DE4_9E9C_F1E788C84CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummyWnd.h : header file
//

#include "structs.h"
#include "header.h"
#include "TrayIcon.h"
#include "Database.h"
#include "WSASocket.h"
#include "Message.h"
#include "ChannelManager.h"
#include "ClientManager.h"
#include "cConsol.h"
#include <list>
#include <string>

using namespace std;

#define WM_TRAYNOTIFY		(WM_USER + 10)
#define DBPOOLSIZE			20
#define SENDLENGTH			1024

#define BASERATIO			0.03f

class CDummyWnd;

typedef struct
{
	CDataBase *pDb;
	BOOL bBusy;
	BOOL bAvailable;
	int id;

} DBCONNPOOL, FAR* LPDBCONNPOOL;


typedef struct
{
	CWnd *pWnd;
	SOCKET socket;

} THREADPARAM, FAR* LPTHREADPARAM;

typedef struct
{
	CDummyWnd *pWnd;
	USERINFO userinfo;
	int nCode;

} QUERYPARAM, FAR *LPQUERYPARAM;


typedef struct
{
	int sent;
	int total;
	LPBYTE dataptr;
	CURSEND sendmode;
	SOCKET socket;

} SENDDATAINFO, FAR* LPSENDDATAINFO;


/////////////////////////////////////////////////////////////////////////////
// CDummyWnd frame

class CDummyWnd : public CWnd
{
	DECLARE_DYNCREATE(CDummyWnd)
public:
	CDummyWnd();           // protected constructor used by dynamic creation

// Attributes
public:
	CTrayIcon m_TrayIcon;
	CWSASocket m_ServerSocket;				// Lobby의 Connection을 담당하는 소켓
	CWSASocket m_ClientSocket;				// 채널서버에 접속할 소켓
	CChannelManager m_ChannelManager;
	CClientManager m_ClientManager;
	cConsol m_Console;

	UINT m_nMyPort;
	UINT m_nChSvrPort;
	UINT m_nClientPort;

	UINT m_nServerID;
	UINT m_nChannels;
	UINT m_nRooms;
	UINT m_nMaxUsers;

	string m_strMyIP;
	string m_strChServerIP;
	string m_strDSN;
	string m_strDBID;
	string m_strDBPW;

	list<DBCONNPOOL> dbconnlist;

// Operations
public:
	void SendErrorMessage(SOCKET socket, FAILURE failure);
	LPDBCONNPOOL GetAvailableDbConn();
	void ClientLogin(USERINFO userinfo);
	void SendServerInfo();
	BOOL InitServer();
	void ProcessMessageServer(CMessage *pMsg);
	void ProcessMessageClient(CMessage *pMsg);

public:
	void UpdateGameState(CMessage *pMsg);
	void CancelNewGame(CMessage *pMsg);
	void SendReqNewGame(CMessage *pMsg);
	void SendReqNewChat(CMessage *pMsg);
	void UpdateClientInfo(USERINFO userinfo);
	void SendCompetitorInfo(LPROOM lproom);
	void GetoutRoom(CMessage *pMsg);
	void UpdateChatMessage(CMessage *pMsg);
	void ClientDisconnected(SOCKET socket);
	void NotifyClientUpdate(LPUSERINFO lpuserinfo);
	void NotifyRoomUpdate(LPROOM lproom);
	void NotifyAllUpdate(LPUSERINFO lpuserinfo, LPROOM lproom);
	void JoinRoom(CMessage *pMsg);
	void MakeRoom(CMessage *pMsg);
	void SendAllRoomsInfo(CMessage *pMsg);
	void SendAllUsersInfo(CMessage *pMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDummyWnd();

	// Generated message map functions
	//{{AFX_MSG(CDummyWnd)
	afx_msg void OnExit();
	afx_msg void OnServerconfig();
	afx_msg void OnStatistics();
	afx_msg void OnStopserver();
	afx_msg void OnStartserver();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSocketMsgChServer(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSocketMsgClient(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYWND_H__43A11F23_8673_4DE4_9E9C_F1E788C84CDC__INCLUDED_)
