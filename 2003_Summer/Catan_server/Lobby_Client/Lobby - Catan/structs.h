#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// ä�μ����� ���Ӽ����� ��ſ� ���� ��Ŷ ����ü

#include "winsock2.h"
#include <string>
#include <list>

using namespace std;

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	unsigned char	id[32];		// ������ ���̵�
	unsigned char	name[32];	// ������ ����
	unsigned char	alias[32];	// ������ �г���
	unsigned char	ip[32];		// ������ ������
	unsigned short	channelno;	// ������ ä�� ��ȣ
	short			roomno;		// ������ �� ��ȣ -1 : ����
	unsigned long	socket;		// ����
	unsigned long	score;		// ����
	unsigned long	playcount;	// �� ���� ��
	unsigned long	victorycount;	// �� ��
	unsigned long	losscount;	// �� ����
	unsigned long	drawcount;	// ��� ��
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
	unsigned short	channelno;	// ä�� ��ȣ
	unsigned short	maximum;	// �ִ� ��� �ο�
	unsigned short	present;	// ���� ������ ��

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
// ���� ��ſ� ��Ŷ

typedef struct
{
	unsigned short header;
	unsigned long length;

} COMMON_PACKET; // 4byte




//////////////////////////////////////////
// ä�μ��� -- ���� ������ ��� ����ü
// �������� ����ü

typedef struct
{
	COMMON_PACKET cmnpck;

	unsigned short	id;			// ���̵�
	unsigned char	ip[32];	// ������ �ּ�
	unsigned short	port;		// ��Ʈ��ȣ
	unsigned short	channels;	// ä�� ����
	unsigned short	rooms;

} SERVERINFO; // 42 byte

typedef CHANNEL CHANNELINFO, FAR* LPCHANNELINFO;

typedef struct
{
	unsigned short	id;			// �� ��ȣ
	unsigned short	channelno;	// ä�� ��ȣ
	unsigned short	status;		// �� ����
	unsigned short	maximum;	// �ִ� ���� �ο�
	unsigned short	present;	// ���� ���� �ο�
	unsigned short	locked;		// ��й�
	unsigned char	title[255];	// �� ����
	unsigned char	passwd[16];	// ��й� �н�����

} ROOMINFO, FAR *LPROOMINFO;

//////////////////////////////////////////
//ü�� ���� -- Ŭ���̾�Ʈ�� ��� ����ü


#pragma pack(pop)

#endif