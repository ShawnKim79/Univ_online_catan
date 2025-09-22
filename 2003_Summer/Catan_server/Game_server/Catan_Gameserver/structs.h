#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <string>
#include <list>

using namespace std;

#pragma pack(push)
#pragma pack(1) // 각 구조체, 1바이트 메모리 정렬
/***************************************************************
1바이트 메모리 정렬을 하는 이유 :
구조체 내부의 멤버들이 각각의 형을 갖고 있을때, 
메모리 정렬을 해주지 않으면 하나의 구조체가 패킷으로 변환될때
메모리 억세스 하는 부분에 있어서 빈 공간이 발생하게 된다.
또한 구조체를 memcpy로 복제할 때, 메모리 정렬이 안 돼 있으면 대략 낭패.
***************************************************************/

struct ROOM; // ROOM 형 구조체 선언
typedef ROOM FAR *LPROOM; // ROOM FAR == *LPROOM, type define to pointer

typedef struct USER // USER 정보 구조체, D/B 서버 쿼리결과가 저장될 수 있음에 주의.
{
	unsigned char id[32]; // 접속자 ID
	unsigned char name[32]; // 접속자 이름
	unsigned char alias[32]; // 접속자 닉
	unsigned char ip[32]; // 접속자 IP
	unsigned short channelno; // 접속자가 접속한 채널 번호.
	short roomno; // 현재 접속자가 들어가 있는 방번호 : -1은 대기실

	unsigned long socket; // 소켓

	unsigned long score; // 점수
	unsigned long playcount; // 게임 수
	unsigned long victorycount; // 승수
	unsigned long losscount; // 패수
	unsigned long drawcount; // 비긴 수

	unsigned long money;

	LPROOM lproom; // 방 포인터
	USER* talkwith; // 그룹(?)
} USER, FAR *LPUSER;


// 생성된 방 정보(하나의 방은 하나의 게임진행 클라이언트)
struct ROOM
{
	unsigned short id; //방 번호
	unsigned short channelno; // 방이 소속된 채널 번호
	unsigned short status; // 방 상태
	unsigned short maximum; // 최대 수용 인원
	unsigned short present; // 현재 접속 인원
	unsigned short locked; //비밀방 
	unsigned char title[255]; // 방 제목
	unsigned char passwd[16]; // 비밀방 패스워드

	LPUSER users[4]; // [0]은 방장
};

// 채널 정보 구조체
typedef struct
{
	unsigned short id; // 채널 번호
	unsigned short maximum; // 최대 수용 방 갯수
	unsigned short present; // 현재 개설된 방 갯수
	list<ROOM> roomlist; // 방관리 리스트
} CHANNEL, FAR* LPCHANNEL;


//////////////////////////////////////////////////////
// 서버간 통신 구조체

typedef struct
{
	unsigned short header; // 헤더 길이
	unsigned long length;  // 전체 패킷 길이
} COMMON_PACKET;

typedef struct
{
	COMMON_PACKET cmnpacket;
} CONNECTED;

typedef struct
{
	char chatmsg[1000];
	unsigned short msgtype;

	CHARFORMAT cf;
} CHATINFO, FAR* LPCHATINFO;

typedef struct
{
	COMMON_PACKET cmnpacket;

	unsigned short id; // id
	unsigned char ip[32]; // ip 주소
	unsigned short port; // 포트 번호
	unsigned short channels; // 채널 갯수
	unsigned short rooms; //
} SERVERINFO;

typedef struct
{
	unsigned short channelno; // 채널 번호
	unsigned short maximum; // 최대 허용인원
	unsigned short present; // 현재 접속자 수
} CHANNELINFO;

typedef struct
{
	unsigned char id[32]; // 접속자 ID
	unsigned char name[32]; // 접속자 이름
	unsigned char alias[32]; // 접속자 닉
	unsigned char ip[32]; // 접속자 IP
	unsigned short channelno; // 접속자가 접속한 채널 번호.
	short roomno; // 현재 접속자가 들어가 있는 방번호 : -1은 대기실

	unsigned long socket; // 소켓

	unsigned long score; // 점수
	unsigned long playcount; // 게임 수
	unsigned long victorycount; // 승수
	unsigned long losscount; // 패수
	unsigned long drawcount; // 비긴 수

	unsigned long money;
} USERINFO, FAR* LPUSERINFO;

typedef struct
{
	unsigned short id; //방 번호
	unsigned short channelno; // 방이 소속된 채널 번호
	unsigned short status; // 방 상태
	unsigned short maximum; // 최대 수용 인원
	unsigned short present; // 현재 접속 인원
	unsigned short locked; //비밀방 
	unsigned char title[255]; // 방 제목
	unsigned char passwd[16]; // 비밀방 패스워드
} ROOMINFO, FAR* LPROOMINFO;

#pragma pack(pop)
#endif