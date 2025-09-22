// WSASocket.h: interface for the CWSASocket class.
//
//////////////////////////////////////////////////////////////////////
// 소켓 설계 클래스
// 일단은 1:1 설계.

#ifndef _WSASOCK_H_
#define _WSASOCK_H_

#include <winsock2.h>
//#include "structs.h"
//#include "header.h"
#include "cConsol.h"

class CWSASocket  
{
public:
	CWSASocket();
	virtual ~CWSASocket();
	BOOL Initialize(int portno);
	SOCKET Accept();

	SOCKET Get_Socket() {return m_Socket;}
	
public:
	void Close();
	BOOL InitSock();
	BOOL Connect(char *szHost, int Portno);
	int SendMsg(SOCKET sock, char *buf, int size);   // 보내는 메세지 제어 함수
	int SendMsg(char *buf, int size);

	int RecvMsg(SOCKET sock, char *buf, int size);             // 받는 메세지 제어 함수
	int RecvMsg(char *buf, int size);
	BOOL Listen();                                    // 소켓 대기 함수 
	SOCKET m_Socket;   //  소켓형 구조체(윈도우 핸들로 취급됨)
	                   //  서버용 Listen 소켓에서 연결요청 처리.


};

#endif