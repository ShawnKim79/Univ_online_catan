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
// 작성일짜 : 2002년 12월 26일
//
// channellist로부터 해당 번호를 가지는 채널 노드의 포인터를 얻어옴
//
// params
// channelno : 얻고자하는 채널 번호

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
// 작성일짜 : 2002년 12월 26일
//
// channellist로부터 해당 번호를 가지는 채널의 roomlist로 부터 해당 번호의 ROOM 노드의 포인터를 얻어옴
//
// params
// channelno : ROOM이 속해있는 채널 번호, 얻고자 하는 방 번호

LPROOM CChannelManager::GetRoomInfo(int channelno, int roomno)
{
	list<CHANNEL>::iterator i = channellist.begin(); // 채널 시작 노드를 가져옴

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++) // 끝이 아닐 때까지 노드를 하나씩 
	{
		lpchannel = &(*i); // 참조

		if(channelno == lpchannel->id) // 찾고자 하는 채널 번호와 현재 채널의 id가 같으면 
		{
			list<ROOM>::iterator j = lpchannel->roomlist.begin(); // 그 채널이 가지고 있는 roomlist의 처음 노드를 가져옴

			LPROOM lproom;

			for(; j != lpchannel->roomlist.end() ; j++) // 끝이 아닐 때까지 노드를 하나씩
			{
				lproom = &(*j);	// 참조

				if(roomno == lproom->id) return lproom; // 찾고자 하는 방번호와 현재 노드의 ROOM id가 같으면 ROOM 노드 포인터를 리턴
			}
		}
	}
	
	return NULL;
}



////////////////////////////////////////////////////////
//
// 작성일짜 : 2002년 12월 26일
//
// 새로운 채널 리스트 생성 
//
// params
// channelcnt : 채널 크기, roomcnt : 한 채널이 가질 수 있는 방 갯수, maxuser : 방 1개당 최대 참가 인원 수

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

			strcpy((char *)room.title, "빈 방 입니다.");

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
// 작성일짜 : 2002년 12월 26일
//
// 모든 채널 정보를 삭제
//
// params
// 

void CChannelManager::ClearChannel()
{
	list<CHANNEL>::iterator i = channellist.begin(); // 채널 시작 노드를 가져옴

	LPCHANNEL lpchannel;

	for(; i != channellist.end() ; i++) // 끝이 아닐 때까지 노드를 하나씩 
	{
		lpchannel = &(*i); // 참조
		lpchannel->roomlist.clear();
	}

	channellist.clear();
}
