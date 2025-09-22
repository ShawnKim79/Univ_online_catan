// ChannelManager.cpp: implementation of the CChannelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameServer.h"
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
	channellist.clear();
}


////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 26��
//
// channellist�κ��� �ش� ��ȣ�� ������ ä�� ����� �����͸� ����
//
// params
// channelno : ������ϴ� ä�� ��ȣ

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


////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 26��
//
// channellist�κ��� �ش� ��ȣ�� ������ ä���� roomlist�� ���� �ش� ��ȣ�� ROOM ����� �����͸� ����
//
// params
// channelno : ROOM�� �����ִ� ä�� ��ȣ, ����� �ϴ� �� ��ȣ

LPROOM CChannelManager::GetRoomInfo(int channelno, int roomno)
{
	list<CHANNEL>::iterator i = channellist.begin(); // ä�� ���� ��带 ������

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++) // ���� �ƴ� ������ ��带 �ϳ��� 
	{
		lpchannel = &(*i); // ����

		if(channelno == lpchannel->id) // ã���� �ϴ� ä�� ��ȣ�� ���� ä���� id�� ������ 
		{
			list<ROOM>::iterator j = lpchannel->roomlist.begin(); // �� ä���� ������ �ִ� roomlist�� ó�� ��带 ������

			LPROOM lproom;

			for(; j != lpchannel->roomlist.end() ; j++) // ���� �ƴ� ������ ��带 �ϳ���
			{
				lproom = &(*j);	// ����

				if(roomno == lproom->id) return lproom; // ã���� �ϴ� ���ȣ�� ���� ����� ROOM id�� ������ ROOM ��� �����͸� ����
			}
		}
	}
	
	return NULL;
}



////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 26��
//
// ���ο� ä�� ����Ʈ ���� 
//
// params
// channelcnt : ä�� ũ��, roomcnt : �� ä���� ���� �� �ִ� �� ����, maxuser : �� 1���� �ִ� ���� �ο� ��

void CChannelManager::InitializeChannel(int serverid, int channelcnt, int roomcnt, int maxuser)
{
	ClearChannel();

	int maximum = roomcnt * maxuser;

	for(int i = 0 ; i < channelcnt ; i++)
	{
		CHANNEL ch;

		ch.id = i;
		ch.maximum = maximum;
		ch.present = 0;

		for(int j = 0 ; j < roomcnt ; j++)
		{
			ROOM room;

			room.channelno = i;
			room.id = j;
			room.maximum = maxuser;
			room.present = 0;
			
			memset(room.title, 0, sizeof(room.title));
			memset(room.passwd, 0, sizeof(room.passwd));

			strcpy((char *)room.title, "�� �� �Դϴ�.");

			room.locked = 0;
			room.status = 0;
			room.users[0] = NULL;
			room.users[1] = NULL;
			
			ch.roomlist.push_back(room);
		}

		channellist.push_back(ch);
	}
}


////////////////////////////////////////////////////////
//
// �ۼ���¥ : 2002�� 12�� 26��
//
// ��� ä�� ������ ����
//
// params
// 

void CChannelManager::ClearChannel()
{
	list<CHANNEL>::iterator i = channellist.begin(); // ä�� ���� ��带 ������

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++) // ���� �ƴ� ������ ��带 �ϳ��� 
	{
		lpchannel = &(*i); // ����
		lpchannel->roomlist.clear();
	}

	channellist.clear();
}
