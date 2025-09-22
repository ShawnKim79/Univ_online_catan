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
	char Message_type;              // �޼��� Ÿ��
	int  Message_Length;			// �޼��� ����
} PACKETHEADER, *LPPACKETHEADER;

typedef union
{
	PACKETHEADER PHEADER;
	BYTE PACKET[MAX_LENGTH]; // ���� �޼��� ������ ����Ǵ� �迭.
} MESSAGE;
// MESSAGE��(����ü) ����
// MESSAGE�� ���ο��� ����� ���� PACKET�迭�� ���� �ּҴ� �����ϴ�...(union)
// �޼����� ��Ŷ ����� ���� ��Ŷ(���� 10000����Ʈ¥�� �迭)

#pragma pack(pop)

// �̹����� �޼��� ����ü ��� BYTE ���� ���.

*/

class CMessage  
{
public:
	void * GetDataPtr();
/////////////////////////////////////////////////////////////////////////////////
// �����ڿ� �Ҹ���
	CMessage();
	virtual ~CMessage();

//////////////////////////////////////////////////////////////////////////////
// ���� ����
	SOCKET GetSocket();				// 
	void SetSocket(SOCKET sock);	//	�޼��� ��ü�� ������ ����.

	void Clear_Data();				// Message Buffer ����..
	
/////////////////////////
//	Sender(������ �� ó��)  ��Ŷ �� ������ ���� �Լ�

	int SetBuffLength(int length);	// �������� ����
	int SetData(const void *msg, int nStrat, int nLength);	// ������ ���� ����, msg�� ���� �޼���, nLength�� ����.

///////////////////////
//  Binder(�޴��� ó��)
	int GetLength();
	int GetData(void *pBuf, int nStart, int nLength);


private:
	SOCKET m_Socket;		// �޼����� ����.

	int m_nDataLength;		// �۾��� ����(���)�Ǵ� �޼����� ����.
	int m_nMsgLength;		// data �κ� ����.

	LPBYTE m_pBuffer;			// BYTE�� �迭.....�޼��� ����.

};


