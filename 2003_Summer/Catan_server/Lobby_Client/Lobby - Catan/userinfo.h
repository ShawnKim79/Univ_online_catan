#pragma once
#include <windows.h>
/*
typedef struct __declspec(dllexport)
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
	unsigned long	losscount;	// 패 
	unsigned long	drawcount;	// 비김 수
	unsigned long	money;

} USERINFO;
*/
class  __declspec(dllexport) USERINFOEX : public USERINFO
{
public:
	~USERINFOEX();
	USHORT the_length_of_nickname;
	USHORT the_length_of_id;
	USHORT the_length_of_info;
	USHORT the_length_of_point;

	char* strpoint;
	char* strinfo;
	COLORREF	selfcolor;
};
