#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <list>
#include <string>

using namespace std;

#pragma pack(push)
#pragma pack(1)

// ���� ������ ����ü

struct ROOM;
typedef ROOM FAR *LPROOM;

typedef struct USER
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
	unsigned long	losscount;	// �� ��
	unsigned long	drawcount;	// ��� ��
	unsigned long	money;

	LPROOM			lproom;		// �� ������
	USER*			talkwith;

} USER, FAR *LPUSER;


struct ROOM
{
	unsigned short	id;			// �� ��ȣ
	unsigned short	channelno;	// ä�� ��ȣ
	unsigned short	status;		// �� ����
	unsigned short	maximum;	// �ִ� ���� �ο�
	unsigned short	present;	// ���� ���� �ο�
	unsigned short	locked;		// ��й�
	unsigned char	title[255];	// �� ����
	unsigned char	passwd[16];	// ��й� �н�����

	LPUSER users[2];			// [0] : ����
};



typedef struct
{
	unsigned short id;			// ä�� ��ȣ
	unsigned short maximum;		// �ִ� ���� �� ����
	unsigned short present;		// ���� ������ �� ����
	list<ROOM> roomlist;	// �� ���� ����Ʈ

} CHANNEL, FAR* LPCHANNEL;



//////////////////////////////////////////
// ä�μ��� -- ���� ������ ��� ����ü

// ä�μ����� ���Ӽ����� ��ſ� ���� ��Ŷ ����ü

typedef struct
{
	unsigned short header;
	unsigned long length;

} COMMON_PACKET;


typedef struct
{
	COMMON_PACKET cmnpack;

} CONNECTED;


typedef struct
{
	char chatmsg[1000];
	unsigned short msgtype;

	CHARFORMAT cf;

} CHATINFO, FAR* LPCHATINFO;

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

typedef struct
{
	unsigned short	channelno;	// ä�� ��ȣ
	unsigned short	maximum;	// �ִ� ��� �ο�
	unsigned short	present;	// ���� ������ ��

} CHANNELINFO; // 6 byte


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
	unsigned long	losscount;	// �� ��
	unsigned long	drawcount;	// ��� ��
	unsigned long	money;

} USERINFO, FAR* LPUSERINFO;


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
//���� ���� -- Ŭ���̾�Ʈ�� ��� ����ü

#pragma pack(pop)

#endif