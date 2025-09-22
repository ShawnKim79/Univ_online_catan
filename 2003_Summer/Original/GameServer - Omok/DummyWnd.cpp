// DummyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "GameServer.h"
#include "DummyWnd.h"
#include "process.h"
#include "afxmt.h"
#include "GameServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection g_Critical;

unsigned __stdcall threadSQLQuery(LPVOID arg);

/////////////////////////////////////////////////////////////////////////////
// CDummyWnd

IMPLEMENT_DYNCREATE(CDummyWnd, CWnd)

CDummyWnd::CDummyWnd()
{
	m_nServerID = 0;
	m_nMaxUsers = 2;

////////////////////////////////////////////
// DB Connection Pool 생성 기본 DBPOOLSIZE 개

	for(int i = 0 ; i < DBPOOLSIZE ; i++)
	{
		DBCONNPOOL dbconn;

		memset(&dbconn, 0, sizeof(DBCONNPOOL));

		dbconnlist.push_back(dbconn);
	}
}
  
CDummyWnd::~CDummyWnd()
{
	delete this;
}


BEGIN_MESSAGE_MAP(CDummyWnd, CWnd)
	//{{AFX_MSG_MAP(CDummyWnd)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_SERVERCONFIG, OnServerconfig)
	ON_COMMAND(ID_STATISTICS, OnStatistics)
	ON_COMMAND(ID_STOPSERVER, OnStopserver)
	ON_COMMAND(ID_STARTSERVER, OnStartserver)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotify)
	ON_MESSAGE(WM_SOCKETMSGCHSERVER, OnSocketMsgChServer)
	ON_MESSAGE(WM_SOCKETMSGCLIENT, OnSocketMsgClient)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyWnd message handlers

int CDummyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 트레이 아이콘 생성
	if(!m_TrayIcon.Create(this, WM_TRAYNOTIFY, "Omok 게임 서버", LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), IDC_TRAYICON))
	{
		return -1;
	}

	m_Console.SetConsoleTitle("Omok Server...");

	CGameServerDlg dlg;

	if(!dlg.LoadSettings())
	{
		while(dlg.DoModal() == IDOK)
		{
			if(dlg.SaveSettings()) break;
		}
	}

	m_strMyIP = dlg.m_strGSAddr;
	m_nMyPort = dlg.m_nGSPortNo;

	m_strChServerIP = dlg.m_strCSAddr;
	m_nChSvrPort = dlg.m_nCSPortNo;

	m_strDSN = dlg.m_strDSN;
	m_strDBID = dlg.m_strDBID;
	m_strDBPW = dlg.m_strDBPW;

	m_nChannels = dlg.m_nChannel;
	m_nRooms = dlg.m_nRoom;

	// 서버와 통신할 소켓 초기화 
	if(!m_ServerSocket.InitSock()) return -1; 
	if(!m_ServerSocket.Initialize(m_nMyPort))
	{
		AfxMessageBox("서버 소켓 초기화에 실패했습니다.");
		return 0;
	}

	return 0;
}


LRESULT CDummyWnd::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
		case WM_RBUTTONDOWN :

			CMenu Menu;
			CMenu *pSub;
			
			Menu.LoadMenu(IDR_MENU);
			pSub = Menu.GetSubMenu(0);
			
			CPoint p;
			GetCursorPos(&p);
			pSub->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

			break;
	}

	return 0;
}

BOOL CDummyWnd::InitServer()
{
	m_Console.writeConsole("*** InitServer ***\n");
	m_ChannelManager.InitializeChannel(m_nServerID, m_nChannels, m_nRooms, m_nMaxUsers);

	return TRUE;
}

void CDummyWnd::OnStartserver() 
{
	InitServer();

	if(!m_ClientSocket.Connect((LPSTR)m_strChServerIP.data(), m_nChSvrPort))
	{
		AfxMessageBox("채널 서버 접속에 실패했습니다.");
		return;
	}

	m_Console.writeConsole("*** Succeeded to Connect to Channel Server ***\n");

	WSAAsyncSelect(m_ClientSocket.m_Socket, m_hWnd, WM_SOCKETMSGCHSERVER, FD_READ | FD_CLOSE);

	m_Console.writeConsole("*** Start Listen ***\n");

	m_ServerSocket.Listen();
	WSAAsyncSelect(m_ServerSocket.m_Socket, m_hWnd, WM_SOCKETMSGCLIENT, FD_ACCEPT | FD_CLOSE);
}

void CDummyWnd::OnStopserver() 
{
	m_ChannelManager.ClearChannel();
	PostQuitMessage(0);
}

void CDummyWnd::OnServerconfig() 
{
	CGameServerDlg dlg;

	dlg.LoadSettings();

	while(dlg.DoModal() == IDOK)
	{
		if(dlg.SaveSettings()) break;
	}
}

void CDummyWnd::OnStatistics() 
{
}

void CDummyWnd::OnExit() 
{
	list<DBCONNPOOL>::iterator iter = dbconnlist.begin();

	for(; iter != dbconnlist.end() ; iter++)
	{
		LPDBCONNPOOL lpdbpool = &(*iter);

		if(lpdbpool->pDb != NULL) 
		{
			lpdbpool->pDb->Disconnect();
			delete lpdbpool->pDb;

			lpdbpool = NULL;
		}
	}

	m_ChannelManager.ClearChannel();

	m_TrayIcon.RemoveIcon();
	PostQuitMessage(0);
}

long CDummyWnd::OnSocketMsgChServer(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;
	
	CMessage Recv;
	COMMON_PACKET cmnpck;

	memset(&cmnpck, 0, sizeof(cmnpck));

	int nRead1 = 0;
	int nRead2 = 0;
	
	LPBYTE readbuff = NULL;

	DWORD dwError = 0;

	if(WSAGETSELECTEVENT(lParam) == FD_READ)
	{
		nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);
		
		if(nRead2 < (int)sizeof(cmnpck))
		{
			if(nRead2 < 0)
			{
				nRead2 = 0;
				nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);
				
				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			
			else
			{
				nRead1 = recv(socket, (char *)&cmnpck + nRead2, sizeof(cmnpck) - nRead2, 0);
				
				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			
			if(nRead1 < 0) return 0L;
		}
		
		readbuff = new BYTE[cmnpck.length];
		memset(readbuff, 0, sizeof(readbuff));
		memcpy(readbuff, &cmnpck, sizeof(cmnpck));
		
		while(nRead2 < cmnpck.length)
		{	
			nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
			
			if(nRead1 < 0)
			{
				nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
				
				if(nRead1 > 0) nRead2 += nRead1;
			}
			
			else nRead2 += nRead1;
			
			dwError = WSAGetLastError();
			
			if(dwError == WSAESHUTDOWN || dwError == WSAECONNRESET)
				break;
			
			else if(dwError == WSAEWOULDBLOCK)
			{
				unsigned long ByteToRead;
				ioctlsocket(socket, FIONREAD, &ByteToRead);
				
				if(!ByteToRead) return 0L;
			}
			
			m_Console.writeConsole("nRead2(%d) < cmnpck.length(%d)\n", nRead2, cmnpck.length);
		}

		Recv.Clear();
		Recv.SetSocket((DWORD)socket);
		Recv.SetData(readbuff, 0, nRead2);

		delete [] readbuff;

		ProcessMessageServer(&Recv);
	}

	else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{

	}

	return 0L;
}


long CDummyWnd::OnSocketMsgClient(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;

	CMessage Recv;

	DWORD dwError = 0;

	LPBYTE readbuff;
	
	int nRead1 = 0;
	int nRead2 = 0;
	
	COMMON_PACKET cmnpck;

	memset(&cmnpck, 0, sizeof(cmnpck));

	if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)
	{
		SOCKET clientsocket = m_ServerSocket.Accept();  // 접속 요청이 있을 때까지 블록

		if(socket == INVALID_SOCKET) return 0L;		// 잘못된 소켓이면 다시 접속 대기

		m_ClientManager.AddClient(clientsocket);

		WSAAsyncSelect(clientsocket, m_hWnd, WM_SOCKETMSGCLIENT, FD_READ | FD_CLOSE);

		CMessage Send;

		cmnpck.header = GSM_CONNECTED;
		cmnpck.length = sizeof(COMMON_PACKET);
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		m_ClientSocket.SendMsg(clientsocket, (char *)Send.GetDataPtr(), Send.GetLength());
	}

	else if(WSAGETSELECTEVENT(lParam) == FD_READ)
	{
		nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);
		
		if(nRead2 < (int)sizeof(cmnpck))
		{
			if(nRead2 < 0)
			{
				nRead2 = 0;
				nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);
				
				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			
			else
			{
				nRead1 = recv(socket, (char *)&cmnpck + nRead2, sizeof(cmnpck) - nRead2, 0);
				
				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			
			if(nRead1 < 0) return 0L;
		}
		
		readbuff = new BYTE[cmnpck.length];
		memset(readbuff, 0, sizeof(readbuff));
		memcpy(readbuff, &cmnpck, sizeof(cmnpck));
		
		while(nRead2 < cmnpck.length)
		{
			Sleep(10);
			
			nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
			
			if(nRead1 < 0)
			{
				nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
				
				if(nRead1 > 0) nRead2 += nRead1;
			}
			
			else nRead2 += nRead1;
			
			dwError = WSAGetLastError();
			
			if(dwError == WSAESHUTDOWN || dwError == WSAECONNRESET)
				return 0L;
			
			else if(dwError == WSAEWOULDBLOCK)
			{
				unsigned long ByteToRead;
				ioctlsocket(socket, FIONREAD, &ByteToRead);
				
				if(!ByteToRead) return 0L;
			}
			
			m_Console.writeConsole("nRead2(%d) < cmnpck.length(%d)\n", nRead2, cmnpck.length);
		}
		
		Recv.Clear();
		Recv.SetSocket((DWORD)socket);
		Recv.SetData(readbuff, 0, nRead2);
		
		delete [] readbuff;

		ProcessMessageClient(&Recv);
	}

	else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{
		CString str;
		str.Format("%d closed\n", socket);
		m_Console.writeConsole((LPSTR)(LPCTSTR)str);
		ClientDisconnected(socket);
	}

	return  0L;
}


void CDummyWnd::ProcessMessageServer(CMessage *pMsg)
{
	CMessage Send;
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header)
	{
		case CSM_CONNECTED : case CSM_REQCHINFO :
			SendServerInfo();
			break;
	}
}



void CDummyWnd::ProcessMessageClient(CMessage *pMsg)
{
	CMessage Send;
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header)
	{
		case CM_REQLOGIN :
			{
				USERINFO userinfo;
				int nLen = sizeof(COMMON_PACKET);

				UINT ThreadID;
				LPQUERYPARAM qp = new QUERYPARAM;

				nLen += pMsg->GetData(&userinfo.id, nLen, sizeof(userinfo.id));
				nLen += pMsg->GetData(&userinfo.channelno, nLen, sizeof(userinfo.channelno));
				pMsg->GetData(userinfo.ip, nLen, sizeof(userinfo.ip));

				userinfo.socket = (SOCKET)pMsg->GetSocket();

				qp->pWnd = this;
				qp->nCode = CM_REQLOGIN;
				qp->userinfo = userinfo;

				_beginthreadex(NULL, 0, threadSQLQuery, qp, 0, &ThreadID);
			}
			break;

		case CM_REQALLUSERS :
			SendAllUsersInfo(pMsg);
			break;

		case CM_REQALLROOMS :
			SendAllRoomsInfo(pMsg);
			break;

		case CM_REQMAKEROOM :
			MakeRoom(pMsg);
			break;

		case CM_REQJOINROOM :
			JoinRoom(pMsg);
			break;

		case CM_REQGETOUTROOM :
			GetoutRoom(pMsg);
			break;

		case CM_CHATMESSAGE :
			UpdateChatMessage(pMsg);
			break;

		case CM_UPDATEINFO :
			{
				UINT ThreadID;
				USERINFO userinfo;
				LPQUERYPARAM qp = new QUERYPARAM;

				pMsg->GetData(&userinfo, sizeof(COMMON_PACKET), sizeof(userinfo));

				userinfo.socket = (SOCKET)pMsg->GetSocket();

				qp->pWnd = this;
				qp->nCode = CM_UPDATEINFO;
				qp->userinfo = userinfo;

				_beginthreadex(NULL, 0, threadSQLQuery, qp, 0, &ThreadID);
			}
			break;

		case CM_REQNEWCHAT :
			SendReqNewChat(pMsg);
			break;

		case CM_REQNEWGAME :
			SendReqNewGame(pMsg);
			break;

		case CM_REJECTNEWGAME : 
			CancelNewGame(pMsg);
			break;

		case CM_GAMESTATE :
			UpdateGameState(pMsg);
			break;
	}
}


void CDummyWnd::SendServerInfo()
{
	SERVERINFO si;
	CHANNELINFO ci;
	CMessage Send;
				
	si.cmnpck.header = GSM_SERVERINFO;
				
	si.id = m_nServerID;
	memset(si.ip, 0, sizeof(si.ip));
	memcpy(si.ip, m_strMyIP.data(), m_strMyIP.size());
	si.port = m_nMyPort;
	si.channels = m_nChannels;
	si.rooms = m_nRooms;

	si.cmnpck.length = sizeof(si) + sizeof(ci) * m_nChannels;

	int nLen = Send.SetData(&si, 0, sizeof(si));

	UINT maximum = m_nRooms * m_nMaxUsers;

	for(int i = 0 ; i < m_nChannels ; i++)
	{
		LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(i);

		ci.channelno = lpchannel->id;
		ci.maximum = lpchannel->maximum;
		ci.present = lpchannel->present;

		nLen = Send.SetData(&ci, nLen, sizeof(ci));
	}

	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
}


LPDBCONNPOOL CDummyWnd::GetAvailableDbConn()
{
	g_Critical.Lock();

	list<DBCONNPOOL>::iterator iter = dbconnlist.begin();

	int nCount = 0;

///////////////////////////////////////////////////////////
// 이미 만들어진 Connection 객체 중 사용중이 아닌 것을 찾음

	for(; iter != dbconnlist.end() ; iter++)
	{
		LPDBCONNPOOL lpdbconn = &(*iter);

		if(lpdbconn->pDb != NULL && !lpdbconn->bBusy)
		{
			lpdbconn->pDb->Clear();
			lpdbconn->bBusy = TRUE;
			g_Critical.Unlock();

			return lpdbconn;
		}
	}

	iter = dbconnlist.begin();

///////////////////////////////////////////////////////////
// 아직 만들어진 것이 없으면 새로 생성

	for(; iter != dbconnlist.end() ; iter++)
	{
		LPDBCONNPOOL lpdbconn = &(*iter);

		if(lpdbconn->pDb == NULL)
		{
			CDataBase *pDbConn = new CDataBase;

			if(!pDbConn->Connect(3, (LPSTR)m_strDSN.data(), (LPSTR)m_strDBID.data(), (LPSTR)m_strDBPW.data())) 
			{
				// 연결 실패했을 경우 NULL 리턴
				delete pDbConn;
				g_Critical.Unlock();

				return NULL;
			}

			else
			{
				lpdbconn->id = nCount;
				lpdbconn->pDb = pDbConn;
				lpdbconn->pDb->Clear();
				lpdbconn->bBusy = TRUE;

				g_Critical.Unlock();

				return lpdbconn;
			}
		}

		nCount++;
	}

//////////////////////////////////////////////////////////////
// 모두 사용중일 경우 새로 Connection 객체 생성 후 POOL에 추가

	DBCONNPOOL newdbconn;

	newdbconn.bBusy = TRUE;
	newdbconn.pDb = new CDataBase;

	if(!newdbconn.pDb->Connect(3, (LPSTR)m_strDSN.data(), (LPSTR)m_strDBID.data(), (LPSTR)m_strDBPW.data()))
	{
		delete newdbconn.pDb;
		g_Critical.Unlock();
		
		return NULL;
	}

	dbconnlist.push_back(newdbconn);

	g_Critical.Unlock();

	return NULL;
}

void CDummyWnd::SendErrorMessage(SOCKET socket, FAILURE failure)
{
	CMessage Send;

	COMMON_PACKET cmnpck;

	cmnpck.header = GSM_FAILURE;
	cmnpck.length = sizeof(cmnpck) + sizeof(failure);
	
	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&failure, sizeof(cmnpck), sizeof(failure));
	
	m_ClientSocket.SendMsg(socket, (char *)Send.GetDataPtr(), Send.GetLength());
}

void CDummyWnd::SendAllUsersInfo(CMessage *pMsg)
{
	CMessage Send;

	g_Critical.Lock();

	list<USER>::iterator iter = m_ClientManager.clientlist.begin();
	LPUSER lpmyinfo = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());

	int nBuffSize;

	nBuffSize = sizeof(COMMON_PACKET) + sizeof(USERINFO) * m_ClientManager.clientlist.size();

	LPBYTE buff = new BYTE[nBuffSize];

	memset(buff, 0, nBuffSize);

	COMMON_PACKET cmnpck;
	
	cmnpck.header = GSM_ALLUSERSINFO;

	int cnt = sizeof(cmnpck);
	
	for(; iter != m_ClientManager.clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(lpmyinfo->channelno == lpuser->channelno) // 같은 채널에 있는 사람의 정보만 보냄
		{
			memcpy(&buff[cnt], lpuser, sizeof(USERINFO));
			cnt += sizeof(USERINFO);
		}
	}

	cmnpck.length = cnt;
	memcpy(buff, &cmnpck, sizeof(cmnpck));

	m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)buff, cnt);

	g_Critical.Unlock();

	delete [] buff;
}

void CDummyWnd::SendAllRoomsInfo(CMessage *pMsg)
{
	int nBuffSize;
	unsigned short nChannelNo;

	g_Critical.Lock();

	pMsg->GetData(&nChannelNo, sizeof(COMMON_PACKET), sizeof(nChannelNo));

	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(nChannelNo);

	list<ROOM>::iterator iter = lpchannel->roomlist.begin();

	nBuffSize = sizeof(COMMON_PACKET) + sizeof(ROOMINFO) * lpchannel->roomlist.size();

	LPBYTE buff = new BYTE[nBuffSize];

	memset(buff, 0, nBuffSize);

	COMMON_PACKET cmnpck;
	
	cmnpck.header = GSM_ALLROOMSINFO;
	
	int cnt = sizeof(cmnpck);
	
	for(; iter != lpchannel->roomlist.end() ; iter++)
	{
		LPROOM lproom = &(*iter);
		
		memcpy(&buff[cnt], lproom, sizeof(ROOMINFO));
		cnt += sizeof(ROOMINFO);
	}
	
	cmnpck.length = nBuffSize;

	memcpy(buff, &cmnpck, sizeof(cmnpck));
	
	m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)buff, nBuffSize);

//	m_Console.writeConsole2("Sending RoomInfo... Total : %d, Sent : %d\n", nTotalLen, nSent);

	g_Critical.Unlock();

	delete [] buff;
}

void CDummyWnd::MakeRoom(CMessage *pMsg)
{
	m_Console.writeConsole("\n*** MakeRoom ***\n\n");

	int nLen = sizeof(COMMON_PACKET);

	SOCKET socket = (SOCKET)pMsg->GetSocket();
	ROOMINFO roominfo;

	pMsg->GetData(&roominfo, nLen, sizeof(roominfo));

	g_Critical.Lock();

	LPUSER lpuser = m_ClientManager.GetClient(socket);
	LPROOM lproom = m_ChannelManager.GetRoomInfo(lpuser->channelno, roominfo.id);
	
	if(lproom != NULL)
	{		
		if(lproom->present == 0) // 아무도 입장하지 않았으면
		{
			COMMON_PACKET cmnpck;

			cmnpck.header = GSM_OWNTHISROOM; // 방장이 됐음..
			cmnpck.length = sizeof(cmnpck);

			m_ClientSocket.SendMsg(socket, (char *)&cmnpck, sizeof(cmnpck));
			m_Console.writeConsole("::: GSM_OWNTHISROOM (id : %s)\n", (LPCTSTR)lpuser->id);
			

			lpuser->lproom = lproom;		// 입장한 방 포인터
			lpuser->roomno = roominfo.id;	// 입장한 방 번호
			
			strcpy((char *)lproom->title, (char *)roominfo.title);
			lproom->present++;
			lproom->users[0] = lpuser;
			lproom->locked = roominfo.locked;
			strcpy((char *)lproom->passwd, (char *)roominfo.passwd);
			NotifyAllUpdate((LPUSERINFO)lpuser, lproom);
		}
		
		else 
		{
			SendErrorMessage((SOCKET)pMsg->GetSocket(), F_ROOMOCCUPIED);
		}
	}

	g_Critical.Unlock();
}

void CDummyWnd::JoinRoom(CMessage *pMsg)
{
	m_Console.writeConsole("\n*** JoinRoom ***\n\n");
	short roomno;

	g_Critical.Lock();

	LPUSER lpuser = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	pMsg->GetData(&roomno, sizeof(COMMON_PACKET), sizeof(roomno));
	
	LPROOM lproom = m_ChannelManager.GetRoomInfo(lpuser->channelno, roomno);
	
	if(lproom != NULL)
	{
		if(lproom->present == 0)
		{
			SendErrorMessage((SOCKET)pMsg->GetSocket(), F_JOINROOM);
		}

		else if(lproom->present < lproom->maximum)
		{
			lproom->present++;
			lproom->users[1] = lpuser;
			lproom->status = 1;

			lpuser->roomno = roomno;
			lpuser->lproom = lproom;

			COMMON_PACKET cmnpck;

			SendCompetitorInfo(lproom);
			m_Console.writeConsole("SendCompetitorInfo()\n");

			cmnpck.header = GSM_JOINTHISROOM; // 입장 됐음..
			cmnpck.length = sizeof(cmnpck);

			m_ClientSocket.SendMsg(lpuser->socket, (char *)&cmnpck, sizeof(cmnpck));
			m_Console.writeConsole("::: GSM_JOINTHISROOM (id : %s, compet id : %s)\n", (LPCTSTR)lpuser->id, (LPCTSTR)lproom->users[0]->id);

			NotifyAllUpdate((LPUSERINFO)lpuser, lproom);
		}
	}

	g_Critical.Unlock();
}

void CDummyWnd::GetoutRoom(CMessage *pMsg)
{
	m_Console.writeConsole("\n*** GetoutRoom ***\n\n");

	g_Critical.Lock();

	LPUSER lpuser = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	LPROOM lproom = lpuser->lproom;

	if(lproom != NULL)
	{
		lproom->present--;

		lpuser->lproom = NULL;
		lpuser->roomno = -1;
		lproom->status = 0;
		lproom->locked = 0;

		if(lproom->present == 0) // 방에 아무도 없음
		{
			lproom->users[0] = NULL;
			lproom->users[1] = NULL;

			memset(lproom->title, 0, sizeof(lproom->title));
			memset(lproom->passwd, 0, sizeof(lproom->passwd));

			strcpy((char *)lproom->title, "빈 방 입니다.");
		}
		
		else
		{
			if(lproom->users[0] == lpuser) // 방장 일 경우
				lproom->users[0] = lproom->users[1];

			else if(lproom->users[1] == lpuser)
				lproom->users[1] = NULL;

			memset(lproom->title, 0, sizeof(lproom->title));
			memset(lproom->passwd, 0, sizeof(lproom->passwd));

			strcpy((char *)lproom->title, "들어오세요.");

			COMMON_PACKET cmnpck;
			
			cmnpck.header = GSM_OWNTHISROOM; // 방장 됐음..
			cmnpck.length = sizeof(cmnpck);
			
			m_ClientSocket.SendMsg(lproom->users[0]->socket, (char *)&cmnpck, sizeof(cmnpck));
			m_Console.writeConsole("::: GSM_OWNTHISROOM (id : %s)\n", (LPCTSTR)lproom->users[0]->id);
		}

		NotifyAllUpdate((LPUSERINFO)lpuser, lproom);
	}

	g_Critical.Unlock();
}

void CDummyWnd::NotifyAllUpdate(LPUSERINFO lpuserinfo, LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	USERINFO userinfo;
	ROOMINFO roominfo;

	g_Critical.Lock();

	list<USER>::iterator iter = m_ClientManager.clientlist.begin();

	cmnpck.header = GSM_ALLUPDATED;
	cmnpck.length = sizeof(cmnpck) + sizeof(userinfo) + sizeof(ROOMINFO);

	memcpy(&userinfo, lpuserinfo, sizeof(userinfo));
	memcpy(&roominfo, lproom, sizeof(roominfo));

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&userinfo, sizeof(cmnpck), sizeof(userinfo));
	Send.SetData(&roominfo, sizeof(cmnpck) + sizeof(userinfo), sizeof(roominfo));

	for(; iter != m_ClientManager.clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(lpuser->channelno == lproom->channelno)
		{
			int nResult = m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

			m_Console.writeConsole("AllUpdate : %s\n", (LPCTSTR)lpuser->id);
		}
	}

	g_Critical.Unlock();
}

void CDummyWnd::NotifyRoomUpdate(LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	ROOMINFO roominfo;

	g_Critical.Lock();

	list<USER>::iterator iter = m_ClientManager.clientlist.begin();

	cmnpck.header = GSM_ROOMUPDATED;
	cmnpck.length = sizeof(cmnpck) + sizeof(ROOMINFO);

	memcpy(&roominfo, lproom, sizeof(roominfo));

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&roominfo, sizeof(cmnpck), sizeof(roominfo));

	for(; iter != m_ClientManager.clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(lpuser->channelno == lproom->channelno)
		{
			int nResult = m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

			m_Console.writeConsole("RoomUpdate : %s\n", (LPCTSTR)lpuser->id);
		}
	}

	g_Critical.Unlock();
}



void CDummyWnd::NotifyClientUpdate(LPUSERINFO lpuserinfo)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	USERINFO userinfo;

	g_Critical.Lock();

	list<USER>::iterator iter = m_ClientManager.clientlist.begin();

	cmnpck.header = GSM_CLIENTUPDATED;
	cmnpck.length = sizeof(cmnpck) + sizeof(USERINFO);

	memcpy(&userinfo, lpuserinfo, sizeof(userinfo));

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&userinfo, sizeof(cmnpck), sizeof(userinfo));

//	m_Console.writeConsole("******\n");

	for(; iter != m_ClientManager.clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(lpuser->channelno == lpuserinfo->channelno)
		{
			int nResult = m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());
//			m_Console.writeConsole1("%d\n", lpuser->socket);
		}
	}

//	m_Console.writeConsole("******\n\n");

	g_Critical.Unlock();
}

void CDummyWnd::ClientDisconnected(SOCKET socket)
{
	CMessage Send;
	COMMON_PACKET cmnpck;

	g_Critical.Lock();

	LPUSER lpuser = m_ClientManager.GetClient(socket); // 접속 해제된 클라이언트 검색
	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);
	LPROOM lproom = lpuser->lproom;

	if(strlen((char *)lpuser->id) > 0)
	{
		lpchannel->present--;

		lpuser->roomno = -2; // 접속 해제한 클라이언트 표시
		NotifyClientUpdate((LPUSERINFO)lpuser);
	}

	if(lpuser->lproom != NULL) // 방에 들어있었을 경우
	{
		if(lproom->status == 2) // 게임 중에 나갔을 경우
		{
			if(lproom->users[0] == lpuser) // 방장이 접속 해제됐을 경우
			{
				lproom->users[0]->playcount++;
				lproom->users[0]->losscount++;

				lproom->users[1]->playcount++;
				lproom->users[1]->victorycount++;

				lproom->users[1]->score += ( lproom->users[0]->score * BASERATIO );
				lproom->users[0]->score -= ( lproom->users[0]->score * BASERATIO );
			}

			else
			{
				lproom->users[1]->playcount++;
				lproom->users[1]->losscount++;

				lproom->users[0]->playcount++;
				lproom->users[0]->victorycount++;

				lproom->users[0]->score += ( lproom->users[1]->score * BASERATIO );
				lproom->users[1]->score -= ( lproom->users[1]->score * BASERATIO );
			}

			UpdateClientInfo(*(LPUSERINFO)(lproom->users[0]));
			UpdateClientInfo(*(LPUSERINFO)(lproom->users[1]));
		}

		lproom->present--;
		lproom->status = 0;

		if(lproom->present > 0) // 방장 위임
		{
			lproom->users[0] = lproom->users[1];

			cmnpck.header = GSM_OWNTHISROOM;
			cmnpck.length = sizeof(cmnpck);

			m_ClientSocket.SendMsg(lproom->users[0]->socket, (char *)&cmnpck, sizeof(cmnpck));
		}

		else
		{
			lproom->users[0] = NULL;
			lproom->users[1] = NULL;

			memset(lproom->passwd, 0, sizeof(lproom->passwd));
			memset(lproom->title, 0, sizeof(lproom->title));

			strcpy((char *)lproom->title, "빈 방 입니다.");

			lproom->locked = 0;
			lproom->present = 0;
			lproom->status = 0;
		}

		m_ClientManager.DeleteClient(socket);
		NotifyRoomUpdate(lproom);
	}

	else m_ClientManager.DeleteClient(socket);

	g_Critical.Unlock();
}

void CDummyWnd::UpdateChatMessage(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	LPUSER lpuserinfo = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());

	g_Critical.Lock();

	list<USER>::iterator iter = m_ClientManager.clientlist.begin();

	cmnpck.header = GSM_CHATMESSAGE;
	cmnpck.length = pMsg->GetLength();

	pMsg->SetData(&cmnpck, 0, sizeof(cmnpck));

	for(; iter != m_ClientManager.clientlist.end() ; iter++)
	{
		LPUSER lpuser = &(*iter);

		if(lpuser->channelno == lpuserinfo->channelno)
		{
			m_ClientSocket.SendMsg(lpuser->socket, (char *)pMsg->GetDataPtr(), pMsg->GetLength());
		}
	}

	g_Critical.Unlock();
}

void CDummyWnd::SendCompetitorInfo(LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	USERINFO compet[2];
	
	cmnpck.header = GSM_COMPETITINFO;
	cmnpck.length = sizeof(cmnpck) + sizeof(USERINFO);

	memset(&compet[0], 0, sizeof(USERINFO));
	memcpy(&compet[0], lproom->users[1], sizeof(USERINFO));

	memset(&compet[1], 0, sizeof(USERINFO));
	memcpy(&compet[1], lproom->users[0], sizeof(USERINFO));

	for(int i = 0 ; i < lproom->maximum ; i++)
	{
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		Send.SetData(&compet[i], sizeof(cmnpck), sizeof(USERINFO));

		m_ClientSocket.SendMsg(lproom->users[i]->socket, (char *)Send.GetDataPtr(), Send.GetLength());
		Send.Clear();
	}
}

void CDummyWnd::ClientLogin(USERINFO userinfo)
{
////////////////////////////////////////////////////////////
// 다시 한번 입장 가능한지 검사

	LPCHANNEL lpch = m_ChannelManager.GetChannelInfo(userinfo.channelno);

	// 다른 클라이언트가 접근하지 못하도록 Lock
	g_Critical.Lock();

	// 채널이 찼을 경우
	if(lpch->present == lpch->maximum)
	{
		SendErrorMessage(userinfo.socket, F_CHANNELFULL);
		m_ClientManager.DeleteClient(userinfo.socket);
		g_Critical.Unlock();

		return;
	}

	LPUSER user = m_ClientManager.GetClient((char *)userinfo.id);

	CMessage Send;

	COMMON_PACKET cmnpck;

	if(user != NULL) 
	{
		g_Critical.Unlock();
		SendErrorMessage(userinfo.socket, F_ALREADYLOGIN);
	}

	else
	{
		LPDBCONNPOOL lpavdb = GetAvailableDbConn();
		
		if(lpavdb == NULL)
		{
			SendErrorMessage(userinfo.socket, F_DBERROR);
			g_Critical.Unlock();
			return;
		}

		CString strQuery;
		
		user = m_ClientManager.GetClient(userinfo.socket);
		
		strQuery.Format("SELECT userid, name, alias, score, playcount, victorycount, losscount, drawcount \
			FROM userinfo WHERE userid = '%s'", userinfo.id);
		
		lpavdb->pDb->SQLQuery(strQuery);
		
		while(lpavdb->pDb->Fetch() != SQL_NO_DATA)
		{
			lpavdb->pDb->GetStr(1, (char *)user->id);
			lpavdb->pDb->GetStr(2, (char *)user->name);
			lpavdb->pDb->GetStr(3, (char *)user->alias);

			user->score = lpavdb->pDb->GetInt(4);
			user->playcount = lpavdb->pDb->GetInt(5); 
			user->victorycount = lpavdb->pDb->GetInt(6);
			user->losscount = lpavdb->pDb->GetInt(7);
			user->drawcount = lpavdb->pDb->GetInt(8);
		}

		lpavdb->pDb->Clear();

		user->channelno = userinfo.channelno;
		user->roomno = -1;
		user->talkwith = NULL;
		strcpy((char *)user->ip, (char *)userinfo.ip);
		
		lpch->present++;

		lpavdb->bBusy = FALSE;

		/////////////////////////////////////////////////////
		// 클라이언트에 알려주기 위한 userinfo

		strcpy((char *)userinfo.id, (char *)user->id);
		strcpy((char *)userinfo.name, (char *)user->name);
		strcpy((char *)userinfo.alias, (char *)user->alias);

		LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(user->channelno);

		cmnpck.header = GSM_CLIENTINFO;
		cmnpck.length = sizeof(cmnpck) + sizeof(userinfo);

		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		Send.SetData(user, sizeof(cmnpck), sizeof(USERINFO));

		m_ClientSocket.SendMsg(userinfo.socket, (char *)Send.GetDataPtr(), Send.GetLength());
		NotifyClientUpdate((LPUSERINFO)user);

		g_Critical.Unlock();
	}
}


void CDummyWnd::UpdateClientInfo(USERINFO userinfo)
{
	g_Critical.Lock();

	LPUSER lpuser = m_ClientManager.GetClient(userinfo.socket);

	lpuser->victorycount = userinfo.victorycount;
	lpuser->losscount = userinfo.losscount;
	lpuser->drawcount = userinfo.drawcount;
	lpuser->playcount = userinfo.playcount;
	lpuser->score = userinfo.score;
//	lpuser->money = userinfo.money;

	LPDBCONNPOOL lpavdb = GetAvailableDbConn();
	
	if(lpavdb == NULL)
	{
		SendErrorMessage(userinfo.socket, F_DBERROR);
		g_Critical.Unlock();
		return;
	}
	
	CString strQuery;
	
	strQuery.Format("UPDATE userinfo SET score='%d', playcount='%d', victorycount='%d', losscount='%d', drawcount='%d' WHERE userid = '%s'", 
		lpuser->score,
		lpuser->playcount,
		lpuser->victorycount,
		lpuser->losscount,
		lpuser->drawcount,
		lpuser->id);
	
	g_Critical.Unlock();

	m_Console.writeConsole("DBConn id : [%d], %s\n", lpavdb->id, strQuery);

	lpavdb->pDb->SQLQuery(strQuery);
	lpavdb->pDb->Clear();
	lpavdb->bBusy = FALSE;
}

unsigned __stdcall threadSQLQuery(LPVOID arg)
{
	LPQUERYPARAM lpqp = (LPQUERYPARAM)arg;

	switch(lpqp->nCode)
	{
		case CM_REQLOGIN :
			lpqp->pWnd->ClientLogin(lpqp->userinfo);
			break;

		case CM_UPDATEINFO :
			lpqp->pWnd->UpdateClientInfo(lpqp->userinfo);
			break;
	}

	delete [] lpqp;


	_endthreadex(0);
	return 0;
}

void CDummyWnd::SendReqNewChat(CMessage *pMsg)
{
	char id[32] = {0, };
	COMMON_PACKET cmnpck;
	CMessage Send;

	pMsg->GetData(id, sizeof(cmnpck), sizeof(id));

	LPUSER lpuser = m_ClientManager.GetClient(id);
	LPUSER lprequester = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());

	if(lpuser != NULL)
	{
		if(lpuser->roomno == -1)
		{
			cmnpck.header = GSM_REQNEWCHAT;
			cmnpck.length = sizeof(cmnpck) + sizeof(id);
			
			strcpy(id, (char *)lprequester->id);
			
			Send.SetData(&cmnpck, 0, sizeof(cmnpck));
			Send.SetData(id, sizeof(cmnpck), sizeof(id));
			
			m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());
		}

		else 
		{
			cmnpck.header = GSM_REJECTNEWCHAT;
			cmnpck.length = sizeof(cmnpck);
			
			Send.SetData(&cmnpck, 0, sizeof(cmnpck));
			m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
		}
	}
}


void CDummyWnd::SendReqNewGame(CMessage *pMsg)
{
	char id[32] = {0, };
	COMMON_PACKET cmnpck;
	CMessage Send;

	pMsg->GetData(id, sizeof(cmnpck), sizeof(id));

	LPUSER lpuser = m_ClientManager.GetClient(id);
	LPUSER lprequester = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());

	if(lpuser != NULL)
	{
		if(lpuser->roomno == -1)
		{
			g_Critical.Lock();

			LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);

			list<ROOM>::iterator iter = lpchannel->roomlist.begin();

			for(; iter != lpchannel->roomlist.end() ; iter++)
			{
				LPROOM lproom = &(*iter);

				if(lproom->present == 0)
				{
					// 방에 입장시킴
					COMMON_PACKET cmnpck;
					
					cmnpck.header = GSM_OWNTHISROOM; // 방장이 됐음..
					cmnpck.length = sizeof(cmnpck);
					
					m_ClientSocket.SendMsg(lprequester->socket, (char *)&cmnpck, sizeof(cmnpck));
					m_Console.writeConsole("::: GSM_OWNTHISROOM (id : %s)\n", (LPCTSTR)lprequester->id);
					
					lprequester->lproom = lproom;		// 입장한 방 포인터
					lprequester->roomno = lproom->id;	// 입장한 방 번호
					
					strcpy((char *)lproom->title, "*** 예약 ***");
					lproom->present = 1;
					lproom->users[0] = lprequester;
					lproom->locked = 0;
					NotifyAllUpdate((LPUSERINFO)lprequester, lproom);

					cmnpck.header = GSM_REQNEWGAME;
					cmnpck.length = sizeof(cmnpck) + sizeof(id) + sizeof(lproom->id);

					strcpy(id, (char *)lprequester->id);
					
					Send.SetData(&cmnpck, 0, sizeof(cmnpck));
					Send.SetData(id, sizeof(cmnpck), sizeof(id));
					Send.SetData(&lproom->id, sizeof(cmnpck) + sizeof(id), sizeof(lproom->id));
					
					g_Critical.Unlock();

					m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

					return;
				}
			}

			g_Critical.Unlock();
			SendErrorMessage((SOCKET)pMsg->GetSocket(), F_NOROOM);
		}

		else 
		{
			cmnpck.header = GSM_REJECTNEWGAME;
			cmnpck.length = sizeof(cmnpck);
			
			Send.SetData(&cmnpck, 0, sizeof(cmnpck));
			m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
		}
	}
}

void CDummyWnd::CancelNewGame(CMessage *pMsg)
{
	char id[32] = {0, };

	pMsg->GetData(id, sizeof(COMMON_PACKET), sizeof(id));

	LPUSER lpuser = m_ClientManager.GetClient(id);

	if(lpuser != NULL)
	{
		pMsg->SetSocket(lpuser->socket);
		GetoutRoom(pMsg);

		CMessage Send;
		COMMON_PACKET cmnpck;
		
		cmnpck.header = GSM_REJECTNEWGAME;
		cmnpck.length = sizeof(cmnpck);
		
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
	}
}

void CDummyWnd::UpdateGameState(CMessage *pMsg)
{
	SOCKET socket = (SOCKET)pMsg->GetSocket();
	short gamestatus;

	pMsg->GetData(&gamestatus, sizeof(COMMON_PACKET), sizeof(gamestatus));

	LPUSER lpuser = m_ClientManager.GetClient(socket); // 접속 해제된 클라이언트 검색
	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);
	LPROOM lproom = lpuser->lproom;

	if(lproom != NULL) lproom->status = gamestatus;
}