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
	m_nMaxUser = 2;	// �ִ� ������ �� ����..

	

////////////////////////////////////////////
// DB Connection Pool ���� �⺻ DBPOOLSIZE ��

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
	// TrayIcon ���� ��....
	if(!m_TrayIcon.Create(this, WM_TRAYNOTIFY, "īź ���� ����", LoadIcon(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDR_MAINFRAME)), IDC_TRAYICON))
	{
		return -1;
	}
	
	// Dialog â ����.
	CCatan_GameserverDlg Dlg;

	if(!Dlg.LoadSetting())
	{
		while(Dlg.DoModal() == IDOK)
		{
			if(Dlg.SaveSetting()) break;
		}
	}

	// Dialog�� ���� ���� ���� �޾� ����.
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

	// ������ ��� ���� �ʱ�ȭ...
	// Ŭ���̾�Ʈ ��ſ�.... ���ø����̼��� ����� �Բ� ����ȴ�.
	if(!m_ServerSocket.InitSock()) return -1;
	if(!m_ServerSocket.Initialize(m_nGameServerPort))
	{
		m_pLogDialog->AddLogString("���� ���� �ʱ�ȭ ����");
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

// ���� ����
BOOL CServerWork::InitServer()
{
	 m_pLogDialog->AddLogString("*** InitServer ***");	// Log â�� ǥ��
	 m_ChannelManager.InitializeChannel(m_nServerID, m_nMaxChannelCount, m_nMaxRoomCount, m_nMaxUser);
	 // Channel Initialize... ä�� ����.

	 return TRUE;
}

// ! ���� ! : ������ ������ ä�� ���� ���� �ʴ´�!
void CServerWork::OnServerstart() 
{
	// TODO: Add your command handler code here
	InitServer();

	m_pLogDialog->ShowWindow(SW_SHOW);

	// ä�μ������� ����...
	if(!m_ClientSocket.Connect((LPSTR)m_strChannelIP.data(), m_nChannelPort))
	{
		m_pLogDialog->AddLogString("ä�μ��� ���� ����...");
		m_pLogDialog->AddLogString("IP   ��ȣ : %s", m_strChannelIP.data());
		m_pLogDialog->AddLogString("��Ʈ ��ȣ : %d", m_nChannelPort);
		return;
	}

	m_pLogDialog->AddLogString("ä�� �������� ���� �Ϸ�...");
	m_pLogDialog->AddLogString("��Ʈ ��ȣ : %d", m_nChannelPort);

	SendServerInfo();
	
	m_pLogDialog->AddLogString("ä�� ������ ���� ���� ���� ����");
	
	// Ŭ���̾�Ʈ ���� ���,
	WSAAsyncSelect(m_ClientSocket.m_Socket, m_hWnd, WM_SOCKETMSGCHSERVER, FD_READ | FD_CLOSE);
					// 1. �̺�Ʈ �˸��� �䱸�ϴ� ���� 2. ��Ʈ��ũ �̺�Ʈ �߻��� �޼��� ���� ������ �ڵ�
					// 3. ��Ʈ��ũ �̺�Ʈ �߻��� ���� �޽���. 4. ��Ʈ��ũ �̺�Ʈ �޼����� �߻���Ű���� �ϴ� �̺�Ʈ��...
	// ���Ͽ� ��Ʈ��ũ �̺�Ʈ(FD_READ | FD_CLOSE)�� �Ͼ�� ������ �޼��� WM_SOCKETMSGCHSERVER �߻�
	// ������ �ͺ��ŷ���� �۵��ϰ� �ȴ�.

	// �κ� Ŭ���̾�Ʈ�� ������ ���....
	m_pLogDialog->AddLogString("*** Listen ���� ***");
	m_ServerSocket.Listen();
	WSAAsyncSelect(m_ServerSocket.m_Socket, m_hWnd, WM_SOCKETMSGCLIENT, FD_ACCEPT | FD_CLOSE);
	// ServerSocket�� ���� ��� ���·�...
	// ���� �̺�Ʈ FD_ACCEPT | FD_CLOSE �� ������ �޼��� WM_SOCKETMSGCLIENT �߻�

	
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
	m_pLogDialog->AddLogString("*** ���� ���� ***");
	m_ChannelManager.ClearChannel();
	// ���� ������ ä�ε��� ���� ����.
	// ClearChannel �Լ��� ä�� ������ ��鵵 ���� �ı��Ѵ�.

	//PostQuitMessage(0);
	// ���ø����̼� ���� �޼���.
	
}

///////////////////////////////////////////////////////////////
// Channel Server���Լ� �޼����� �޾������� �̺�Ʈ ó��.....
long CServerWork::OnSocketMsgChServer(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;
	// �Ķ���� ���ڰ����� �Ѿ�� ������ ����Ѵ�.
	// �Ѿ�� ������ ä�� ������ Connecting �� ����....

	CMessage Recv;			// ���� �޼����� ó���� ����ü...
	COMMON_PACKET cmnpck;	// Message Header -> ��ü �޼����� ���̿� Ÿ���� ���� �˾� ó���ϱ� ���� ���.

	memset(&cmnpck, 0, sizeof(cmnpck));
	// ��� ��Ŷ �ʱ�ȭ...

	// ���� ��Ŷ�� Byte ũ�� ������ ����...
	int nRead1 = 0;	// ��õ���
	int nRead2 = 0;	// �ϴ� �̰� �ַ� ����.

	LPBYTE readbuff = NULL;

	DWORD dwError = 0;

	// �̺κ��� ���ƿ� ��Ŷ�� ������� ��� �κ��̴�.
	// ���� ��Ŷ�� ���۵ɶ�, ��� �޺κ��� �߷��� �� ���, ��ü ��Ŷ�� ���� ���� �ޱ����� 
	// ����� ���� �޾� �ޱ� ó���� �Ѵ�....
	if(WSAGETSELECTEVENT(lParam) == FD_READ)	// �̺�Ʈ�� �߻��� ������ �̺�Ʈ �Ķ����....
	{
		nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);	
		// nRead�� ��� ���� ����..recv �Լ��� cmnpck�� ��Ŷ �����鼭 ���� ����Ʈ �� ��ȯ.

		if(nRead2 < (int)sizeof(cmnpck))	// ���� ��Ŷ ũ�Ⱑ ������ (�ΰ����� ��� : �� �޾Ұų�, �� �޾Ұų�)
		{
			if(nRead2 < 0)	// nRead�Լ��� ��Ŷ�� �����ϰ� ������ -1�� �����Ѵ�(�ƿ� ���޾Ҵ�)
			{
				nRead2 = 0;
				nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);	// �߸��޾����� �ٽ� �ޱ�
				
				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			else	// �ޱ� �޾Ҵµ� ������ ���.
			{
				nRead1 = recv(socket, (char *)&cmnpck+nRead2, sizeof(cmnpck)-nRead2, 0);

				if(nRead1 > 0)
					nRead2 += nRead1;
			}

			if(nRead1 < 0) return 0L;		// �ƿ� ������ ���ߴ�.
		}
	

		readbuff = new BYTE[cmnpck.length];		// ���� ��Ŷ�� ������ ���� ����.
		memset(readbuff, 0, sizeof(readbuff));
		memcpy(readbuff, &cmnpck, sizeof(cmnpck));	// ���� ��Ŷ ����.

		// ����� �ڵ忡�� ��ü ���� ��Ŷ�� ����� �����Ͽ� ���� ��Ŷ�� �޾ƾ� �ϴ��� �����Ͽ���.
		// �� �κп����� ������ ��Ŷ�� �޾� ó���ϴ� �κ��̴�.
		// nRead2�� ��Ŷ ���̰� ��ġ�Ҷ� ��ü ��Ŷ�� �� �°�.
		while(nRead2 < cmnpck.length)	// �翬�� �۰���...��Ŷ ��� ���̸� ����� nRead2�ϰ� ��ü ���̰� ��õ� cmnpck.length�ϰ� ���� ���� �ֳ�.
		{
			nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
			// nRead1�� ��� ��Ŷ�� �޺κ�(������ �κ�)�� ��ü ��Ŷ ����-�����Ŷ���� ��ŭ �޴´�...
		
			if(nRead1 < 0)	// -1 �߻���(����)
			{
				nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);

				if(nRead1 > 0) nRead2 += nRead1;	// nRead2�� while������ ��� �˻��....
			}
			else nRead2 += nRead1; 

			dwError = WSAGetLastError();	// ���Ͽ����� ���� �̺�Ʈ Ȯ��.....

			if(dwError == WSAESHUTDOWN || dwError == WSAECONNRESET)
				break;								// ������ �׾��� ��� �ٷ� �ƿ�...(�۾��� ���ǹ�...���� ���� ����)
			else if(dwError == WSAEWOULDBLOCK)		// ���� �� ��찡 �ƴ϶��, �ϴ� ������ �ڻ쳪������ �ʾҴ� ���.
			{
				unsigned long ByteToRead;			// ���� ����Ʈ �� ����.
				ioctlsocket(socket, FIONREAD, &ByteToRead);

				if(!ByteToRead) return 0L;
			}

		}

		// ��Ŷ�� ���� �޾����Ƿ� ���� ��Ŷ�� �޼��� ����ü�� �����Ѵ�.
		// �޼��� ����ü���� ������ �ִ� ������, ���� �޼����� ���� ���� ó���� ���ֱ� ���ؼ��̴�.
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
// Lobby Client���Լ� �޼����� ���� ����� �̺�Ʈ ó��....
long CServerWork::OnSocketMsgClient(WPARAM wParam, LPARAM lParam)
{
	// Packet ó�� �ʱ�ȭ �۾�
	// recv�Լ��� ������ ����, ��Ŷ �쵵�� ������ ����ü, ��ü �޽����� ������ ����....
	SOCKET socket = (SOCKET)wParam;

	CMessage Recv;

	DWORD dwError;
	LPBYTE readbuff;

	int nRead1 = 0;
	int nRead2 = 0;

	COMMON_PACKET cmnpck;

	memset(&cmnpck, 0, sizeof(cmnpck));

	if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)	// �������� ���� ��û ó��...
	{
		SOCKET clientsocket = m_ServerSocket.Accept();
		// ���� ���� ��û�� ���� �������� ���� �۾�...............

		// if(socket == INVALID_SOCKET) return 0L;

		m_ClientManager.AddClient(clientsocket);
		// ������ �㰡�� ������ Ŭ���̾�Ʈ �Ŵ����� �߰�.

		WSAAsyncSelect(clientsocket, m_hWnd, WM_SOCKETMSGCLIENT, FD_READ | FD_CLOSE);
		// ���� �㰡�� ���Ͽ��� �߻��� �� �ִ� ��Ʈ�� �̺�Ʈ�� ���� ������ �ͺ�� �����·� �д�.

		// ������ �㰡 �Ǿ����Ƿ� Ŭ���̾�Ʈ���� �㰡 �޼��� ������..
		CMessage Send;
		Send.Clear_Data();

		cmnpck.header = GSM_CONNECTLB;
		cmnpck.length = sizeof(COMMON_PACKET);
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));

		m_ClientSocket.SendMsg(clientsocket, (char *)Send.GetDataPtr(), Send.GetLength());
	}
	else if(WSAGETSELECTEVENT(lParam) == FD_READ)	// ������ �۾��� ��û�ϴ� �޼���....
	{
		nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);	// ��Ŷ �ޱ�(�����)
		
		if(nRead2 < (int)sizeof(cmnpck)) // �������� �̻��� ��...
		{
			if(nRead2 < 0)	// ���� �߻��� ���....
			{	
				nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);

				if(nRead1 > 0)
					nRead2 += nRead1;
			}
			else	// �ޱ� �޾Ҵµ� ������ ���....
			{
				// ������ �κ��� �޾� �����ش�...
				nRead1 = recv(socket, (char *)&cmnpck + nRead2, sizeof(cmnpck) - nRead2, 0);

				nRead2 += nRead1;
			}
			
			if (nRead1 < 0) return 0L;	// ���� ���....
		}

		readbuff = new BYTE[cmnpck.length];
		memset(readbuff, 0, sizeof(readbuff));
		memcpy(readbuff, &cmnpck, sizeof(cmnpck));	// ����κ��� ��ü ��Ŷ �κп� ����...

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
		ClientDisconnected(socket);	// Ŭ���̾�Ʈ�κ��� FD_CLOSE �̺�Ʈ �߻��� �ش� Ŭ���̾�Ʈ�� ��Ŀ��Ʈ �۾�...
	}
	
	return 0L;
}

///////////////////////////////////////////////////////////////
// Channel server���� ���� �޼��� ó��....
void CServerWork::ProcessMessageServer(CMessage *pMsg)
{
	CMessage Send;
	COMMON_PACKET cmnpck;
	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	/*
	#define CSM_CONNECTGS	0x0100	// ���� Ȯ��
	#define CSM_SERVERINFO	0x0101	// ä�� ���� ���� ���� (���� ������, IP, Port no...) 
	#define CSM_REQUESTINFO	0x0102	// ���� ������ ������û(�ִ� ���, ������ ��)
	#define CSM_REQUESTMAN	0x0103	// ���� ������ ������ ��� �� ��û(���� �ð����� �ֱ��� �߻�)
	*/
	switch(cmnpck.header)
	{
	case CSM_CONNECTGS :
//		m_pLogDialog->AddLogString("ä�� ���� Ȯ�� �޼��� ����");
	case CSM_REQUESTINFO :
	case CSM_SERVERINFO :
		SendServerInfo();
//		m_pLogDialog->AddLogString("ä�� ���� Ȯ�� �޼��� ����");
		break;
		
	}
}

///////////////////////////////////////////////////////////////
// Chnnel server���� ���� ������ ���� ����....
void CServerWork::SendServerInfo()
{
	SERVERINFO si;	// ���� ���� ����.
	CHANNELINFO ci;	// (���Ӽ����� ������)ä�� ����.
	CMessage Send;	// ���� ��Ŷ....

	// ��Ŷ ������ ���� ����.
	si.cmnpacket.header = GSM_SERVERINFO;

	// ���� ���� ���� ����.....���̾�α� �ڽ����� ���� ������ ����.
	si.id = m_nServerID;	// ���� ���̵�� 0
	memset(si.ip, 0, sizeof(si.ip));
	memcpy(si.ip, m_strGameServerIP.data(), m_strGameServerIP.size());
	si.port = m_nGameServerPort;
	si.channels = m_nMaxChannelCount;
	si.rooms = m_nMaxRoomCount;

	si.cmnpacket.length = sizeof(si) + sizeof(ci) * m_nMaxChannelCount;
	// ��ü ��Ŷ�� ���̴� ���Ӽ��� ���� + (ä�� �ϳ��� ���� * ä�� ��)

	int nLen = Send.SetData(&si, 0, sizeof(si));	// ServerInfo ��Ŷ�� ����.

	UINT maximum = m_nMaxRoomCount * m_nMaxUser;

	
	
	// �� ä�ε��� ���� �Է�.
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
// Lobby Client���� ���� �ż��� ó��....
void CServerWork::ProcessMessageClient(CMessage *pMsg)
{
	CMessage Send;
	COMMON_PACKET cmnpck;
	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	/*
	#define LM_CONNECTGS	0x0600	// Game Server�� ����
	#define LM_REQROOMINFO	0x0601	// Game Server ���� ��û
	#define LM_REQMANINFO	0x0602	// ��ü ����� ��û

	#define LM_REQGMN		0x0610	// ���� ��û(�� �����.)
	#define LM_REQJOIN		0x0611	// �� ���� ��û
	#define LM_REQOUT		0x0612	// �� ������ ��û
	#define LM_INVITACK		0x0613	// �ʴ� ���
	#define LM_INVITNOT		0x0614	// �ʴ� �ź�

	#define LM_USERUPDATE	0x0620	// ������ ���� ����....

	#define LM_CHATMSG		0x06ff	// ä�� �޼���
	#define LM_TGTMSG		0x060f	// Ÿ�� ���� ä�� �޼���
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
// DB Connection ��ü ��Ʈ��.....
LPDBCONNPOOL CServerWork::GetAvailableDbconn()
{
	g_Critical.Lock();
	m_pLogDialog->AddLogString("GetAvailableDbConn() : Lock()");

	list<DBCONNPOOL>::iterator iter = dbconnlist.begin();

	int nCount = 0;

	///////////////////////////////////////////////////////////
	//	�̹� ������� Connection ��ü �� ������� �ƴ� ���� ã��

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
	// ���� ������� ���� ������ ���� ����

	for(; iter != dbconnlist.end() ; iter++)
	{
		LPDBCONNPOOL lpdbconn = &(*iter);

		if(lpdbconn->pDb == NULL)
		{
			CDataBase *pDbConn = new CDataBase;

			if(!pDbConn->Connect(3, (LPSTR)m_strDBName.data(), (LPSTR)m_strDBID.data(), (LPSTR)m_strDBPassword.data())) 
			{
				// ���� �������� ��� NULL ����
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
	// ��� ������� ��� ���� Connection ��ü ���� �� POOL�� �߰�

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
// �����ڰ� �䱸�� ������ ���� ������ ����ü......
void CServerWork::SendUserInfo(CMessage *pMsg)
{
	CMessage Send;

	g_Critical.Lock();

	//LPUSERINFO lpuserinfo;
	m_pLogDialog->AddLogString("Send all User info() : Lock()");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();
	// ������ �������� ������ �б� ���� STL ������ ����
	// ClientManager ������ clientlist(������ Ŭ���̾�Ʈ ���� ����)�� ù �Ӹ��� ����.....

	LPUSER lpmyinfo = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	// ���� �䱸�� ����� ���� ���� ����(�������� ã�� Ȯ��)

	int nBuffSize;

	nBuffSize = sizeof(COMMON_PACKET) + sizeof(USERINFO) * m_ClientManager.clientlist.size();
	// ���� ��Ŷ�� ������  = COMMON_PACKET + ���� �ѻ���� ���� * ��ü �������� ��......

	LPBYTE buff = new BYTE[nBuffSize];
	// ��Ŷ�� ������ �޸� ����.

	memset(buff, 0, nBuffSize);

	COMMON_PACKET cmnpck;
	cmnpck.header = GSM_ALLMANDATA; // ��ü ���� ���� ���������� �˸�.....

	int cnt = sizeof(cmnpck);

	for(; i != m_ClientManager.clientlist.end() ; i++)
	{
		LPUSER lpuser = &(*i);
		
		// �ڽŰ� ���� ä�ο� �����ϴ� ����鸸�� ������ Ȯ��....
		if(lpmyinfo->channelno == lpuser->channelno)
		{
			memcpy(&buff[cnt], lpuser, sizeof(USERINFO));
			cnt += sizeof(USERINFO);
		}
	}

	cmnpck.length = cnt; // ��ü ���� ����.....
	memcpy(buff, &cmnpck, sizeof(cmnpck));

	m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)buff, cnt);

	g_Critical.Unlock();

	m_pLogDialog->AddLogString("Send all User Info() : UnLock() *");

	delete [] buff;
}


////////////////////////////////////////////////////////////////
// �����ڰ� �䱸�� �� ���� ������ ����ü......
void CServerWork::SendRoomInfo(CMessage *pMsg)
{
	CMessage Send;
	g_Critical.Lock();

	m_pLogDialog->AddLogString("Send All Room Info() : LOCK()");

	//LPROOMINFO roominfo;
	int nBuffSize;

	unsigned short nChannelno;	// ����ڰ� ������ ä���� �����ϱ� ���� ����....
	pMsg->GetData(&nChannelno, sizeof(COMMON_PACKET), sizeof(nChannelno));

	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(nChannelno);
	// ����ڰ� ������ ä���� ��ȣ�� ���� ä�� ������ ȹ��,....

	list<ROOM>::iterator i = lpchannel->roomlist.begin();
	// ä�� �����Ϳ��� ä�� ������ RoomList�� ���� �� �� �ִ�......

	nBuffSize = sizeof(COMMON_PACKET) + sizeof(ROOMINFO) * lpchannel->roomlist.size();
	// ��ü ��Ŷ ���� = COMMON_PACKET���� + ���ϳ��� ���� ũ�� *  ��ü �� ����...

	LPBYTE buff = new BYTE[nBuffSize];
	// ��Ŷ �����͸� ������ ����.....
	memset(buff, 0, nBuffSize);

	COMMON_PACKET cmnpck;
	cmnpck.header = GSM_ALLROOMDATA;	// ���Ӽ����� �� �������� �˸�.

	int cnt = sizeof(cmnpck);	// ����� length�� �ֱ����� ����......

	// ����� ���� ����......
	for(; i != lpchannel->roomlist.end() ; i++)
	{
		LPROOM lproom = &(*i);

		memcpy(&buff[cnt], lproom, sizeof(ROOMINFO));
		cnt += sizeof(ROOMINFO);
	}

	cmnpck.length = cnt;	// ��Ŷ�� ���� ����....

	memcpy(buff, &cmnpck, sizeof(cmnpck));
	
	m_ClientSocket.SendMsg((SOCKET)pMsg->GetSocket(), (char *)buff, nBuffSize);

	g_Critical.Unlock();

	m_pLogDialog->AddLogString("Send all Room info() : Unlock() *");

	delete [] buff;
}

////////////////////////////////////////////////////////////////
// �� �����
// �������� : �� ���� �Ѱ迡 �ε����� ��.
void CServerWork::MakeRoom(CMessage *pMsg)
{
	m_pLogDialog->AddLogString("*** Make Room ***");

	int nLen = sizeof(COMMON_PACKET);

	SOCKET socket = pMsg->GetSocket();
	// ���ƿ� ��Ŷ�� ������ ����....

	ROOMINFO roominfo;	// ���ο� ���� �����ϱ� ���� �� ����....

	pMsg->GetData(&roominfo, nLen, sizeof(roominfo));

	g_Critical.Lock();
	m_pLogDialog->AddLogString("Make Room() : Lock()");

	LPUSER lpuser = m_ClientManager.GetClient(socket);	// ������ �̿��� ��Ŷ���� Ŭ���̾�Ʈ�� ���� ����...
	LPROOM lproom = m_ChannelManager.GetRoomInfo(lpuser->channelno, roominfo.id);

	if(lproom != NULL)
	{
		if(lproom->present == 0)	// �� ���� �ο��� 0�϶�...
		{
			COMMON_PACKET cmnpck;

			cmnpck.header = GSM_GMNGACK;	// ���� ����....
			cmnpck.length = sizeof(cmnpck);

			// ������ ���� ������ �˸�.
			m_ClientSocket.SendMsg(socket, (char *)&cmnpck, sizeof(cmnpck));
			m_pLogDialog->AddLogString("::: GSM_GMNGACK (ID : %s)", (LPCTSTR)lpuser->id);

			// ������ ���� ������ �� ���� ���� ��ü�� ����.....
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
// ���� ����...
// �������� : �� ���� �ο����� �ε����� ��....
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
		if(lproom->present == 0)	// �����ϰ��� �ϴ� �濡 �ο��� ���� ���...
		{
			SendErrorMessage((SOCKET)pMsg->GetSocket(), F_JOINROOM);
		}
		else if(lproom->present < lproom->maximum)	// �����ϰ��� �ϴ� �濡 ����� ���� ���϶�...
		{
			int cnt;
			unsigned char lpRoomManager_ip[32];	// ���� ip������ ������.
			//lpRoomManager_ip = new BYTE[32];

			lproom->present++;
			lproom->users[1] = lpuser;
			lproom->status = 1;

			//memcpy(&lpRoomManager_ip, &lproom->users[0]->ip, sizeof(lpRoomManager_ip));	// ���� IP�� ����Ȯ����Ŷ�� �Ǿ� �����ش�.
			
			// �����ڰ� ������ �� ��ȣ�� ������ ����.....
			lpuser->roomno = roomno;
			lpuser->lproom = lproom;	

			COMMON_PACKET cmnpck;
			
		//	SendCompetitorInfo(lproom);
		// 	m_pLogDialog->AddLogString("SendCompetitorInfo()");

			cmnpck.header = GSM_JOINACK;
			cnt = sizeof(COMMON_PACKET) + sizeof(lpRoomManager_ip);
			cmnpck.length = cnt;
			memcpy(&lpRoomManager_ip, &lproom->users[0]->ip, sizeof(lpRoomManager_ip));	// ���� IP�� ����Ȯ����Ŷ�� �Ǿ� �����ش�.

			LPBYTE temppacket = new BYTE[cnt];	// ������ ������ ��Ŷ.

			memset(temppacket, 0, sizeof(temppacket));

			memcpy(temppacket, &cmnpck, sizeof(COMMON_PACKET));
			memcpy(&temppacket[sizeof(COMMON_PACKET)], (char *)lpRoomManager_ip, sizeof(lpRoomManager_ip));


			// �޼��� �޺κп� ip ����.

			m_ClientSocket.SendMsg(lpuser->socket, (char *)temppacket, cmnpck.length);
			m_pLogDialog->AddLogString("::: GSM_JOINACK (id : %s, compet id : %s)",(LPCTSTR)lpuser->id, (LPCTSTR)lproom->users[0]->id);

			NotifyAllUpdate((LPUSERINFO)lpuser, lproom);

		}
	}

	g_Critical.Unlock();
	m_pLogDialog->AddLogString("JoinRoom() : Unlock() *");

}

////////////////////////////////////////////////////////////////
// �� ������......
// ���� : 1. �����ڰ� ���� ��, 2. ������ ������ ��....
void CServerWork::OutRoom(CMessage *pMsg)
{
	m_pLogDialog->AddLogString("*** OutRoom ***");

	g_Critical.Lock();
	m_pLogDialog->AddLogString("OutRoom() : Lock()");

	// ���� ��Ŷ�� ���� ������ ã�� (SOCKET �̿�)
	LPUSER lpuser = m_ClientManager.GetClient((SOCKET)pMsg->GetSocket());
	// ���� ��Ŷ�� ���� ������ ����ִ� �� ���� �������� ����.....
	LPROOM lproom = lpuser->lproom;

	if(lproom != NULL)
	{
		lproom->present--;	// ������ �Ҽӵ� ���� �ο��� ����....
		
		lpuser->lproom = NULL;	// ������ �� �����͸� NULL�� 
		lpuser->roomno = -1;	// ������ ��ġ : -1 (����)
		
		if(lproom->present == 0)	// �濡 �ƹ��� �ȳ��� ������ (���� �������� ����� ������ ����϶�)
		{
			lproom->users[0] = NULL;
			lproom->users[1] = NULL;

			memset(lproom->title, 0, sizeof(lproom->title));
			memset(lproom->passwd, 0, sizeof(lproom->passwd));

			strcpy((char *)lproom->title, "���");
		}
		else	
		{
			if(lproom->users[0] == lpuser)	// ������ ���....
				lproom->users[0] = lproom->users[1];	// �ڽ� ������ ����� �������� �Ӹ�.....
			else if(lproom->users[1] == lpuser)
				lproom->users[1] = NULL;

			memset(lproom->title, 0, sizeof(lproom->title));
			memset(lproom->passwd, 0, sizeof(lproom->passwd));

			strcpy((char *)lproom->title, "��������.");

			COMMON_PACKET cmnpck;

			// ������ �Ǿ����� �뺸......
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
	CMessage Send;	// ������ ���� �޼���...

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

	// �̺κ� ���� �ʿ�.....
	// 4���� ��� �� ���� �����ڸ� �� �� �ְ�....

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

	// Disconnected�� ������ ������ ä�� ��ġ ���� ���.
	LPUSER lpuser = m_ClientManager.GetClient(socket);
	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);
	LPROOM lproom = lpuser->lproom;

	if(strlen((char *)lpuser->id) > 0)
	{
		lpchannel->present--;

		lpuser->roomno = -2;	// ���� ������ Ŭ���̾�Ʈ...
		NotifyClientUpdate((LPUSERINFO)lpuser);

	}

	if(lpuser->lproom != NULL)	// �濡 ��� �־��� ���...
	{
		if(lproom->status == 2)	// �����߿� Ƣ�� ���� ���.
		{
			if(lproom->users[0] == lpuser)	// �����̸� ���� ������ ���
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

			strcpy((char *)lproom->title, "���");

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
//	������ ������Ʈ......���� ä�ο� �ִ� ����鿡�� �ٲ� �� ���� �˸�.
void CServerWork::NotifyRoomUpdate(LPROOM lproom)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	ROOMINFO roominfo;

	g_Critical.Lock();
	m_pLogDialog->AddLogString("NotifyRoomUpdate() : Lock() *");

	list<USER>::iterator i = m_ClientManager.clientlist.begin();	// ���� ä�ο� �����ϴ� �����ڵ�.
	
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

					strcpy((char *)lproom->title, "***�� ��***");
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
	
	LPUSER lpuser = m_ClientManager.GetClient(socket); // ���� ������ Ŭ���̾�Ʈ �˻�
	LPCHANNEL lpchannel = m_ChannelManager.GetChannelInfo(lpuser->channelno);
	LPROOM lproom = lpuser->lproom;
	
	if(lproom != NULL) lproom->status = gamestatus;

	
}
