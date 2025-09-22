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
	// member ���� �ʱ�ȭ..
	m_Socket = 0;
	m_nMsgLength = 0;   // ��Ŷ �ʱ� ����.
	m_nDataLength = 0;
	m_pBuffer = NULL;

	m_pBuffer = new BYTE[0];
}

CMessage::~CMessage()
{
	if(m_pBuffer) delete [] m_pBuffer;
}

////////////////////////////////////////////////////////////////////
// Socket ó��
void CMessage::SetSocket(SOCKET s)
{
	m_Socket = s;
}

SOCKET CMessage::GetSocket()
{
	return m_Socket;
}

///////////////////////////////////////////////////////////////////////
// ��Ŷ ���� û��.
void CMessage::Clear_Data()
{
	memset(m_pBuffer, 0, m_nDataLength);
	m_nDataLength = 0;
}
//////////////////////////////////////////////////////////////////////
//	Sender(������ �� ó��) ->1. ��Ŷ ���� 2. ��Ŷ�� Data ����

// ��Ŷ ���� ���� & m_pBuffer�� �޸𸮵� �Ҵ�.
// Packet ũ�� ������....���� length�� ������ �޸� ũ��.
int CMessage::SetBuffLength(int length)
{
	LPBYTE buf = new BYTE[length];			// Buffer ����(����)
//	m_pBuffer = new BYTE[length];

	memset(buf, 0, length);						// ������ Buffer 0 ���� Set
	memcpy(buf, m_pBuffer, m_nDataLength);		// Buffer ���� ����.(Member -> ����)
												// m_pBuffer�� ���� ���� ��� ��ȣ�ȴ�.

	if(m_pBuffer) delete [] m_pBuffer;			// m_pBuffer�� �Ҵ�� �޸� ��ȯ
	
	m_pBuffer = buf;							// m_pBuffer�� �ּҸ� buf�� �ּҷ�
	m_nMsgLength = length;						// �޼��� ũ�� ��ȯ...���� ���� �޼��� ũ��.

	return m_nMsgLength;						// �޼��� ũ�� �ݳ�



}


// Packet�� ���� ������ Setup
int CMessage::SetData(const void *msg, int nStart, int nLength)
{
	// �޼��� ũ�⺸�� ���� ���� �������� ��ġ�� ũ�Ⱑ Ŭ�� : ���� Ȯ��.
	if((nStart + nLength) > m_nMsgLength) m_nDataLength += nLength;
		SetBuffLength(nStart+nLength);
	memcpy(m_pBuffer + nStart, msg, nLength);

	if(nStart == m_nDataLength) m_nDataLength += nLength; 
	// �����͸� ����� ��ġ�� ���� ��Ŷ ũ��� �¸��� ��� : ������ ũ�� ����.
	else if(nStart > m_nDataLength) m_nDataLength = nStart+nLength;
	// �����͸� ����� ��ġ�� ���� ��Ŷ ũ�⺸�� ���� ��� : ���� ���� ũ�� + ������ġ ��ŭ ����.

	return m_nDataLength;
}



/////////////////////////////////////////////////////////////
//  Binder(�޴��� ó��)

//  ���� ��� 	
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
