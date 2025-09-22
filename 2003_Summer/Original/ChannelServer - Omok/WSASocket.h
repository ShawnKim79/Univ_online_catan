// WSASocket.h: interface for the CWSASocket class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WSASOCK_H_
#define _WSASOCK_H_

#include "winsock2.h"

class CWSASocket
{
public:
	BOOL Initialize(int portno);
	SOCKET Accept();
	CWSASocket();
	virtual ~CWSASocket();

public:
	void Disconnect();
	void Close();
	BOOL InitSock();
	BOOL Connect(char *szHost, int Portno);
	int SendMsg(SOCKET sock, char *buf, int size);
	int SendMsg(char *buf, int size);
	int RecvMsg(SOCKET sock, char *buf, int nLen);
	BOOL Listen();
	SOCKET m_Socket;
};

#endif
