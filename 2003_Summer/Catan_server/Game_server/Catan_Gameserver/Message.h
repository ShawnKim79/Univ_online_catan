// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "header.h"
#include "winsock2.h"

/*
#pragma pack(push)
#pragma pack(1)

typedef struct
{
	char Message_type;              // 메세지 타입
	int  Message_Length;			// 메세지 길이
} PACKETHEADER, *LPPACKETHEADER;

typedef union
{
	PACKETHEADER PHEADER;
	BYTE PACKET[MAX_LENGTH]; // 실제 메세지 내용이 저장되는 배열.
} MESSAGE;
// MESSAGE형(공용체) 정의
// MESSAGE형 내부에서 헤더와 실제 PACKET배열의 시작 주소는 동일하다...(union)
// 메세지는 패킷 헤더와 실제 패킷(길이 10000바이트짜리 배열)

#pragma pack(pop)

// 이번에는 메세지 구조체 대신 BYTE 버퍼 사용.

*/

class CMessage  
{
public:
	void * GetDataPtr();
/////////////////////////////////////////////////////////////////////////////////
// 생성자와 소멸자
	CMessage();
	virtual ~CMessage();

//////////////////////////////////////////////////////////////////////////////
// 소켓 설정
	SOCKET GetSocket();				// 
	void SetSocket(SOCKET sock);	//	메세지 객체와 소켓을 연결.

	void Clear_Data();				// Message Buffer 정리..
	
/////////////////////////
//	Sender(보내는 쪽 처리)  패킷 내 데이터 삽입 함수

	int SetBuffLength(int length);	// 데이터의 길이
	int SetData(const void *msg, int nStrat, int nLength);	// 데이터 버퍼 생성, msg는 보낼 메세지, nLength는 길이.

///////////////////////
//  Binder(받는쪽 처리)
	int GetLength();
	int GetData(void *pBuf, int nStart, int nLength);


private:
	SOCKET m_Socket;		// 메세지의 소켓.

	int m_nDataLength;		// 작업중 저장(계산)되는 메세지의 길이.
	int m_nMsgLength;		// data 부분 길이.

	LPBYTE m_pBuffer;			// BYTE형 배열.....메세지 저장.

};


