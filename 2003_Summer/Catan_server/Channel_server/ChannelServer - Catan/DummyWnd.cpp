// DummyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "DummyWnd.h"
#include "process.h"
#include "afxmt.h"
#include "header.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_INTERVAL		1000

CCriticalSection g_Critical;


/////////////////////////////////////////////////////////////////////////////
// CDummyWnd

IMPLEMENT_DYNCREATE(CDummyWnd, CWnd)

CDummyWnd::CDummyWnd()
{
	m_nPortServer = 10000;
	m_nPortClient = 12000;
}

CDummyWnd::~CDummyWnd()
{
}


BEGIN_MESSAGE_MAP(CDummyWnd, CWnd)
	//{{AFX_MSG_MAP(CDummyWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_STARTSERVER, OnStartserver)
	ON_COMMAND(ID_STOPSERVER, OnStopserver)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotify)
	ON_MESSAGE(WM_SOCKETMSGSERVER, OnSocketMsgGameServer)
	ON_MESSAGE(WM_SOCKETMSGCLIENT, OnSocketMsgClient)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyWnd message handlers



////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 트레이 아이콘 마우스 메세지 헨틀러 함수
//
// params
// 

LRESULT CDummyWnd::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("SSS");

	switch(LOWORD(lParam))
	{
		case WM_RBUTTONDOWN :
			{
		
			CMenu Menu;
			CMenu *pSub;
		
			Menu.LoadMenu(IDR_MENU);
			pSub = Menu.GetSubMenu(0);
		
			CPoint p;
			GetCursorPos(&p);
			pSub->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

			break;
			}

		case WM_MOUSEMOVE :

			break;
	}

	return 0;
}

int CDummyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		AfxMessageBox("프로그램이 죽었부렀습니다");
		return -1;
	}
		
	// 트레이 아이콘 생성
	if(!m_TrayIcon.Create(this, WM_TRAYNOTIFY, "Catan Channel", LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), IDC_TRAYICON))
	{
		AfxMessageBox("프로그램이 죽었부렀습니다");
		return -1;
	}

/*
	Start.LoadBitmap(IDB_START);
	Stop.LoadBitmap(IDB_STOP);
	Conf.LoadBitmap(IDB_CONF);
	Exit.LoadBitmap(IDB_EXIT);

	CMenu Menu;
	CMenu *pSub;

	Menu.LoadMenu(IDR_MENU);
	pSub = Menu.GetSubMenu(0);

	pSub->SetMenuItemBitmaps(ID_STARTSERVER, MF_BYCOMMAND, &Start, &Start);
	pSub->SetMenuItemBitmaps(ID_STOPSERVER, MF_BYCOMMAND, &Stop, &Stop);
	pSub->SetMenuItemBitmaps(ID_CONFIG, MF_BYCOMMAND, &Conf, &Conf);
	pSub->SetMenuItemBitmaps(ID_EXIT, MF_BYCOMMAND, &Exit, &Exit);
*/
	// 서버와 통신할 소켓 초기화 
	if(!m_ServerSocket.InitSock()) return -1;
	if(!m_ServerSocket.Initialize(m_nPortServer)) // m_nPortServer = 10000;
	{
		AfxMessageBox("서버 소켓 초기화에 실패했습니다.");
		return -1;
	}
	//AfxMessageBox("서버 소켓 초기화완료(GameServer)");

	// 클라이언트와 통신할 소켓 초기화
	if(!m_ClientSocket.Initialize(m_nPortClient)) // m_nPortClient = 12000;
	{
		AfxMessageBox("서버 소켓 초기화에 실패했습니다.");
		return -1;
	}
	//AfxMessageBox("서버 소켓 초기화완료(LobbyClient)");
	
	//AfxMessageBox("로비 접속 준비 완료", MB_OK);

	return 0;
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 서버 시작
//
// params
// 

void CDummyWnd::OnStartserver()
{
	AfxMessageBox("채널 서버 Start", MB_OK);
	CMenu Menu;
	CMenu *pSub;

	Menu.LoadMenu(IDR_MENU);
	pSub = Menu.GetSubMenu(0);

	pSub->EnableMenuItem(ID_STARTSERVER, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	pSub->EnableMenuItem(ID_STOPSERVER, MF_BYCOMMAND | MF_ENABLED);

	m_ServerSocket.Listen();
	int nResult = WSAAsyncSelect(m_ServerSocket.m_Socket, m_hWnd, WM_SOCKETMSGSERVER, FD_ACCEPT | FD_CLOSE);

	m_ClientSocket.Listen();
	nResult = WSAAsyncSelect(m_ClientSocket.m_Socket, m_hWnd, WM_SOCKETMSGCLIENT, FD_ACCEPT | FD_CLOSE);

	SetTimer(0, TIMER_INTERVAL, NULL);
}



////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 서버 중지
//
// params
// 

void CDummyWnd::OnStopserver() 
{
	CMenu Menu;
	CMenu *pSub;

	Menu.LoadMenu(IDR_MENU);
	pSub = Menu.GetSubMenu(0);

	pSub->EnableMenuItem(ID_STARTSERVER, MF_BYCOMMAND | MF_ENABLED);
	pSub->EnableMenuItem(ID_STOPSERVER, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	KillTimer(0);
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 서버 설정
//
// params
// 

void CDummyWnd::OnConfig() 
{
	// TODO: Add your command handler code here
	
}



////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 서버 데몬 종료
//
// params
// 

void CDummyWnd::OnExit() 
{
	m_ServerSocket.Close();

	Start.DeleteObject();
	Stop.DeleteObject();
	Conf.DeleteObject();
	Exit.DeleteObject();

	m_TrayIcon.RemoveIcon();
	PostQuitMessage(0);
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 29일
//
// 주기적으로 채널 정보 요청
//
// params
// 

void CDummyWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(0);

	SOCKET *sockets = NULL;
	int nLen = 0;

	m_ServerManager.GetServerSockets((DWORD**)&sockets, &nLen);

	CMessage Send;
	COMMON_PACKET cmnpck;

	cmnpck.header = CSM_SERVERINFO;
	cmnpck.length = sizeof(cmnpck);

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));


	// 현재 연결된 서버에 채널정보 요청
	for(int i = 0 ; i < nLen ; i++)
		m_ServerSocket.SendMsg(sockets[i], (char *)Send.GetDataPtr(), Send.GetLength());

	TRACE("REQUEST CHANNEL INFO\n");
	
	SetTimer(0, TIMER_INTERVAL, NULL);

	CWnd::OnTimer(nIDEvent);
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 게임서버와 통신하는 thread
//
// params
// arg : CDummyWnd의 인스턴스 포인터, 접속한 서버의 socket

long CDummyWnd::OnSocketMsgGameServer(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;

	CMessage Send, Recv;
	COMMON_PACKET cmnpck;

	memset(&cmnpck, 0, sizeof(cmnpck));

	int nRead1 = 0;
	int nRead2 = 0;

	LPBYTE readbuff = NULL;
	DWORD dwError = 0;

	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT : {

				SOCKET serversocket = m_ServerSocket.Accept();  // 접속 요청이 있을 때까지 블록
				if(serversocket == INVALID_SOCKET) return 0L;		// 잘못된 소켓이면 다시 접속 대기
				m_ServerManager.AddServer(serversocket);	// CServerManager 클레스에 서버 추가

				WSAAsyncSelect(serversocket, GetSafeHwnd(), WM_SOCKETMSGSERVER, FD_READ | FD_CLOSE);
				
				cmnpck.header = CSM_CONNECTGS;
				cmnpck.length = sizeof(COMMON_PACKET);
				Send.SetData(&cmnpck, 0, sizeof(cmnpck));
				m_ServerSocket.SendMsg(serversocket, (char *)Send.GetDataPtr(), Send.GetLength());
			}
			break;

		case FD_READ : {
			
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
	
					TRACE2("nRead2(%d) < cmnpck.length(%d)\n", nRead2, cmnpck.length);
				}
				
				Recv.Clear();
				Recv.SetSocket((DWORD)socket);
				Recv.SetData(readbuff, 0, nRead2);
				
				delete [] readbuff;

				ProcessMessageServer(&Recv);
			}
			break;

		case FD_CLOSE :

			m_ServerManager.DeleteServer(socket);

			break;
	}

	 return 0L;
}



////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 클라이언트와 통신하는 thread
//
// params
// arg : CDummyWnd의 인스턴스 포인터, 접속한 서버의 socket


long CDummyWnd::OnSocketMsgClient(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("로비 메세지", MB_OK);

	SOCKET socket = (SOCKET)wParam;
	
	CMessage Send, Recv;
	COMMON_PACKET cmnpck;			

	memset(&cmnpck, 0, sizeof(cmnpck));

	int nRead1 = 0;
	int nRead2 = 0;

	LPBYTE readbuff = NULL;
	DWORD dwError = 0;
		
	if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)
	{
		SOCKET clientsocket = m_ClientSocket.Accept();  // 접속 요청이 있을 때까지 블록
		if(clientsocket == INVALID_SOCKET) return 0L;		// 잘못된 소켓이면 다시 접속 대기
		
		WSAAsyncSelect(clientsocket, GetSafeHwnd(), WM_SOCKETMSGCLIENT, FD_READ | FD_CLOSE);
		
		cmnpck.header = CSM_CONNECTLB;
		cmnpck.length = sizeof(COMMON_PACKET);
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		m_ClientSocket.SendMsg(clientsocket, (char *)Send.GetDataPtr(), Send.GetLength());

//		AfxMessageBox("로비 접속", MB_OK);
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
			
			TRACE2("nRead2(%d) < cmnpck.length(%d)\n", nRead2, cmnpck.length);
		}
		
		Recv.Clear();
		Recv.SetSocket((DWORD)socket);
		Recv.SetData(readbuff, 0, nRead2);
		
		delete [] readbuff;
		
		ProcessMessageClient(&Recv);
		
	}

	else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{
		closesocket(socket);
	}

	return  0L;
}


////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 23일
//
// 네크웤으로부터 들어온 실제 패킷 정보를 처리
//
// params
// arg : CDummyWnd의 인스턴스 포인터

void CDummyWnd::ProcessMessageServer(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header) // 패킷 헤더 검색
	{
		case GSM_SERVERINFO : // 게임 서버 정보일 경우
			{
				//AfxMessageBox("게임 서버 정보 저장.");
				SERVERINFO si;
				CHANNELINFO ci;

				int nLen = pMsg->GetData(&si, 0, sizeof(si));  // 우선 게임 서버 자체의 정보를 얻음

				LPSERVER server = m_ServerManager.GetServer((SOCKET)pMsg->GetSocket()); // 패킷을 보낸 서버를 찾음

				if(server != NULL) // 정상적인 서버 포인터를 가져왔을 경우
				{
					server->id = si.id;	// 서버의 아이디 저장
					server->ip = (char *)si.ip;	// 아이피 저장
					server->portno = si.port;	// 포트번호 저장
					server->maxchannel = si.channels;
					server->rooms = si.rooms;

					if(!server->channel.size())	// 최초에 접속한 서버일 경우 채널정보가 없음
					{
						CHANNEL channel;

						for(int i = 0 ; i < si.channels ; i++) // 전체 채널갯수만큼
						{
							nLen += pMsg->GetData(&ci, nLen, sizeof(ci)); // 채널 정보를 하나씩 읽어옴

							channel = ci;	// 채널정보를 하나씩 저장
							server->channel.push_back(channel); // 채널을 리스트에 추가
						}
					}
					else // 이미 채널 정보가 존재할 경우 // 주기적인 서버 정보의 요청일 경우
					{
						g_Critical.Lock();
						
						list<CHANNEL>::iterator i = server->channel.begin();
						
						for(; i != server->channel.end() ; i++)
						{
							LPCHANNEL lpchannel = &(*i);
							
							nLen += pMsg->GetData(&ci, nLen, sizeof(ci));
							
							*lpchannel = ci;
						}
						
						g_Critical.Unlock();
					}
				}
			}
			break;
	}
}

// 로비 클라이언트와의 통신.
void CDummyWnd::ProcessMessageClient(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header) // 패킷 해더 검색
	{
		case LM_LOGIN :
			SendChInfo((SOCKET)pMsg->GetSocket());
			break;

		case LM_REQCHINFO :
			SendChInfo((SOCKET)pMsg->GetSocket());
			//RedirectClient(pMsg);
			break;
//#define LM_REQGSMSVR	0x0502	// 게임서버 입장
		case LM_REQGSMSVR :
			RedirectClient(pMsg);
			break;
	}
}


void CDummyWnd::SendChInfo(SOCKET socket)
{
	CMessage Send;
	COMMON_PACKET cmnpck;
	int nPacketLen = sizeof(cmnpck);
	int nLen = m_ServerManager.GetLength();

	for(int i = 0 ; i < nLen ; i++)
	{
		LPSERVER lpserver = m_ServerManager.GetServer(i);

		list<CHANNEL>::iterator iter = lpserver->channel.begin();

		for(; iter != lpserver->channel.end() ; iter++)
		{
			CHANNELINFO channel = *iter;

			nPacketLen = Send.SetData(&channel, nPacketLen, sizeof(CHANNEL));
		}
	}

	cmnpck.header = CSM_SERVERINFO;
	cmnpck.length = nPacketLen;

	Send.SetData(&cmnpck, 0, sizeof(COMMON_PACKET));
	m_ClientSocket.SendMsg(socket, (char *)Send.GetDataPtr(), Send.GetLength());
}

void CDummyWnd::RedirectClient(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;
	unsigned short channelno;

	int nPacketLen = pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));
	pMsg->GetData(&channelno, nPacketLen, sizeof(channelno));

	int nLen = m_ServerManager.GetLength();
	int nTotalChannel = 0;
	int serverid = 0;

	LPSERVER lpserver = NULL;
	//lpserver = new SERVER; 

	// 클라이언트가 입장할 채널을 포함하는 서버 아이디를 구함
	// 서버마다 채널 크기가 각각 다를 수 있음
	for(int i = 0 ; i < nLen ; i++)
	{
		lpserver = m_ServerManager.GetServer(i);
		// GetServer는 lpserver를 return....
		// 여기서 메모리 주소 문제 발생.....
		//

		if((nTotalChannel + lpserver->maxchannel) <= channelno)
		{
			serverid++;
			nTotalChannel += lpserver->maxchannel;
		}

		else
		{
			channelno -= nTotalChannel;
			break;
		}
	}

	if(lpserver != NULL)
	{
		CMessage Send;
		SERVERINFO si;

		si.cmnpck.header = CSM_DESTSVRINFO;
		si.cmnpck.length = sizeof(SERVERINFO);
		si.id = lpserver->id;
		strcpy((char *)si.ip, lpserver->ip.data());
		si.port = lpserver->portno;
		si.rooms = channelno; // rooms에 대신 channelno가 들어감 rooms는 나중에 Game Server로부터 직접 받음
		
	//	LPCHANNEL temp = 

		Send.SetData(&si, 0, sizeof(SERVERINFO));

		m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
	}
}
