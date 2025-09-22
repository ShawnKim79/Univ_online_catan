#include "cSocket.h"


cSocket::cSocket()
{
	// DLL 을 초기화 한다.
	WSADATA sa;
	WSAStartup(MAKEWORD(2,2),&sa);

	m_socket = INVALID_SOCKET;	

	m_bconnected = FALSE;
}

cSocket::~cSocket()
{
	WSACleanup();
}

void cSocket::readySocket()
{
	m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
}

void cSocket::endSocket(SOCKET socket)
{
	closesocket(socket == DEFAULTSOCKET ? m_socket:socket);
	m_bconnected = FALSE;
}

void cSocket::serverAsyncSelect(HWND hWnd)
{
	sockaddr_in sa;
	sa.sin_port = htons(OMOKPORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(m_socket,(sockaddr *)&sa,sizeof(sockaddr));
	
	WSAAsyncSelect(m_socket,hWnd == NULL ? m_hParent:hWnd,WM_SOCKETMSG,FD_SERVERTYPE);		
}

void cSocket::clientAsyncSelect(HWND hWnd)
{
	WSAAsyncSelect(m_socket,hWnd == NULL ? m_hParent:hWnd,WM_SOCKETMSG,FD_CLIENTTYPE);
}

BOOL cSocket::listenSocket()
{
	if(!listen(m_socket,WAITQUEUESIZE))
	{
		return TRUE;
	}

	
	return FALSE;
}

BOOL cSocket::acceptSocket()
{
	sockaddr sa = {0,};
	int addrsize = sizeof(sockaddr);

	int tryacceptcount = 0;

	SOCKET sock = accept(m_socket,&sa,&addrsize);
	
	if(sock == INVALID_SOCKET)
	{
		MessageBox(NULL,"accept","error",MB_OK);
		m_bconnected = FALSE;
		return FALSE;
	}
	
	Clientsocket = sock;

	m_bconnected = TRUE;
	return TRUE;
}

BOOL cSocket::connectSocket(const char* serverAddress)
{
	hostent* host;
	host = gethostbyname(serverAddress);

	int tryconnect = 0;

	sockaddr_in sa = {0,};
	sa.sin_family = AF_INET;
	sa.sin_port = htons(OMOKPORT);
	sa.sin_addr = *((struct in_addr *)host->h_addr_list[0]);

TYRCONNECT:
	
	if(connect(m_socket,(sockaddr *)&sa,sizeof(sockaddr)) == 0)
	{	
		m_bconnected = TRUE;
		return TRUE;
	}

	if(WSAGetLastError() == WSAEWOULDBLOCK)
	{
		m_bconnected = TRUE;
		return TRUE;
	}

	tryconnect++;

	if(tryconnect < 10)
	{
		Sleep(50);
		goto TYRCONNECT;
	}

	MessageBox(NULL,"연결이 이루어 지지 않았습니다. 다시 시도해 주세요","에러",MB_OK);
	m_bconnected = FALSE;
	return FALSE;
}

BOOL cSocket::receiveData(SOCKET client,char* recvbuff,int& len)
{
	if(!m_bconnected)
		return FALSE;

	ULONG size;
	
	if((ioctlsocket(client,FIONREAD,&size) == 0) && size > 0)
	{
		int recvbyte = 0;
		for(UINT i = 0 ; i < size;)
		{
			recvbyte = recv(client,&recvbuff[i],size-i,0);
			
			if(recvbyte > 0)
				i += recvbyte;
			
		}
	}
	len = size;
	return TRUE;
}

BOOL cSocket::sendData(SOCKET sendsocket,char* msg,int len)
{
	if(!m_bconnected)
		return FALSE;

	int sendsize = 0;

	for(int i =0; i < len;)
	{
		sendsize = send(sendsocket,&msg[i],len-i,0);
		if(sendsize == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			
			if( error == WSAEWOULDBLOCK)
			{
				Sleep(10);
			}

			if(error == WSAESHUTDOWN || error == WSAENETRESET)
				return FALSE;

		}

		if(sendsize > 0)
			i += sendsize;
	}

	return TRUE;
}

void cSocket::disconnected()
{
	m_bconnected = FALSE;
}

void cSocket::SetHandle(HWND hWnd)
{
	m_hParent = hWnd;
}