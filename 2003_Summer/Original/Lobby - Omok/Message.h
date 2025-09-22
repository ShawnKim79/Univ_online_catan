// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__42ECCAFD_664F_4570_85A8_2A8CEA3AD34C__INCLUDED_)
#define AFX_MESSAGE_H__42ECCAFD_664F_4570_85A8_2A8CEA3AD34C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMessage  
{
public:
	void* GetParam();
	void SetParam(void *param);
	void SetSocket(DWORD socket);
	DWORD GetSocket();
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
	UINT m_nDataLength;
	UINT m_nBuffLength;
	DWORD m_Socket;
	void *m_pParam;

};

#endif // !defined(AFX_MESSAGE_H__42ECCAFD_664F_4570_85A8_2A8CEA3AD34C__INCLUDED_)
