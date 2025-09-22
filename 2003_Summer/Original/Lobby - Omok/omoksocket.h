
#pragma once
#pragma comment (lib,"ws2_32")

#include "omoksdef.h"
#include "originwindow.h"

class __declspec(dllexport) omoksock : public ORIGINWINDOW
{
	DWORD remaind_size;														// ���� ����Ÿ ũ�⸦ ���
	
public:
	omoksock();
	~omoksock();

// variable
	SOCKET	m_socket;
	SOCKET	Clientsocket;
	BOOL	m_bconnected;

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Destroy();		
	void subclass(){};														// empty function 
	HWND Create(HWND parent,HINSTANCE hInst);								// m_hWnd = parent
};

