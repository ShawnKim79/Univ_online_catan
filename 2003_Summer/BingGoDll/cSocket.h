
#pragma once
#include <winsock2.h>
#include "BGCore.h"

#pragma comment (lib,"ws2_32")

class __declspec(dllexport) cSocket 
{
	DWORD remaind_size;														// ���� ����Ÿ ũ�⸦ ���
	
public:
	cSocket();
	~cSocket();

// variable
	SOCKET	m_socket;
	SOCKET	Clientsocket;
	BOOL	m_bconnected;
	HWND	m_hParent;

// function

	BOOL listenSocket();						
	BOOL acceptSocket();		
	
	void readySocket();														// socket descripter�� ��´�.
	void endSocket(SOCKET socket = DEFAULTSOCKET);							// ��Ĺ�� �ݴ´�.

	void serverAsyncSelect(HWND hWnd = NULL);								// �񵿱� ��� Select�� �Ѵ�. 
	void clientAsyncSelect(HWND hWnd = NULL);								// �񵿱� ���� select�� �Ѵ�. �ɼ��� �ٸ���.

	void disconnected();

	BOOL sendData(SOCKET sendsocket,char* msg,int len);						
	BOOL receiveData(SOCKET client,char* recvbuff,int& len);				

	BOOL connectSocket(const char* serverAddress = SERVERADDRESS);			// ������ ���� �Ѵ�.

	void SetHandle(HWND hWnd);
	
};

