// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lobby.h"
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
	m_pBuff = NULL;
	m_nDataLength = 0;
	m_nBuffLength = 0;
	m_Socket = 0;
}

CMessage::~CMessage()
{
	if(m_pBuff) delete [] m_pBuff;
}

int CMessage::SetData(const void *pSrc, int nStart, int nLen)
{
	if((nStart + nLen) > m_nBuffLength)
		SetBuffLength(nStart+nLen);

	memcpy( m_pBuff+nStart, pSrc, nLen );

	if(nStart == m_nDataLength) m_nDataLength += nLen;
	else if(nStart > m_nDataLength) m_nDataLength = nStart + nLen;

	return m_nDataLength;
}


int CMessage::SetBuffLength(int nLen)
{
	LPBYTE buff = new BYTE[nLen];

	memset(buff, 0, nLen);

	memcpy(buff, m_pBuff, m_nDataLength);
	if(m_pBuff) delete [] m_pBuff;
	m_pBuff =  buff;

	m_nBuffLength = nLen;

	return m_nBuffLength;
}

int CMessage::GetLength()
{
	return m_nDataLength;
}

int CMessage::GetData(void *pBuff, int nStart, int nLen)
{
	memcpy( pBuff, m_pBuff+nStart, nLen );

	return nLen;
}

void CMessage::Clear()
{
	memset(m_pBuff, 0, m_nBuffLength);

	m_nDataLength = 0;
}

void* CMessage::GetDataPtr()
{
	return m_pBuff;
}

DWORD CMessage::GetSocket()
{
	return m_Socket;
}

void CMessage::SetSocket(DWORD socket)
{
	m_Socket = socket;
}

void CMessage::SetParam(void *param)
{
	m_pParam = param;
}

void* CMessage::GetParam()
{
	return m_pParam;
}
