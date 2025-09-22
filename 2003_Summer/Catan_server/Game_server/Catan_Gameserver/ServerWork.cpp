// ServerWork.cpp : implementation file
//

#include "stdafx.h"
#include "Catan_Gameserver.h"
#include "Catan_GameserverDlg.h"
#include "ServerWork.h"
#include "afxmt.h"
#include "process.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection g_Critical;


unsigned __stdcall threadSQLQuery(LPVOID arg);

/////////////////////////////////////////////////////////////////////////////
// CServerWork

IMPLEMENT_DYNCREATE(CServerWork, CWnd)

CServerWork::CServerWork()
{
	m_nServerID = 0;
	m_nMaxUser = 2;	// 최대 접속자 수 제한..

	

////////////////////////////////////////////
// DB Connection Pool 생성 기본 DBPOOLSIZE 개

	for(int i = 0 ; i < DBPOOLSIZE ; i++)
	{
		DBCONNPOOL dbconn;

		memset(&dbconn, 0, sizeof(DBCONNPOOL));

		dbconnlist.push_back(dbconn);
	}
}

CServerWork::~CServerWork()
{
	delete this;
}



BEGIN_MESSAGE_MAP(CServerWork, CWnd)
	//{{AFX_MSG_MAP(CServerWork)
	ON_WM_CREATE()
	ON_COMMAND(ID_SERVERSTART, OnServerstart)
	ON_COMMAND(ID_SERVEREXIT, OnServerexit)
	ON_COMMAND(ID_SERVERLOG, OnServerlog)
	ON_COMMAND(ID_SERVERSETUP, OnServersetup)
	ON_COMMAND(ID_SERVERSTOP, OnServerstop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotify)
	ON_MESSAGE(WM_SOCKETMSGCHSERVER, OnSocketMsgChServer)
	ON_MESSAGE(WM_SOCKETMSGCLIENT, OnSocketMsgClient)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerWork message handlers


int CServerWork::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// TrayIcon 생성 및....
	if(!m_TrayIcon.Create(this, WM_TRAYNOTIFY, "카탄 게임 서버", LoadIcon(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDR_MAINFRAME)), IDC_TRAYICON))
	{
		return -1;
	}
	
	// Dialog 창 생성.
	CCatan_GameserverDlg Dlg;

	if(!Dlg.LoadSetting())
	{
		while(Dlg.DoModal() == IDOK)
		{
			if(Dlg.SaveSetting()) break;
		}
	}

	// Dialog로 부터 서버 정보 받아 저장.
	m_strGameServerIP = Dlg.m_strGameserverIP;
	m_nGameServerPort = Dlg.m_nGameserverPORT;
	
	m_strChannelIP = Dlg.m_strChannelserverIP;
	m_nChannelPort = Dlg.m_nChannelPORT;

	m_nMaxChannelCount = Dlg.m_nChannel;
	m_nMaxRoomCount = Dlg.m_nRoom;
	
	m_strDBName = Dlg.m_strDBNAME;
	m_strDBID = Dlg.m_strDBID;
	m_strDBPassword = Dlg.m_strDBPASSWD;

	m_pLogDialog = new CLogDialog();
	m_pLogDialog->Create(IDD_LOGDIALOG, this);

	// 서버의 통신 소켓 초기화...
	// 클라이언트 통신용.... 애플리케이션의 실행과 함께 실행된다.
	if(!m_ServerSocket.InitSock()) return -1;
	if(!m_ServerSocket.Initialize(m_nGameServerPort))
	{
		m_pLogDialog->AddLogString("서버 소켓 초기화 실패");
		return 0;
	}
	
	
	return 0;
}


LRESULT CServerWork::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
	case WM_RBUTTONDOWN :
		CMenu Menu;
		CMenu *pSub;

		Menu.LoadMenu(IDR_GSMENU);
		pSub = Menu.GetSubMenu(0);

		CPoint p;
		GetCursorPos(&p);
		pSub->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

		break;
	}
	return 0;
}

// 서버 시작
BOOL CServerWork::InitServer()
{
	 m_pLogDialog->AddLogString("*** InitServer ***");	// Log 창에 표시
	 m_ChannelManager.InitializeChannel(m_nServerID, m_nMaxChannelCount, m_nMaxRoomCount, m_nMaxUser);
	 // Channel Initialize... 채널 설정.

	 return TRUE;
}

// ! 주의 ! : 서버는 시작한 채로 돌아 가지 않는다!
void CServerWork::OnServerstart() 
{
	// TODO: Add your command handler code here
	InitServer();

	m_pLogDialog->ShowWindow(SW_SHOW);

	// 채널서버와의 접속...
	if(!m_ClientSocket.Connect((LPSTR)m_strChannelIP.data(), m_nChannelPort))
	{
		m_pLogDialog->AddLogString("채널서버 접속 실패...");
		m_pLogDialog->AddLogString("IP   번호 : %s", m_strChannelIP.data());
		m_pLogDialog->AddLogString("포트 번호 : %d", m_nChannelPort);
		return;
	}

	m_pLogDialog->AddLogString("채널 서버와의 접속 완료...");
	m_pLogDialog->AddLogString("포트 번호 : %d", m_nChannelPort);

	SendServerInfo();
	
	m_pLogDialog->AddLogString("채널 서버에 게임 서버 정보 전송");
	
	// 클라이언트 접속 대기,
	WSAAsyncSelect(m_ClientSocket.m_Socket, m_hWnd, WM_SOCKETMSGCHSERVER, FD_READ | FD_CLOSE);
					// 1. 이벤트 알림을 요구하는 소켓 2. 네트워크 이벤트 발생시 메세지 받을 윈도우 핸들
					// 3. 네트워크 이벤트 발생시 받을 메시지. 4. 네트워크 이벤트 메세지를 발생시키고자 하는 이벤트들...
	// 소켓에 네트워크 이벤트(FD_READ | FD_CLOSE)가 일어나면 윈도우 메세지 WM_SOCKETMSGCHSERVER 발생
	// 소켓이 넌블로킹모드로 작동하게 된다.

	// 로비 클라이언트의 접속을 대기....
	m_pLogDialog->AddLogString("*** Listen 시작 ***");
	m_ServerSocket.Listen();
	WSAAsyncSelect(m_ServerSocket.m_Socket, m_hWnd, WM_SOCKETMSGCLIENT, FD_ACCEPT | FD_CLOSE);
	// ServerSocket을 접속 대기 상태로...
	// 소켓 이벤트 FD_ACCEPT | FD_CLOSE 시 윈도우 메세지 WM_SOCKETMSGCLIENT 발생

	
	//SendServerInfo();
	
	
}

void CServerWork::OnServerexit() 
{
	// TODO: Add your command handler code here
	m_ChannelManager.ClearChannel();
	m_TrayIcon.RemoveIcon();
	m_pLogDialog->DestroyWindow();
	delete m_pLogDialog;
	PostQuitMessage(0);
	
}

void CServerWork::OnServerlog() 
{
	// TODO: Add your command handler code here
	m_pLogDialog->ShowWindow(SW_SHOW);
	
	
}

void CServerWork::OnServersetup() 
{
	// TODO: Add your command handler code here
	CCatan_GameserverDlg dlg;
	
	dlg.LoadSetting();

	while(dlg.DoModal() == IDOK)
	{
		if(dlg.SaveSetting()) break;
	}
	
}

void CServerWork::OnServerstop() 
{
	// TODO: Add your command handler code here
	m_pLogDialog->AddLogString("*** 서버 정지 ***");
	m_ChannelManager.ClearChannel();
	// 현재 개설된 채널들을 전부 삭제.
	// ClearChannel 함수는 채널 내부의 방들도 전부 파괴한다.

	//PostQuitMessage(0);
	// 애플리케이션 종료 메세지.
	
}

///////////////////////////////////////////////////////////////
// Channel Server에게서 메세지를 받았을때의 이벤트 처리.....
long CServerWork::OnSocketMsgChServer(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;
	// 파라메터 인자값으로 넘어온 소켓을 사용한다.
	// 넘어온 소켓은 채널 서버와 Connecting 된 소켓....

	CMessage Recv;			// 받은 메세지를 처리할 구조체...
	COMMON_PACKET cmnpck;	// Message Header -> 전체 메세지의 길이와 타입을 먼저 알아 처리하기 위해 사용.

	memset(&cmnpck, 0, sizeof(cmnpck));
	// 헤더 패킷 초기화...

	// 받은 패킷의 Byte 크기 저장할 변수...
	int nRead1 = 0;	// 재시도용
	int nRead2 = 0;	// 일단 이걸 주로 쓴다.

	LPBYTE readbuff = NULL;

	DWORD dwError = 0;

	// 이부분은 날아온 패킷의 헤더만을 잡는 부분이다.
	// 실제 패킷이 전송될때, 헤더 뒷부분이 잘려소 올 경우, 전체 패킷을 무리 없이 받기위해 
	// 헤더를 먼저 받아 받기 처리를 한다....
	if(WSAGETSELECTEVENT(lParam) == FD_READ)	// 이벤트가 발생한 소켓의 이벤트 파라미터....
	{
		nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);	
		// nRead에 헤더 파일 받음..recv 함수는 cmnpck에 패킷 받으면서 받은 바이트 수 반환.

		if(nRead2 < (int)sizeof(cmnpck))	// 받은 패킷 크기가 부족함 (두가지의 경우 : 못 받았거나, 덜 받았거나)
		{
			if(nRead2 < 0)	// nRead함수는 패킷을 부족하게 받으면 -1을 리턴한다(아예 못받았다)
			{
				nRead2 = 0;
				nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);	// 잘못받았으니 다시 받기
				
				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			else	// 받긴 받았는데 부족할 경우.
			{
				nRead1 = recv(socket, (char *)&cmnpck+nRead2, sizeof(cmnpck)-nRead2, 0);

				if(nRead1 > 0)
					nRead2 += nRead1;
			}

			if(nRead1 < 0) return 0L;		// 아예 소켓이 망했다.
		}
	

		readbuff = new BYTE[cmnpck.length];		// 받은 패킷을 가지고 있을 버퍼.
		memset(readbuff, 0, sizeof(readbuff));
		memcpy(readbuff, &cmnpck, sizeof(cmnpck));	// 받은 패킷 보관.

		// 상단의 코드에서 전체 받을 패킷의 헤더만 추출하여 얼마의 패킷을 받아야 하는지 결정하였다.
		// 이 부분에서는 실제의 패킷을 받아 처리하는 부분이다.
		// nRead2와 패킷 길이가 일치할때 전체 패킷이 다 온것.
		while(nRead2 < cmnpck.length)	// 당연히 작것지...패킷 헤다 길이만 저장된 nRead2하고 전체 길이가 명시된 cmnpck.length하고 같을 수가 있나.
		{
			nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
			// nRead1에 헤더 패킷의 뒷부분(데이터 부분)을 전체 패킷 길이-헤더패킷길이 만큼 받는다...
		
			if(nRead1 < 0)	// -1 발생시(에라)
			{
				nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);

				if(nRead1 > 0) nRead2 += nRead1;	// nRead2는 while문으로 계속 검사됨....
			}
			else nRead2 += nRead1; 

			dwError = WSAGetLastError();	// 소켓에서의 에러 이벤트 확인.....

			if(dwError == WSAESHUTDOWN || dwError == WSAECONNRESET)
				break;								// 소켓이 죽었을 경우 바로 아웃...(작업이 무의미...무한 루프 돈다)
			else if(dwError == WSAEWOULDBLOCK)		// 위의 두 경우가 아니라면, 일단 소켓이 박살나지느는 않았단 얘기.
			{
				unsigned long ByteToRead;			// 읽은 바이트 수 저장.
				ioctlsocket(socket, FIONREAD, &ByteToRead);

				if(!ByteToRead) return 0L;
			}

		}

		// 패킷을 전부 받았으므로 받은 패킷을 메세지 구조체에 저장한다.
		// 메세지 구조체에도 소켓이 있는 이유는, 맏은 메세지에 따른 응답 처리를 해주기 위해서이다.
		Recv.Clear_Data();	
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

///////////////////////////////////////////////////////////////
// Lobby Client에게서 메세지가 왔을 경우의 이벤트 처리....
long CServerWork::OnSocketMsgClient(WPARAM wParam, LPARAM lParam)
{
	// Packet 처리 초기화 작업
	// recv함수가 리턴할 변수, 패킷 헤도를 저장할 구조체, 전체 메시지를 저장할 공간....
	SOCKET socket = (SOCKET)wParam;

	CMessage Recv;

	DWORD dwError;
	LPBYTE readbuff;

	int nRead1 = 0;
	int nRead2 = 0;

	COMMON_PACKET cmnpck;

	memset(&cmnpck, 0, sizeof(cmnpck));

	if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)	// 서버와의 연결 요청 처리...
	{
		SOCKET clientsocket = m_ServerSocket.Accept();
		// 접속 연결 요청이 들어온 소켓으로 접속 작업...............

		// if(socket == INVALID_SOCKET) return 0L;

		m_ClientManager.AddClient(clientsocket);
		// 접속이 허가된 소켓을 클라이언트 매니저에 추가.

		WSAAsyncSelect(clientsocket, m_hWnd, WM_SOCKETMSGCLIENT, FD_READ | FD_CLOSE);
		// 접속 허가된 소켓에서 발생할 수 있는 네트웍 이벤트에 대해 소켓을 넌블록 대기상태로 둔다.

		// 연결이 허가 되었으므로 클라이언트에게 허가 메세지 보낸다..
		CMessage Send;
		Send.Clear_Data();

		cmnpck.header = GSM_CONNECTLB;
		cmnpck.length = sizeof(COMMON_PACKET);
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));

		m_ClientSocket.SendMsg(clientsocket, (char *)Send.GetDataPtr(), Send.GetLength());
	}
	else if(WSAGETSELECTEVENT(lParam) == FD_READ)	// 서버에 작업을 요청하는 메세지....
	{
		nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);	// 패킷 받기(헤더만)
		
		if(nRead2 < (int)sizeof(cmnpck)) // 받은것이 이상할 때...
		{
			if(nRead2 < 0)	// 에러 발생한 경우....
			{	
				nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);

				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			else	// 받긴 받았는데 부족할 경우....
			{
				// 부족한 부분을 받아 더해준다...
				nRead1 = recv(socket, (char *)&cmnpck + nRead2, sizeof(cmnpck) - nRead2, 0);

				nRead2 += nRead1;
			}
			
			if (nRead1 < 0) return 0L;	// 소켓 사망....
		}

		readbuff = new BYTE[cmnpck.length];
		memset(readbuff, 0, sizeof(readbuff));
		memcpy(readbuff, &cmnpck, sizeof(cmnpck));	// 헤더부분을 전체 패킷 부분에 저장...

		while(nRead2 < cmnpck.length)
		{
			nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
			if(nRead1 <0)
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
		
		}

		Recv.Clear_Data();
		Recv.SetSocket((DWORD)socket);
		Recv.SetData(readbuff, 0, nRead2);

		delete [] readbuff;

		ProcessMessageClient(&Recv);
	}
	else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{
		ClientDisconnected(socket);	// 클라이언트로부터 FD_CLOSE 이벤트 발생시 해당 클라이언트의 디스커넥트 작업...
	}
	
	return 0L;
}

///////////////////////////////////////////////////////////////
// Channel server에게 받은 메세지 처리....
void CServerWork::ProcessMessageServer(CMessage *pMsg)
{
	CMessage Send;
	COMMON_PACKET cmnpck;
	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	/*
	#define CSM_CONNECTGS	0x0100	// 연결 확인
	#define CSM_SERVERINFO	0x0101	// 채널 서버 정보 제공 (연결 유지용, IP, Port no...) 
	#define CSM_REQUESTINFO	0x0102	// 게임 서버에 정보요청(최대 방수, 접속자 수)
	#define CSM_REQUESTMAN	0x0103	// 게임 서버에 접속한 사람 수 요청(일정 시간마다 주기적 발생)
	*/
	switch(cmnpck.header)
	{
	case CSM_CONNECTGS :
//		m_pLogDialog->AddLogString("채널 서버 확인 메세지 받음");
	case CSM_REQUESTINFO :
	case CSM_SERVERINFO :
		SendServerInfo();
//		m_pLogDialog->AddLogString("채널 서버 확인 메세지 받음");
		break;
		
	}
}

///////////////////////////////////////////////////////////////
// Chnnel server에게 게임 서버의 정보 보냄....
void CServerWork::SendServerInfo()
{
	SERVERINFO si;	// 게임 서버 정보.
	CHANNELINFO ci;	// (게임서버가 개설한)채널 정보.
	CMessage Send;	// 보낼 패킷....

	// 패킷 종류는 서버 정보.
	si.cmnpacket.header = GSM_SERVERINFO;

	// 게임 서버 정보 삽입.....다이얼로그 박스에서 얻어온 정보로 가능.
	si.id = m_nServerID;	// 서버 아이디는 0
	memset(si.ip, 0, sizeof(si.ip));
	memcpy(si.ip, m_strGameServerIP.data(), m_strGameServerIP.size());
	si.port = m_nGameServerPort;
	si.channels = m_nMaxChannelCount;
	si.rooms = m_nMaxRoomCount;

	si.cmnpacket.length = sizeof(si) + sizeof(ci) * m_nMaxChannelCount;
	// 전체 패킷의 길이는 게임서버 정보 + (채널 하나의 정보 * 채널 수)

	int nLen = Send.SetData(&si, 0, sizeof(si));	// ServerInfo 패킷에 저장.

	UINT maximum = m_nMaxRoomCount * m_nMaxUser;

	
	
	// 각 채널들의 정보 입력.
	for(int i=0 ; i<m_nMaxChannelCount ; i++)
	{
		LPCHANNEL lpchannel;
		lpchannel = new CHANNEL;
			
		lpchannel = m_ChannelManager.GetChannelInfo(i);
		

		ci.channelno = lpchannel->id;
		ci.maximum = lpchannel->maximum;
		ci.present = lpchannel->present;

		nLen = Send.SetData(&ci, nLen, sizeof(ci));
	}
	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

}

///////////////////////////////////////////////////////////////
// Lobby Client에게 받은 매세지 처리....
void CServerWork::ProcessMessageClient(CMessage *pMsg)
{
	CMessage Send;
	COMMON_PACKET cmnpck;
	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	/*
	#define LM_CONNECTGS	0x0600	// Game Server와 연결
	#define LM_REQROOMINFO	0x0601	// Game Server 정보 요청
	#define LM_REQMANINFO	0x0602	// 전체 사용자 요청

	#define LM_REQGMN		0x0610	// 방장 신청(방 만들기.)
	#define LM_REQJOIN		0x0611	// 방 들어가기 신청
	#define LM_REQOUT		0x0612	// 방 나가기 신청
	#define LM_INVITACK		0x0613	// 초대 허락
	#define LM_INVITNOT		0x0614	// 초대 거부

	#define LM_USERUPDATE	0x0620	// 유저의 정보 변경....

	#define LM_CHATMSG		0x06ff	// 채팅 메세지
	#define LM_TGTMSG		0x060f	// 타켓 지정 채팅 메세지
	*/

	switch(cmnpck.header)
	{
	//#define LM_LOGINGS		0x0604
	case LM_LOGINGS:
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
			qp->nCode = LM_LOGINGS;
			qp->userinfo = userinfo;

			_beginthreadex(NULL, 0, threadSQLQuery, qp, 0, &ThreadID);
		}
		break;
		
	case LM_REQMANINFO:
		SendUserInfo(pMsg);
		break;

	case LM_REQROOMINFO:
		SendRoomInfo(pMsg);
		break;
		
	case LM_REQGMN:
		MakeRoom(pMsg);
		break;

	case LM_REQJOIN:
		JoinRoom(pMsg);
		break;

	case LM_REQOUT:
		OutRoom(pMsg);
		break;

	case LM_CHATMSG:
		ChatMessage(pMsg);
		break;

	case LM_USERUPDATE:
		{
			UINT ThreadID;
			USERINFO userinfo;
			LPQUERYPARAM qp = new QUERYPARAM;

			pMsg->GetData(&userinfo, sizeof(COMMON_PACKET), sizeof(userinfo));
			
			userinfo.socket = (SOCKET)pMsg->GetSocket();

			qp->pWnd = this;
			qp->nCode = LM_USERUPDATE;
			qp->userinfo = userinfo;

			_beginthreadex(NULL, 0, threadSQLQuery, qp, 0, &ThreadID);
		}
		break;

	case LM_REQNEWGAME:
		SendReqNewGame(pMsg);
		break;

	case LM_REQRJTGAME:
		CancelNewGame(pMsg);
		break;

	case LM_GAMESTATE :
		UpdateGameState(pMsg);
		break;

	}
	
}

///////////////////////////////////////////////////////
// DB Connection 객체 컨트롤.....
LPDBCONNPOOL CServerWork::GetAvailableDbconn()
{
	g_Critical.Lock();
	m_pLogDialog->AddLogString("GetAvailableDbConn() : Lock()");

	list<DBCONNPOOL>::iterator iter = dbconnlist.begin();

	int nCount = 0;

	///////////////////////////////////////////////////////////
	//	이미 만들어진 Connection 객체 중 사용중이 아닌 것을 찾음

	for(; iter != dbconnlist.end() ; iter++)
	{
		LPDBCONNPOOL lpdbconn = &(*iter);

		if(lpdbconn->pDb != NULL && !lpdbconn->bBusy)
		{
			lpdbconn->pDb->Clear();
			lpdbconn->bBusy = TRUE;

			g_Critical.Unlock();
			m_pLogDialog->AddLogString("GetAvailableDbConn() : Unock()");

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

			if(!pDbConn->Connect(3, (LPSTR)m_strDBName.data(), (LPSTR)m_strDBID.data(), (LPSTR)m_strDBPassword.data())) 
			{
				// 연결 실패했을 경우 NULL 리턴
				delete pDbConn;
				g_Critical.Unlock();
				m_pLogDialog->AddLogString("GetAvailableDbConn() : Unlock() *");

				return NULL;
			}

			else
			{
				lpdbconn->id = nCount;
				lpdbconn->pDb = pDbConn;
				lpdbconn->pDb->Clear();
				lpdbconn->bBusy = TRUE;

				g_Critical.Unlock();
				m_pLogDialog->AddLogString("GetAvailableDbConn() : Unlock() *");

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

	if(!newdbconn.pDb->Connect(3, (LPSTR)m_strDBName.data(), (LPSTR)m_strDBID.data(), (LPSTR)m_strDBPassword.data()))
	{
		delete newdbconn.pDb;
		g_Critical.Unlock();
		m_pLogDialog->AddLogString("GetAvailableDbConn() : Unlock() *");
		
		return NULL;
	}

	dbconnlist.push_back(newdbconn);

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("GetAvailableDbConn() : Unlock() *");

	return NULL;

}

////////////////////////////////////////////////////////////////
// 접속자가 요구한 유저의 정보 보내는 구조체......
void CServerWork::SendUserInfo(CMessage *pMsg)
{
	CMessage Send;

	g_Critical.Lock();

	//LPUSERINFO lpuserinfo;
	m_pLogDialog->AddLogString("Send all User info() : Lock()");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();
	// 접속한 유저들의 정보를 읽기 위해 STL 연산자 정의
	// ClientManager 내부의 clientlist(접속한 클라이언트 정보 저장)의 첫 머리를 저장.....

	LPUSER lpmyinfo = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	// 현재 요구한 사람의 유저 정보 저장(소켓으로 찾아 확인)

	int nBuffSize;

	nBuffSize = sizeof(COMMON_PACKET) + sizeof(USERINFO) * m_ClientManager.clientlist.size();
	// 보낼 패킷의 사이즈  = COMMON_PACKET + 유저 한사람의 정보 * 전체 유저들의 수......

	LPBYTE buff = new BYTE[nBuffSize];
	// 패킷을 저장할 메모리 버퍼.

	memset(buff, 0, nBuffSize);

	COMMON_PACKET cmnpck;
	cmnpck.header = GSM_ALLMANDATA; // 전체 접속 유저 데이터임을 알림.....

	int cnt = sizeof(cmnpck);

	for(; i != m_ClientManager.clientlist.end() ; i++)
	{
		LPUSER lpuser = &(*i);
		
		// 자신과 같은 채널에 존재하는 사람들만의 정보만 확인....
		if(lpmyinfo->channelno == lpuser->channelno)
		{
			memcpy(&buff[cnt], lpuser, sizeof(USERINFO));
			cnt += sizeof(USERINFO);
		}
	}

	cmnpck.length = cnt; // 전체 버퍼 길이.....
	memcpy(buff, &cmnpck, sizeof(cmnpck));

	m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)buff, cnt);

	g_Critical.Unlock();

	m_pLogDialog->AddLogString("Send all User Info() : UnLock() *");

	delete [] buff;
}


////////////////////////////////////////////////////////////////
// 접속자가 요구한 방 정보 보내는 구조체......
void CServerWork::SendRoomInfo(CMessage *pMsg)
{
	CMessage Send;
	g_Critical.Lock();

	m_pLogDialog->AddLogString("Send All Room Info() : LOCK()");

	//LPROOMINFO roominfo;
	int nBuffSize;

	unsigned short nChannelno;	// 사용자가 접속한 채널을 저장하기 위한 변수....
	pMsg->GetData(&nChannelno, sizeof(COMMON_PACKET), sizeof(nChannelno));

	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(nChannelno);
	// 사용자가 접속한 채널의 번호를 가진 채널 포인터 획득,....

	list<ROOM>::iterator i = lpchannel->roomlist.begin();
	// 채널 포인터에서 채널 내부의 RoomList에 접근 할 수 있다......

	nBuffSize = sizeof(COMMON_PACKET) + sizeof(ROOMINFO) * lpchannel->roomlist.size();
	// 전체 패킷 길이 = COMMON_PACKET길이 + 방하나의 정보 크기 *  전체 방 갯수...

	LPBYTE buff = new BYTE[nBuffSize];
	// 패킷 데이터를 저장할 버퍼.....
	memset(buff, 0, nBuffSize);

	COMMON_PACKET cmnpck;
	cmnpck.header = GSM_ALLROOMDATA;	// 게임서버내 방 정보임을 알림.

	int cnt = sizeof(cmnpck);	// 헤더의 length에 넣기위한 정보......

	// 방들의 정보 저장......
	for(; i != lpchannel->roomlist.end() ; i++)
	{
		LPROOM lproom = &(*i);

		memcpy(&buff[cnt], lproom, sizeof(ROOMINFO));
		cnt += sizeof(ROOMINFO);
	}

	cmnpck.length = cnt;	// 패킷에 길이 저장....

	memcpy(buff, &cmnpck, sizeof(cmnpck));
	
	m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)buff, nBuffSize);

	g_Critical.Unlock();

	m_pLogDialog->AddLogString("Send all Room info() : Unlock() *");

	delete [] buff;
}

////////////////////////////////////////////////////////////////
// 방 만들기
// 제약조건 : 방 생성 한계에 부딪혔을 때.
void CServerWork::MakeRoom(CMessage *pMsg)
{
	m_pLogDialog->AddLogString("*** Make Room ***");

	int nLen = sizeof(COMMON_PACKET);

	SOCKET socket = pMsg->GetSocket();
	// 날아온 패킷의 소켓을 저장....

	ROOMINFO roominfo;	// 새로운 방을 생성하기 위한 방 정보....

	pMsg->GetData(&roominfo, nLen, sizeof(roominfo));

	g_Critical.Lock();
	m_pLogDialog->AddLogString("Make Room() : Lock()");

	LPUSER lpuser = m_ClientManager.GetClient(socket);	// 소켓을 이용해 패킷보낸 클라이언트의 정보 얻음...
	LPROOM lproom = m_ChannelManager.GetRoomInfo(lpuser->channelno, roominfo.id);

	if(lproom != NULL)
	{
		if(lproom->present == 0)	// 방 접속 인원이 0일때...
		{
			COMMON_PACKET cmnpck;

			cmnpck.header = GSM_GMNGACK;	// 방장 승인....
			cmnpck.length = sizeof(cmnpck);

			// 방장이 승인 됐음을 알림.
			m_ClientSocket.SendMsg(socket, (char *)&cmnpck, sizeof(cmnpck));
			m_pLogDialog->AddLogString("::: GSM_GMNGACK (ID : %s)", (LPCTSTR)lpuser->id);

			// 생성된 룸의 정보를 방 정보 관리 객체에 저장.....
			lpuser->lproom = lproom;
			lpuser->roomno = roominfo.id;

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

		g_Critical.Unlock();
		m_pLogDialog->AddLogString("MakeRoom() : UnLock() *");
	}


}

////////////////////////////////////////////////////////////////
// 게임 참가...
// 제약조건 : 방 참가 인원수에 부딪혔을 때....
void CServerWork::JoinRoom(CMessage *pMsg)
{
	m_pLogDialog->AddLogString("*** JoinRoom ***");
	short roomno;

	g_Critical.Lock();
	m_pLogDialog->AddLogString("join room() : Lock()");
	
	LPUSER lpuser = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	pMsg->GetData(&roomno, sizeof(COMMON_PACKET), sizeof(roomno));

	LPROOM lproom = m_ChannelManager.GetRoomInfo(lpuser->channelno, roomno);

	if(lproom != NULL)
	{
		if(lproom->present == 0)	// 참가하고자 하는 방에 인원이 없을 경우...
		{
			SendErrorMessage((SOCKET)pMsg->GetSocket(), F_JOINROOM);
		}
		else if(lproom->present < lproom->maximum)	// 참가하고자 하는 방에 사람이 적정 수일때...
		{
			int cnt;
			unsigned char lpRoomManager_ip[32];	// 방장 ip저장할 포인터.
			//lpRoomManager_ip = new BYTE[32];

			lproom->present++;
			lproom->users[1] = lpuser;
			lproom->status = 1;

			//memcpy(&lpRoomManager_ip, &lproom->users[0]->ip, sizeof(lpRoomManager_ip));	// 방장 IP를 입장확인패킷에 실어 보내준다.
			
			// 접속자가 참가한 방 번호와 포인터 저장.....
			lpuser->roomno = roomno;
			lpuser->lproom = lproom;	

			COMMON_PACKET cmnpck;
			
		//	SendCompetitorInfo(lproom);
		// 	m_pLogDialog->AddLogString("SendCompetitorInfo()");

			cmnpck.header = GSM_JOINACK;
			cnt = sizeof(COMMON_PACKET) + sizeof(lpRoomManager_ip);
			cmnpck.length = cnt;
			memcpy(&lpRoomManager_ip, &lproom->users[0]->ip, sizeof(lpRoomManager_ip));	// 방장 IP를 입장확인패킷에 실어 보내준다.

			LPBYTE temppacket = new BYTE[cnt];	// 실제로 전송할 패킷.

			memset(temppacket, 0, sizeof(temppacket));

			memcpy(temppacket, &cmnpck, sizeof(COMMON_PACKET));
			memcpy(&temppacket[sizeof(COMMON_PACKET)], (char *)lpRoomManager_ip, sizeof(lpRoomManager_ip));


			// 메세지 뒷부분에 ip 삽입.

			m_ClientSocket.SendMsg(lpuser->socket, (char *)temppacket, cmnpck.length);
			m_pLogDialog->AddLogString("::: GSM_JOINACK (id : %s, compet id : %s)",(LPCTSTR)lpuser->id, (LPCTSTR)lproom->users[0]->id);

			NotifyAllUpdate((LPUSERINFO)lpuser, lproom);

		}
	}

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("JoinRoom() : Unlock() *");

}

////////////////////////////////////////////////////////////////
// 방 나가기......
// 조건 : 1. 참가자가 원할 때, 2. 게임이 끝났을 때....
void CServerWork::OutRoom(CMessage *pMsg)
{
	m_pLogDialog->AddLogString("*** OutRoom ***");

	g_Critical.Lock();
	m_pLogDialog->AddLogString("OutRoom() : Lock()");

	// 현재 패킷을 보낸 유저를 찾음 (SOCKET 이용)
	LPUSER lpuser = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	// 현재 패킷을 보낸 유저가 들어있는 방 정보 변경위한 변수.....
	LPROOM lproom = lpuser->lproom;

	if(lproom != NULL)
	{
		lproom->present--;	// 유저가 소속된 방의 인원수 감소....
		
		lpuser->lproom = NULL;	// 유저의 방 포인터를 NULL로 
		lpuser->roomno = -1;	// 유저의 위치 : -1 (대기실)
		
		if(lproom->present == 0)	// 방에 아무도 안남아 있을때 (현재 빠져나간 사람이 마지막 사람일때)
		{
			lproom->users[0] = NULL;
			lproom->users[1] = NULL;

			memset(lproom->title, 0, sizeof(lproom->title));
			memset(lproom->passwd, 0, sizeof(lproom->passwd));

			strcpy((char *)lproom->title, "빈방");
		}
		else	
		{
			if(lproom->users[0] == lpuser)	// 방장일 경우....
				lproom->users[0] = lproom->users[1];	// 자신 다음의 사람을 방장으로 임명.....
			else if(lproom->users[1] == lpuser)
				lproom->users[1] = NULL;

			memset(lproom->title, 0, sizeof(lproom->title));
			memset(lproom->passwd, 0, sizeof(lproom->passwd));

			strcpy((char *)lproom->title, "들어오세요.");

			COMMON_PACKET cmnpck;

			// 방장이 되었음을 통보......
			cmnpck.header = GSM_OWNROOM;
			cmnpck.length = sizeof(cmnpck);

			m_ClientSocket.SendMsg(lproom->users[0]->socket, (char *)&cmnpck, sizeof(cmnpck));

			m_pLogDialog->AddLogString("::: GSM_OWNROOM (id : %s)", (LPCTSTR)lproom->users[0]->id);

		}

		NotifyAllUpdate((LPUSERINFO)lpuser, lproom);
	}

	g_Critical.Unlock();

	m_pLogDialog->AddLogString("outroom() : unlock() *");

}

void CServerWork::ChatMessage(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	LPUSER lpuserinfo = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());

	g_Critical.Lock();
	m_pLogDialog->AddLogString("ChatMessage() : Lock()");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();

	cmnpck.header = GSM_CHATMSG;
	cmnpck.length = pMsg->GetLength();

	pMsg->SetData(&cmnpck, 0, sizeof(cmnpck));

	for(; i != m_ClientManager.clientlist.end() ; i++)
	{
		LPUSER lpuser = &(*i);

		if(lpuser->channelno == lpuserinfo->channelno)
		{
			m_ClientSocket.SendMsg(lpuser->socket, (char *)pMsg->GetDataPtr(), pMsg->GetLength());
		}
	}

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("ChatMessage() : UnLock()");

}

void CServerWork::NotifyAllUpdate(LPUSERINFO lpuserinfo, LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;

	USERINFO userinfo;
	ROOMINFO roominfo;

	g_Critical.Lock();
	m_pLogDialog->AddLogString("Notify All Update() : Lock()");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();
	
	cmnpck.header = GSM_ALLUPDATE;
	cmnpck.length = sizeof(COMMON_PACKET) + sizeof(userinfo) + sizeof(roominfo);

	memcpy(&userinfo, lpuserinfo, sizeof(userinfo));
	memcpy(&roominfo, lproom, sizeof(roominfo));

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&userinfo, sizeof(cmnpck), sizeof(userinfo));
	Send.SetData(&roominfo, sizeof(cmnpck) + sizeof(userinfo), sizeof(roominfo));

	for(; i != m_ClientManager.clientlist.end() ; i++)
	{	
		LPUSER lpuser = &(*i);

		if(lpuser->channelno == lproom->channelno)
		{
			int nResult = m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

			m_pLogDialog->AddLogString("All update : &s", (LPCTSTR)lpuser->id);

		}
	}	

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("NotifyAllUpdate() : unlock() *");

}

void CServerWork::SendErrorMessage(SOCKET socket, FAILURE failure)
{
	CMessage Send;	// 에러를 보낼 메세지...

	COMMON_PACKET cmnpck;

	cmnpck.header = GSM_FAILURE;
	cmnpck.length = sizeof(cmnpck) + sizeof(failure);

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&failure, sizeof(cmnpck), sizeof(failure));

	m_ClientSocket.SendMsg(socket, (char *)Send.GetDataPtr(), Send.GetLength());

}

void CServerWork::SendCompetitorInfo(LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	USERINFO compet[4];

	cmnpck.header = GSM_COMPETINFO;
	cmnpck.length = sizeof(cmnpck) + sizeof(USERINFO);

	// 이부분 보완 필요.....
	// 4명이 모두 다 같이 접속자를 볼 수 있게....

	memset(&compet[0], 0, sizeof(USERINFO));
	memcpy(&compet[0], lproom->users[0], sizeof(USERINFO));

	memset(&compet[1], 0, sizeof(USERINFO));
	memcpy(&compet[1], lproom->users[1], sizeof(USERINFO));

	for(int i=0 ; i<lproom->status ; i++)
	{
		Send.SetData(&cmnpck, 0,  sizeof(cmnpck));
		Send.SetData(&compet[i], sizeof(cmnpck), sizeof(USERINFO));

		m_ClientSocket.SendMsg(lproom->users[i]->socket, (char *)Send.GetDataPtr(), Send.GetLength());
		Send.Clear_Data();
	}
}

void CServerWork::ClientDisconnected(SOCKET socket)
{
	CMessage Send;
	COMMON_PACKET cmnpck;

	g_Critical.Lock();
	m_pLogDialog->AddLogString("ClientDisconnected() : Lock()");

	// Disconnected된 유저의 정보와 채널 위치 정보 얻기.
	LPUSER lpuser = m_ClientManager.GetClient(socket);
	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);
	LPROOM lproom = lpuser->lproom;

	if(strlen((char *)lpuser->id) > 0)
	{
		lpchannel->present--;

		lpuser->roomno = -2;	// 접속 해제한 클라이언트...
		NotifyClientUpdate((LPUSERINFO)lpuser);

	}

	if(lpuser->lproom != NULL)	// 방에 들어 있었을 경우...
	{
		if(lproom->status == 2)	// 게임중에 튀어 나간 경우.
		{
			if(lproom->users[0] == lpuser)	// 하필이면 지가 방장일 경우
			{
				lproom->users[0]->playcount++;
				lproom->users[0]->losscount++;

				lproom->users[1]->playcount++;
				lproom->users[1]->victorycount++;

				lproom->users[2]->playcount++;
				lproom->users[2]->victorycount++;

				lproom->users[3]->playcount++;
				lproom->users[3]->victorycount++;
		
			}
			else if (lproom->users[1] == lpuser)
			{
				lproom->users[1]->playcount++;
				lproom->users[1]->losscount++;

				lproom->users[0]->playcount++;
				lproom->users[0]->victorycount++;

				lproom->users[2]->playcount++;
				lproom->users[2]->victorycount++;

				lproom->users[3]->playcount++;
				lproom->users[3]->victorycount++;

			}
			else if (lproom->users[2] == lpuser)
			{
				lproom->users[2]->playcount++;
				lproom->users[2]->losscount++;

				lproom->users[1]->playcount++;
				lproom->users[1]->victorycount++;

				lproom->users[0]->playcount++;
				lproom->users[0]->victorycount++;

				lproom->users[3]->playcount++;
				lproom->users[3]->victorycount++;

			}
			else if (lproom->users[3] == lpuser)
			{
				lproom->users[3]->playcount++;
				lproom->users[3]->losscount++;

				lproom->users[1]->playcount++;
				lproom->users[1]->victorycount++;

				lproom->users[2]->playcount++;
				lproom->users[2]->victorycount++;

				lproom->users[0]->playcount++;
				lproom->users[0]->victorycount++;

			}

			UpdateClientInfo(*(LPUSERINFO)(lproom->users[0]));
			UpdateClientInfo(*(LPUSERINFO)(lproom->users[1]));
			UpdateClientInfo(*(LPUSERINFO)(lproom->users[2]));
			UpdateClientInfo(*(LPUSERINFO)(lproom->users[3]));
		}

		lproom->present--;
		lproom->status = 0;

		if(lproom->present > 0)	// 
		{
			lproom->users[0] = lproom->users[1];
			lproom->users[1] = lproom->users[2];
			lproom->users[2] = lproom->users[3];
			lproom->users[3] = NULL;

			cmnpck.header = GSM_OWNROOM;
			cmnpck.length = sizeof(cmnpck);

			m_ClientSocket.SendMsg(lproom->users[0]->socket, (char *)&cmnpck, sizeof(cmnpck));
		}
		else
		{
			lproom->users[0] = NULL;
			lproom->users[1] = NULL;
			lproom->users[2] = NULL;
			lproom->users[3] = NULL;

			memset(lproom->passwd, 0, sizeof(lproom->passwd));
			memset(lproom->title, 0, sizeof(lproom->title));

			strcpy((char *)lproom->title, "빈방");

			lproom->locked = 0;
			lproom->present = 0;
			lproom->status = 0;

		}
		m_ClientManager.DeleteClient(socket);
		NotifyRoomUpdate(lproom);
	}

	else m_ClientManager.DeleteClient(socket);

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("ClientDisconnected() : Unlock() *");

}

/////////////////////////////////////////////////////////////////////
//	
void CServerWork::NotifyClientUpdate(LPUSERINFO lpuserinfo)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	USERINFO userinfo;

	g_Critical.Lock();
	m_pLogDialog->AddLogString("Notifyclientupdate() : Lock()");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();

	cmnpck.header = GSM_UPDATEROOM;
	cmnpck.length = sizeof(cmnpck) + sizeof(userinfo);

	memcpy(&userinfo, lpuserinfo, sizeof(userinfo));

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&userinfo, sizeof(cmnpck), sizeof(userinfo));

	for(; i != m_ClientManager.clientlist.end() ; i++)
	{
		LPUSER lpuser = &(*i);
		
		if(lpuser->channelno == lpuserinfo->channelno)
		{
			int nResult = m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

			m_pLogDialog->AddLogString("%d", lpuser->socket);
		}
	
	}

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("Notifyclientupdate() : UnLock()");
}

/////////////////////////////////////////////////////////////////////
//	방정보 업데이트......같은 채널에 있는 사람들에게 바뀐 방 정보 알림.
void CServerWork::NotifyRoomUpdate(LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	ROOMINFO roominfo;

	g_Critical.Lock();
	m_pLogDialog->AddLogString("NotifyRoomUpdate() : Lock() *");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();	// 같은 채널에 존재하는 접속자들.
	
	cmnpck.header = GSM_UPDATEROOM;
	cmnpck.length = sizeof(cmnpck) + sizeof(ROOMINFO);

	memcpy(&roominfo, lproom, sizeof(roominfo));

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&roominfo, sizeof(cmnpck), sizeof(roominfo));
	
	for(; i != m_ClientManager.clientlist.end() ; i++)
	{
		LPUSER lpuser = &(*i);

		if(lpuser->channelno == lproom->channelno)
		{
			int nResult = m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

			m_pLogDialog->AddLogString("RoomUpdate : %s", (LPCTSTR)lpuser->id);
		}
	}
	
	g_Critical.Unlock();
	m_pLogDialog->AddLogString("NotifyRoomUpdate() : UnLock() *");
}

void CServerWork::ClientLogin(USERINFO userinfo)
{
	LPCHANNEL lpch = m_ChannelManager.GetChannelInfo(userinfo.channelno);
	
	g_Critical.Lock();
	m_pLogDialog->AddLogString("ClientLogin() : Lock()");

	if(lpch->present == lpch->maximum)
	{
		SendErrorMessage(userinfo.socket, F_CHANNELFULL);
		m_ClientManager.DeleteClient(userinfo.socket);
		g_Critical.Unlock();
		m_pLogDialog->AddLogString("ClientLogin() : UnLock()");

		return;
	}

	LPUSER user = m_ClientManager.GetClient((char *)userinfo.id);

	CMessage Send;

	COMMON_PACKET cmnpck;

	if(user != NULL)
	{
		g_Critical.Unlock();
		m_pLogDialog->AddLogString("ClientLogin() : UnLock()");
		SendErrorMessage(userinfo.socket, F_ALREADYLOGIN);
	}
	else
	{
		LPDBCONNPOOL lpavdb = GetAvailableDbconn();
							  
		if(lpavdb == NULL)
		{
			SendErrorMessage(userinfo.socket, F_DBERROR);
			g_Critical.Unlock();
			m_pLogDialog->AddLogString("ClientLogin() : UnLock()");

			return;
		}

		CString strQuery;

		user = m_ClientManager.GetClient(userinfo.socket);
		strQuery.Format("Select userid, name, alias, score, playcount, victorycount, losscount, drawcount From userinfo WHERE userid = '%s'", userinfo.id);
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

		m_pLogDialog->AddLogString("Before Unlock()");

		g_Critical.Unlock();
		m_pLogDialog->AddLogString("ClientLogin() : UnLock()");
	}

}


void CServerWork::UpdateClientInfo(USERINFO userinfo)
{
	g_Critical.Lock();
	m_pLogDialog->AddLogString("UpdateClientInfo() : Lock()");

	LPUSER lpuser = m_ClientManager.GetClient(userinfo.socket);

	lpuser->victorycount = userinfo.victorycount;
	lpuser->losscount = userinfo.losscount;
	lpuser->drawcount = userinfo.drawcount;
	lpuser->playcount = userinfo.playcount;
	lpuser->score = userinfo.score;

	LPDBCONNPOOL lpavdb = GetAvailableDbconn();

	if(lpavdb == NULL)
	{
		SendErrorMessage(userinfo.socket, F_DBERROR);
		g_Critical.Unlock();
		m_pLogDialog->AddLogString("UpdateClientInfo() : UnLock()");

		return;
	}

	CString strQuery;

	strQuery.Format("UPDATE userinfo SET score2='%d', playcount2='%d', victorycount2='%d', losscount2='%d', drawcount2='%d', WHERE userid='%s'",
		lpuser->score,
		lpuser->playcount,
		lpuser->victorycount,
		lpuser->losscount,
		lpuser->drawcount,
		lpuser->id);

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("UpdateClientInfo() : UnLock()");
	
	m_pLogDialog->AddLogString("DBConn id : [%d], %s", lpavdb->id, strQuery);

	lpavdb->pDb->SQLQuery(strQuery);
	lpavdb->pDb->Clear();
	lpavdb->bBusy = FALSE;

}


unsigned __stdcall threadSQLQuery(LPVOID arg)
{
	LPQUERYPARAM lpqp = (LPQUERYPARAM)arg;

	switch(lpqp->nCode)
	{
	//#define LM_LOGINGS		0x0604
	case LM_LOGINGS:
		lpqp->pWnd->ClientLogin(lpqp->userinfo);
		break;

	case LM_REQMANINFO:
		lpqp->pWnd->UpdateClientInfo(lpqp->userinfo);
		break;
	}

	delete [] lpqp;

	_endthreadex(0);
	return 0;
}

void CServerWork::SendReqNewGame(CMessage *pMsg)
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
			m_pLogDialog->AddLogString("SendReqNewGame() : Lock()");

			LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);

			list<ROOM>::iterator i = lpchannel->roomlist.begin();

			for(; i != lpchannel->roomlist.end() ; i++)
			{
				LPROOM lproom = &(*i);

				if(lproom->present == 0)
				{
					COMMON_PACKET cmnpck;

					cmnpck.header = GSM_OWNROOM;
					cmnpck.length = sizeof(cmnpck);

					m_ClientSocket.SendMsg(lprequester->socket, (char *)&cmnpck, sizeof(cmnpck));
					m_pLogDialog->AddLogString("::: GSM_OWNROOM(id : %s)", (LPCTSTR)lprequester->id);

					lprequester->lproom = lproom;
					lprequester->roomno = lproom->id;

					strcpy((char *)lproom->title, "***예 약***");
					lproom->present = 1;
					lproom->users[0] = lprequester;
					lproom->locked = 0;
					NotifyAllUpdate((LPUSERINFO)lprequester, lproom);

					cmnpck.header = GSM_REQNEWGAME;
					cmnpck.length = sizeof(cmnpck) + sizeof(id) + sizeof(lproom->id);

					strcpy(id, (char *)lprequester->id);

					Send.SetData(&cmnpck, 0, sizeof(cmnpck));
					Send.SetData(id, sizeof(cmnpck), sizeof(id));
					Send.SetData(&lproom->id, sizeof(cmnpck)+sizeof(id), sizeof(lproom->id));

					g_Critical.Unlock();
					m_pLogDialog->AddLogString("SnedReqNewGame() : Unlock() *");

					m_ClientSocket.SendMsg(lpuser->socket, (char *)Send.GetDataPtr(), Send.GetLength());

					return;
				}
			}
			g_Critical.Unlock();
			m_pLogDialog->AddLogString("SnedReqNewGame() : Unlock() *");

			SendErrorMessage((SOCKET)pMsg->GetSocket(), F_NOROOM);
		}
		else
		{
			cmnpck.header = GSM_REJECTGAME;
			cmnpck.length = sizeof(cmnpck);
			Send.SetData(&cmnpck, 0, sizeof(cmnpck));
			m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
		}
	}
	
}

void CServerWork::CancelNewGame(CMessage *pMsg)
{
	char id[32] = {0, };
	pMsg->GetData(id, sizeof(COMMON_PACKET), sizeof(id));

	LPUSER lpuser = m_ClientManager.GetClient(id);

	if(lpuser != NULL)
	{
		pMsg->SetSocket(lpuser->socket);
		OutRoom(pMsg);

		CMessage Send;
		COMMON_PACKET cmnpck;

		cmnpck.header = GSM_REJECTGAME;
		cmnpck.length = sizeof(cmnpck);

		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
	}
}

void CServerWork::UpdateGameState(CMessage *pMsg)
{
	SOCKET socket = pMsg->GetSocket();

	short gamestatus;

	pMsg->GetData(&gamestatus, sizeof(COMMON_PACKET), sizeof(gamestatus));
	
	LPUSER lpuser = m_ClientManager.GetClient(socket); // 접속 해제된 클라이언트 검색
	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);
	LPROOM lproom = lpuser->lproom;
	
	if(lproom != NULL) lproom->status = gamestatus;

	
}
