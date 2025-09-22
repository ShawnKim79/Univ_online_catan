// ChannelManager.cpp: implementation of the CChannelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Catan_Gameserver.h"
#include "ChannelManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChannelManager::CChannelManager()
{

}

CChannelManager::~CChannelManager()
{

}

void CChannelManager::ClearChannel()
{
	list<CHANNEL>::iterator i = channellist.begin();

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++)
	{
		lpchannel = &(*i);

		lpchannel->roomlist.clear(); // ä�� ������ �� ����Ʈ ����.
	}
	channellist.clear(); // ä�� ����
}

// ä�� ������ �� �θ鼭 �� ä�δ� ���� ������ ������ �ش�.
void CChannelManager::InitializeChannel(int serverid, int channelcnt, int roomcnt, int amxuser)
{
	ClearChannel();

	int maximum = roomcnt * amxuser;

	// ä�� ����
	for(int i=0 ; i<channelcnt ; i++)
	{
		CHANNEL ch;

		ch.id = i ; 
		ch.maximum = maximum;
		ch.present=0;

		for(int j=0 ; j<roomcnt ; j++)
		{
			ROOM room;

			room.channelno = i;
			room.id = j;
			room.maximum = amxuser;
			room.present = 0;

			memset(room.title, 0, sizeof(room.title));
			memset(room.passwd, 0, sizeof(room.passwd));

			strcpy((char *)room.title, "�� ��");

			room.locked = 0;
			room.status = 0;
			room.users[0] = NULL;
			room.users[1] = NULL;
			room.users[2] = NULL;
			room.users[3] = NULL;

			ch.roomlist.push_back(room);

		}

		channellist.push_back(ch);
	}
}

LPROOM CChannelManager::GetRoomInfo(int channelno, int roomno)
{
	list<CHANNEL>::iterator i = channellist.begin();

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++)
	{
		lpchannel = &(*i);

		if(channelno == lpchannel->id)
		{
			list<ROOM>::iterator j = lpchannel->roomlist.begin();

			LPROOM lproom;

			for(; j != lpchannel->roomlist.end() ; j++)
			{
				lproom = &(*j);
				if(roomno == lproom->id) return lproom;
			}
		}
	}
	return NULL;
}


// channelno�� �´� ä�� ID�� ���� ä�� ������ �����͸� �����Ѵ�....
LPCHANNEL CChannelManager::GetChannelInfo(int channelno)
{
	list<CHANNEL>::iterator i = channellist.begin();

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++)
	{
		lpchannel = &(*i);
		if(channelno == lpchannel->id) return lpchannel;
	}
	return NULL;
}
