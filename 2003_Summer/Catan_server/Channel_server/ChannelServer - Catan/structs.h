#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// 채널서버와 게임서버간 통신용 공용 패킷 구조체

#include "winsock2.h"
#include <string>
#include <list>

using namespace std;

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	unsigned short	channelno;	// 채널 번호
	unsigned short	maximum;	// 최대 허용 인원
	unsigned short	present;	// 현재 접속자 수

} CHANNEL, FAR* LPCHANNEL;

typedef struct
{
	DWORD socket;
	UINT	id;
	string	ip;
	UINT	portno;
	UINT	maxchannel;
	UINT	rooms;
	
	list<CHANNEL> channel;

} SERVER, FAR* LPSERVER;


typedef struct
{
	DWORD socket;

} CLIENT, FAR* LPCLIENT;

//////////////////////////////////////////////////////////////
// 소켓 통신용 패킷

typedef struct
{
	unsigned short header;
	unsigned long length;

} COMMON_PACKET; // 4byte




//////////////////////////////////////////
// 채널서버 -- 게임 서버간 통신 구조체
// 서버정보 구조체

typedef struct
{
	COMMON_PACKET cmnpck;

	unsigned short	id;			// 아이디
	unsigned char	ip[32];	// 아이피 주소
	unsigned short	port;		// 포트번호
	unsigned short	channels;	// 채널 갯수
	unsigned short	rooms;

} SERVERINFO; // 42 byte

typedef CHANNEL CHANNELINFO;
typedef CHANNEL FAR* LPCHANNELINFO;


//////////////////////////////////////////
//체널 서버 -- 클라이언트간 통신 구조체


#pragma pack(pop)

#endif