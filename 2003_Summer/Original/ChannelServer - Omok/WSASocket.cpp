// WSASocket.cpp: implementation of the CWSASocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSASocket.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWSASocket::CWSASocket()
{
	m_Socket = NULL;
}

CWSASocket::~CWSASocket()
{
}

SOCKET CWSASocket::Accept()
{
	SOCKET ListenSocket;

	struct sockaddr_in sa;
	int Length;
	
	ZeroMemory(&sa, sizeof(sa));
	Length = sizeof(sa);				
	
	ListenSocket = accept(m_Socket, (struct sockaddr *)&sa, &Length);				
						
	if(ListenSocket == INVALID_SOCKET)
	{
		::MessageBox(NULL, "���ο� ���� �Ҵ翡 �����߽��ϴ�.", "", MB_OK);
		return 0;
	}

	return ListenSocket;
}

BOOL CWSASocket::Initialize(int portno)
{
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if(m_Socket == INVALID_SOCKET)
	{
		::MessageBox(NULL, "Listen Socket ���� ����!!", NULL, MB_OK);
		return FALSE;
	}

	SOCKADDR_IN * pSock_sin = new SOCKADDR_IN;
	
	pSock_sin->sin_family = AF_INET;		// ���ε� ���� ����, ����
	pSock_sin->sin_addr.S_un.S_addr = INADDR_ANY;
	pSock_sin->sin_port = htons(portno);

	// ListenSocket�� ���� �ӽ��� IP, ���� PORT��� ���ε�.	
	if(bind(m_Socket,(struct sockaddr FAR *) pSock_sin, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		::MessageBox(NULL, "���� ���ε� ����", NULL, MB_OK);
		return FALSE;
	}

	delete pSock_sin;  // �޸� ����

	return TRUE;
}



BOOL CWSASocket::Listen()
{
	if(listen(m_Socket, 5) != 0)
	{
		::MessageBox(NULL, "���� ���� ����", "", MB_OK);
		return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////
//
// �б� ���۷κ��� �����͸� �о���δ�.

int CWSASocket::RecvMsg(SOCKET sock, char *buf, int nLen)
{
	int i = 0;
	int nRecvByte;

	while(!(i == nLen))
	{
		nRecvByte = recv(sock, &buf[i], nLen-i, 0);
		i += nRecvByte;
	}

	return nLen;
}



//////////////////////////////////////////////////////////////////
//
// ���� ���۷� �����͸� ����.

int CWSASocket::SendMsg(SOCKET sock, char *buf, int size)
{
	int nSent = 0;
	int result = send(sock, buf, size, 0);

	int dwError = 0;

	nSent = result;

	while(nSent < size)
	{
		if(result < 0)
		{
			dwError = WSAGetLastError();

			if(dwError != 0 && dwError != WSAEWOULDBLOCK) return -1;

			else if(dwError == WSAEWOULDBLOCK)
			{
				while(dwError == WSAEWOULDBLOCK)
				{
					SetLastError(0);

					TRACE("WSAEWOULDBLOCK\n");
					
					Sleep(10);
					result = send(sock, buf+nSent, size-nSent, 0);
					dwError = WSAGetLastError();
					
					if(dwError != 0 && dwError != WSAEWOULDBLOCK) return -1;
				}

				if(result > 0) nSent += result;
			}
		}

		else nSent += result;

		dwError = WSAGetLastError();			
		if(dwError != 0 && dwError != WSAEWOULDBLOCK) return -1;
	}

	return nSent;
}


int CWSASocket::SendMsg(char *buf, int size)
{
	if(m_Socket != NULL)
	{
		int nSent = 0;
		int result = send(m_Socket, buf, size, 0);
		
		int dwError = 0;
		nSent = result;
		
		while(nSent < size)
		{
			if(result < 0)
			{
				dwError = WSAGetLastError();
				
				if(dwError != 0 && dwError != WSAEWOULDBLOCK) return -1;
				
				else if(dwError == WSAEWOULDBLOCK)
				{
					while(dwError == WSAEWOULDBLOCK)
					{
						SetLastError(0);
						
						TRACE("WSAEWOULDBLOCK\n");
						
						Sleep(10);
						result = send(m_Socket, buf+nSent, size-nSent, 0);
						dwError = WSAGetLastError();
						
						if(dwError != 0 && dwError != WSAEWOULDBLOCK) return -1;
					}		
					if(result > 0) nSent += result;
				}
			}	
			else nSent += result;

			dwError = WSAGetLastError();			
			if(dwError != 0 && dwError != WSAEWOULDBLOCK) return -1;
		}
		return nSent;
	}

	else return -1;
}


BOOL CWSASocket::Connect(char *szHost, int Portno)
{
	hostent* host;
	sockaddr_in sa;

	if((host = gethostbyname(szHost)) == NULL)
	{
		::MessageBox(NULL, "gethostbyname() �Լ� ����", "", MB_OK);
		return FALSE;
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons(Portno);
	sa.sin_addr =  *((struct in_addr *)host->h_addr_list[0]);

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);

	int result = connect(m_Socket, (struct sockaddr *)&sa, 16);
	
	if(result == SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAEWOULDBLOCK){}
		else return FALSE;
	}

	return TRUE;
}

BOOL CWSASocket::InitSock()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	int error = WSAStartup(wVersionRequested, &wsaData);
	if(error != 0)
	{
		::MessageBox(NULL, "DLL �ʱ�ȭ ����", "DLL ����", MB_OK);
		return FALSE;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		::MessageBox(NULL, "Winsock DLL 2.2 ���� ����", "���� ����", MB_OK);
		WSACleanup();

		return FALSE;
	}

	return TRUE;
}

void CWSASocket::Close()
{
	closesocket(m_Socket);
	WSACleanup();
}

void CWSASocket::Disconnect()
{
	closesocket(m_Socket);
}
