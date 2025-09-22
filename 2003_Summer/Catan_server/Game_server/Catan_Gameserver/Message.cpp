// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Catan_Gameserver.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage()
{
	// member 변수 초기화..
	m_Socket = 0;
	m_nMsgLength = 0;   // 패킷 초기 길이.
	m_nDataLength = 0;
	m_pBuffer = NULL;

	m_pBuffer = new BYTE[0];
}

CMessage::~CMessage()
{
	if(m_pBuffer) delete [] m_pBuffer;
}

////////////////////////////////////////////////////////////////////
// Socket 처리
void CMessage::SetSocket(SOCKET s)
{
	m_Socket = s;
}

SOCKET CMessage::GetSocket()
{
	return m_Socket;
}

///////////////////////////////////////////////////////////////////////
// 패킷 내부 청소.
void CMessage::Clear_Data()
{
	memset(m_pBuffer, 0, m_nDataLength);
	m_nDataLength = 0;
}
//////////////////////////////////////////////////////////////////////
//	Sender(보내는 쪽 처리) ->1. 패킷 생성 2. 패킷에 Data 저장

// 패킷 길이 설정 & m_pBuffer에 메모리도 할당.
// Packet 크기 부족시....인자 length는 생성할 메모리 크기.
int CMessage::SetBuffLength(int length)
{
	LPBYTE buf = new BYTE[length];			// Buffer 생성(지역)
//	m_pBuffer = new BYTE[length];

	memset(buf, 0, length);						// 생성된 Buffer 0 으로 Set
	memcpy(buf, m_pBuffer, m_nDataLength);		// Buffer 내용 복사.(Member -> 지역)
												// m_pBuffer에 값이 있을 경우 보호된다.

	if(m_pBuffer) delete [] m_pBuffer;			// m_pBuffer에 할당된 메모리 반환
	
	m_pBuffer = buf;							// m_pBuffer의 주소를 buf의 주소로
	m_nMsgLength = length;						// 메세지 크기 변환...새로 계산된 메세지 크기.

	return m_nMsgLength;						// 메세지 크기 반납



}


// Packet에 보낼 데이터 Setup
int CMessage::SetData(const void *msg, int nStart, int nLength)
{
	// 메세지 크기보다 집어 넣을 데이터의 위치와 크기가 클때 : 버퍼 확장.
	if((nStart + nLength) > m_nMsgLength) m_nDataLength += nLength;
		SetBuffLength(nStart+nLength);
	memcpy(m_pBuffer + nStart, msg, nLength);

	if(nStart == m_nDataLength) m_nDataLength += nLength; 
	// 데이터를 기록한 위치가 현재 패킷 크기와 맞먹을 경우 : 데이터 크기 증가.
	else if(nStart > m_nDataLength) m_nDataLength = nStart+nLength;
	// 데이터를 기록한 위치가 현재 패킷 크기보다 작을 경우 : 집어 넣은 크기 + 시작위치 만큼 증가.

	return m_nDataLength;
}



/////////////////////////////////////////////////////////////
//  Binder(받는쪽 처리)

//  길이 얻기 	
int CMessage::GetLength()
{
	return m_nDataLength;
}

int CMessage::GetData(void *pBuf, int nStart, int nLength)
{
	memcpy(pBuf, m_pBuffer+nStart, nLength);

	return nLength;
}




void * CMessage::GetDataPtr()
{
	return m_pBuffer;
}
