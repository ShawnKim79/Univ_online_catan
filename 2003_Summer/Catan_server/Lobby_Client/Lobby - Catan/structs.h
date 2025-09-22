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
	unsigned char	id[32];		// 접속자 아이디
	unsigned char	name[32];	// 접속자 네임
	unsigned char	alias[32];	// 접속자 닉네임
	unsigned char	ip[32];		// 접속자 아이피
	unsigned short	channelno;	// 접속한 채널 번호
	short			roomno;		// 접속한 방 번호 -1 : 대기실
	unsigned long	socket;		// 소켓
	unsigned long	score;		// 점수
	unsigned long	playcount;	// 총 게임 수
	unsigned long	victorycount;	// 승 수
	unsigned long	losscount;	// 패 수수
	unsigned long	drawcount;	// 비김 수
	unsigned long	money;

} USERINFO, FAR* LPUSERINFO;

class  __declspec(dllexport) USERINFOEX : public USERINFO
{
public:
	USERINFOEX(){};
	~USERINFOEX(){};
	USHORT the_length_of_nickname;
	USHORT the_length_of_id;
	USHORT the_length_of_info;
	USHORT the_length_of_point;

	char* strpoint;
	char* strinfo;
	COLORREF	selfcolor;
};


//typedef USER USERINFO, FAR* LPUSERINFO;

typedef struct
{
	unsigned short	channelno;	// 채널 번호
	unsigned short	maximum;	// 최대 허용 인원
	unsigned short	present;	// 현재 접속자 수

} CHANNEL, FAR* LPCHANNEL;

typedef struct
{
	DWORD socket;
	unsigned short	id;
	string	ip;
	unsigned short	portno;
	
	list<CHANNEL> channel;

} SERVER, FAR* LPSERVER;


typedef struct
{
	char chatmsg[1000];
	unsigned short msgtype;

	CHARFORMAT cf;

} CHATINFO, FAR* LPCHATINFO;

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

typedef CHANNEL CHANNELINFO, FAR* LPCHANNELINFO;

typedef struct
{
	unsigned short	id;			// 방 번호
	unsigned short	channelno;	// 채널 번호
	unsigned short	status;		// 방 상태
	unsigned short	maximum;	// 최대 수용 인원
	unsigned short	present;	// 현재 접속 인원
	unsigned short	locked;		// 비밀방
	unsigned char	title[255];	// 방 제목
	unsigned char	passwd[16];	// 비밀방 패스워드

} ROOMINFO, FAR *LPROOMINFO;

//////////////////////////////////////////
//체널 서버 -- 클라이언트간 통신 구조체


#pragma pack(pop)

#endif