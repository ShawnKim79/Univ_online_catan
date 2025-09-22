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
	unsigned short	channelno;	// ä�� ��ȣ
	unsigned short	maximum;	// �ִ� ��� �ο�
	unsigned short	present;	// ���� ������ ��

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

typedef CHANNEL CHANNELINFO;
typedef CHANNEL FAR* LPCHANNELINFO;


//////////////////////////////////////////
//ü�� ���� -- Ŭ���̾�Ʈ�� ��� ����ü


#pragma pack(pop)

#endif