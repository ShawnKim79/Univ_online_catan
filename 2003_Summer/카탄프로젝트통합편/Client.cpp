// Client.cpp : implementation file
//

#include "stdafx.h"
#include "īź.h"
#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClient

CClient::CClient()
{
	strcpy(memo,"");
	use=true;
	no=-1;
	memo_len_n=0;
}

CClient::~CClient()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClient, CAsyncSocket)
	//{{AFX_MSG_MAP(CClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClient member functions

void CClient::OnReceive(int nErrorCode) 
{
	CAsyncSocket::OnReceive(nErrorCode);
}

int CClient::memo_copy(char *copymemo)
{
	//////////////////////////////////////////////
	// ���ݲ� ���� �޸� copymemo �� �����ϰ�	//
	// �޸� ������ �����մϴ�.					//
	//////////////////////////////////////////////

	int n;
	static char temp[4];

	if(memo_len_n!=4) {
		while((n=Receive(temp+memo_len_n,4-memo_len_n))>0) {
			if(n==SOCKET_ERROR) Sleep(10);
			else memo_len_n+=n;
		}
		
		if(memo_len_n==4) {
			memcpy(&memo_len,temp,4);
			memo_n=0;
		}
	}
	if(memo_len_n==4 && memo_len!=memo_n) {
		while((n=Receive(memo+memo_n,memo_len-memo_n))>0) {
			if(n==SOCKET_ERROR) Sleep(10);
			memo_n+=n;
		}
	}

	if(memo_len_n==4 && memo_n==memo_len) {
		for(int i=0;i<memo_len;i++)copymemo[i]=memo[i];	copymemo[i]=0;
		memo_len=0;
		memo_n=0;
		memo_len_n=0;
		return i;
	}
	return 0;
}

void CClient::ch_send(char *data, int len)
{
	char temp[1000];
	int i,m=0,l;

	memcpy(temp,&len,4);
	for(i=0;i<len;i++)temp[4+i]=data[i];
	
	len+=4;

	while(1) {
		l=this->Send(temp+m,len-m);
		if(l==SOCKET_ERROR) Sleep(10);
		m+=l;
		if(m>=len) break;
	}
}

void CClient::OnConnect(int nErrorCode) {
	if(nErrorCode==WSAECONNREFUSED) use=false;		// ���ӽ��н� �ź�
	CAsyncSocket::OnConnect(nErrorCode);
}

void CClient::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	use=false;
	CAsyncSocket::OnClose(nErrorCode);
}