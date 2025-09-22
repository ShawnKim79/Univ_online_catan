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
// ����, ���� ���� ������ ���(IP. Port, �ִ� ������, �� ������ ���...
public:
	CTrayIcon m_TrayIcon; // Application�� ������.
	CChannelManager m_ChannelManager;	// ä�� ���� ���� ����.
	CClientManager m_ClientManager;		// ������ Ŭ���̾�Ʈ ���� ����.
										// Ŭ���̾�Ʈ���� ���� ����Ʈ�� ������ ����� Ŭ������.

	CLogDialog *m_pLogDialog;

	// ��ſ� ����

	CWSASocket m_ServerSocket;	// Lobby Client ���� ����.
								// WSAAsycSelect�� ��� ����.
	CWSASocket m_ClientSocket;	// ä�� ���� ���� ����. (ä�ο��� �־ ���� ������ Ŭ���̾�Ʈ)

	// Dialog�� ���� �޾ƿ� ����.

	// 1. IP
	string m_strChannelIP;
	string m_strGameServerIP;
	
	// 2. Port
	UINT	m_nGameServerPort;	// GameServer�� ���� Port
	UINT	m_nChannelPort;		// Channel ���� Port

	UINT	m_nClientPort;		// Lobby Client ���� Port

	UINT	m_nMaxChannelCount;	// ���� ä�� �Ѱ�
	UINT	m_nMaxRoomCount;	// ���� �� �Ѱ�

	// etc... : D/B Query
	string	m_strDBID;
	string	m_strDBName;
	string	m_strDBPassword;

	// ������ ����ϰ� �־�� �� ����.
	UINT	m_nMaxUser;			// ������ ��.
	UINT	m_nRoom;			// �� ��.
	UINT	m_nServerID;		// ������ ���̵�.

	list<DBCONNPOOL> dbconnlist;

	//list<user> UserInfo;
	//lisr<room> RoomInfo;


// Operations
// �ʿ��� �Լ���
// 1. �޼��� �Լ� (������ �޼���)
// 2. �޼��� �߻��� ó���� ���� �۾� �Լ�.
// 3. ���� ���۽� ���� ���� ���� �Լ�.
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
	// �޼����� �߻��Ҷ� �ҷ����� �Լ���...
	//{{AFX_MSG(CServerWork)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnServerstart();
	afx_msg void OnServerexit();
	afx_msg void OnServerlog();
	afx_msg void OnServersetup();
	afx_msg void OnServerstop();
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);

	// ���� �̺�Ʈ�� �Ͼ�� �߻��ϴ� ������ �޼����� �����ϴ� �Լ���...
	afx_msg long OnSocketMsgChServer(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSocketMsgClient(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERWORK_H__BDC56D7D_B4D2_4181_A305_150B86D7AF0D__INCLUDED_)
