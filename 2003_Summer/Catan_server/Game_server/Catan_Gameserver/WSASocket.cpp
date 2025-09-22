// WSASocket.cpp: implementation of the CWSASocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSASocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWSASocket::CWSASocket()
{

}

CWSASocket::~CWSASocket()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
SOCKET CWSASocket::Accept()
{
	SOCKET ListenSocket;          // ListenSocket 생성

	struct sockaddr_in sa;        // 소켓 정보 저장
	int Length;

	ZeroMemory(&sa, sizeof(sa));
	Length = sizeof(sa);

	ListenSocket = accept(m_Socket, (struct sockaddr *)&sa, &Length);
	// ListenSocket에 클라이언트의 연결요청을 수행한다.
	// m_Socket은 대기 상태의 서버소켓.
	// 대기상태의 소켓이 접속처리를 한 후, 그 소켓은 다시 대기 상태로 돌아가며,
	// 함수 내부에서 생성한 소켓은 클라이언트와 접속된 상태로 반환된다.....

	if(ListenSocket == INVALID_SOCKET)
	{
		::MessageBox(NULL, "새 소켓 할당에 실패했습니다.", "", MB_OK);
		return 0;
	} // accept에서 소켓이 생성 안되면.....

	return ListenSocket;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CWSASocket::Initialize(int portno)
{
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	// 소켓을 만들어 m_Socket에 저장...
	// AF_INET : 인터넷 주소 기반, SOCK_STREAM : 소켓 특성. 
    // IPPROTO_TCP : 소켓과 그 네트웍이 사용하는 프로토콜
	// WSA_FLAG_OVERLAPPED : 소켓 기능.

	if(m_Socket == INVALID_SOCKET)
	{
		::MessageBox(NULL, "Listen Socket 생성 실패!", NULL, MB_OK);
		return FALSE;
	}

	// 서버 소켓에 필요한 Listen Socket -> 접속 요구를 기다림

	SOCKADDR_IN * pSock_sin = new SOCKADDR_IN;  // (시스템 주소와 서비스 주소 저장)

	pSock_sin->sin_family = AF_INET;
	pSock_sin->sin_addr.S_un.S_addr = INADDR_ANY; // 서버가 여러군데 연결돼 있을 경우 다중 IP를 위한 옵션
	pSock_sin->sin_port = htons(portno);           // 어플리케이션이 쓸 포트와 소켓을 연결

	// 바인딩 작업
	// 소켓을 시스템 주소와 서비스 지점에 연결
	if(bind (m_Socket, (struct sockaddr FAR *) pSock_sin, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		::MessageBox(NULL, "소켓 바인딩 에러.", NULL, MB_OK);
		return FALSE;
	}

	delete pSock_sin;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Listen Socket 처리
BOOL CWSASocket::Listen()
{
	if(listen(m_Socket, 512) != 0)
	{
		::MessageBox(NULL, "소켓 리슨 에러", "", MB_OK);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 읽기 버퍼로부터 데이터 읽기
int CWSASocket::RecvMsg(SOCKET sock, char *buf, int size)
{
	UINT i=0;
	DWORD dwTotalByte;
	int nRecvByte;
	ioctlsocket(sock, FIONREAD, &dwTotalByte);

	while(!(i == dwTotalByte))
	{
		nRecvByte = recv(sock, &buf[i], dwTotalByte-1, 0);
		i += nRecvByte;
	}

	return dwTotalByte;
}



int CWSASocket::RecvMsg(char *buf, int nLen)
{
	int i = 0;
	int nRecvByte;
	
	if(m_Socket != NULL)
	{
		while(!(i == nLen))
		{
			nRecvByte = recv(m_Socket, &buf[i], nLen-i, 0);
			i += nRecvByte;
		}
	}

	else return -1;

	return nLen;
}

//////////////////////////////////////////////////////////////////
//
// 쓰기 버퍼로 데이터를 쓴다.


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


//////////////////////////////////////////////////////////////////////////
// 연결되었을때 동작지정 (클라이언트)
BOOL CWSASocket::Connect(char *szHost, int Portno)
{
	SOCKET sock;          
	hostent* host;
	sockaddr_in sa;

	if((host = gethostbyname(szHost)) == NULL )
	{
		::MessageBox(NULL, "Get Host By Name ERROR!!", "", MB_OK);
		return FALSE;
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons(Portno);
	sa.sin_addr = *((struct in_addr *)host->h_addr_list[0]);

	sock = socket(AF_INET, SOCK_STREAM, 0); // 소켓 설정.

	int result = connect(sock, (struct sockaddr *)&sa, 16);

	if (result == SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAEWOULDBLOCK){}
		else return FALSE;
	}

	m_Socket = sock;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// 소켓 스타트 업 (소켓 DLL 로드)
BOOL CWSASocket::InitSock()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	int error = WSAStartup(wVersionRequested, &wsaData);

	if (error != 0)
	{
		::MessageBox(NULL, "DLL 초기화 실패...", "", MB_OK);
		return FALSE;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) !=2 )
	{
		::MessageBox(NULL, "Winsock DLL 2.2 설정 실패", "설정 실패", MB_OK);
		WSACleanup();

		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//
void CWSASocket::Close()
{
	closesocket(m_Socket);
	WSACleanup();
}
