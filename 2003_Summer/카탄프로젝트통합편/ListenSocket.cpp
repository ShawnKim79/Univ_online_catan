// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "īź.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
	srand(time(NULL));

	int i,j,r;
	for(i=0;i<4;i++){
		socket[i] = NULL;
		player[i].turn_dice = 0;
		
		player[i].tmp=false;
		player[i].half=false;
		player[i].resource_move=false;
		player[i].soldier=0;
		player[i].victory=0;
		player[i].ingame=false;

		for(j=0;j<5;j++)player[i].resource[j]=0;
	}
	
	for(i=0;i<20;i++)half_resource[i]=0;
	for(i=0;i<5;i++)resource[i]=19;
	
	for(i=0;i<25;i++) {
		while(1) {
			r=rand()%25;
			for(j=0;j<i;j++)if(r==card[j]) break;
			if(j==i) break;
		}
		card[i]=r;
	}
	cardn=25;

	longest_road=5;
	large_ami=5;
	plague_move=false;
	game_run=-1;
}

CListenSocket::~CListenSocket()
{
	for(int i=0;i<4;i++) if(socket[i]) delete socket[i];
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	for(int i=0;i<4;i++) {
		if(!socket[i]) {
			socket[i] = new (CServerSocket);
			Accept(*socket[i]);
			
			char message[4];
			message[0]='0';
			message[1]='0';
			message[2]='0'+i;
			socket[i]->ch_send(message,3);
			break;
		}
	}

	if(i==4) {
		CServerSocket *temp = new (CServerSocket);
		Accept(*temp);
		temp->Close();
		delete temp;
	}

	CAsyncSocket::OnAccept(nErrorCode);
}


void CListenSocket::all_send(char *buffer, int len)
{
	for(int i=0;i<4;i++)if(socket[i] && socket[i]->use) socket[i]->ch_send(buffer,len);
}

void CListenSocket::turn_set()
{
	int i,j,temp;

	for(i=0;i<4;i++)turn_sort[i]=i;
	
	for(i=0;i<4;i++) {
		for(j=i;j<4;j++) {
			if(player[turn_sort[i]].turn_dice<player[turn_sort[j]].turn_dice) {
				temp=turn_sort[i];
				turn_sort[i]=turn_sort[j];
				turn_sort[j]=temp;
			}
		}
	}
	turn=0;
}

void CListenSocket::turn_clear()
{
	for(int i=0;i<4;i++)turn_use[i]=0;
}
