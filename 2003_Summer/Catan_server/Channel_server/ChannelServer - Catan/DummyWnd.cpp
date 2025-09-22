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
// �ۼ���¥ : 2002�� 12�� 23��
//
// Ʈ���� ������ ���콺 �޼��� ��Ʋ�� �Լ�
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
		AfxMessageBox("���α׷��� �׾��η����ϴ�");
		return -1;
	}
		
	// Ʈ���� ������ ����
	if(!m_TrayIcon.Create(this, WM_TRAYNOTIFY, "Catan Channel", LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), IDC_TRAYICON))
	{
		AfxMessageBox("���α׷��� �׾��η����ϴ�");
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
	// ������ ����� ���� �ʱ�ȭ 
	if(!m_ServerSocket.InitSock()) return -1;
	if(!m_ServerSocket.Initialize(m_nPortServer)) // m_nPortServer = 10000;
	{
		AfxMessageBox("���� ���� �ʱ�ȭ�� �����߽��ϴ�.");
		return -1;
	}
	//AfxMessageBox("���� ���� �ʱ�ȭ�Ϸ�(GameServer)");

	// Ŭ���̾�Ʈ�� ����� ���� �ʱ�ȭ
	if(!m_ClientSocket.Initialize(m_nPortClient)) // m_nPortClient = 12000;
	{
		AfxMessageBox("���� ���� �ʱ�ȭ�� �����߽��ϴ�.");
		return -1;
	}
	//AfxMessageBox("���� ���� �ʱ�ȭ�Ϸ�(LobbyClient)");
	
	//AfxMessageBox("�κ� ���� �غ� �Ϸ�", MB_OK);

	return 0;
}


////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 23��
//
// ���� ����
//
// params
// 

void CDummyWnd::OnStartserver()
{
	AfxMessageBox("ä�� ���� Start", MB_OK);
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
// �ۼ���¥ : 2002�� 12�� 23��
//
// ���� ����
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
// �ۼ���¥ : 2002�� 12�� 23��
//
// ���� ����
//
// params
// 

void CDummyWnd::OnConfig() 
{
	// TODO: Add your command handler code here
	
}



////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 23��
//
// ���� ���� ����
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
// �ۼ���¥ : 2002�� 12�� 29��
//
// �ֱ������� ä�� ���� ��û
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


	// ���� ����� ������ ä������ ��û
	for(int i = 0 ; i < nLen ; i++)
		m_ServerSocket.SendMsg(sockets[i], (char *)Send.GetDataPtr(), Send.GetLength());

	TRACE("REQUEST CHANNEL INFO\n");
	
	SetTimer(0, TIMER_INTERVAL, NULL);

	CWnd::OnTimer(nIDEvent);
}


////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 23��
//
// ���Ӽ����� ����ϴ� thread
//
// params
// arg : CDummyWnd�� �ν��Ͻ� ������, ������ ������ socket

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

				SOCKET serversocket = m_ServerSocket.Accept();  // ���� ��û�� ���� ������ ���
				if(serversocket == INVALID_SOCKET) return 0L;		// �߸��� �����̸� �ٽ� ���� ���
				m_ServerManager.AddServer(serversocket);	// CServerManager Ŭ������ ���� �߰�

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
// �ۼ���¥ : 2002�� 12�� 23��
//
// Ŭ���̾�Ʈ�� ����ϴ� thread
//
// params
// arg : CDummyWnd�� �ν��Ͻ� ������, ������ ������ socket


long CDummyWnd::OnSocketMsgClient(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("�κ� �޼���", MB_OK);

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
		SOCKET clientsocket = m_ClientSocket.Accept();  // ���� ��û�� ���� ������ ���
		if(clientsocket == INVALID_SOCKET) return 0L;		// �߸��� �����̸� �ٽ� ���� ���
		
		WSAAsyncSelect(clientsocket, GetSafeHwnd(), WM_SOCKETMSGCLIENT, FD_READ | FD_CLOSE);
		
		cmnpck.header = CSM_CONNECTLB;
		cmnpck.length = sizeof(COMMON_PACKET);
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		m_ClientSocket.SendMsg(clientsocket, (char *)Send.GetDataPtr(), Send.GetLength());

//		AfxMessageBox("�κ� ����", MB_OK);
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
// �ۼ���¥ : 2002�� 12�� 23��
//
// ��ũ�p���κ��� ���� ���� ��Ŷ ������ ó��
//
// params
// arg : CDummyWnd�� �ν��Ͻ� ������

void CDummyWnd::ProcessMessageServer(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header) // ��Ŷ ��� �˻�
	{
		case GSM_SERVERINFO : // ���� ���� ������ ���
			{
				//AfxMessageBox("���� ���� ���� ����.");
				SERVERINFO si;
				CHANNELINFO ci;

				int nLen = pMsg->GetData(&si, 0, sizeof(si));  // �켱 ���� ���� ��ü�� ������ ����

				LPSERVER server = m_ServerManager.GetServer((SOCKET)pMsg->GetSocket()); // ��Ŷ�� ���� ������ ã��

				if(server != NULL) // �������� ���� �����͸� �������� ���
				{
					server->id = si.id;	// ������ ���̵� ����
					server->ip = (char *)si.ip;	// ������ ����
					server->portno = si.port;	// ��Ʈ��ȣ ����
					server->maxchannel = si.channels;
					server->rooms = si.rooms;

					if(!server->channel.size())	// ���ʿ� ������ ������ ��� ä�������� ����
					{
						CHANNEL channel;

						for(int i = 0 ; i < si.channels ; i++) // ��ü ä�ΰ�����ŭ
						{
							nLen += pMsg->GetData(&ci, nLen, sizeof(ci)); // ä�� ������ �ϳ��� �о��

							channel = ci;	// ä�������� �ϳ��� ����
							server->channel.push_back(channel); // ä���� ����Ʈ�� �߰�
						}
					}
					else // �̹� ä�� ������ ������ ��� // �ֱ����� ���� ������ ��û�� ���
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

// �κ� Ŭ���̾�Ʈ���� ���.
void CDummyWnd::ProcessMessageClient(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header) // ��Ŷ �ش� �˻�
	{
		case LM_LOGIN :
			SendChInfo((SOCKET)pMsg->GetSocket());
			break;

		case LM_REQCHINFO :
			SendChInfo((SOCKET)pMsg->GetSocket());
			//RedirectClient(pMsg);
			break;
//#define LM_REQGSMSVR	0x0502	// ���Ӽ��� ����
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

	// Ŭ���̾�Ʈ�� ������ ä���� �����ϴ� ���� ���̵� ����
	// �������� ä�� ũ�Ⱑ ���� �ٸ� �� ����
	for(int i = 0 ; i < nLen ; i++)
	{
		lpserver = m_ServerManager.GetServer(i);
		// GetServer�� lpserver�� return....
		// ���⼭ �޸� �ּ� ���� �߻�.....
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
		si.rooms = channelno; // rooms�� ��� channelno�� �� rooms�� ���߿� Game Server�κ��� ���� ����
		
	//	LPCHANNEL temp = 

		Send.SetData(&si, 0, sizeof(SERVERINFO));

		m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)Send.GetDataPtr(), Send.GetLength());
	}
}
