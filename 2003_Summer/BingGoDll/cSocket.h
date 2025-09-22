
#pragma once
#include <winsock2.h>
#include "BGCore.h"

#pragma comment (lib,"ws2_32")

class __declspec(dllexport) cSocket 
{
	DWORD remaind_size;														// 남는 데이타 크기를 기록
	
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
	
	void readySocket();														// socket descripter를 얻는다.
	void endSocket(SOCKET socket = DEFAULTSOCKET);							// 소캣을 닫는다.

	void serverAsyncSelect(HWND hWnd = NULL);								// 비동기 모드 Select를 한다. 
	void clientAsyncSelect(HWND hWnd = NULL);								// 비동기 모드로 select를 한다. 옵션이 다르다.

	void disconnected();

	BOOL sendData(SOCKET sendsocket,char* msg,int len);						
	BOOL receiveData(SOCKET client,char* recvbuff,int& len);				

	BOOL connectSocket(const char* serverAddress = SERVERADDRESS);			// 서버에 연결 한다.

	void SetHandle(HWND hWnd);
	
};

