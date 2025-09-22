// WSASocket.h: interface for the CWSASocket class.
//
//////////////////////////////////////////////////////////////////////
// ���� ���� Ŭ����
// �ϴ��� 1:1 ����.

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
	int SendMsg(SOCKET sock, char *buf, int size);   // ������ �޼��� ���� �Լ�
	int SendMsg(char *buf, int size);

	int RecvMsg(SOCKET sock, char *buf, int size);             // �޴� �޼��� ���� �Լ�
	int RecvMsg(char *buf, int size);
	BOOL Listen();                                    // ���� ��� �Լ� 
	SOCKET m_Socket;   //  ������ ����ü(������ �ڵ�� ��޵�)
	                   //  ������ Listen ���Ͽ��� �����û ó��.


};

#endif