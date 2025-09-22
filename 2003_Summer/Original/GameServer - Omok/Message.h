// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__42ECCAFD_664F_4570_85A8_2A8CEA3AD34C__INCLUDED_)
#define AFX_MESSAGE_H__42ECCAFD_664F_4570_85A8_2A8CEA3AD34C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "header.h"

class CMessage  
{
public:
	DWORD GetSocket();
	void SetSocket(DWORD socket);
	void* GetDataPtr();
	void Clear();
	int SetBuffLength(int nLen);
	int GetData(void *pBuff, int nStart, int nLen);
	int GetLength();
	int SetData(const void *pSrc, int nStart, int nLen);
	CMessage();
	virtual ~CMessage();

private:

	LPBYTE m_pBuff;
	int m_nDataLength;
	int m_nBuffLength;
	DWORD m_Socket;
};

#endif // !defined(AFX_MESSAGE_H__42ECCAFD_664F_4570_85A8_2A8CEA3AD34C__INCLUDED_)
